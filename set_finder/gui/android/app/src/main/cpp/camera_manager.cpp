/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <utility>
#include <queue>
#include <unistd.h>
#include <cinttypes>
#include <cstring>
#include <camera/NdkCameraManager.h>
#include <media/NdkImage.h>
#include "camera_manager.h"
#include "utils/native_debug.h"
#include "utils/camera_utils.h"

/**
 * Range of Camera Exposure Time:
 *     Camera's capability range have a very long range which may be disturbing
 *     on camera. For this sample purpose, clamp to a range showing visible
 *     video on preview: 100000ns ~ 250000000ns
 */
static const uint64_t kMinExposureTime = static_cast<uint64_t>(1000000);
static const uint64_t kMaxExposureTime = static_cast<uint64_t>(250000000);

NDKCamera::NDKCamera()
    : cameraMgr_(nullptr),
      activeCameraId_(""),
      cameraFacing_(ACAMERA_LENS_FACING_BACK),
      cameraOrientation_(0),
      outputContainer_(nullptr),
      captureSessionState_(CaptureSessionState::MAX_STATE),
      exposureTime_(static_cast<int64_t>(0)) {
  valid_ = false;
  cameras_.clear();
  cameraMgr_ = ACameraManager_create();
  ASSERT(cameraMgr_, "Failed to create cameraManager");

  // Pick up a back-facing camera to preview
  EnumerateCamera();
  ASSERT(activeCameraId_.size(), "Unknown ActiveCameraIdx");

  // Create back facing camera device
  CALL_MGR(openCamera(cameraMgr_, activeCameraId_.c_str(), GetDeviceListener(),
                      &cameras_[activeCameraId_].device_));

  CALL_MGR(registerAvailabilityCallback(cameraMgr_, GetManagerListener()));

  // Initialize camera controls(exposure time and sensitivity), pick
  // up value of 2% * range + min as starting value (just a number, no magic)
  ACameraMetadata* metadataObj;
  CALL_MGR(getCameraCharacteristics(cameraMgr_, activeCameraId_.c_str(),
                                    &metadataObj));
  ACameraMetadata_const_entry val = {
      0,
  };
  camera_status_t status = ACameraMetadata_getConstEntry(
      metadataObj, ACAMERA_SENSOR_INFO_EXPOSURE_TIME_RANGE, &val);
  if (status == ACAMERA_OK) {
    exposureRange_.min_ = val.data.i64[0];
    if (exposureRange_.min_ < kMinExposureTime) {
      exposureRange_.min_ = kMinExposureTime;
    }
    exposureRange_.max_ = val.data.i64[1];
    if (exposureRange_.max_ > kMaxExposureTime) {
      exposureRange_.max_ = kMaxExposureTime;
    }
    exposureTime_ = exposureRange_.value(2);
  } else {
    LOGW("Unsupported ACAMERA_SENSOR_INFO_EXPOSURE_TIME_RANGE");
    exposureRange_.min_ = exposureRange_.max_ = 0l;
    exposureTime_ = 0l;
  }
  status = ACameraMetadata_getConstEntry(
      metadataObj, ACAMERA_SENSOR_INFO_SENSITIVITY_RANGE, &val);

  if (status == ACAMERA_OK){
    sensitivityRange_.min_ = val.data.i32[0];
    sensitivityRange_.max_ = val.data.i32[1];

    sensitivity_ = sensitivityRange_.value(2);
  } else {
    LOGW("failed for ACAMERA_SENSOR_INFO_SENSITIVITY_RANGE");
    sensitivityRange_.min_ = sensitivityRange_.max_ = 0;
    sensitivity_ = 0;
  }
  valid_ = true;
}

/**
 * A helper class to assist image size comparison, by comparing the absolute
 * size
 * regardless of the portrait or landscape mode.
 */
class DisplayDimension {
 public:
  DisplayDimension(int32_t w, int32_t h) : w_(w), h_(h), portrait_(false) {
    if (h > w) {
      // make it landscape
      w_ = h;
      h_ = w;
      portrait_ = true;
    }
  }
  DisplayDimension(const DisplayDimension& other) {
    w_ = other.w_;
    h_ = other.h_;
    portrait_ = other.portrait_;
  }

  DisplayDimension(void) {
    w_ = 0;
    h_ = 0;
    portrait_ = false;
  }
  DisplayDimension& operator=(const DisplayDimension& other) {
    w_ = other.w_;
    h_ = other.h_;
    portrait_ = other.portrait_;

    return (*this);
  }

  bool IsSameRatio(DisplayDimension& other) {
    return (w_ * other.h_ == h_ * other.w_);
  }
  bool operator>(DisplayDimension& other) {
    return (w_ >= other.w_ & h_ >= other.h_);
  }
  bool operator==(DisplayDimension& other) {
    return (w_ == other.w_ && h_ == other.h_ && portrait_ == other.portrait_);
  }
  DisplayDimension operator-(DisplayDimension& other) {
    DisplayDimension delta(w_ - other.w_, h_ - other.h_);
    return delta;
  }
  void Flip(void) { portrait_ = !portrait_; }
  bool IsPortrait(void) { return portrait_; }
  int32_t width(void) { return w_; }
  int32_t height(void) { return h_; }
  int32_t org_width(void) { return (portrait_ ? h_ : w_); }
  int32_t org_height(void) { return (portrait_ ? w_ : h_); }

 private:
  int32_t w_, h_;
  bool portrait_;
};

/**
 * Find a compatible camera modes:
 *    1) the same aspect ration as the native display window, which should be a
 *       rotated version of the physical device
 *    2) the smallest resolution in the camera mode list
 * This is to minimize the later color space conversion workload.
 */
bool NDKCamera::MatchCaptureSizeRequest(int32_t requestWidth,
                                        int32_t requestHeight,
                                        AIMAGE_FORMATS image_format,
                                        ImageSize* resView) {
  DisplayDimension disp(requestWidth, requestHeight);
  if (cameraOrientation_ == 90 || cameraOrientation_ == 270) {
    disp.Flip();
  }
  ACameraMetadata* metadata;
  CALL_MGR(
      getCameraCharacteristics(cameraMgr_, activeCameraId_.c_str(), &metadata));
  ACameraMetadata_const_entry entry;
  CALL_METADATA(getConstEntry(
      metadata, ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS, &entry));

  // format of the data: format, width, height, input?, type int32
  bool foundIt = false;
  DisplayDimension foundRes(4000, 4000);

  for (uint32_t i = 0; i < entry.count; i += 4) {
    int32_t input = entry.data.i32[i + 3];
    int32_t format = entry.data.i32[i + 0];
    if (input) continue;

    if (format == image_format) {
      DisplayDimension res(entry.data.i32[i + 1],
                           entry.data.i32[i + 2]);
      LOGI("!!!! supported res, w:%d, h:%d", res.width(), res.height());
      if (!disp.IsSameRatio(res)) continue;
      if (foundRes > res) {
        foundIt = true;
        foundRes = res;
      }
    }
  }

  if (foundIt) {
    resView->width = foundRes.org_width();
    resView->height = foundRes.org_height();
  } else {
    LOGW("Did not find any compatible camera resolution, taking 640x480");
    if (disp.IsPortrait()) {
      resView->width = 480;
      resView->height = 640;
    } else {
      resView->width = 640;
      resView->height = 480;
    }
  }

  return foundIt;
}

void NDKCamera::CreateSession(ANativeWindow* previewWindow,
                              bool jpg_format,
                              int32_t imageRotation) {
  // Create output from this app's ANativeWindow, and add into output container
  m_request_info.outputNativeWindow_ = previewWindow;
  m_request_info.template_ = TEMPLATE_PREVIEW;

  CALL_CONTAINER(create(&outputContainer_));
  if (!m_request_info.outputNativeWindow_) return;

  ANativeWindow_acquire(m_request_info.outputNativeWindow_);
  CALL_OUTPUT(create(m_request_info.outputNativeWindow_, &m_request_info.sessionOutput_));
  CALL_CONTAINER(add(outputContainer_, m_request_info.sessionOutput_));
  CALL_TARGET(create(m_request_info.outputNativeWindow_, &m_request_info.target_));
  CALL_DEV(createCaptureRequest(cameras_[activeCameraId_].device_,
                                  m_request_info.template_, &m_request_info.request_));
  CALL_REQUEST(addTarget(m_request_info.request_, m_request_info.target_));

  // Create a capture session for the given preview request
  captureSessionState_ = CaptureSessionState::READY;
  CALL_DEV(createCaptureSession(cameras_[activeCameraId_].device_,
                                outputContainer_, GetSessionListener(),
                                &captureSession_));

  if (jpg_format) {
    ACaptureRequest_setEntry_i32(m_request_info.request_,
                                 ACAMERA_JPEG_ORIENTATION, 1, &imageRotation);
  }
}

NDKCamera::~NDKCamera() {
  valid_ = false;
  // stop session if it is on:
  if (captureSessionState_ == CaptureSessionState::ACTIVE) {
    ACameraCaptureSession_stopRepeating(captureSession_);
  }
  ACameraCaptureSession_close(captureSession_);

  if (!m_request_info.outputNativeWindow_) return;

  CALL_REQUEST(removeTarget(m_request_info.request_, m_request_info.target_));
  ACaptureRequest_free(m_request_info.request_);
  ACameraOutputTarget_free(m_request_info.target_);

  CALL_CONTAINER(remove(outputContainer_, m_request_info.sessionOutput_));
  ACaptureSessionOutput_free(m_request_info.sessionOutput_);

  ANativeWindow_release(m_request_info.outputNativeWindow_);

  ACaptureSessionOutputContainer_free(outputContainer_);

  for (auto& cam : cameras_) {
    if (cam.second.device_) {
      CALL_DEV(close(cam.second.device_));
    }
  }
  cameras_.clear();
  if (cameraMgr_) {
    CALL_MGR(unregisterAvailabilityCallback(cameraMgr_, GetManagerListener()));
    ACameraManager_delete(cameraMgr_);
    cameraMgr_ = nullptr;
  }
}

/**
 * EnumerateCamera()
 *     Loop through cameras on the system, pick up
 *     1) back facing one if available
 *     2) otherwise pick the first one reported to us
 */
void NDKCamera::EnumerateCamera() {
  ACameraIdList* cameraIds = nullptr;
  CALL_MGR(getCameraIdList(cameraMgr_, &cameraIds));

  for (int i = 0; i < cameraIds->numCameras; ++i) {
    const char* id = cameraIds->cameraIds[i];

    ACameraMetadata* metadataObj;
    CALL_MGR(getCameraCharacteristics(cameraMgr_, id, &metadataObj));

    int32_t count = 0;
    const uint32_t* tags = nullptr;
    ACameraMetadata_getAllTags(metadataObj, &count, &tags);
    for (int tagIdx = 0; tagIdx < count; ++tagIdx) {
      if (ACAMERA_LENS_FACING == tags[tagIdx]) {
        ACameraMetadata_const_entry lensInfo = {
            0,
        };
        CALL_METADATA(getConstEntry(metadataObj, tags[tagIdx], &lensInfo));
        CameraId cam(id);
        cam.facing_ = static_cast<acamera_metadata_enum_android_lens_facing_t>(
            lensInfo.data.u8[0]);
        cam.owner_ = false;
        cam.device_ = nullptr;
        cameras_[cam.id_] = cam;
        if (cam.facing_ == ACAMERA_LENS_FACING_BACK) {
          activeCameraId_ = cam.id_;
        }
        break;
      }
    }
    ACameraMetadata_free(metadataObj);
  }

  ASSERT(cameras_.size(), "No Camera Available on the device");
  if (activeCameraId_.length() == 0) {
    // if no back facing camera found, pick up the first one to use...
    activeCameraId_ = cameras_.begin()->second.id_;
  }
  ACameraManager_deleteCameraIdList(cameraIds);
}

/**
 * GetSensorOrientation()
 *     Retrieve current sensor orientation regarding to the phone device
 * orientation
 *     SensorOrientation is NOT settable.
 */
bool NDKCamera::GetSensorOrientation(int32_t* facing, int32_t* angle) {
  if (!cameraMgr_) {
    return false;
  }

  ACameraMetadata* metadataObj;
  ACameraMetadata_const_entry face, orientation;
  CALL_MGR(getCameraCharacteristics(cameraMgr_, activeCameraId_.c_str(),
                                    &metadataObj));
  CALL_METADATA(getConstEntry(metadataObj, ACAMERA_LENS_FACING, &face));
  cameraFacing_ = static_cast<int32_t>(face.data.u8[0]);

  CALL_METADATA(
      getConstEntry(metadataObj, ACAMERA_SENSOR_ORIENTATION, &orientation));

  LOGI("====Current SENSOR_ORIENTATION: %8d", orientation.data.i32[0]);

  ACameraMetadata_free(metadataObj);
  cameraOrientation_ = orientation.data.i32[0];

  if (facing) *facing = cameraFacing_;
  if (angle) *angle = cameraOrientation_;
  return true;
}

/**
 * StartPreview()
 *   Toggle preview start/stop
 */
void NDKCamera::StartPreview(bool start) {
  if (start) {
    CALL_SESSION(setRepeatingRequest(captureSession_, nullptr, 1,
                                     &m_request_info.request_,
                                     nullptr));
  } else if (!start && captureSessionState_ == CaptureSessionState::ACTIVE) {
    ACameraCaptureSession_stopRepeating(captureSession_);
  }
}
