// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudMail.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"
#include <vector>

#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"

namespace BrainCloud
{
	BrainCloudMail::BrainCloudMail(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudMail::sendBasicEmail(const char *profileId, const char *subject, const char *body, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = profileId;
		message[OperationParam::Subject.getValue()] = subject;
		message[OperationParam::Body.getValue()] = body;

		ServerCall *sc = new ServerCall(ServiceName::Mail, ServiceOperation::SendBasicEmail, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMail::sendAdvancedEmail(const char *profileId, const std::string &jsonServiceParams, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ProfileId.getValue()] = profileId;

		Json::Value jsonData = JsonUtil::jsonStringToValue(jsonServiceParams);
		message[OperationParam::ServiceParams.getValue()] = jsonData;

		ServerCall *sc = new ServerCall(ServiceName::Mail, ServiceOperation::SendAdvancedEmail, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMail::sendAdvancedEmailByAddress(const char *emailAddress, const std::string &jsonServiceParams, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EmailAddress.getValue()] = emailAddress;

		Json::Value jsonData = JsonUtil::jsonStringToValue(jsonServiceParams);
		message[OperationParam::ServiceParams.getValue()] = jsonData;

		ServerCall *sc = new ServerCall(ServiceName::Mail, ServiceOperation::SendAdvancedEmailByAddress, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMail::sendAdvancedEmailByAddresses(const std::vector<std::string> &emailAddresses, const std::string &serviceParams, IServerCallback *callback)
	{
		Json::Value message;
		Json::Value emailAddresses = JsonUtil::stringVectorToJson(emailAddresses);
		message[OperationParam::EmailAddresses.getValue()] = emailAddresses;

		Json::Value jsonData = JsonUtil::jsonStringToValue(serviceParams);
		message[OperationParam::ServiceParams.getValue()] = jsonData;

		ServerCall *sc = new ServerCall(ServiceName::Mail, ServiceOperation::SendAdvancedEmailByAddresses, message, callback);
		m_client->sendRequest(sc);
	}
}