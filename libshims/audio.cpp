// Symbol the Android 7 MTK audio HAL imports that Android 12's libaudioclient no longer exports.
#include <stdint.h>

extern "C" {

// android::AudioSystem::getDeviceConnectionState(unsigned int, char const*)
// The HAL only uses this as a routing hint, and calling back into the audio policy service
// from inside the HAL is unsafe: report AUDIO_POLICY_DEVICE_STATE_UNAVAILABLE.
int _ZN7android11AudioSystem24getDeviceConnectionStateEjPKc(uint32_t /*device*/, const char* /*address*/) {
    return 0;
}

}  // extern "C"
