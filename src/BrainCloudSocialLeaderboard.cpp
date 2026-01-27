// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudSocialLeaderboard.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/JsonUtil.h"
#include <time.h>

namespace BrainCloud
{
	BrainCloudSocialLeaderboard::BrainCloudSocialLeaderboard(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudSocialLeaderboard::getSocialLeaderboard(const char *leaderboardId, bool replaceName, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceReplaceName.getValue()] = replaceName;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetSocialLeaderboard, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getSocialLeaderboardIfExists(const char *leaderboardId, bool replaceName, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceReplaceName.getValue()] = replaceName;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetSocialLeaderboardIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getSocialLeaderboardByVersion(const char *leaderboardId, bool replaceName, int versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceReplaceName.getValue()] = replaceName;
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetSocialLeaderboardByVersion, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getSocialLeaderboardByVersionIfExists(const char *leaderboardId, bool replaceName, int versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceReplaceName.getValue()] = replaceName;
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetSocialLeaderboardByVersionIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getMultiSocialLeaderboard(const std::vector<std::string> &leaderboardIds, int leaderboardResultCount, bool replaceName, IServerCallback *callback)
	{
		Json::Value message;
		if (leaderboardIds.size() > 0)
		{
			message[OperationParam::SocialLeaderboardServiceLeaderboardIds.getValue()] = JsonUtil::stringVectorToJson(leaderboardIds);
		}
		message[OperationParam::SocialLeaderboardServiceLeaderboardResultCount.getValue()] = leaderboardResultCount;
		message[OperationParam::SocialLeaderboardServiceReplaceName.getValue()] = replaceName;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetMultiSocialLeaderboard, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardPage(const char *leaderboardId, SortOrder sortOrder, int startIndex,
															   int endIndex, IServerCallback *callback)
	{
		std::string sortOrder = sortOrderToString(sortOrder);

		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceStartIndex.getValue()] = startIndex;
		message[OperationParam::SocialLeaderboardServiceEndIndex.getValue()] = endIndex;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardPage, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardPageIfExists(const char *leaderboardId, SortOrder sortOrder, int startIndex,
																	   int endIndex, IServerCallback *callback)
	{
		std::string sortOrder = sortOrderToString(sortOrder);

		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceStartIndex.getValue()] = startIndex;
		message[OperationParam::SocialLeaderboardServiceEndIndex.getValue()] = endIndex;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardPageIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardPageByVersion(const char *leaderboardId, SortOrder sortOrder, int startIndex, int endIndex, int versionId, IServerCallback *callback)
	{
		std::string sortOrder = sortOrderToString(sortOrder);

		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceStartIndex.getValue()] = startIndex;
		message[OperationParam::SocialLeaderboardServiceEndIndex.getValue()] = endIndex;
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardPage, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardPageByVersionIfExists(const char *leaderboardId, SortOrder sortOrder, int startIndex, int endIndex, int versionId, IServerCallback *callback)
	{
		std::string sortOrder = sortOrderToString(sortOrder);

		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceStartIndex.getValue()] = startIndex;
		message[OperationParam::SocialLeaderboardServiceEndIndex.getValue()] = endIndex;
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardPageIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardView(const char *leaderboardId, SortOrder sortOrder, int beforeCount,
															   int afterCount, IServerCallback *callback)
	{
		getGlobalLeaderboardViewByVersion(leaderboardId, sortOrder, beforeCount, afterCount, -1, callback);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardViewIfExists(const char *leaderboardId, SortOrder sortOrder, int beforeCount,
																	   int afterCount, IServerCallback *callback)
	{
		getGlobalLeaderboardViewByVersionIfExists(leaderboardId, sortOrder, beforeCount, afterCount, -1, callback);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardViewByVersion(const char *leaderboardId, SortOrder sortOrder, int beforeCount,
																		int afterCount, int versionId, IServerCallback *callback)
	{
		std::string sortOrder = sortOrderToString(sortOrder);

		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceBeforeCount.getValue()] = beforeCount;
		message[OperationParam::SocialLeaderboardServiceAfterCount.getValue()] = afterCount;
		if (versionId != -1)
			message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardView, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardViewByVersionIfExists(const char *leaderboardId, SortOrder sortOrder, int beforeCount,
																				int afterCount, int versionId, IServerCallback *callback)
	{
		std::string sortOrder = sortOrderToString(sortOrder);

		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceBeforeCount.getValue()] = beforeCount;
		message[OperationParam::SocialLeaderboardServiceAfterCount.getValue()] = afterCount;
		if (versionId != -1)
			message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardViewIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardVersions(const char *leaderboardId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardVersions, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToLeaderboard(const char *leaderboardId, int64_t score, const std::string &jsonOtherData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceScore.getValue()] = (Json::Int64)score;
		message[OperationParam::SocialLeaderboardServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonOtherData);

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScore, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToDynamicLeaderboardUsingConfig(
		const char *leaderboardId,
		int64_t score,
		const std::string &scoreData,
		const std::string &configJson,
		IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceScore.getValue()] = (Json::Int64)score;
		message[OperationParam::SocialLeaderboardServiceScoreData.getValue()] = JsonUtil::jsonStringToValue(scoreData);
		message[OperationParam::SocialLeaderboardServiceConfigJson.getValue()] = JsonUtil::jsonStringToValue(configJson);

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScoreDynamicUsingConfig, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToDynamicLeaderboardUTC(
		const char *leaderboardId,
		int64_t score,
		const std::string &jsonData,
		SocialLeaderboardType leaderboardType,
		RotationType rotationType,
		int64_t rotationResetUTC,
		int retainedCount,
		IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceScore.getValue()] = (Json::Int64)score;
		message[OperationParam::SocialLeaderboardServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		message[OperationParam::SocialLeaderboardServiceLeaderboardType.getValue()] = leaderboardTypeToString(leaderboardType).c_str();
		message[OperationParam::SocialLeaderboardServiceRotationType.getValue()] = leaderboardRotationTypeToString(rotationType).c_str();
		message[OperationParam::SocialLeaderboardServiceRotationResetTime.getValue()] = (Json::UInt64)rotationResetUTC;
		message[OperationParam::SocialLeaderboardServiceRetainedCount.getValue()] = retainedCount;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScoreDynamic, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToDynamicLeaderboardDaysUTC(
		const char *leaderboardId,
		int64_t score,
		const std::string &jsonData,
		SocialLeaderboardType leaderboardType,
		int64_t rotationResetUTC,
		int32_t retainedCount,
		int32_t numDaysToRotate,
		IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::SocialLeaderboardServiceScore.getValue()] = (Json::Int64)score;
		message[OperationParam::SocialLeaderboardServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		message[OperationParam::SocialLeaderboardServiceLeaderboardType.getValue()] = leaderboardTypeToString(leaderboardType).c_str();
		message[OperationParam::SocialLeaderboardServiceRotationType.getValue()] = "DAYS";
		message[OperationParam::NumDaysToRotate.getValue()] = numDaysToRotate;
		message[OperationParam::SocialLeaderboardServiceRotationResetTime.getValue()] = (Json::UInt64)rotationResetUTC;
		message[OperationParam::SocialLeaderboardServiceRetainedCount.getValue()] = retainedCount;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScoreDynamic, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToDynamicGroupLeaderboardUsingConfig(const char *leaderboardId, const char *groupId, int32_t score, const std::string &scoreData, const std::string &configJson, IServerCallback *callback)
	{
		Json::Value data;
		data[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		data[OperationParam::GroupId.getValue()] = groupId;
		data[OperationParam::SocialLeaderboardServiceScore.getValue()] = (Json::Int64)score;
		data[OperationParam::SocialLeaderboardServiceScoreData.getValue()] = JsonUtil::jsonStringToValue(scoreData);
		data[OperationParam::SocialLeaderboardServiceConfigJson.getValue()] = JsonUtil::jsonStringToValue(configJson);

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScoreToDynamicGroupLeaderboardUsingConfig, data, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::removePlayerScore(const char *leaderboardId, int32_t versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		if (versionId != -1)
			message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::RemovePlayerScore, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::playerTournamentReward(const char *eventName, uint64_t multiplier, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceEventName.getValue()] = eventName;
		message[OperationParam::SocialLeaderboardServiceEventMultiplier.getValue()] = (Json::UInt64)multiplier;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::RewardTournament, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGroupSocialLeaderboard(const char *leaderboardId, const char *groupId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGroupSocialLeaderboard, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGroupSocialLeaderboardByVersion(const char *leaderboardId, const char *groupId, int versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGroupSocialLeaderboardByVersion, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getPlayersSocialLeaderboard(const char *leaderboardId, std::vector<std::string> profileIds, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::ProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetPlayersSocialLeaderboard, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getPlayersSocialLeaderboardIfExists(const char *leaderboardId, std::vector<std::string> profileIds, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::ProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetPlayersSocialLeaderboardIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getPlayersSocialLeaderboardByVersion(const char *leaderboardId, std::vector<std::string> profileIds, int versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::ProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetPlayersSocialLeaderboardByVersion, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getPlayersSocialLeaderboardByVersionIfExists(const char *leaderboardId, std::vector<std::string> profileIds, int versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::ProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetPlayersSocialLeaderboardByVersionIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::listAllLeaderboards(IServerCallback *callback)
	{
		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::ListAllLeaderboards, Json::nullValue, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardEntryCount(const char *leaderboardId, IServerCallback *callback)
	{
		getGlobalLeaderboardEntryCountByVersion(leaderboardId, -1, callback);
	}

	void BrainCloudSocialLeaderboard::getGlobalLeaderboardEntryCountByVersion(const char *leaderboardId, int32_t versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		if (versionId != -1)
			message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGlobalLeaderboardEntryCount, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getPlayerScore(const char *leaderboardId, int32_t versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		if (versionId != -1)
			message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetPlayerScore, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getPlayerScores(const char *leaderboardId, int32_t versionId, int32_t maxResults, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		if (versionId != -1)
			message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;
		message[OperationParam::SocialLeaderboardServiceMaxResults.getValue()] = maxResults;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetPlayerScores, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getPlayerScoresFromLeaderboards(const std::vector<std::string> &leaderboardIds, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardIds.getValue()] = JsonUtil::stringVectorToJson(leaderboardIds);

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetPlayerScoresFromLeaderboards, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToGroupLeaderboard(const char *leaderboardId, const char *groupId, int32_t score, const std::string &jsonData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::SocialLeaderboardServiceScore.getValue()] = score;
		message[OperationParam::SocialLeaderboardServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonData);

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScoreToGroupLeaderboard, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToDynamicGroupLeaderboardUTC(const char *leaderboardId, const char *groupId, int32_t score, const std::string &jsonData, const char *leaderboardType,
																			const char *rotationType, int64_t rotationResetUTC, int32_t retainedCount, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::SocialLeaderboardServiceScore.getValue()] = score;
		message[OperationParam::SocialLeaderboardServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonData);
		message[OperationParam::SocialLeaderboardServiceLeaderboardType.getValue()] = leaderboardType;
		message[OperationParam::SocialLeaderboardServiceRotationType.getValue()] = rotationType;
		message[OperationParam::SocialLeaderboardServiceRotationResetTime.getValue()] = (Json::UInt64)rotationResetUTC;
		message[OperationParam::SocialLeaderboardServiceRetainedCount.getValue()] = retainedCount;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScoreToDynamicGroupLeaderboard, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::postScoreToDynamicGroupLeaderboardDaysUTC(const char *leaderboardId, const char *groupId, int32_t score, const std::string &jsonData, const char *leaderboardType,
																				int64_t rotationResetUTC, int32_t retainedCount, int32_t numDaysToRotate, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::SocialLeaderboardServiceScore.getValue()] = score;
		message[OperationParam::SocialLeaderboardServiceData.getValue()] = JsonUtil::jsonStringToValue(jsonData);
		message[OperationParam::SocialLeaderboardServiceLeaderboardType.getValue()] = leaderboardType;
		message[OperationParam::NumDaysToRotate.getValue()] = numDaysToRotate;
		message[OperationParam::SocialLeaderboardServiceRotationType.getValue()] = "DAYS";
		message[OperationParam::SocialLeaderboardServiceRotationResetTime.getValue()] = (Json::UInt64)rotationResetUTC;
		message[OperationParam::SocialLeaderboardServiceRetainedCount.getValue()] = retainedCount;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::PostScoreToDynamicGroupLeaderboard, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::removeGroupScore(const char *leaderboardId, const char *groupId, int32_t versionId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::RemoveGroupScore, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGroupLeaderboardView(const char *leaderboardId, const char *groupId, SortOrder sortOrder, int32_t beforeCount, int32_t afterCount, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		std::string sortOrder = sortOrderToString(sortOrder);
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceBeforeCount.getValue()] = beforeCount;
		message[OperationParam::SocialLeaderboardServiceAfterCount.getValue()] = afterCount;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGroupLeaderboardView, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudSocialLeaderboard::getGroupLeaderboardViewByVersion(const char *leaderboardId, const char *groupId, int32_t versionId, SortOrder sortOrder, int32_t beforeCount, int32_t afterCount, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::SocialLeaderboardServiceLeaderboardId.getValue()] = leaderboardId;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::SocialLeaderboardServiceVersionId.getValue()] = versionId;
		std::string sortOrder = sortOrderToString(sortOrder);
		message[OperationParam::SocialLeaderboardServiceSortOrder.getValue()] = sortOrder;
		message[OperationParam::SocialLeaderboardServiceBeforeCount.getValue()] = beforeCount;
		message[OperationParam::SocialLeaderboardServiceAfterCount.getValue()] = afterCount;

		ServerCall *sc = new ServerCall(ServiceName::Leaderboard, ServiceOperation::GetGroupLeaderboardView, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	std::string BrainCloudSocialLeaderboard::leaderboardTypeToString(SocialLeaderboardType type)
	{
		std::string typeString = "HIGH_VALUE";

		if (type == CUMULATIVE)
			typeString = "CUMULATIVE";
		else if (type == LAST_VALUE)
			typeString = "LAST_VALUE";
		else if (type == LOW_VALUE)
			typeString = "LOW_VALUE";

		return typeString;
	}

	std::string BrainCloudSocialLeaderboard::leaderboardRotationTypeToString(RotationType type)
	{
		std::string typeString = "NEVER";

		switch (type)
		{
		case DAILY:
			typeString = "DAILY";
			break;
		case WEEKLY:
			typeString = "WEEKLY";
			break;
		case MONTHLY:
			typeString = "MONTHLY";
			break;
		case YEARLY:
			typeString = "YEARLY";
			break;
		default:
			break;
		}

		return typeString;
	}

	std::string BrainCloudSocialLeaderboard::sortOrderToString(SortOrder sortOrder)
	{
		std::string sortOrder = "HIGHEST_RANKED";

		if (sortOrder == HIGH_TO_LOW)
		{
			sortOrder = "HIGH_TO_LOW";
		}
		else if (sortOrder == LOW_TO_HIGH)
		{
			sortOrder = "LOW_TO_HIGH";
		}
		else
		{
			sortOrder = "HIGHEST_RANKED";
		}
		return sortOrder;
	}
}
