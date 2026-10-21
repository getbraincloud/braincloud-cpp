// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#ifndef TESTBCAPPCHECK_H
#define TESTBCAPPCHECK_H

#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"

namespace AppCheckTests
{
    class AppCheckClient : public BrainCloud::BrainCloudClient
    {
    public:
        AppCheckClient();
        const Json::Value& payload() const;
    };

    // Offline fixture: captures requests without authenticating against a server.
    class TestBCAppCheck : public testing::Test
    {
    protected:
        AppCheckClient client;

        std::string serializedPayload();
        std::string legacyPayload();
    };
}

#endif
