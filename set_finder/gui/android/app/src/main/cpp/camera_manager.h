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

#ifndef CAMERA_NATIVE_CAMERA_H
#define CAMERA_NATIVE_CAMERA_H
#include <string>
#include <vector>
#include <map>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraMetadataTags.h>
#include <media/NdkImage.h>

enum class CaptureSessionState : int32_t {
  READY = 0,  // session is ready
  ACTIVE,     // session is busy
  CLOSED,     // session is closed(by itself or a new session evicts)
  MAX_STATE
};

struct ImageSize {
  int32_t width;
  int32_t height;
};

template <typename T>
class RangeValue {
 public:
  T min_, max_;
  /**
   * return absolute value from relative value
   * value: in percent (50 for 50%)
   */
  T value(int percent) {
    return static_cast<T>(min_ + (max_ - min_) * percent / 100);
  }
  RangeValue() { min_ = max_ = static_cast<T>(0); }

  bool Supported(void) const { return (min_ != max_); }
};

struct CaptureRequestInfo {
  ANativeWindow* outputNativeWindow_;
  ACaptureSessionOutput* sessionOutput_;
  ACameraOutputTarget* target_;
  ACaptureRequest* request_;
  ACameraDevice_request_template template_;
  int sessionSequenceId_;
};

class CameraId;
class NDKCamera {
 private:
  ACameraManager* cameraMgr_;
  std::map<std::string, CameraId> cameras_;
  std::string activeCameraId_;
  uint32_t cameraFacing_;
  uint32_t cameraOrientation_;

  CaptureRequestInfo m_request_info;

  ACaptureSessionOutputContainer* outputContainer_;
  ACameraCaptureSession* captureSession_;
  CaptureSessionState captureSessionState_;

  // set up exposure control
  int64_t exposureTime_;
  RangeValue<uint64_t> exposureRange_;
  int32_t sensitivity_;
  RangeValue<int32_t> sensitivityRange_;
  volatile bool valid_;

  ACameraManager_AvailabilityCallbacks* GetManagerListener();
  ACameraDevice_stateCallbacks* GetDeviceListener();
  ACameraCaptureSession_stateCallbacks* GetSessionListener();

 public:
  NDKCamera();
  ~NDKCamera();
  void EnumerateCamera(void);
  bool MatchCaptureSizeRequest(int32_t requestWidth, int32_t requestHeight, AIMAGE_FORMATS image_format, ImageSize* view);
  void CreateSession(ANativeWindow* previewWindow, bool jpg_format, int32_t imageRotation = 0);

  bool GetSensorOrientation(int32_t* facing, int32_t* angle);
  void OnCameraStatusChanged(const char* id, bool available);
  void OnDeviceState(ACameraDevice* dev);
  void OnDeviceError(ACameraDevice* dev, int err);
  void OnSessionState(ACameraCaptureSession* ses, CaptureSessionState state);
  void OnCaptureSequenceEnd(ACameraCaptureSession* session, int sequenceId,
                            int64_t frameNumber);
  void OnCaptureFailed(ACameraCaptureSession* session, ACaptureRequest* request,
                       ACameraCaptureFailure* failure);

  void StartPreview(bool start);
};

// helper classes to hold enumerated camera
class CameraId {
 public:
  ACameraDevice* device_;
  std::string id_;
  acamera_metadata_enum_android_lens_facing_t facing_;
  bool available_;  // free to use ( no other apps are using
  bool owner_;      // we are the owner of the camera
  explicit CameraId(const char* id)
      : device_(nullptr),
        facing_(ACAMERA_LENS_FACING_FRONT),
        available_(false),
        owner_(false) {
    id_ = id;
  }

  explicit CameraId(void) { CameraId(""); }
};

#endif  // CAMERA_NATIVE_CAMERA_H
