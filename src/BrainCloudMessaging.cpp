// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudMessaging.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"

namespace BrainCloud
{
	BrainCloudMessaging::BrainCloudMessaging(BrainCloudClient *client)
		: m_client(client)
	{
	}

	void BrainCloudMessaging::deleteMessages(const std::string &msgbox, const std::vector<std::string> &msgIds, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::MessagingMsgbox.getValue()] = msgbox;
		message[OperationParam::MessagingMsgIds.getValue()] = JsonUtil::stringVectorToJson(msgIds);

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::DeleteMessages, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::getMessageboxes(IServerCallback *callback)
	{
		Json::Value message;

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::GetMessageboxes, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::getMessageCounts(IServerCallback *callback)
	{
		Json::Value message;

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::GetMessageCounts, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::getMessages(const std::string &msgbox, const std::vector<std::string> &msgIds, bool markAsRead, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::MessagingMsgbox.getValue()] = msgbox;
		message[OperationParam::MessagingMsgIds.getValue()] = JsonUtil::stringVectorToJson(msgIds);
		message[OperationParam::MessagingMarkAsRead.getValue()] = markAsRead;

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::GetMessages, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::getMessagesPage(const std::string &context, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::MessagingContext.getValue()] = JsonUtil::jsonStringToValue(context.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::GetMessagesPage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::getMessagesPageOffset(const std::string &context, int pageOffset, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::MessagingContext.getValue()] = context;
		message[OperationParam::MessagingPageOffset.getValue()] = pageOffset;

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::GetMessagesPageOffset, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::sendMessage(const std::vector<std::string> &toProfileIds, std::string contentJson, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::MessagingToProfileIds.getValue()] = JsonUtil::stringVectorToJson(toProfileIds);
		message[OperationParam::MessagingContentJson.getValue()] = JsonUtil::jsonStringToValue(contentJson.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::MessagingSendMessage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::sendMessageSimple(const std::vector<std::string> &toProfileIds, const std::string &messageText, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::MessagingToProfileIds.getValue()] = JsonUtil::stringVectorToJson(toProfileIds);
		message[OperationParam::MessagingText.getValue()] = messageText;

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::SendMessageSimple, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudMessaging::markMessagesRead(const std::string &msgbox, const std::vector<std::string> &msgIds, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::MessagingMsgbox.getValue()] = msgbox;
		message[OperationParam::MessagingMsgIds.getValue()] = JsonUtil::stringVectorToJson(msgIds);

		ServerCall *sc = new ServerCall(ServiceName::Messaging, ServiceOperation::MarkMessagesRead, message, callback);
		m_client->sendRequest(sc);
	}
};
