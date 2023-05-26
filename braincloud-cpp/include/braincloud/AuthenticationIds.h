// Copyright 2022 bitHeads, Inc. All Rights Reserved.

#pragma once

#include <string>

namespace BrainCloud
{
    struct AuthenticationIds
    {
        std::string externalId;
        std::string authenticationToken;
        std::string authenticationSubType; // Empty string for most auth types
    };
};
