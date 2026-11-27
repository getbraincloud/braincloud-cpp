#include "braincloud/internal/NativeTuning.h"

namespace
{
    void (*s_fill)(unsigned char*, unsigned long) = 0;
}

extern "C" void BrainCloud_Bootstrap(void (*fill)(unsigned char* out, unsigned long length))
{
    s_fill = fill;
}

namespace BrainCloud
{
    bool readTuning(std::vector<uint8_t>& out)
    {
        if (s_fill == 0)
            return false;
        out.assign(32, 0);
        s_fill(&out[0], static_cast<unsigned long>(out.size()));
        return true;
    }
}
