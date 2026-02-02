// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#if defined(__APPLE__) && !defined(HG_PLATFORM_BB)
#include "TargetConditionals.h"
#endif

#include "braincloud/BrainCloudGamification.h"

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
    BrainCloudGamification::BrainCloudGamification(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudGamification::readAllGamification(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::Read, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readQuests(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadQuests, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readMilestones(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadMilestones, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readAchievements(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadAchievements, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readXpLevelsMetaData(IServerCallback *callback)
    {
        Json::Value message;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadXpLevels, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readAchievedAchievements(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadAchievedAchievements, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readCompletedQuests(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadCompletedQuests, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readInProgressQuests(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadInProgressQuests, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readNotStartedQuests(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadNotStartedQuests, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readQuestsWithStatus(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadQuestsWithStatus, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readQuestsWithBasicPercentage(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadQuestsWithBasicPercentage, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readQuestsWithComplexPercentage(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadQuestsWithComplexPercentage, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readCompletedMilestones(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadCompletedMilestones, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readInProgressMilestones(bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadInProgressMilestones, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readQuestsByCategory(const char *category, bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceCategory.getValue()] = category;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadQuestsByCategory, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::readMilestonesByCategory(const char *category, bool includeMetaData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GamificationServiceCategory.getValue()] = category;
        message[OperationParam::GamificationServiceIncludeMetaData.getValue()] = includeMetaData;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::ReadMilestonesByCategory, message, callback);
        m_client->sendRequest(sc);
    }

    void BrainCloudGamification::awardAchievements(const std::vector<std::string> &achievements, IServerCallback *callback)
    {
        Json::Value message;
        Json::Value achievementArray = JsonUtil::stringVectorToJson(achievements);
        message[OperationParam::GamificationServiceAchievementsName.getValue()] = achievementArray;

        ServerCall *sc = new ServerCall(ServiceName::Gamification, ServiceOperation::AwardAchievements, message, callback);
        m_client->sendRequest(sc);
    }
}
