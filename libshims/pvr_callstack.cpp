/*
 * PowerVR DDK 1.9 (libsrv_um, gralloc.mt8173) imports android::CallStack, which Android 12
 * moved from libutils to libutilscallstack. The driver only uses it to print a backtrace in
 * debug paths; these do-nothing versions let it load in every process that draws, where
 * libutilscallstack is not necessarily visible.
 */
#include <utils/String8.h>

namespace android {

class CallStack {
  public:
    CallStack();
    CallStack(const char* logtag, int32_t ignoreDepth);
    ~CallStack();
    void update(int32_t ignoreDepth, pid_t tid);
    String8 toString(const char* prefix) const;
};

__attribute__((visibility("default"))) CallStack::CallStack() {}
__attribute__((visibility("default"))) CallStack::CallStack(const char*, int32_t) {}
__attribute__((visibility("default"))) CallStack::~CallStack() {}
__attribute__((visibility("default"))) void CallStack::update(int32_t, pid_t) {}
__attribute__((visibility("default"))) String8 CallStack::toString(const char*) const {
    return String8();
}

}  // namespace android
