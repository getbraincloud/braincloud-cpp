#pragma once

#include <cstdint>
#include <vector>

// Not on Apple: CocoaPods frameworks need it exported.
#if (defined(__GNUC__) || defined(__clang__)) && !defined(__APPLE__)
#define BC_NATIVE_HIDDEN __attribute__((visibility("hidden")))
#else
#define BC_NATIVE_HIDDEN
#endif

// Called by the generated braincloud_tuning source compiled into the app.
extern "C" BC_NATIVE_HIDDEN void BrainCloud_Bootstrap(void (*fill)(unsigned char* out, unsigned long length));

namespace BrainCloud
{
    // False when the app has none.
    BC_NATIVE_HIDDEN bool readTuning(std::vector<uint8_t>& out);
}
