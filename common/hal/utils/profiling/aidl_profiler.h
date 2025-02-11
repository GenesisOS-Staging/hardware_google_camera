/*
 * Copyright (C) 2022 The Android Open Source Project
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

#ifndef HARDWARE_GOOGLE_CAMERA_UTIL_AIDL_SERVICE_AIDL_PROFILER_H_
#define HARDWARE_GOOGLE_CAMERA_UTIL_AIDL_SERVICE_AIDL_PROFILER_H_

#include <functional>
#include <memory>
#include <string>

#include "profiler.h"
#include "profiler_util.h"

namespace android {
namespace google_camera_hal {

class AidlScopedProfiler {
 public:
  AidlScopedProfiler(std::shared_ptr<google::camera_common::Profiler> profiler,
                     const std::string name, int id,
                     std::function<void()> end_callback);

  ~AidlScopedProfiler();

 private:
  std::shared_ptr<google::camera_common::Profiler> profiler_;
  const std::string name_;
  int id_;
  std::function<void()> end_callback_;
};

class AidlProfiler {
 public:
  virtual ~AidlProfiler() = default;

  static std::shared_ptr<AidlProfiler> Create(uint32_t camera_id);

  // Make a ScopedProfiler for given type.
  virtual std::unique_ptr<AidlScopedProfiler> MakeScopedProfiler(
      EventType type,
      std::unique_ptr<google::camera_common::Profiler> custom_latency_profiler,
      std::unique_ptr<google::camera_common::Profiler> custom_fps_profiler) = 0;

  // Call when first frame is requested.
  virtual void FirstFrameStart() = 0;

  // Call when all bufer in first frame is received.
  virtual void FirstFrameEnd() = 0;

  // Call when a request is made for a reprocess capture.
  // Provide a profiler for custom implementation, or a default will be created.
  // Repeated calls create a new profiler if there is none set.
  virtual void ReprocessingRequestStart(
      std::unique_ptr<google::camera_common::Profiler> custom_reprocessing_profiler,
      int32_t id) = 0;

  // Call when reprocess capture result is received.
  // Delete reprocessing profiler if all open requests have ended.
  virtual void ReprocessingResultEnd(int32_t id) = 0;

  // Call to profile frame rate for each stream.
  virtual void ProfileFrameRate(const std::string& name) = 0;

  virtual uint32_t GetCameraId() const = 0;
  virtual int32_t GetLatencyFlag() const = 0;
  virtual int32_t GetFpsFlag() const = 0;
  virtual int32_t GetReprocessLatencyFlag() const = 0;

 protected:
  AidlProfiler() = default;
};

}  // namespace google_camera_hal
}  // namespace android

#endif  // HARDWARE_GOOGLE_CAMERA_UTIL_AIDL_SERVICE_AIDL_PROFILER_H_
