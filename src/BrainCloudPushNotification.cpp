// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudPushNotification.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"
#include <iostream>
#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"
#include "braincloud/Platform.h"
#include "braincloud/reason_codes.h"

namespace BrainCloud
{
	BrainCloudPushNotification::BrainCloudPushNotification(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudPushNotification::deregisterAllPushNotificationDeviceTokens(IServerCallback *callback)
	{
		Json::Value message = Json::nullValue;
		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::DeregisterAll, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::deregisterPushNotificationDeviceToken(const Platform &platform, const char *token, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PushNotificationRegisterParamDeviceType.getValue()] = platform.toString().c_str();
		message[OperationParam::PushNotificationRegisterParamDeviceToken.getValue()] = token;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::Deregister, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::registerPushNotificationDeviceToken(const Platform &platform, const char *token, IServerCallback *callback)
	{
		// Validate token
		if (token == nullptr || strlen(token) == 0)
		{
			if (callback)
			{
				int statusCode = 400;				   // Bad Request
				int reasonCode = INVALID_DEVICE_TOKEN; // Invalid Device Token

				// Build error JSON
				std::string errorJson =
					"{\"status\":" + std::to_string(statusCode) +
					",\"reason_code\":" + std::to_string(reasonCode) +
					",\"message\":\"Invalid device token: " + token + " \"}";

				callback->serverError(
					ServiceName::PushNotification,
					ServiceOperation::Register,
					statusCode,
					reasonCode,
					errorJson);
			}
			// FL: ToDo would be better to have a check if logging is enabled to see if we want to print this message like other libs have
			std::cout << "BCC: Push notification token not registered - empty/null tokens are invalid" << std::endl;

			return;
		}

		Json::Value message;
		message[OperationParam::PushNotificationRegisterParamDeviceType.getValue()] = platform.toString().c_str();
		message[OperationParam::PushNotificationRegisterParamDeviceToken.getValue()] = token;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::Register, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendSimplePushNotification(const char *toProfileId, const char *message, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PushNotificationSendParamToPlayerId.getValue()] = toProfileId;
		message[OperationParam::PushNotificationSendParamMessage.getValue()] = message;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendSimple, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendRichPushNotification(const char *toProfileId, int32_t notificationTemplateId, IServerCallback *callback)
	{
		sendRichPushNotification(toProfileId, notificationTemplateId, NULL, callback);
	}

	void BrainCloudPushNotification::sendRichPushNotificationWithParams(const char *toProfileId, int32_t notificationTemplateId, const char *substitutionJson, IServerCallback *callback)
	{
		sendRichPushNotification(toProfileId, notificationTemplateId, substitutionJson, callback);
	}

	void BrainCloudPushNotification::sendTemplatedPushNotificationToGroup(const char *groupId, int32_t notificationTemplateId, std::string substitutionsJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::PushNotificationSendParamNotificationTemplateId.getValue()] = notificationTemplateId;

		if (StringUtil::IsOptionalParameterValid(substitutionsJson))
		{
			message[OperationParam::PushNotificationSendParamSubstitution.getValue()] = JsonUtil::jsonStringToValue(substitutionsJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendTemplatedToGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendNormalizedPushNotificationToGroup(const char *groupId, std::string alertContentJson, std::string customDataJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;
		message[OperationParam::AlertContent.getValue()] = JsonUtil::jsonStringToValue(alertContentJson);

		if (StringUtil::IsOptionalParameterValid(customDataJson))
		{
			message[OperationParam::CustomData.getValue()] = JsonUtil::jsonStringToValue(customDataJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendNormalizedToGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::scheduleRawPushNotificationUTC(const char *profileId, std::string fcmContent, std::string iosContent, std::string facebookContent, int64_t startTimeUTC, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = profileId;

		if (StringUtil::IsOptionalParameterValid(fcmContent))
		{
			message[OperationParam::FcmContent.getValue()] = JsonUtil::jsonStringToValue(fcmContent);
		}

		if (StringUtil::IsOptionalParameterValid(iosContent))
		{
			message[OperationParam::IosContent.getValue()] = JsonUtil::jsonStringToValue(iosContent);
		}

		if (StringUtil::IsOptionalParameterValid(facebookContent))
		{
			message[OperationParam::FacebookContent.getValue()] = JsonUtil::jsonStringToValue(facebookContent);
		}

		message[OperationParam::StartDateUTC.getValue()] = (Json::Int64)startTimeUTC;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::ScheduleRawNotification, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::scheduleRawPushNotificationMinutes(const char *profileId, std::string fcmContent, std::string iosContent, std::string facebookContent, int32_t minutesFromNow, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = profileId;

		if (StringUtil::IsOptionalParameterValid(fcmContent))
		{
			message[OperationParam::FcmContent.getValue()] = JsonUtil::jsonStringToValue(fcmContent);
		}

		if (StringUtil::IsOptionalParameterValid(iosContent))
		{
			message[OperationParam::IosContent.getValue()] = JsonUtil::jsonStringToValue(iosContent);
		}

		if (StringUtil::IsOptionalParameterValid(facebookContent))
		{
			message[OperationParam::FacebookContent.getValue()] = JsonUtil::jsonStringToValue(facebookContent);
		}

		message[OperationParam::MinutesFromNow.getValue()] = minutesFromNow;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::ScheduleRawNotification, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendRawPushNotification(const char *profileId, std::string fcmContent, std::string iosContent, std::string facebookContent, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PushNotificationSendParamToPlayerId.getValue()] = profileId;

		if (StringUtil::IsOptionalParameterValid(fcmContent))
		{
			message[OperationParam::FcmContent.getValue()] = JsonUtil::jsonStringToValue(fcmContent);
		}

		if (StringUtil::IsOptionalParameterValid(iosContent))
		{
			message[OperationParam::IosContent.getValue()] = JsonUtil::jsonStringToValue(iosContent);
		}

		if (StringUtil::IsOptionalParameterValid(facebookContent))
		{
			message[OperationParam::FacebookContent.getValue()] = JsonUtil::jsonStringToValue(facebookContent);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendRaw, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendRawPushNotificationBatch(std::vector<std::string> profileIds, std::string fcmContent, std::string iosContent, std::string facebookContent, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);

		if (StringUtil::IsOptionalParameterValid(fcmContent))
		{
			message[OperationParam::FcmContent.getValue()] = JsonUtil::jsonStringToValue(fcmContent);
		}

		if (StringUtil::IsOptionalParameterValid(iosContent))
		{
			message[OperationParam::IosContent.getValue()] = JsonUtil::jsonStringToValue(iosContent);
		}

		if (StringUtil::IsOptionalParameterValid(facebookContent))
		{
			message[OperationParam::FacebookContent.getValue()] = JsonUtil::jsonStringToValue(facebookContent);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendRawBatch, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendRawPushNotificationToGroup(const char *groupId, std::string fcmContent, std::string iosContent, std::string facebookContent, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::GroupId.getValue()] = groupId;

		if (StringUtil::IsOptionalParameterValid(fcmContent))
		{
			message[OperationParam::FcmContent.getValue()] = JsonUtil::jsonStringToValue(fcmContent);
		}

		if (StringUtil::IsOptionalParameterValid(iosContent))
		{
			message[OperationParam::IosContent.getValue()] = JsonUtil::jsonStringToValue(iosContent);
		}

		if (StringUtil::IsOptionalParameterValid(facebookContent))
		{
			message[OperationParam::FacebookContent.getValue()] = JsonUtil::jsonStringToValue(facebookContent);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendRawToGroup, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::scheduleNormalizedPushNotificationUTC(const char *toProfileId, std::string alertContentJson, std::string customDataJson, int64_t startTimeUTC, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = toProfileId;
		message[OperationParam::AlertContent.getValue()] = JsonUtil::jsonStringToValue(alertContentJson);

		if (StringUtil::IsOptionalParameterValid(customDataJson))
		{
			message[OperationParam::CustomData.getValue()] = JsonUtil::jsonStringToValue(customDataJson);
		}

		message[OperationParam::StartDateUTC.getValue()] = (Json::Int64)startTimeUTC;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::ScheduleNormalizedNotification, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::scheduleNormalizedPushNotificationMinutes(const char *toProfileId, std::string alertContentJson, std::string customDataJson, int32_t minutesFromNow, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = toProfileId;
		message[OperationParam::AlertContent.getValue()] = JsonUtil::jsonStringToValue(alertContentJson);

		if (StringUtil::IsOptionalParameterValid(customDataJson))
		{
			message[OperationParam::CustomData.getValue()] = JsonUtil::jsonStringToValue(customDataJson);
		}

		message[OperationParam::MinutesFromNow.getValue()] = minutesFromNow;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::ScheduleNormalizedNotification, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::scheduleRichPushNotificationUTC(const char *toProfileId, int32_t notificationTemplateId, std::string substitutionsJson, int64_t startTimeUTC, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = toProfileId;
		message[OperationParam::PushNotificationSendParamNotificationTemplateId.getValue()] = notificationTemplateId;

		if (StringUtil::IsOptionalParameterValid(substitutionsJson))
		{
			message[OperationParam::PushNotificationSendParamSubstitution.getValue()] = JsonUtil::jsonStringToValue(substitutionsJson);
		}

		message[OperationParam::StartDateUTC.getValue()] = (Json::Int64)startTimeUTC;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::ScheduleRichNotification, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::scheduleRichPushNotificationMinutes(const char *toProfileId, int32_t notificationTemplateId, std::string substitutionsJson, int32_t minutesFromNow, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = toProfileId;
		message[OperationParam::PushNotificationSendParamNotificationTemplateId.getValue()] = notificationTemplateId;

		if (StringUtil::IsOptionalParameterValid(substitutionsJson))
		{
			message[OperationParam::PushNotificationSendParamSubstitution.getValue()] = JsonUtil::jsonStringToValue(substitutionsJson);
		}

		message[OperationParam::MinutesFromNow.getValue()] = minutesFromNow;

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::ScheduleRichNotification, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendNormalizedPushNotification(const char *toProfileId, std::string alertContentJson, std::string customDataJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PushNotificationSendParamToPlayerId.getValue()] = toProfileId;
		message[OperationParam::AlertContent.getValue()] = JsonUtil::jsonStringToValue(alertContentJson);

		if (StringUtil::IsOptionalParameterValid(customDataJson))
		{
			message[OperationParam::CustomData.getValue()] = JsonUtil::jsonStringToValue(customDataJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendNormalized, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendNormalizedPushNotificationBatch(std::vector<std::string> profileIds, std::string alertContentJson, std::string customDataJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileIds.getValue()] = JsonUtil::stringVectorToJson(profileIds);
		message[OperationParam::AlertContent.getValue()] = JsonUtil::jsonStringToValue(alertContentJson);

		if (StringUtil::IsOptionalParameterValid(customDataJson))
		{
			message[OperationParam::CustomData.getValue()] = JsonUtil::jsonStringToValue(customDataJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendNormalizedBatch, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudPushNotification::sendRichPushNotification(const char *toProfileId, int32_t notificationTemplateId, const char *substitutionJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::PushNotificationSendParamToPlayerId.getValue()] = toProfileId;
		message[OperationParam::PushNotificationSendParamNotificationTemplateId.getValue()] = notificationTemplateId;

		if (StringUtil::IsOptionalParameterValid(substitutionJson))
		{
			message[OperationParam::PushNotificationSendParamSubstitution.getValue()] = JsonUtil::jsonStringToValue(substitutionJson);
		}

		ServerCall *sc = new ServerCall(ServiceName::PushNotification, ServiceOperation::SendRich, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}
}