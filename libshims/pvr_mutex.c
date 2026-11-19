/*
 * PowerVR DDK 1.7 (libsrv_um) destroys one of its locks when a GL context is torn down and
 * locks it again on the next teardown (glFinish from hwui's destroyRenderingContext). Android 7
 * returned EBUSY for that; since Android 9 bionic aborts the process for callers targeting a
 * current SDK ("pthread_mutex_lock called on a destroyed mutex"). That killed system_server
 * after it had drawn a dialog, and apps whose cached process was trimmed twice.
 *
 * Interpose pthread_mutex_destroy for libsrv_um so its locks are never marked destroyed. A
 * bionic mutex owns no resources, so nothing leaks.
 */
#include <pthread.h>

int pthread_mutex_destroy(pthread_mutex_t* mutex) {
    (void)mutex;
    return 0;
}
