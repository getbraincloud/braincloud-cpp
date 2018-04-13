#include "braincloud/internal/BrainCloudComms.h"

namespace BrainCloud
{
    IBrainCloudComms* IBrainCloudComms::create(BrainCloudClient* in_client)
    {
        return new BrainCloudComms(in_client);
    }
}
