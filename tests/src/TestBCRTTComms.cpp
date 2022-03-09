#include <stdlib.h>
#include "TestBCRTTComms.h"
#include "braincloud/reason_codes.h"
#include "braincloud/http_codes.h"
#include "braincloud/IRTTCallback.h"

#include <chrono>
#include <thread>

#define RETRY_ENABLE_RTT_ON_FAIL 1

class RTTCallback final : public BrainCloud::IRTTCallback
{
public:
    RTTCallback(
        BrainCloud::BrainCloudClient* pBC,
        const std::string& expectedServiceName,
        const std::string& expectedOperation = "",
        int waitTimeSec = 30)
        : m_pBC(pBC)
        , m_expectedServiceName(expectedServiceName)
        , m_expectedOperation(expectedOperation)
        , m_waitTimeSec(waitTimeSec)
    {
    }

    void rttCallback(const std::string& jsonData) override
    {
        Json::Value eventJson;
        Json::Reader reader;
        reader.parse(jsonData, eventJson);

        if (eventJson["service"].asString() == m_expectedServiceName &&
            (eventJson["operation"].asString() == m_expectedOperation ||
             m_expectedOperation.empty()))

        {
            m_receivedCallback = true;
        }
    }

    bool receivedCallback()
    {
        // We wait 10 seconds
        auto startTime = std::chrono::steady_clock::now();
        while (
            std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime) <
            std::chrono::seconds(m_waitTimeSec))
        {
            m_pBC->runCallbacks();
            if (m_receivedCallback)
            {
                return true;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        printf("RTT receivedCallback timeout after %isec\n", m_waitTimeSec);
        return false; // Timed out
    }

    void reset()
    {
        m_receivedCallback = false;
    }

private:
    BrainCloud::BrainCloudClient* m_pBC;
    std::string m_expectedServiceName;
    std::string m_expectedOperation;
    int m_waitTimeSec;
    bool m_receivedCallback = false;
};

TEST_F(TestBCRTTComms, RTTRegisterWithoutEnableRTT)
{
    TestResult tr;

    // Register for RTT lobby
    RTTCallback rttCallback(m_bc, "event");
    m_bc->getRTTService()->registerRTTEventCallback(&rttCallback);
    
    // Create a lobby
    auto& profileId = m_bc->getAuthenticationService()->getProfileId();
    m_bc->getEventService()->sendEvent(
        profileId.c_str(), "test", "{\"testData\":42}", &tr);
    tr.run(m_bc);

    // Now check if we get the event
    // Wait 10sec. We shouldn't get it, because RTT is not enabled
    EXPECT_FALSE(rttCallback.receivedCallback());
}

TEST_F(TestBCRTTComms, RequestClientConnection)
{
    TestResult tr;

    m_bc->getRTTService()->requestClientConnection(&tr);
    tr.run(m_bc);
}

TEST_F(TestBCRTTComms, DISABLED_EnableDisableRTTWithTCP)
{
    TestResult tr;
    // disable, then re-enable. Should still succeed
    m_bc->getRTTService()->disableRTT();
    m_bc->getRTTService()->enableRTT(&tr, false);
    tr.run(m_bc);
}

TEST_F(TestBCRTTComms, DISABLED_EnableDisableRTTWithWS)
{
    TestResult tr;
    // disable, then re-enable. Should still succeed
    m_bc->getRTTService()->disableRTT();
    m_bc->getRTTService()->enableRTT(&tr, true);
    tr.run(m_bc);
}

TEST_F(TestBCRTTComms, RTTChatCallback)
{
    TestResult tr;

#if RETRY_ENABLE_RTT_ON_FAIL
    int tries = 3;
    while (tries > 0)
    {
        m_bc->getRTTService()->enableRTT(&tr, true);
        tr.run(m_bc, true);

        tries--;
        if (tr.m_result) break; // success
        EXPECT_TRUE(tries > 0);

        // Failed? Sleep 20sec then try again
        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
#else
    m_bc->getRTTService()->enableRTT(&tr, true);
    tr.run(m_bc);
#endif

    // Get channel Id
    m_bc->getChatService()->getChannelId("gl", "valid", &tr);
    tr.run(m_bc);
    std::string channelId = tr.m_response["data"]["channelId"].asString();

    // Connect to channel
    m_bc->getChatService()->channelConnect(channelId, 50, &tr);
    tr.run(m_bc);

    // Register for RTT chat
    RTTCallback rttCallback(m_bc, "chat", "INCOMING");
    m_bc->getRTTService()->registerRTTChatCallback(&rttCallback);

    // Send a chat message
    m_bc->getChatService()->postChatMessageSimple(channelId, 
        "Unit test message", true, &tr);
    tr.run(m_bc);

    // Now check if we get the chat message
    EXPECT_TRUE(rttCallback.receivedCallback());
    rttCallback.reset();

    // Now deregister and make sure we don't receive it
    m_bc->getRTTService()->deregisterRTTChatCallback();

    // Send a chat message again
    m_bc->getChatService()->postChatMessageSimple(channelId, 
        "Unit test message 2", true, &tr);
    tr.run(m_bc);

    // Wait 10sec and make sure we don't get the event
    EXPECT_FALSE(rttCallback.receivedCallback());
}

TEST_F(TestBCRTTComms, RTTLobbyCallback)
{
    TestResult tr;

#if RETRY_ENABLE_RTT_ON_FAIL
    int tries = 3;
    while (tries > 0)
    {
        m_bc->getRTTService()->enableRTT(&tr, true);
        tr.run(m_bc);

        tries--;
        if (tr.m_result) break; // success
        EXPECT_TRUE(tries > 0);

        // Failed? Sleep 20sec then try again
        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
#else
    m_bc->getRTTService()->enableRTT(&tr, true);
    tr.run(m_bc);
#endif

    // Register for RTT lobby
    RTTCallback rttCallback(m_bc, "lobby", "", 60);
    m_bc->getRTTService()->registerRTTLobbyCallback(&rttCallback);

    // Create a lobby
    std::vector<std::string> otherUserCxIds;
    m_bc->getLobbyService()->createLobby("MATCH_UNRANKED", 0, otherUserCxIds, false, "{}", "all", "{}", &tr);
    tr.run(m_bc);

    // Now check if we get the chat message
    // Wait 60sec, creating lobby can take time
    EXPECT_TRUE(rttCallback.receivedCallback());
}

TEST_F(TestBCRTTComms, RTTEventCallback)
{
    TestResult tr;

#if RETRY_ENABLE_RTT_ON_FAIL
    int tries = 3;
    while (tries > 0)
    {
        m_bc->getRTTService()->enableRTT(&tr, true);
        tr.run(m_bc);

        tries--;
        if (tr.m_result) break; // success
        EXPECT_TRUE(tries > 0);

        // Failed? Sleep 20sec then try again
        std::this_thread::sleep_for(std::chrono::seconds(20));
    }
#else
    m_bc->getRTTService()->enableRTT(&tr, true);
    tr.run(m_bc);
#endif

    // Register for RTT lobby
    RTTCallback rttCallback(m_bc, "event");
    m_bc->getRTTService()->registerRTTEventCallback(&rttCallback);

    // Create a lobby
    auto& profileId = m_bc->getAuthenticationService()->getProfileId();
    m_bc->getEventService()->sendEvent(
        profileId.c_str(), "test", "{\"testData\":42}", &tr);
    tr.run(m_bc);

    // Now check if we get the chat message
    // Wait 60sec, creating lobby can take time
    EXPECT_TRUE(rttCallback.receivedCallback());
}

