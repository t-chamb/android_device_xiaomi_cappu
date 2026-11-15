LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := ui/GraphicBuffer.cpp ui/LegacyGui.cpp
LOCAL_SHARED_LIBRARIES := \
    libui \
    libgui \
    libbinder \
    libutils \
    libcutils \
    libc++

LOCAL_MODULE := libshim_ui
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_PROPRIETARY_MODULE := true
LOCAL_ALLOW_UNDEFINED_SYMBOLS := true
LOCAL_LDFLAGS += -Wl,--export-dynamic
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := libshim_showlogo.cpp
LOCAL_MODULE := libshim_showlogo
LOCAL_SHARED_LIBRARIES := libgui libutils
LOCAL_MODULE_TAGS := optional
#LOCAL_VENDOR_MODULE := true
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := camera.cpp
LOCAL_MODULE := libcamera_shim
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := audio.cpp
LOCAL_MODULE := libshim_cappu_audio
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := gralloc_subchannel.c
LOCAL_MODULE := libshim_cappu_gralloc
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := camera_legacy.cpp
LOCAL_MODULE := libshim_cappu_camera
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
LOCAL_SHARED_LIBRARIES := libui libsensor libinput libutilscallstack libutils
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := skia_stub.c
LOCAL_MODULE := libskia_cappu_stub
LOCAL_INSTALLED_MODULE_STEM := libskia.so
LOCAL_LDFLAGS := -Wl,-soname,libskia.so
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
LOCAL_MULTILIB := 64
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := omx_guiext.cpp
LOCAL_MODULE := libshim_cappu_omx
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true
include $(BUILD_SHARED_LIBRARY)
