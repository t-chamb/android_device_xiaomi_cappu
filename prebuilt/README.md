# Prebuilt kernel

`Image.gz-dtb` is the stock-derived 3.18 kernel, used through
`TARGET_FORCE_PREBUILT_KERNEL` because the clang-built kernel from the
Android build never boots on this board.

How it was produced:

- source: `kernel/xiaomi/cappu` at commit `a3149ff2c` (the tree before the
  LineageOS 19.1 port changes)
- compiler: GCC 4.9 (`aarch64-linux-android-4.9`), `cappu_defconfig`
- defconfig changes on top of that commit: `cappu_defconfig.diff`
  - `RT_GROUP_SCHED` off: with it, real-time timers of unprivileged groups
    are blocked (Bluetooth aborted)
  - `CPUSETS` off: MediaTek hotplug keeps only CPU0 online at boot and the
    3.18 cpuset code never re-adds cores, pinning every app to CPU0
  - `SDCARD_FS` off: sdcardfs oopses in `show_vfsmnt` shortly after boot
  - `NETFILTER_XT_MATCH_OWNER` on: needed by netd's firewall rules
