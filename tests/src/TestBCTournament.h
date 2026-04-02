#ifndef _TESTBCTIME_H_
#define _TESTBCTIME_H_

#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestFixtureBase.h"
#include "json/json.h"
#include "braincloud/BrainCloudTypes.h"

using namespace BrainCloud;

class TestBCTournament: public TestFixtureBase
{
protected:
	const char* _divSetId;
	const char* _leaderboardId;
	const char* _tournamentCode;
	const char* _groupLeaderboardId;
	std::string _groupId;

	int32_t JoinTournament();
	void LeaveTournament();
	void JoinDivision();
	void LeaveDivisionInstance();
	bool CreateGroup();
	void DeleteGroup();

public:
	TestBCTournament()
	{
		_divSetId = "testDivSetId";
		_leaderboardId = "testTournamentLeaderboard";
		_tournamentCode = "testTournament";
		_groupLeaderboardId = "groupTournament";
		_groupId = "";
	}
};

#endif
