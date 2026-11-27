#pragma once

#include <cstddef>
#include <cstdint>

namespace BrainCloud
{
    // Fills the buffer from the OS CSPRNG. Returns false if it failed; the buffer is then unusable.
    bool secureRandomBytes(uint8_t* out, size_t length);
}
