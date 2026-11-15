#include <new>

#include <ui/GraphicBuffer.h>
#include <ui/GraphicBufferMapper.h>
#include <ui/Rect.h>
#include <string>

__attribute__((visibility("default")))
extern "C" {
    void _ZN7android19GraphicBufferMapper4lockEPK13native_handleiRKNS_4RectEPPv(
        buffer_handle_t handle,
        int usage,
        const android::Rect& bounds,
        void** vaddr) {
        android::GraphicBufferMapper::get().lock(
            handle,
            static_cast<uint32_t>(usage),
            bounds,
            vaddr);
    }

    __attribute__((weak))
    void _ZN7android19GraphicBufferMapper4lockEPK13native_handlejRKNS_4RectEPPv(
        buffer_handle_t handle,
        uint32_t usage,
        const android::Rect& bounds,
        void** vaddr) {
        android::GraphicBufferMapper::get().lock(handle, usage, bounds, vaddr);
    }

    void _ZN7android19GraphicBufferMapper9lockYCbCrEPK13native_handleiRKNS_4RectEP13android_ycbcr(
        buffer_handle_t handle,
        int usage,
        const android::Rect& bounds,
        android_ycbcr* ycbcr) {
        android::GraphicBufferMapper::get().lockYCbCr(
            handle,
            static_cast<uint32_t>(usage),
            bounds,
            ycbcr);
    }

    // lockYCbCr(handle, uint32_t usage, ...) still exists in libui with the same
    // signature; defining it here would make the shim call itself forever.

    // GraphicBuffer(w, h, format, usage): the requestor-name argument was added later.
    // The caller allocated the object, so construct the current class in place.
    // (64-bit blobs are patched to allocate the current object size, see
    // vendor/xiaomi/cappu/patch-graphicbuffer-size.py.)
    void _ZN7android13GraphicBufferC1Ejjij(
        void* pthis,
        uint32_t inWidth,
        uint32_t inHeight,
        int inFormat,
        uint32_t inUsage) {
        new (pthis) android::GraphicBuffer(inWidth, inHeight, inFormat, inUsage, "<mtk-blob>");
    }
}
