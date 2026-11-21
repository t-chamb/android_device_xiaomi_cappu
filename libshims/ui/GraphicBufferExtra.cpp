/*
 * libui_ext (Android 7) uses the C++ wrapper android::GraphicBufferExtra from libgralloc_extra.
 * The newer libgralloc_extra that comes with PowerVR DDK 1.9 keeps buffer metadata in GED and
 * only exports the C entry points, so provide the four symbols libui_ext imports on top of
 * gralloc_extra_query(). The wrapper object holds no state.
 */
#include <pthread.h>
#include <stdint.h>

struct native_handle;

extern "C" int gralloc_extra_query(const native_handle* handle, int attr, void* out);

extern "C" {

// android::GraphicBufferExtra::GraphicBufferExtra()
__attribute__((visibility("default"))) void _ZN7android18GraphicBufferExtraC1Ev(void* self) {
    (void)self;
}

// android::GraphicBufferExtra::query(native_handle const*, GRALLOC_EXTRA_ATTRIBUTE_QUERY, void*)
__attribute__((visibility("default"))) int
_ZN7android18GraphicBufferExtra5queryEPK13native_handle29GRALLOC_EXTRA_ATTRIBUTE_QUERYPv(
        void* self, const native_handle* handle, int attr, void* out) {
    (void)self;
    return gralloc_extra_query(handle, attr, out);
}

// android::Singleton<android::GraphicBufferExtra>::sLock / ::sInstance, used by the inline
// getInstance() compiled into libui_ext. A zeroed bionic mutex is an initialized mutex.
__attribute__((visibility("default"))) pthread_mutex_t
        _ZN7android9SingletonINS_18GraphicBufferExtraEE5sLockE = PTHREAD_MUTEX_INITIALIZER;
__attribute__((visibility("default"))) void* _ZN7android9SingletonINS_18GraphicBufferExtraEE9sInstanceE =
        nullptr;

}  // extern "C"
