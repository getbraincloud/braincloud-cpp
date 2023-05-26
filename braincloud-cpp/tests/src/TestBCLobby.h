#ifndef _TESTBCLOBBY_H_
#define _TESTBCLOBBY_H_

#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestFixtureBase.h"
#include "json/json.h"

using namespace BrainCloud;

class TestBCLobby : public TestFixtureBase
{
public:
};

class TestBCLobbyNoAuth : public TestFixtureBase
{
public:
    virtual bool ShouldSkipAuthenticate() override
    {
        return true;
    }
};

#endif /* _TESTBCLOBBY_H_ */
