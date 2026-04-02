// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudTournament.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"
#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"
#include <time.h>
#include "braincloud/internal/timegm.h"

namespace BrainCloud
{
	BrainCloudTournament::BrainCloudTournament(BrainCloudClient* client) : m_client(client) { }

	void BrainCloudTournament::claimTournamentReward(const char * leaderboardId, int32_t versionId, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;

		if (versionId > 0)
			message[OperationParam::VersionId.getValue()] = versionId;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::ClaimTournamentReward, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::getDivisionInfo(const std::string& divSetId, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::DivSetID.getValue()] = divSetId;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::GetDivisionInfo, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::getMyDivisions(IServerCallback * callback)
	{
		Json::Value message;
		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::GetMyDivisions, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::getTournamentStatus(const char * leaderboardId, int32_t versionId, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;

		if (versionId > 0)
			message[OperationParam::VersionId.getValue()] = versionId;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::GetTournamentStatus, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::joinDivision(const std::string& divSetId, const std::string& tournamentCode, int64_t initialScore, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::DivSetID.getValue()] = divSetId;
		message[OperationParam::TournamentCode.getValue()] = tournamentCode;
		message[OperationParam::InitialScore.getValue()] = (Json::Int64) initialScore;

		
		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::JoinDivision, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::joinTournament(const char * leaderboardId, const char * tournamentCode, int64_t initialScore, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::TournamentCode.getValue()] = tournamentCode;
		message[OperationParam::InitialScore.getValue()] = (Json::Int64) initialScore;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::JoinTournament, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::leaveDivisionInstance(const std::string& leaderboardId, IServerCallback * callback)
	{
		Json::Value message; 
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::LeaveDivisionInstance, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::leaveTournament(const char * leaderboardId, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::LeaveTournament, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::postTournamentScoreUTC(const char * leaderboardId, int64_t score, const std::string & jsonData, int64_t roundStartedTimeUTC, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::Score.getValue()] = (Json::Int64) score;

		message[OperationParam::RoundStartedEpoch.getValue()] = (Json::Int64) roundStartedTimeUTC;

		if (StringUtil::IsOptionalParameterValid(jsonData))
			message[OperationParam::Data.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::PostTournamentScore, message, callback);
		m_client->sendRequest(sc);
	}

		void BrainCloudTournament::postTournamentScoreWithResultsUTC(
		const char * leaderboardId,
		int64_t score,
		const std::string & jsonData,
		int64_t roundStartedTimeUTC,
		SortOrder sort,
		int32_t beforeCount,
		int32_t afterCount,
		int64_t initialScore,
		IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::Score.getValue()] = (Json::Int64) score;

		message[OperationParam::RoundStartedEpoch.getValue()] = (Json::Int64) roundStartedTimeUTC;

		if (StringUtil::IsOptionalParameterValid(jsonData))
			message[OperationParam::Data.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = BrainCloudSocialLeaderboard::sortOrderToString(sort);
		message[OperationParam::SocialLeaderboardServiceBeforeCount.getValue()] = beforeCount;
		message[OperationParam::SocialLeaderboardServiceAfterCount.getValue()] = afterCount;
		message[OperationParam::InitialScore.getValue()] = (Json::Int64) initialScore;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::PostTournamentScoreWithResults, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::viewCurrentReward(const char * leaderboardId, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::ViewCurrentReward, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::viewReward(const char * leaderboardId, int32_t versionId, IServerCallback * callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;

		if (versionId > 0)
			message[OperationParam::VersionId.getValue()] = versionId;

		ServerCall * sc = new ServerCall(ServiceName::Tournament, ServiceOperation::ViewReward, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::getGroupDivisionInfo(const std::string &divSetId, const std::string &groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::DivSetID.getValue()] = divSetId;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::GetGroupDivisionInfo, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::getGroupDivisions(const std::string &groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::GetGroupDivisions, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::getGroupTournamentStatus(const char *leaderboardId, const std::string &groupId, int32_t versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;

		if (versionId > 0)
			message[OperationParam::VersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::GetGroupTournamentStatus, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::joinGroupDivision(const std::string &divSetId, const std::string &tournamentCode, const std::string &groupId, int64_t initialScore, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::DivSetID.getValue()] = divSetId;
		message[OperationParam::TournamentCode.getValue()] = tournamentCode;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::InitialScore.getValue()] = (Json::Int64)initialScore;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::JoinGroupDivision, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::joinGroupTournament(const char *leaderboardId, const std::string &tournamentCode, const std::string &groupId, int64_t initialScore, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::TournamentCode.getValue()] = tournamentCode;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::InitialScore.getValue()] = (Json::Int64)initialScore;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::JoinGroupTournament, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::leaveGroupDivisionInstance(const std::string &leaderboardId, const std::string &groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::LeaveGroupDivisionInstance, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::leaveGroupTournament(const char *leaderboardId, const std::string &groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::LeaveGroupTournament, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::postGroupTournamentScore(const char *leaderboardId, const std::string &groupId, int64_t score, const std::string &jsonData, int64_t roundStartedTimeUTC, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::Score.getValue()] = (Json::Int64)score;
		message[OperationParam::RoundStartedEpoch.getValue()] = (Json::Int64)roundStartedTimeUTC;

		if (StringUtil::IsOptionalParameterValid(jsonData))
			message[OperationParam::Data.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::PostGroupTournamentScore, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudTournament::postGroupTournamentScoreWithResults(const char *leaderboardId, const std::string &groupId, int64_t score, const std::string &jsonData, int64_t roundStartedTimeUTC, SortOrder sort, int32_t beforeCount, int32_t afterCount, int64_t initialScore, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::LeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::Score.getValue()] = (Json::Int64)score;
		message[OperationParam::RoundStartedEpoch.getValue()] = (Json::Int64)roundStartedTimeUTC;
		message[OperationParam::InitialScore.getValue()] = (Json::Int64)initialScore;

		if (StringUtil::IsOptionalParameterValid(jsonData))
			message[OperationParam::Data.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = BrainCloudSocialLeaderboard::sortOrderToString(sort);
		message[OperationParam::SocialLeaderboardServiceBeforeCount.getValue()] = beforeCount;
		message[OperationParam::SocialLeaderboardServiceAfterCount.getValue()] = afterCount;

		ServerCall *sc = new ServerCall(ServiceName::Tournament, ServiceOperation::PostGroupTournamentScoreWithResults, message, callback);
		m_client->sendRequest(sc);
	}
}