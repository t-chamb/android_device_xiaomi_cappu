/*
 * The IMG gralloc blob starts a "subchannel" thread in every process that loads it. The thread
 * select()s on a pipe whose write end is meant to live in the compositor, then peeks at it with
 * tee(). On Android 12 nothing keeps that write end open, so the pipe is permanently at EOF:
 * select() reports it readable, tee() returns 0, and the blob loops without sleeping, burning
 * most of a CPU core per graphical process.
 *
 * Interpose tee() for the gralloc library: when it reports EOF/no data, back off so the loop
 * idles. The thread still wakes through its wakeup pipe, so shutdown keeps working.
 */
#define _GNU_SOURCE
#include <fcntl.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

ssize_t tee(int fd_in, int fd_out, size_t len, unsigned int flags) {
    ssize_t ret = syscall(__NR_tee, fd_in, fd_out, len, flags);
    if (ret == 0) {
        struct timespec ts = {0, 250 * 1000 * 1000};
        nanosleep(&ts, NULL);
    }
    return ret;
}
