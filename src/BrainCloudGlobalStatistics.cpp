// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudGlobalStatistics.h"

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
    BrainCloudGlobalStatistics::BrainCloudGlobalStatistics(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudGlobalStatistics::readAllGlobalStats(IServerCallback *callback)
    {
        Json::Value message;
        ServerCall *sc = new ServerCall(ServiceName::GlobalGameStatistics, ServiceOperation::Read, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGlobalStatistics::readGlobalStatsSubset(const std::vector<std::string> &statistics, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsServiceStats.getValue()] = JsonUtil::stringVectorToJson(statistics);

        ServerCall *sc = new ServerCall(ServiceName::GlobalGameStatistics, ServiceOperation::ReadSubset, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGlobalStatistics::readGlobalStatsForCategory(const char *category, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceCategory.getValue()] = category;

        ServerCall *sc = new ServerCall(ServiceName::GlobalGameStatistics, ServiceOperation::ReadForCategory, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGlobalStatistics::incrementGlobalGameStat(const std::string &jsonData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsServiceStats.getValue()] = JsonUtil::jsonStringToValue(jsonData);

        ServerCall *sc = new ServerCall(ServiceName::GlobalGameStatistics, ServiceOperation::UpdateIncrement, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGlobalStatistics::processStatistics(const std::string &jsonData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStatisticsServiceStats.getValue()] = JsonUtil::jsonStringToValue(jsonData);

        ServerCall *sc = new ServerCall(ServiceName::GlobalGameStatistics, ServiceOperation::ProcessStatistics, message, callback);
        m_client->sendRequest(sc);
    }
}