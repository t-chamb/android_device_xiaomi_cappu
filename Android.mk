# Copyright (C) 2014 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

ifneq ($(filter cappu,$(TARGET_DEVICE)),)

LOCAL_PATH := $(call my-dir)

# RemovePackages is left out: the 17.1 tree's empty prebuilt is rejected on Android 12.
include $(call all-named-subdir-makefiles,audio_remote_submix gatekeeper hidl libbt-vendor-mtk libmtk_symbols libshims libxlog power rootdir sensor vndk wifi_hal wpa_supplicant_8_lib)

endif
