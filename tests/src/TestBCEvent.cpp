#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "TestBCEvent.h"

using namespace BrainCloud;

TEST_F(TestBCEvent, Send)
{
	m_didCallback = false;
	m_bc->registerEventCallback(this);

	SendDefaultMessage();

	EXPECT_TRUE(m_didCallback);

	DeleteIncomingMessage();
	m_bc->deregisterEventCallback();
}

TEST_F(TestBCEvent, DeleteIncoming)
{
	SendDefaultMessage();
	DeleteIncomingMessage();
}

TEST_F(TestBCEvent, DeleteIncomingEvents)
{
	TestResult tr;

	SendDefaultMessage();
	auto id1 = m_eventId;

	SendDefaultMessage();
	auto id2 = m_eventId;

	m_bc->getEventService()->deleteIncomingEvents({ id1, id2 }, &tr);
	tr.run(m_bc);

	// Get events and make sure they are not there anymore
	m_bc->getEventService()->getEvents(&tr);
	tr.run(m_bc);

	bool foundDeletedEvents = false;
	for (auto eventJson : tr.m_response["data"]["incoming_events"])
	{
		auto eventId = eventJson["evId"].asString();
		foundDeletedEvents |= eventId == id1;
		foundDeletedEvents |= eventId == id2;
	}
	EXPECT_FALSE(foundDeletedEvents);
}

TEST_F(TestBCEvent, DeleteIncomingEventsOlderThan)
{
	TestResult tr;

	SendDefaultMessage();
	auto id1 = m_eventId;

	SendDefaultMessage();
	auto id2 = m_eventId;

	// Deleted older than now + 2mins so its within the time 
	int64_t milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + (2 * 60 * 1000); //add 2 minutes 

	m_bc->getEventService()->deleteIncomingEventsOlderThan(milliseconds_since_epoch, &tr);
	tr.run(m_bc);

	// Get events and make sure they are not there anymore
	m_bc->getEventService()->getEvents(&tr);
	tr.run(m_bc);

	bool foundDeletedEvents = false;
	for (auto eventJson : tr.m_response["data"]["incoming_events"])
	{
		auto eventId = eventJson["evId"].asString();
		foundDeletedEvents |= eventId == id1;
		foundDeletedEvents |= eventId == id2;
	}
	EXPECT_FALSE(foundDeletedEvents);
}

TEST_F(TestBCEvent, DeleteIncomingEventsByTypeOlderThan)
{
	TestResult tr;

	SendDefaultMessage();
	auto id1 = m_eventId;

	SendDefaultMessage();
	auto id2 = m_eventId;

	m_bc->getEventService()->getEvents(&tr);
	tr.run(m_bc);
	int64_t createdAt2 = 0;
	const auto &eventsJson = tr.m_response["data"]["incoming_events"];
	for (const auto &eventJson : eventsJson)
	{
		if (eventJson["evId"] == id2)
		{
			createdAt2 = eventJson["createdAt"].asInt64();
			break;
		}
	}

	m_bc->getEventService()->deleteIncomingEventsByTypeOlderThan(m_eventType, createdAt2 + 1, &tr);
	tr.run(m_bc);

	m_bc->getEventService()->getEvents(&tr);
	tr.run(m_bc);
	bool foundDeletedEvents = false;
	for (auto eventJson : tr.m_response["data"]["incoming_events"])
	{
		auto eventId = eventJson["evId"].asString();
		foundDeletedEvents |= eventId == id1;
		foundDeletedEvents |= eventId == id2;
	}
	EXPECT_FALSE(foundDeletedEvents);
}

TEST_F(TestBCEvent, UpdateIncoming)
{
	SendDefaultMessage();

	TestResult tr;
	Json::FastWriter fw;
	Json::Value eventData;
	eventData[m_eventDataKey] = "testEventValue";

	m_bc->getEventService()->updateIncomingEventData(m_eventId.c_str(), fw.write(eventData).c_str(), &tr);
	tr.run(m_bc);

	DeleteIncomingMessage();
}

TEST_F(TestBCEvent, GetEvents)
{
	SendDefaultMessage();

	TestResult tr;
	m_bc->getEventService()->getEvents(true, true, &tr);
	tr.run(m_bc);

	DeleteIncomingMessage();
}

void TestBCEvent::SendDefaultMessage()
{
	TestResult tr;
	Json::FastWriter fw;
	Json::Value eventData;
	eventData[m_eventDataKey] = "testEventValue";

	m_bc->getEventService()->sendEvent(GetUser(UserA)->m_profileId, m_eventType, fw.write(eventData).c_str(), &tr);
	tr.run(m_bc);
	m_eventId = tr.m_response["data"]["evId"].asString();
}

void TestBCEvent::DeleteIncomingMessage()
{
	TestResult tr;
	m_bc->getEventService()->deleteIncomingEvent(m_eventId.c_str(), &tr);
	tr.run(m_bc);
}

void TestBCEvent::eventCallback(const std::string &jsonData)
{
	m_didCallback = true;
	printf("%s %s\n", "Events Received:", jsonData.c_str());
}




