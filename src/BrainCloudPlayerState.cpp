// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudPlayerState.h"

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
    BrainCloudPlayerState::BrainCloudPlayerState(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudPlayerState::readUserState(IServerCallback *callback, const char *entityTypeFilter)
    {
        Json::Value data = Json::nullValue;
        if (entityTypeFilter != NULL)
        {
            data[OperationParam::PlayerStateServiceReadEntitySubtype.getValue()] = entityTypeFilter;
        }

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::Read, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::deleteUser(IServerCallback *callback)
    {
        Json::Value data = Json::nullValue;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::FullReset, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::resetUserState(IServerCallback *callback)
    {
        Json::Value data = Json::nullValue;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::DataReset, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::logout(IServerCallback *callback)
    {
        // [dsl] Before calling logout into the server, we need to reset communications on relay/rtt
        m_client->getRelayComms()->resetCommunication();
        m_client->getRTTComms()->resetCommunication();

        Json::Value data = Json::nullValue;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::Logout, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::updateUserName(const char *name, IServerCallback *callback)
    {
        Json::Value data = Json::nullValue;
        if (name != NULL)
        {
            data[OperationParam::PlayerStateServiceUpdateNameData.getValue()] = name;
        }

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::UpdateUserName, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::updateSummaryFriendData(const char *jsonSummaryData, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::PlayerStateServiceUpdateSummaryFriendData.getValue()] = JsonUtil::jsonStringToValue(jsonSummaryData);

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::UpdateSummary, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::getAttributes(IServerCallback *callback)
    {
        Json::Value data = Json::nullValue;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::GetAttributes, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::updateAttributes(const std::string &jsonAttributes, bool wipeExisting, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceAttributes.getValue()] = JsonUtil::jsonStringToValue(jsonAttributes);
        data[OperationParam::PlayerStateServiceWipeExisting.getValue()] = wipeExisting;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::UpdateAttributes, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::removeAttributes(const std::vector<std::string> &attributeNames, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceAttributes.getValue()] = JsonUtil::stringVectorToJson(attributeNames);

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::RemoveAttributes, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::updateTimeZoneOffset(int32_t timeZoneOffset, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceTimeZoneOffset.getValue()] = timeZoneOffset;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::UpdateTimeZoneOffset, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::updateLanguageCode(const std::string &languageCode, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceLanguageCode.getValue()] = languageCode;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::UpdateLanguageCode, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::updateUserPictureUrl(const char *pictureUrl, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServicePictureUrl.getValue()] = pictureUrl;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::UpdatePlayerPicture, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::updateContactEmail(const char *contactEmail, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceContactEmail.getValue()] = contactEmail;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::UpdateContactEmail, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::clearUserStatus(const char *statusName, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceStatusName.getValue()] = statusName;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::ClearUserStatus, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::extendUserStatus(const char *statusName, int32_t additionalSecs, const std::string &details, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceStatusName.getValue()] = statusName;
        data[OperationParam::PlayerStateServiceAdditionalSecs.getValue()] = additionalSecs;
        data[OperationParam::PlayerStateServiceDetails.getValue()] = JsonUtil::jsonStringToValue(details);

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::ExtendUserStatus, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::getUserStatus(const char *statusName, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceStatusName.getValue()] = statusName;

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::GetUserStatus, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudPlayerState::setUserStatus(const char *statusName, int32_t durationSecs, const std::string &details, IServerCallback *callback)
    {
        Json::Value data;
        data[OperationParam::PlayerStateServiceStatusName.getValue()] = statusName;
        data[OperationParam::PlayerStateServiceDurationSecs.getValue()] = durationSecs;
        data[OperationParam::PlayerStateServiceDetails.getValue()] = JsonUtil::jsonStringToValue(details);

        ServerCall *sc = new ServerCall(ServiceName::PlayerState, ServiceOperation::SetUserStatus, data, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

}