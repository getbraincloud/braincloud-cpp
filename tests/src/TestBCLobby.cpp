#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/http_codes.h"
#include "braincloud/reason_codes.h"
#include "braincloud/IRTTCallback.h"
#include "TestResult.h"
#include "TestBCLobby.h"
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace BrainCloud;

TEST_F(TestBCLobby, FindLobby)
{
	TestResult tr;

	std::vector<std::string> otherUserCxIds;
	m_bc->getLobbyService()->findLobby("MATCH_UNRANKED", 0, 1, "{\"strategy\":\"ranged-absolute\",\"alignment\":\"center\",\"ranges\":[1000]}", "{}", otherUserCxIds, true, "{}", "all", &tr);

	tr.run(m_bc);
}

class TestBCLobbyRTTCallback final : public IRTTCallback
{
public:
	TestBCLobbyRTTCallback(const std::function<void(const Json::Value&)>& callback)
		: m_callback(callback)
	{
	}

	void rttCallback(const std::string& jsonData) override
	{
		std::stringstream ss(jsonData);
		Json::Value json;
		ss >> json;
		m_callback(json);
	}

private:
	std::function<void(const Json::Value&)> m_callback;
};

TEST_F(TestBCLobbyNoAuth, CreateAndJoinLobby)
{
	std::mutex mutex;
	std::condition_variable cond;
	std::atomic_bool userBConnected(false);

	std::thread userAThread([this, &mutex, &cond, &userBConnected]
	{
		bool BJoined = false;

		// SetUp
		TestResult tr;
		BrainCloudWrapper *wrapper = new BrainCloudWrapper("bctests_userA");
		wrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "", "");
		auto bc = wrapper->getBCClient();
		bc->enableLogging(true);
		bc->getAuthenticationService()->authenticateUniversal(GetUser(UserA)->m_id, GetUser(UserA)->m_password, true, &tr);
		tr.run(bc);

		// Wait for the other user to connect to RTT (Put that at 60sec timeout)
		// because we will send him an RTT event after we created the lobby
		std::unique_lock<std::mutex> lock(mutex);
		cond.wait_until(lock, std::chrono::steady_clock::now() + std::chrono::seconds(60));
		if (!userBConnected)
		{
			GTEST_NONFATAL_FAILURE_("timeout 60 sec while waiting for userB to connect to RTT\n");
			return;
		}

		// User A will create the lobby
		bc->getRTTService()->enableRTT(&tr);
		tr.run(bc);

		TestBCLobbyRTTCallback rttCallback([this, bc, &BJoined](const Json::Value &message)
		{
			if (message["service"] == "lobby" && 
				message["operation"] == "MEMBER_JOIN")
			{
				auto profileId = message["data"]["member"]["profileId"].asString();
				if (profileId == GetUser(UserA)->m_profileId)
				{
					auto lobbyId = message["data"]["lobbyId"].asString();
					bc->getEventService()->sendEvent(GetUser(UserB)->m_profileId, "lobbyid", "{\"lobbyId\":\"" + lobbyId + "\"}");
				}
				else if (profileId == GetUser(UserB)->m_profileId)
				{
					// Success! He joinned our game
					printf("B joined our game!\n");
					BJoined = true;
				}
			}
		});
		bc->getRTTService()->registerRTTLobbyCallback(&rttCallback);

		bc->getLobbyService()->createLobby("JoinLobby", 0, {}, false, "{}", "all", "{}", &tr);
		tr.run(bc);

		// Main loop
		auto timeBefore = std::chrono::steady_clock::now();
		while (!BJoined)
		{
			// Timeout so we don't infinite loop. 60sec seems fair for this test
			auto timeNow = std::chrono::steady_clock::now();
			if (timeNow - timeBefore > std::chrono::seconds(60))
			{
				// Timeout
				GTEST_NONFATAL_FAILURE_("timeout 60 sec while waiting for UserB to join the game\n");
				return;
			}

			bc->runCallbacks();

			// Sleep a bit, simulate a game running at 60 fps
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		// TearDown
		bc->getPlayerStateService()->logout(&tr);
		tr.run(bc);
		delete wrapper;
	});

	std::thread userBThread([this, &cond, &userBConnected]
	{
		bool joined = false;

		// SetUp
		TestResult tr;
		BrainCloudWrapper *wrapper = new BrainCloudWrapper("bctests_userB");
		wrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "", "");
		auto bc = wrapper->getBCClient();
		bc->enableLogging(true);
		bc->getAuthenticationService()->authenticateUniversal(GetUser(UserB)->m_id, GetUser(UserB)->m_password, true, &tr);
		tr.run(bc);

		bc->getRTTService()->enableRTT(&tr);
		tr.run(bc);

		TestBCLobbyRTTCallback rttCallback([this, bc, &tr, &joined](const Json::Value &message)
		{
			auto service = message["service"].asString();
			auto operation = message["operation"].asString();

			if (service == "event" && operation == "GET_EVENTS")
			{
				if (message["data"]["eventType"] == "lobbyid")
				{
					auto lobbyId = message["data"]["eventData"]["lobbyId"].asString();

					bc->getLobbyService()->joinLobby(lobbyId, false, "{}", "all", {}, &tr);
					tr.run(bc);
				}
			}
			else if (service == "lobby" && operation == "MEMBER_JOIN")
			{
				auto profileId = message["data"]["member"]["profileId"].asString();
				if (profileId == GetUser(UserB)->m_profileId)
				{
					// Success! We joinned his game
					printf("We joined A's game!\n");
					joined = true;
				}
			}
		});
		bc->getRTTService()->registerRTTLobbyCallback(&rttCallback);
		bc->getRTTService()->registerRTTEventCallback(&rttCallback);

		userBConnected = true;
		cond.notify_one();

		// Main loop
		auto timeBefore = std::chrono::steady_clock::now();
		while (!joined)
		{
			// Timeout so we don't infinite loop. 120sec seems fair for this test
			auto timeNow = std::chrono::steady_clock::now();
			if (timeNow - timeBefore > std::chrono::seconds(120))
			{
				// Timeout
				GTEST_NONFATAL_FAILURE_("timeout 120 sec while waiting to join UserA's game\n");
				return;
			}

			bc->runCallbacks();
		}

		// TearDown
		bc->getPlayerStateService()->logout(&tr);
		tr.run(bc);
	});

	// Join threads
	userAThread.join();
	userBThread.join();
}

TEST_F(TestBCLobby, CreateLobby)
{
	TestResult tr;

	std::vector<std::string> otherUserCxIds;
	m_bc->getLobbyService()->createLobby("MATCH_UNRANKED", 0, otherUserCxIds, true, "{}", "all", "{}", &tr);

	tr.run(m_bc);
}

TEST_F(TestBCLobby, FindOrCreateLobby)
{
	TestResult tr;

	std::vector<std::string> otherUserCxIds;
	m_bc->getLobbyService()->findOrCreateLobby("MATCH_UNRANKED", 0, 1, "{\"strategy\":\"ranged-absolute\",\"alignment\":\"center\",\"ranges\":[1000]}", "{}", otherUserCxIds, "{}", true, "{}", "all", &tr);

	tr.run(m_bc);
}

// Lot of expect fail here. This is ok, but we make sure the failed reason 
// is not about a bad or missing argument. We validate that the API is
// written correctly, not that the output from BC is correct. This is
// verified more in depth by use case and JS unit tests.

TEST_F(TestBCLobby, GetLobbyData)
{
	TestResult tr;

	m_bc->getLobbyService()->getLobbyData("wrongLobbyId", &tr);
	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, LeaveLobby)
{
	TestResult tr;

	m_bc->getLobbyService()->leaveLobby("wrongLobbyId", &tr);

	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, JoinLobby)
{
	TestResult tr;
	std::vector<std::string> otherUserCxIds;
	//otherUserCxIds.push_back("5555");
	//otherUserCxIds.push_back("aaa-bbb-ccc-ddd");
	//m_bc->getLobbyService()->createLobby("4v4", 76, otherUserCxIds, true, "{}", "red", "{}", &tr);
	//m_bc->getLobbyService()->joinLobby("20001:4v4:1", true, "{}", "red", otherUserCxIds, &tr);
	m_bc->getLobbyService()->joinLobby("wrongLobbyId", true, "{}", "red", otherUserCxIds, &tr);

	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}


TEST_F(TestBCLobby, RemoveMember)
{
	TestResult tr;

	m_bc->getLobbyService()->removeMember("wrongLobbyId", "wrongConId", &tr);

	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, SendSignal)
{
	TestResult tr;

	m_bc->getLobbyService()->sendSignal("wrongLobbyId", "{\"msg\":\"test\"}", &tr);

	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, SwitchTeam)
{
	TestResult tr;

	m_bc->getLobbyService()->switchTeam("wrongLobbyId", "all", &tr);

	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, UpdateReady)
{
	TestResult tr;

	m_bc->getLobbyService()->updateReady("wrongLobbyId", true, "{}", &tr);

	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, UpdateSettings)
{
	TestResult tr;

	m_bc->getLobbyService()->updateSettings("wrongLobbyId", "{\"msg\":\"test\"}", &tr);

	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, CancelFindRequest)
{
	TestResult tr;

	//m_bc->getLobbyService()->cancelFindRequest("MATCH_UNRANKED", m_bc->getRttConnectionId(), &tr);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//need to come back to this test. When I send a bad cxId, it actually sends the parameter cxId to the server. But when I send a proper 
	//cxId, it only sends the lobbyType and no cxId parameter, so it always says that the cxId parameter is missing. 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_bc->getLobbyService()->cancelFindRequest("MATCH_UNRANKED", "badcxId", &tr);
	//40653 is cxId must belong to caller
	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, 40653);
}

// We include all tests regarding pings in there
TEST_F(TestBCLobby, PingRegions)
{
    TestResult tr;

    std::vector<std::string> otherUserCxIds;

    // Test trying to call a function <>withPingData without having fetched pings
    m_bc->getLobbyService()->findOrCreateLobbyWithPingData("MATCH_UNRANKED", 0, 1, "{\"strategy\":\"ranged-absolute\",\"alignment\":\"center\",\"ranges\":[1000]}", "{}", otherUserCxIds, "{}", true, "{}", "all", &tr);
    tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, MISSING_REQUIRED_PARAMETER);

    // Fetch pings
    m_bc->getLobbyService()->getRegionsForLobbies({ "MATCH_UNRANKED" }, & tr);
    tr.run(m_bc);

    // Ping regions 2 times to make sure we see in the log there's no caching happening and that they don't all end up at 0 on the second or third time
    m_bc->getLobbyService()->pingRegions(&tr);
    tr.run(m_bc);
    m_bc->getLobbyService()->pingRegions(&tr);
    tr.run(m_bc);
    auto pingData = m_bc->getLobbyService()->getPingData();
    int total = 0;
    for (auto kv : pingData)
    {
        total += kv.second;
    }
    if (total == 0)
    {
        EXPECT_TRUE(false);
    }

    // Call all the <>WithPingData functions and make sure they go through braincloud
    m_bc->getLobbyService()->findOrCreateLobbyWithPingData("MATCH_UNRANKED", 0, 1, "{\"strategy\":\"ranged-absolute\",\"alignment\":\"center\",\"ranges\":[1000]}", "{}", otherUserCxIds, "{}", true, "{}", "all", &tr);
    tr.run(m_bc);

    m_bc->getLobbyService()->findLobbyWithPingData("MATCH_UNRANKED", 0, 1, "{\"strategy\":\"ranged-absolute\",\"alignment\":\"center\",\"ranges\":[1000]}", "{}", otherUserCxIds, true, "{}", "all", &tr);
    tr.run(m_bc);

    m_bc->getLobbyService()->createLobbyWithPingData("MATCH_UNRANKED", 0, otherUserCxIds, true, "{}", "all", "{}", &tr);
    tr.run(m_bc);

    m_bc->getLobbyService()->joinLobbyWithPingData("wrongLobbyId", true, "{}", "red", otherUserCxIds, &tr);
    tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, LOBBY_NOT_FOUND);
}

TEST_F(TestBCLobby, GetLobbyInstances)
{
    TestResult tr;

    m_bc->getLobbyService()->getLobbyInstances("MATCH_UNRANKED", "{\"rating\":{\"min\":1,\"max\":1000}}", &tr);
    tr.run(m_bc);
}

TEST_F(TestBCLobby, GetLobbyInstancesWithPingData)
{
    TestResult tr;

    // Fetch pings
    m_bc->getLobbyService()->getRegionsForLobbies({ "MATCH_UNRANKED" }, & tr);
    tr.run(m_bc);
	
    m_bc->getLobbyService()->pingRegions(&tr);
    tr.run(m_bc);

    m_bc->getLobbyService()->getLobbyInstancesWithPingData("MATCH_UNRANKED", "{\"rating\":{\"min\":1,\"max\":1000},\"ping\":{\"max\":100}}", &tr);
    tr.run(m_bc);
}
