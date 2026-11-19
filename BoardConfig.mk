#
# Copyright (C) 2018 The LineageOS Project
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
#

# mt6753 platform boardconfig

# Path
LOCAL_PATH := device/xiaomi/cappu

# Architecture Extensions
ARCH_ARM_HAVE_TLS_REGISTER := true

# Audio
BOARD_USES_MTK_AUDIO := true
USE_XML_AUDIO_POLICY_CONF := 1

# Architecture
TARGET_ARCH := arm64
TARGET_ARCH_VARIANT := armv8-a
TARGET_CPU_VARIANT := generic
TARGET_CPU_ABI := arm64-v8a

TARGET_CPU_SMP := true

TARGET_2ND_ARCH := arm
TARGET_2ND_ARCH_VARIANT := armv7-a-neon
TARGET_2ND_CPU_VARIANT := cortex-a15
TARGET_2ND_CPU_ABI := armeabi-v7a
TARGET_2ND_CPU_ABI2 := armeabi

# Binder API
TARGET_USES_64_BIT_BINDER := true

# Bootloader
TARGET_BOOTLOADER_BOARD_NAME := cappu
TARGET_NO_BOOTLOADER := true

# Bluetooth
BOARD_HAVE_BLUETOOTH := true
BOARD_HAVE_BLUETOOTH_BCM := true
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(LOCAL_PATH)/bluetooth

# Boot animation
TARGET_BOOTANIMATION_HALF_RES := true
TARGET_BOOTANIMATION_MULTITHREAD_DECODE := true

# Camera
USE_CAMERA_STUB := false

# dexpre-opt
ifeq ($(HOST_OS),linux)
  ifneq ($(TARGET_BUILD_VARIANT),eng)
      WITH_DEXPREOPT := true
      WITH_DEXPREOPT_DEBUG_INFO := false
      USE_DEX2OAT_DEBUG := false
  else
      WITH_DEXPREOPT := true
  endif
endif

WITH_DEXPREOPT_BOOT_IMG_AND_SYSTEM_SERVER_ONLY := false

# ELF
BUILD_BROKEN_ELF_PREBUILT_PRODUCT_COPY_FILES := true

# The legacy RenderScript clang prebuilt (clang-3289846) links against
# libncurses.so.5/libtinfo.so.5; the build host provides genuine ABI-5
# builds of them via LD_LIBRARY_PATH, which ninja's env allowlist strips
# unless listed here.
BUILD_BROKEN_NINJA_USES_ENV_VARS := LD_LIBRARY_PATH

# Hardware
BOARD_HAS_MTK_HARDWARE := true
BOARD_USES_MTK_HARDWARE := true
MTK_HARDWARE := true

JAVA_SOURCE_OVERLAYS := org.lineageos.hardware|$(LOCAL_PATH)/lineagehw|**/*.java

# Legacy blobs
TARGET_NEEDS_PLATFORM_TEXT_RELOCATIONS := true

# Disable memcpy opt (for audio libraries)
TARGET_CPU_MEMCPY_OPT_DISABLE := true

# Flags
BOARD_GLOBAL_CFLAGS += -DNO_SECURE_DISCARD
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true

# Fonts
EXTENDED_FONT_FOOTPRINT := true

# Platform
TARGET_BOARD_PLATFORM := mt8173
TARGET_NO_BOOTLOADER := true
TARGET_NO_FACTORYIMAGE := true
TARGET_BOOTLOADER_BOARD_NAME := mt8173

# Graphics
BOARD_EGL_CFG := system/vendor/lib/egl/egl.cfg
TARGET_FORCE_HWC_FOR_VIRTUAL_DISPLAYS := true
NUM_FRAMEBUFFER_SURFACE_BUFFERS := 3
TARGET_RUNNING_WITHOUT_SYNC_FRAMEWORK := true

# Panel vsync offsets
PRESENT_TIME_OFFSET_FROM_VSYNC_NS := 0
MAX_VIRTUAL_DISPLAY_DIMENSION := 1
MTK_HWC_SUPPORT := yes
MTK_HWC_VERSION := 1.4.1
TARGET_REQUIRES_SYNCHRONOUS_SETSURFACE := true

# Kernel
BOARD_BOOT_HEADER_VERSION := 0
BOARD_KERNEL_CMDLINE := bootopt=64S3,32N2,64N2 androidboot.selinux=permissive androidboot.hardware=mt8173
BOARD_KERNEL_PAGESIZE := 2048
BOARD_NAME := 1479347649
BOARD_KERNEL_BASE := 0x40078000
BOARD_KERNEL_OFFSET := 0x00008000
BOARD_RAMDISK_OFFSET := 0x08f88000
BOARD_TAGS_OFFSET := 0x0df88000
BOARD_SECOND_OFFSET := 0x00f00000
BOARD_MKBOOTIMG_ARGS := --board $(BOARD_NAME) --base $(BOARD_KERNEL_BASE) --pagesize $(BOARD_KERNEL_PAGESIZE) --kernel_offset $(BOARD_KERNEL_OFFSET) --ramdisk_offset $(BOARD_RAMDISK_OFFSET) --tags_offset $(BOARD_TAGS_OFFSET) --second_offset $(BOARD_SECOND_OFFSET)
TARGET_KERNEL_ARCH := arm64

# Kernel Build
TARGET_KERNEL_SOURCE := kernel/xiaomi/cappu
TARGET_KERNEL_CONFIG := cappu_defconfig
# This 3.18 VDSO link is driven through Clang but requires the matching GNU
# binutils linker; do not let Clang search the host PATH for an unqualified ld.
KERNEL_LD := LD=$(abspath prebuilts/gcc/linux-x86/aarch64/aarch64-linux-android-4.9/bin/aarch64-linux-android-ld.bfd)
BOARD_KERNEL_IMAGE_NAME := Image.gz-dtb
# The clang-12 build of this 3.18 kernel does not boot. Ship the GCC 4.9 build of the
# pre-port source (kernel commit a3149ff2c); see prebuilt/README.md.
TARGET_FORCE_PREBUILT_KERNEL := true
TARGET_PREBUILT_KERNEL := device/xiaomi/cappu/prebuilt/Image.gz-dtb
MTK_APPENDED_DTB_SUPPORT := yes

# FS
BOARD_CACHEIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_USERDATAIMAGE_FILE_SYSTEM_TYPE := ext4
TARGET_USERIMAGES_USE_EXT4 := true

# Offmode Charging
HEALTHD_ENABLE_TRICOLOR_LED := true
BOARD_CHARGER_ENABLE_SUSPEND := true
BOARD_CHARGER_SHOW_PERCENTAGE := true
BOARD_CHARGER_DISABLE_INIT_BLANK := true
BACKLIGHT_PATH := /sys/devices/platform/leds-mt65xx/leds/lcd-backlight/brightness
RED_LED_PATH := /sys/class/leds/red/brightness
GREEN_LED_PATH := /sys/class/leds/green/brightness
BLUE_LED_PATH := /sys/class/leds/blue/brightness

# Panel vsync offsets
PRESENT_TIME_OFFSET_FROM_VSYNC_NS := 0

# Per-application sizes for shader cache
MAX_EGL_CACHE_SIZE := 4194304
MAX_EGL_CACHE_ENTRY_SIZE := 262144

# Partitions
BOARD_BOOTIMAGE_PARTITION_SIZE := 67108864
BOARD_RECOVERYIMAGE_PARTITION_SIZE := 67108864
BOARD_CACHEIMAGE_PARTITION_SIZE := 419430400
BOARD_SYSTEMIMAGE_PARTITION_SIZE := 2684354560
BOARD_USERDATAIMAGE_PARTITION_SIZE := 6442450944
BOARD_FLASH_BLOCK_SIZE := 131072

BOARD_HAS_LARGE_FILESYSTEM := true
TARGET_USERIMAGES_USE_EXT4 := true

# PowerHAL
TARGET_POWERHAL_VARIANT := mt8173

# Display
TARGET_SCREEN_WIDTH := 2048
TARGET_SCREEN_HEIGHT := 1536

# LightHAL
TARGET_PROVIDES_LIBLIGHT := true

# Sensors
TARGET_NO_SENSOR_PERMISSION_CHECK := true

# Recovery
BOARD_HAS_NO_SELECT_BUTTON := true
BOOTLOADER_MESSAGE_OFFSET := 4096
TARGET_RECOVERY_FSTAB := $(LOCAL_PATH)/rootdir/etc/fstab.mt8173

# VINTF
DEVICE_MANIFEST_FILE := $(LOCAL_PATH)/manifest.xml

# Include
TARGET_SPECIFIC_HEADER_PATH += $(LOCAL_PATH)/include

TARGET_SYSTEM_PROP := $(LOCAL_PATH)/system.prop

# RenderScript
BOARD_DISABLE_RENDERSCRIPT_GPU := true
BOARD_OVERRIDE_RS_CPU_VARIANT_32 := cortex-a53
BOARD_OVERRIDE_RS_CPU_VARIANT_64 := cortex-a72

# SELinux
SELINUX_IGNORE_NEVERALLOWS := true
BOARD_VENDOR_SEPOLICY_DIRS += \
        device/xiaomi/cappu/sepolicy-mtk/basic/non_plat \
        device/xiaomi/cappu/sepolicy-mtk/bsp/non_plat \
        device/xiaomi/cappu/sepolicy-mt8173/basic \
        device/xiaomi/cappu/sepolicy-mt8173/bsp \
        device/xiaomi/cappu/sepolicy

SYSTEM_EXT_PUBLIC_SEPOLICY_DIRS += \
        device/xiaomi/cappu/sepolicy-mtk/basic/plat_public \
        device/xiaomi/cappu/sepolicy-mtk/bsp/plat_public

SYSTEM_EXT_PRIVATE_SEPOLICY_DIRS += \
        device/xiaomi/cappu/sepolicy-mtk/basic/plat_private \
        device/xiaomi/cappu/sepolicy-mtk/bsp/plat_private

# Shim Libraries
TARGET_LD_SHIM_LIBS := \
	/system/vendor/lib/libui_ext.so|libshim_ui.so \
	/system/vendor/lib64/libui_ext.so|libshim_ui.so \
	/system/vendor/lib/libui_ext.so|libcamera_shim.so \
	/system/vendor/lib64/libui_ext.so|libcamera_shim.so \
	/system/vendor/lib/libui_ext.so|libshim_showlogo.so \
	/system/vendor/lib64/libui_ext.so|libshim_showlogo.so \
	/system/vendor/lib/libui_ext.so|libmtk_symbols.so \
	/system/vendor/lib64/libui_ext.so|libmtk_symbols.so \
	/system/vendor/lib/libgui_ext.so|libshim_ui.so \
	/system/vendor/lib/libgui_ext.so|libmtk_symbols.so \
	/system/vendor/lib64/libgui_ext.so|libshim_ui.so \
	/system/vendor/lib64/libgui_ext.so|libmtk_symbols.so \
	/system/vendor/lib/hw/hwcomposer.mt8173.so|libshim_ui.so \
	/system/vendor/lib/hw/hwcomposer.mt8173.so|libmtk_symbols.so \
	/system/vendor/lib64/hw/hwcomposer.mt8173.so|libshim_ui.so \
	/system/vendor/lib64/hw/hwcomposer.mt8173.so|libmtk_symbols.so \
	/system/vendor/lib/hw/gralloc.mt8173.img.so|libshim_cappu_gralloc.so \
	/system/vendor/lib/libsrv_um.so|libshim_cappu_pvr.so \
	/system/vendor/lib64/libsrv_um.so|libshim_cappu_pvr.so \
	/system/vendor/lib64/hw/gralloc.mt8173.img.so|libshim_cappu_gralloc.so \
	/system/vendor/lib/hw/audio.primary.mt8173.so|libshim_cappu_audio.so \
	/system/vendor/lib64/hw/audio.primary.mt8173.so|libshim_cappu_audio.so \
	/system/vendor/lib/libMtkOmxVenc.so|libshim_ui.so \
	/system/vendor/lib/libMtkOmxVdecEx.so|libshim_cappu_omx.so \
	/system/vendor/lib/libMtkOmxVdecEx.so|libshim_ui.so \
	/system/vendor/lib64/libcam_utils.so|libshim_ui.so \
	/system/vendor/lib64/libcam_utils.so|libshim_cappu_camera.so \
	/system/vendor/lib64/libmtk_mmutils.so|libshim_ui.so \
	/system/vendor/lib64/libcam.camnode.so|libshim_ui.so \
	/system/vendor/lib64/libcam.client.so|libshim_ui.so \
	/system/vendor/lib64/libcam.utils.sensorlistener.so|libshim_cappu_camera.so \
	/system/vendor/lib64/libcam.paramsmgr.so|libshim_cappu_camera.so \
	/system/vendor/lib64/libcam.camadapter.so|libshim_cappu_camera.so \

# ThermalHAL
TARGET_THERMALHAL_VARIANT := mt8173

# Wifi: the tablet has a Broadcom BCM4356 (bcmdhd, built into the kernel), not a MediaTek combo chip
BOARD_WLAN_DEVICE := bcmdhd
WPA_SUPPLICANT_VERSION := VER_0_8_X
BOARD_HOSTAPD_DRIVER := NL80211
BOARD_HOSTAPD_PRIVATE_LIB := lib_driver_cmd_bcmdhd
BOARD_WPA_SUPPLICANT_DRIVER := NL80211
BOARD_WPA_SUPPLICANT_PRIVATE_LIB := lib_driver_cmd_bcmdhd
WIFI_DRIVER_FW_PATH_PARAM := "/sys/module/bcmdhd/parameters/firmware_path"
WIFI_DRIVER_FW_PATH_STA := "/system/etc/firmware/fw_bcmdhd.bin"
WIFI_DRIVER_FW_PATH_AP := "/system/etc/firmware/fw_bcmdhd_apsta.bin"
WIFI_DRIVER_FW_PATH_P2P := "/system/etc/firmware/fw_bcmdhd.bin"

# Zygote whitelist extra paths
ZYGOTE_WHITELIST_PATH_EXTRA := "/dev/pvr_sync","/dev/pvrsrvkm"

# Security patch level of the stock proprietary blob set
VENDOR_SECURITY_PATCH := 2018-12-01

# Inherit from the proprietary version
-include vendor/xiaomi/cappu/BoardConfigVendor.mk

# Mount points for the MTK partitions listed in fstab.mt8173
BOARD_ROOT_EXTRA_FOLDERS += protect_f protect_s nvdata cust persist

# Charging control HAL (vendor.lineage.health)
TARGET_HEALTH_CHARGING_CONTROL_CHARGING_PATH := /sys/devices/mediatek,battery_common/Charging_Enable
TARGET_HEALTH_CHARGING_CONTROL_CHARGING_ENABLED := 1
TARGET_HEALTH_CHARGING_CONTROL_CHARGING_DISABLED := 0
TARGET_HEALTH_CHARGING_CONTROL_SUPPORTS_TOGGLE := true
TARGET_HEALTH_CHARGING_CONTROL_SUPPORTS_BYPASS := false
TARGET_HEALTH_CHARGING_CONTROL_SUPPORTS_DEADLINE := false
