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
