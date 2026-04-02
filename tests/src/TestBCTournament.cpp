#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "TestBCTournament.h"
#include "braincloud/reason_codes.h"
#include "braincloud/BrainCloudSocialLeaderboard.h"
#include <ctime>
#include <chrono>

using namespace BrainCloud;


TEST_F(TestBCTournament, ClaimTournamentReward)
{
	int32_t version = JoinTournament();

	TestResult tr;

	m_bc->getTournamentService()->claimTournamentReward(_leaderboardId, version, &tr);
	tr.runExpectFail(m_bc, 400, VIEWING_REWARD_FOR_NON_PROCESSED_TOURNAMENTS);

	LeaveTournament();
}

TEST_F(TestBCTournament, GetDivisionInfo)
{
	TestResult tr;
	m_bc->getTournamentService()->getDivisionInfo("Invalid_Id", &tr);
	tr.runExpectFail(m_bc, 400, DIVISION_SET_DOESNOT_EXIST);
}

TEST_F(TestBCTournament, GetMyDivisions)
{
	TestResult tr; 
	m_bc->getTournamentService()->getMyDivisions(&tr);
	tr.run(m_bc);
}

TEST_F(TestBCTournament, JoinDivision)
{
	TestResult tr; 
	m_bc->getTournamentService()->joinDivision("Invalid_Id", _tournamentCode, 0,  &tr);
	tr.runExpectFail(m_bc, 400, DIVISION_SET_DOESNOT_EXIST);
}

TEST_F(TestBCTournament, LeaveDivisionInstance)
{
	TestResult tr; 
	m_bc->getTournamentService()->leaveDivisionInstance(_leaderboardId, &tr);
	tr.runExpectFail(m_bc, 400, LEADERBOARD_NOT_DIVISION_SET_INSTANCE);
}

TEST_F(TestBCTournament, GetTournamentStatus)
{
	int32_t version = JoinTournament();

	TestResult tr;

	m_bc->getTournamentService()->getTournamentStatus(_leaderboardId, version, &tr);
	tr.run(m_bc);

	LeaveTournament();
}

TEST_F(TestBCTournament, JoinTournament)
{
	JoinTournament();
	LeaveTournament();
}

TEST_F(TestBCTournament, LeaveTournament)
{
	JoinTournament();
	LeaveTournament();
}

TEST_F(TestBCTournament, PostTournamentScoreUTC)
{
	int32_t version = JoinTournament();

	TestResult tr;
	int64_t milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	m_bc->getTournamentService()->postTournamentScoreUTC(_leaderboardId, 200, "", milliseconds_since_epoch, &tr);
	tr.run(m_bc);

	LeaveTournament();
}

TEST_F(TestBCTournament, PostTournamentScoreWithResultsUTC)
{
	int32_t version = JoinTournament();
	TestResult tr;
	int64_t milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	m_bc->getTournamentService()->postTournamentScoreWithResultsUTC(_leaderboardId, 200, "", milliseconds_since_epoch, HIGH_TO_LOW, 10, 10, 0, &tr);
	tr.run(m_bc);

	LeaveTournament();
}

TEST_F(TestBCTournament, ViewCurrentReward)
{
	JoinTournament();

	TestResult tr;
	m_bc->getTournamentService()->viewCurrentReward(_leaderboardId, &tr);
	tr.run(m_bc);

	LeaveTournament();
}

TEST_F(TestBCTournament, ViewReward)
{
	JoinTournament();

	TestResult tr;
	m_bc->getTournamentService()->viewReward(_leaderboardId, -1, &tr);
	tr.runExpectFail(m_bc, 400, PLAYER_NOT_ENROLLED_IN_TOURNAMENT);

	LeaveTournament();
}

TEST_F(TestBCTournament, GetGroupDivisionInfo)
{
	if (!CreateGroup())
	{
		FAIL() << "Failed to create test group";
	}

	TestResult tr;
	m_bc->getTournamentService()->getGroupDivisionInfo("bronzeGroup", _groupId, &tr);
	tr.run(m_bc);

	DeleteGroup();
}

TEST_F(TestBCTournament, GetGroupDivisions)
{
	if (!CreateGroup())
	{
		FAIL() << "Failed to create test group";
	}

	TestResult tr;
	m_bc->getTournamentService()->getGroupDivisions(_groupId, &tr);
	tr.run(m_bc);

	DeleteGroup();
}

TEST_F(TestBCTournament, GetGroupTournamentStatus)
{
	if (!CreateGroup())
	{
		FAIL() << "Failed to create test group";
	}

	TestResult tr;
	m_bc->getTournamentService()->getGroupTournamentStatus(_groupLeaderboardId, _groupId, -1, &tr);
	tr.run(m_bc);

	DeleteGroup();
}

TEST_F(TestBCTournament, JoinAndLeaveGroupDivision)
{
	if (!CreateGroup())
	{
		FAIL() << "Failed to create test group";
	}

	TestResult tr;
	m_bc->getTournamentService()->joinGroupDivision("bronzeGroup", "testGroupTournament", _groupId, 0, &tr);
	tr.run(m_bc);

	std::string leaderboardId = tr.m_response["data"]["leaderboardId"].asString();
	ASSERT_FALSE(leaderboardId.empty()) << "Error reading joinGroupDivision response leaderboardId";

	m_bc->getTournamentService()->leaveGroupDivisionInstance(leaderboardId, _groupId, &tr);
	tr.run(m_bc);

	DeleteGroup();
}

TEST_F(TestBCTournament, JoinPostLeaveGroupTournament)
{
	if (!CreateGroup())
	{
		FAIL() << "Failed to create test group";
	}

	int64_t milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	TestResult tr;
	m_bc->getTournamentService()->joinGroupTournament(_groupLeaderboardId, "testGroupTournament", _groupId, 0, &tr);
	tr.run(m_bc);

	m_bc->getTournamentService()->postGroupTournamentScore(_groupLeaderboardId, _groupId, 10, "{}", milliseconds_since_epoch, &tr);
	tr.run(m_bc);

	m_bc->getTournamentService()->postGroupTournamentScoreWithResults(_groupLeaderboardId, _groupId, 100, "{}", milliseconds_since_epoch, HIGH_TO_LOW, 10, 10, 0, &tr);
	tr.run(m_bc);

	m_bc->getTournamentService()->leaveGroupTournament(_groupLeaderboardId, _groupId, &tr);
	tr.run(m_bc);

	DeleteGroup();
}

int32_t TestBCTournament::JoinTournament()
{
	TestResult tr;
	m_bc->getTournamentService()->joinTournament(_leaderboardId, _tournamentCode, 100, &tr);
	tr.run(m_bc);

	m_bc->getTournamentService()->getTournamentStatus(_leaderboardId, -1, &tr);
	tr.run(m_bc);
	return tr.m_response["data"]["versionId"].asInt();
}

void TestBCTournament::LeaveTournament()
{
	TestResult tr;
	m_bc->getTournamentService()->leaveTournament(_leaderboardId, &tr);
	tr.run(m_bc);
}

bool TestBCTournament::CreateGroup()
{
	TestResult tr;
	m_bc->getGroupService()->createGroup("CppTestGroup", "csharpTest", true, nullptr, "{}", "{\"testInc\": 123}", "{\"test\": \"test\"}", &tr);
	tr.run(m_bc);

	if (!tr.m_result)
		return false;

	_groupId = tr.m_response["data"]["groupId"].asString();
	return !_groupId.empty();
}

void TestBCTournament::DeleteGroup()
{
	if (_groupId.empty())
		return;

	TestResult tr;
	m_bc->getGroupService()->deleteGroup(_groupId.c_str(), -1, &tr);
	tr.run(m_bc);
	_groupId = "";
}
