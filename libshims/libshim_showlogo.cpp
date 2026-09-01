#include <gui/SurfaceComposerClient.h>

namespace android {
    // Android 12 createSurface: parent is now sp<IBinder> const& (was SurfaceControl*)
    // and a trailing uint32_t* outTransformHint was added. Declared void (like the
    // other forwarders) to keep C linkage without a return-type-c-linkage warning.
    extern "C" void _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijRKNS_2spINS_7IBinderEEENS_13LayerMetadataEPj(const String8& name, uint32_t w, uint32_t h, PixelFormat format, uint32_t flags, const sp<IBinder>& parent, android::LayerMetadata metadata, uint32_t* outTransformHint);

    // Old ABI symbol libui_ext.so expects: createSurface(..., SurfaceControl* parent, int windowType, int ownerUid)
    extern "C" void _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijPNS_14SurfaceControlEii(const String8& name, uint32_t w, uint32_t h, PixelFormat format, uint32_t flags, SurfaceControl* parent, int32_t windowType, int32_t ownerUid) {
        (void) parent; (void) windowType; (void) ownerUid;
        android::LayerMetadata metadata;
        _ZN7android21SurfaceComposerClient13createSurfaceERKNS_7String8EjjijRKNS_2spINS_7IBinderEEENS_13LayerMetadataEPj(name, w, h, format, flags, nullptr, metadata, nullptr);
    }

    extern "C" void _ZN7android21SurfaceComposerClient23getInternalDisplayTokenEv(void);

    // Old ABI symbol: getBuiltInDisplay(int id) -> forward to getInternalDisplayToken()
    extern "C" void _ZN7android21SurfaceComposerClient17getBuiltInDisplayEi(int32_t id){
        (void) id;
        _ZN7android21SurfaceComposerClient23getInternalDisplayTokenEv();
    }

    // Android 12 SurfaceControl has no exported destroy(); disconnect() is the teardown.
    extern "C" void _ZN7android14SurfaceControl10disconnectEv(void);

    // Old ABI symbol: SurfaceControl::clear() -> forward to disconnect()
    extern "C" void _ZN7android14SurfaceControl5clearEv(void){
        _ZN7android14SurfaceControl10disconnectEv();
    }

    extern "C" void _ZN7android4base10StartsWithERKNSt3__112basic_stringIcNS1_11char_traitsIcEENS1_9allocatorIcEEEEPKc() { }

}
