/*
 * The MediaTek OMX video decoder asks "GuiExtService" for a conversion buffer
 * pool whenever it sets up output buffers.  Stock firmware hosted that service
 * inside its own SurfaceFlinger; here nothing implements it and the request
 * never returns, which stalls every hardware decode.  The decoder already
 * treats a failed request as "no pool" and carries on, so fail it at once.
 */

#include <errno.h>
#include <stdint.h>

extern "C" {

// android::GuiExtClientProducer::alloc(uint32_t usage, uint32_t w, uint32_t h, uint32_t* poolId)
__attribute__((visibility("default")))
int _ZN7android20GuiExtClientProducer5allocEjjjPj(void* /*self*/, uint32_t /*usage*/,
        uint32_t /*width*/, uint32_t /*height*/, uint32_t* poolId) {
    if (poolId != nullptr) {
        *poolId = 0;
    }
    return -ENOSYS;
}

// android::GuiExtClientProducer::free(uint32_t poolId)
__attribute__((visibility("default")))
int _ZN7android20GuiExtClientProducer4freeEj(void* /*self*/, uint32_t /*poolId*/) {
    return 0;
}

}  // extern "C"
