#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace BrainCloud
{
    // A wipe the compiler can't drop as a dead store.
    inline void secureWipe(void* data, size_t length)
    {
        volatile uint8_t* p = static_cast<volatile uint8_t*>(data);
        while (length--)
            *p++ = 0;
    }

    inline void secureWipe(std::vector<uint8_t>& bytes)
    {
        if (!bytes.empty())
            secureWipe(&bytes[0], bytes.size());
    }

    inline void secureWipe(std::string& text)
    {
        if (!text.empty())
            secureWipe(&text[0], text.size());
    }
}
