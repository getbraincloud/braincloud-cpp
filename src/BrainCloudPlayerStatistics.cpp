// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudPlayerStatistics.h"

#if defined(__APPLE__) && !defined(HG_PLATFORM_BB)
#include "TargetConditionals.h"
#endif

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/JsonUtil.h"

namespace BrainCloud
{
    BrainCloudPlayerStatistics::BrainCloudPlayerStatistics(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudPlayerStatistics::readAllUserStats(IServerCallback *callback)
    {
        Json::Value message;
        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::Read, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::readUserStatsSubset(const std::vector<std::string> &statistics,
                                                         IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsServiceStats.getValue()] = JsonUtil::stringVectorToJson(statistics);
        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::ReadSubset, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::readUserStatsForCategory(const char *category, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceCategory.getValue()] = category;

        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::ReadForCategory, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::resetAllUserStats(IServerCallback *callback)
    {
        Json::Value message;
        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::Reset, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::incrementUserStats(const std::string &jsonData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsServiceStats.getValue()] = JsonUtil::jsonStringToValue(jsonData);

        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::Update, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::getNextExperienceLevel(IServerCallback *callback)
    {
        Json::Value message;
        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::ReadNextXplevel, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::incrementExperiencePoints(int xpValue, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsExperiencePoints.getValue()] = xpValue;
        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::Update, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::setExperiencePoints(int xpValue, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsExperiencePoints.getValue()] = xpValue;
        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::SetXpPoints, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudPlayerStatistics::processStatistics(const std::string &jsonData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsServiceStats.getValue()] = JsonUtil::jsonStringToValue(jsonData);

        ServerCall *sc = new ServerCall(ServiceName::PlayerStatistics, ServiceOperation::ProcessStatistics, message, callback);
        m_client->sendRequest(sc);
    }
}
