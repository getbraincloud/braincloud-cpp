// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudChat.h"
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
	BrainCloudChat::BrainCloudChat(BrainCloudClient *client)
		: m_client(client)
	{
	}

	void BrainCloudChat::channelConnect(const std::string &channelId, int maxReturn, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;
		message[OperationParam::MaxReturn.getValue()] = maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::ChannelConnect, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::channelDisconnect(const std::string &channelId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::ChannelDisconnect, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::deleteChatMessage(const std::string &channelId, const std::string &msgId, int version, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;
		message[OperationParam::MsgId.getValue()] = msgId;
		message[OperationParam::Version.getValue()] = version;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::DeleteChatMessage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::getChannelId(const std::string &channelType, const std::string &channelSubId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelType.getValue()] = channelType;
		message[OperationParam::ChannelSubId.getValue()] = channelSubId;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::GetChannelId, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::getChannelInfo(const std::string &channelId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::GetChannelInfo, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::getChatMessage(const std::string &channelId, const std::string &msgId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;
		message[OperationParam::MsgId.getValue()] = msgId;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::GetChatMessage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::getRecentChatMessages(const std::string &channelId, int maxReturn, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;
		message[OperationParam::MaxReturn.getValue()] = maxReturn;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::GetRecentChatMessages, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::getSubscribedChannels(const std::string &channelType, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelType.getValue()] = channelType;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::GetSubscribedChannels, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::postChatMessage(const std::string &channelId, const std::string &jsonContent, bool recordInHistory, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;
		message[OperationParam::Content.getValue()] = JsonUtil::jsonStringToValue(jsonContent.c_str());
		message[OperationParam::RecordInHistory.getValue()] = recordInHistory;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::PostChatMessage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::postChatMessageSimple(const std::string &channelId, const std::string &text, bool recordInHistory, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;
		Json::Value content;
		content[OperationParam::Text.getValue()] = text;
		message[OperationParam::Content.getValue()] = content;
		message[OperationParam::RecordInHistory.getValue()] = recordInHistory;

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::PostChatMessage, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudChat::updateChatMessage(const std::string &channelId, const std::string &msgId, int version, const std::string &jsonContent, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ChannelId.getValue()] = channelId;
		message[OperationParam::MsgId.getValue()] = msgId;
		message[OperationParam::Version.getValue()] = version;
		message[OperationParam::Content.getValue()] = JsonUtil::jsonStringToValue(jsonContent.c_str());

		ServerCall *sc = new ServerCall(ServiceName::Chat, ServiceOperation::UpdateChatMessage, message, callback);
		m_client->sendRequest(sc);
	}
};
