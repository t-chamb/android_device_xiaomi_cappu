// Symbols the Android 7 MTK graphics blobs (hwcomposer, libui_ext, libgui_ext) import
// that no longer exist in Android 12's libui/libgui.
#include <binder/IBinder.h>
#include <gui/SurfaceComposerClient.h>
#include <ui/GraphicBuffer.h>
#include <utils/Errors.h>

#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"

extern "C" {

// android::GraphicBuffer::lock(unsigned int, void**)
android::status_t _ZN7android13GraphicBuffer4lockEjPPv(android::GraphicBuffer* thiz, uint32_t usage,
                                                       void** vaddr) {
    return thiz->lock(usage, vaddr, nullptr, nullptr);
}

// android::SurfaceComposerClient::getBuiltInDisplay(int)
android::sp<android::IBinder> _ZN7android21SurfaceComposerClient17getBuiltInDisplayEi(int /*id*/) {
    return android::SurfaceComposerClient::getInternalDisplayToken();
}

// android::SurfaceComposerClient::getDisplayInfo(sp<IBinder> const&, DisplayInfo*)
// The DisplayInfo struct is gone; report failure rather than fill a layout we can't match.
android::status_t
_ZN7android21SurfaceComposerClient14getDisplayInfoERKNS_2spINS_7IBinderEEEPNS_11DisplayInfoE(
        const android::sp<android::IBinder>& /*display*/, void* /*info*/) {
    return android::NAME_NOT_FOUND;
}

}  // extern "C"
