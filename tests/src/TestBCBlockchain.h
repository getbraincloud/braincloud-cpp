#pragma once

#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestFixtureBase.h"
#include "json/json.h"

using namespace BrainCloud;

class TestBCBlockchain : public TestFixtureBase
{
protected:
    const std::string _defaultIntegrationId = "default";
    const std::string _defaultContextJson = "{}";
public:

};
