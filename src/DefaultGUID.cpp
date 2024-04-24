#include "braincloud/internal/GUID.h"

#include <stdlib.h>

namespace BrainCloud
{
    std::string GUID::generateGUID()
    {
        srand(time(0)); // should only call one time

        unsigned char buffer[16] = { 0 };

        // This is not great. See BCLD-3069
        for (int i = 0; i < 16; ++i)
        {
            buffer[i] = static_cast<unsigned char>(rand()/static_cast<float>(RAND_MAX) * 256.0f);
        }

        return GUIDToString(buffer);
    }
}
