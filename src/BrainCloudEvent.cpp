// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudEvent.h"

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
	BrainCloudEvent::BrainCloudEvent(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudEvent::sendEvent(const char *toProfileId, const char *eventType, const std::string &jsonEventData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EventServiceSendToId.getValue()] = toProfileId;
		message[OperationParam::EventServiceSendEventType.getValue()] = eventType;
		message[OperationParam::EventServiceSendEventData.getValue()] = JsonUtil::jsonStringToValue(jsonEventData);

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::Send, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::updateIncomingEventData(const char *evId, const std::string &jsonEventData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EvId.getValue()] = evId;
		message[OperationParam::EventServiceUpdateEventDataData.getValue()] = JsonUtil::jsonStringToValue(jsonEventData);

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::UpdateEventData, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::updateIncomingEventDataIfExists(const char *evId, const std::string &jsonEventData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EvId.getValue()] = evId;
		message[OperationParam::EventServiceUpdateEventDataData.getValue()] = JsonUtil::jsonStringToValue(jsonEventData);

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::UpdateEventDataIfExists, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::deleteIncomingEvent(const char *evId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::EvId.getValue()] = evId;

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::DeleteIncoming, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::deleteIncomingEvents(const std::vector<std::string> &eventIds, IServerCallback *callback)
	{
		Json::Value message;

		message[OperationParam::EventServiceEvIds.getValue()] = JsonUtil::stringVectorToJson(eventIds);

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::DeleteIncomingEvents, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::deleteIncomingEventsOlderThan(int64_t dateMillis, IServerCallback *callback)
	{
		Json::Value message;

		message[OperationParam::EventServiceDateMillis.getValue()] = (Json::UInt64)dateMillis;

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::DeleteIncomingEventsOlderThan, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::deleteIncomingEventsByTypeOlderThan(const std::string &eventType, int64_t dateMillis, IServerCallback *callback)
	{
		Json::Value message;

		message[OperationParam::EventServiceEventType.getValue()] = eventType;
		message[OperationParam::EventServiceDateMillis.getValue()] = (Json::UInt64)dateMillis;

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::DeleteIncomingEventsByTypeOlderThan, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::getEvents(IServerCallback *callback)
	{
		Json::Value message;
		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::GetEvents, message, callback);
		m_client->getBrainCloudComms()->addToQueue(sc);
	}

	void BrainCloudEvent::sendEventToProfiles(const std::vector<std::string> &toIds, const char *eventType, const std::string &eventData, IServerCallback *callback)
	{
		Json::Value data;
		Json::Value toIds = JsonUtil::stringVectorToJson(toIds);
		data[OperationParam::EventServiceToIds.getValue()] = toIds;
		data[OperationParam::EventServiceSendEventType.getValue()] = eventType;
		data[OperationParam::EventServiceSendEventData.getValue()] = JsonUtil::jsonStringToValue(eventData);

		ServerCall *sc = new ServerCall(ServiceName::Event, ServiceOperation::SendEventToProfiles, data, callback);

		m_client->getBrainCloudComms()->addToQueue(sc);
	}
}
