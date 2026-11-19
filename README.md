# Xiaomi Mi Pad 3 (cappu) on LineageOS 19.1

Device tree for the Mi Pad 3 (MT8176, Wi-Fi only) running LineageOS 19.1 (Android 12) on the
stock Android 7.0 vendor blobs and the stock-derived 3.18 kernel.

Working: display, touch, Wi-Fi, Bluetooth, sensors, audio playback and capture, both cameras,
hardware video decode and encode, DRM (Widevine L3, as on stock), charging control, SELinux
enforcing. The tablet has no GPS, and the stock GPU driver has no usable Vulkan.

## Building

```
repo init -u https://github.com/LineageOS/android.git -b lineage-19.1 --git-lfs
mkdir -p .repo/local_manifests
curl -o .repo/local_manifests/cappu.xml \
    https://raw.githubusercontent.com/t-chamb/android_device_xiaomi_cappu/cappu-lineage-19.1-port/manifests/cappu.xml
repo sync
source build/envsetup.sh
lunch lineage_cappu-userdebug
mka systemimage bootimage recoveryimage
```

`manifests/cappu.xml` adds this tree, the vendor blobs and the kernel, and replaces twelve
platform repositories with forks whose `cappu-lineage-19.1-port` branch carries the changes the
Android 7 blobs and the 3.18 kernel need (no eBPF, pre-Treble HALs, older binder and ext4).

The build host needs `libncurses.so.5` and `libtinfo.so.5` for the legacy RenderScript compiler.

The kernel is shipped as a prebuilt image because the clang build from the Android tree does not
boot; `prebuilt/README.md` says how it is built and what was fixed in it.

## Notes from the LineageOS 14.1 tree this port started from

This Device Tree is used to build LineageOS 14.1. Currently, the camera and video decoding are not working, while other hardware functions properly.

Vendor:https://github.com/iiowoii3389/android_vendor_xiaomi_cappu

Kernel:https://github.com/iiowoii3389/android_kernel_xiaomi_cappu

Basic        | Spec Sheet
------------:|:------------------------
CPU          | Cortex-A72 & Cortex-A53 | Hexa-Core | MT8176
GPU          | PowerVR GX6250
Memory       | 4GB RAM
Shipped Android Version | 7.0
Storage      | 64GB
Battery      | 6600 mAh Li-Po
Display      | 7.9
Rear Camera  | 13.0 MP , Video
Front Camera | 5.0 MP

Fix (01-01 00:23:14.484   443   443 E HAL     : dlopen failed: cannot locate symbol "_ZN7android13GraphicBufferC1Ejjij" referenced by "/system/vendor/lib64/libui_ext.so"...)
```diff
--- GraphicBuffer_a.cpp	2026-02-26 14:26:07.580471302 +0800
+++ GraphicBuffer.cpp	2026-02-25 00:27:12.030013159 +0800
@@ -19,6 +19,7 @@
 #include <stdlib.h>
 #include <stdint.h>
 #include <sys/types.h>
+#include <new>
 
 #include <utils/Errors.h>
 #include <utils/Log.h>
@@ -415,4 +416,25 @@
 
 // ---------------------------------------------------------------------------
 
+extern "C" {
+    __attribute__((used, visibility("default")))
+    void _ZN7android13GraphicBufferC1Ejjij(
+            void* thisptr,
+            uint32_t width,
+            uint32_t height,
+            int32_t format,
+            uint32_t usage) {
+        new (thisptr) android::GraphicBuffer(
+            width,
+            height,
+            static_cast<android::PixelFormat>(format),
+            usage,
+            0,          
+            nullptr,    
+            false       
+        );
+    }
+}
+
 }; // namespace android
