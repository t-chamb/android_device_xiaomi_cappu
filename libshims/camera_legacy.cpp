/*
 * Symbols the Android 7 MediaTek camera HAL expects from platform libraries
 * that changed signature (or moved) by Android 12.
 *
 * PropertyMap (now in libinput) and CallStack (now in libutilscallstack) are
 * satisfied by linking this shim against those libraries.
 */

#include <new>

#include <sensor/SensorEventQueue.h>
#include <sensor/SensorManager.h>
#include <ui/GraphicBuffer.h>
#include <utils/String16.h>
#include <utils/String8.h>

namespace android {

// SensorManager::createEventQueue(String8, int) gained an attribution tag.
sp<SensorEventQueue> shimCreateEventQueue(SensorManager* self, String8 packageName, int mode)
        __asm__("_ZN7android13SensorManager16createEventQueueENS_7String8Ei");

__attribute__((visibility("default")))
sp<SensorEventQueue> shimCreateEventQueue(SensorManager* self, String8 packageName, int mode) {
    return self->createEventQueue(packageName, mode, String16(""));
}

}  // namespace android

extern "C" {

// GraphicBuffer(w, h, format, usage, stride, handle, keepOwnership): layerCount was inserted.
__attribute__((visibility("default")))
void _ZN7android13GraphicBufferC1EjjijjP13native_handleb(void* self, uint32_t width,
        uint32_t height, int format, uint32_t usage, uint32_t stride, native_handle_t* handle,
        bool keepOwnership) {
    new (self) android::GraphicBuffer(width, height, format, 1 /* layerCount */, usage, stride,
                                      handle, keepOwnership);
}

// Xiaomi addition to CameraParameters in the stock libcamera_client.
__attribute__((visibility("default")))
extern const char _ZN7android16CameraParameters13KEY_JPEG_FLIPE[] = "jpeg-flip";

}  // extern "C"
