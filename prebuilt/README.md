# Prebuilt kernel

`Image.gz-dtb` is the stock-derived 3.18 kernel, used through
`TARGET_FORCE_PREBUILT_KERNEL` because the clang-built kernel from the
Android build never boots on this board.

How it was produced:

- source: branch `cappu-lineage-19.1-ddk19` of
  https://github.com/t-chamb/android_kernel_xiaomi_cappu: commit `a3149ff2c`
  (the tree before the clang build changes), the defconfig changes and the
  four fixes below (branch `cappu-lineage-19.1-gcc`, one commit each), and the
  PowerVR DDK 1.9 GPU driver described at the end
- compiler: GCC 4.9 from the LineageOS 19.1 prebuilts
  (`aarch64-linux-android-4.9`, `arm-linux-androideabi-4.9`):
  `make O=out ARCH=arm64 CROSS_COMPILE=aarch64-linux-android- CROSS_COMPILE_ARM32=arm-linux-androidkernel- cappu_defconfig Image.gz-dtb`
- defconfig changes on top of `a3149ff2c`: `cappu_defconfig.diff`
  - `RT_GROUP_SCHED` off: with it, real-time timers of unprivileged groups
    are blocked (Bluetooth aborted)
  - `CPUSETS` off: MediaTek hotplug keeps only CPU0 online at boot and the
    3.18 cpuset code never re-adds cores, pinning every app to CPU0
  - `SDCARD_FS` off: sdcardfs oopses in `show_vfsmnt` shortly after boot
- source fixes on top of `a3149ff2c`, also kept here as patches (`patch -p1`):
  - `0001-random-do-not-block-dev-random-once-seeded.patch`: `/dev/random`
    and `getrandom(GRND_RANDOM)` read from the non-blocking pool once it is
    seeded, as mainline does since 5.6. The board has no hardware RNG and an
    idle system credits about one bit of entropy per second, so anything
    seeding itself from `/dev/random` (mbedTLS in the Sonos app) stalled for
    minutes while holding the dynamic linker lock.
  - `0002-mtk-uart-fix-four-ways-the-Bluetooth-port-lost-data.patch`:
    Bluetooth failed to start about one time in three and its stack aborted
    with HCI command timeouts. Four bugs in the MediaTek UART driver:
    1. `old_status` (last CTS level) is not refreshed while the port is
       closed; when the status interrupt for the low level at open was not
       seen, the later rise showed no change and the serial core's
       `hw_stopped` was never released. CTS is now compared with the core's
       flow state as well.
    2. `start_tx` did nothing when the holding register was still occupied,
       and `stop_tx` had switched the transmit interrupt off, so nothing
       restarted transmission. It now re-arms the interrupt.
    3. The interrupt handler ran on another CPU while `set_termios` had LCR
       switched to the divisor-latch/EFR view, where the IER address is the
       divisor latch: its read-modify-write of "IER" wrote 0 into the real
       IER, disabling receive and modem status interrupts. The interrupt
       line is now disabled for the duration of `set_termios`.
    4. A command was sent in the same 100 us in which `dev_wake` was raised;
       the chip asserts CTS at once but drops data until it has woken up
       (the lost command was the HCI timeout). Transmission now waits 15 ms
       after waking the chip, like mainline's `hci_bcm`.
  - `0003-audit-do-not-start-disabled.patch`: Xiaomi's tree initialises
    `audit_initialized` to `AUDIT_DISABLED`, so the kernel never logged a
    single SELinux denial. Needed to write policy.
  - `0004-selinux-remove-the-forced-permissive-hack.patch`: the community
    kernel this port started from overwrote every write to
    `/sys/fs/selinux/enforce` with 0 ("Forced Permissive", commit 2b8d67240).
- `NETFILTER_XT_MATCH_OWNER` must stay unset: it hides
  `NETFILTER_XT_MATCH_QTAGUID`, which provides the same `owner` match plus
  the per-UID accounting in `/proc/net/xt_qtaguid` that netd and
  NetworkStatsService use on a kernel without eBPF.


## PowerVR DDK 1.9

The GPU kernel driver is DDK 1.9@4893595 (`drivers/misc/mediatek/gpu/mt8173/m1.9ED4893595`)
together with the matching GED driver, both taken from the MT8173 tree at
https://github.com/iiowoii3389/android_kernel_xiaomi_cappu (branch `lineage-15.1-test`). It pairs
with the DDK 1.9 user-space driver in the vendor tree (`vendor/xiaomi/cappu/import-ddk19.py`).
Compared with DDK 1.7 it runs WebGL content that faulted the GPU, and its Vulkan driver loads.
The firmware is loaded at boot from `/vendor/firmware/rgx.fw.signed`.
