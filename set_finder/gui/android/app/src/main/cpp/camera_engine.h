
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

#ifndef __CAMERA_ENGINE_H__
#define __CAMERA_ENGINE_H__
#include <android/native_activity.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <atomic>
#include <functional>
#include <jni.h>
#include <thread>

#include "camera_manager.h"
#include "preview_processor.h"

class CameraAppEngine
{
public:
    explicit CameraAppEngine(JNIEnv* env, jobject instance, jint w, jint h);
    ~CameraAppEngine();

    // Manage NDKCamera Object
    void CreateCameraSession(jobject surface, const char* save_path);
    void StartPreview(bool start);
    const ImageSize& GetCompatibleCameraRes() const;
    int32_t GetCameraSensorOrientation(int32_t facing);
    jobject GetSurfaceObject();

    void debugRun(bool debug_run);
    void saveImage();

private:
    JNIEnv* env_;
    jobject javaInstance_;
    int32_t requestWidth_;
    int32_t requestHeight_;
    jobject surface_;
    NDKCamera* camera_;
    ImageSize compatibleCameraRes_;

    std::atomic_bool m_save_image = false;
    std::atomic_bool m_debug_run = false;
    AIMAGE_FORMATS m_image_format;
    std::unique_ptr<PreviewProcessor> m_preview_processor;
};
#endif // __CAMERA_ENGINE_H__
