// Copyright 2016 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include <vector>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudEvent
	{
	public:
		BrainCloudEvent(BrainCloudClient* in_client);

		/**
		 * Sends an event to the designated user id with the attached json data.
		 * Any events that have been sent to a user will show up in their
		 * incoming event mailbox. If the in_recordLocally flag is set to true,
		 * a copy of this event (with the exact same event id) will be stored
		 * in the sending user's "sent" event mailbox.
		 *
		 * Note that the list of sent and incoming events for a user is returned
		 * in the "ReadPlayerState" call (in the BrainCloudPlayer module).
		 *
		 * Service Name - event
		 * Service Operation - SEND
		 *
		 * @param in_toProfileId The id of the user who is being sent the event
		 * @param in_eventType The user-defined type of the event.
		 * @param in_jsonEventData The user-defined data for this event encoded in JSON.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendEvent(const char * in_toProfileId, const char * in_eventType, const std::string& in_jsonEventData, IServerCallback * in_callback = NULL);

		/**
		 * Sends an event to multiple users with the attached json data.
		 *
		 * Service Name - Event
		 * Service Operation - SEND_EVENT_TO_PROFILES
		 *
		 * @param in_toIds The profile ids of the users to send the event
		 * @param in_eventType The user-defined type of the event
		 * @param in_eventData The user-defined data for this event encoded in JSON
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendEventToProfiles(const std::vector<std::string> &in_toIds, const char *in_eventType, const std::string &in_eventData, IServerCallback *in_callback);

		/**
		 * Updates an event in the user's incoming event mailbox.
		 *
		 * Service Name - event
		 * Service Operation - UPDATE_EVENT_DATA
		 *
		 * @param in_evId The event id
		 * @param in_jsonEventData The user-defined data for this event encoded in JSON.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void updateIncomingEventData(const char * in_evId, const std::string& in_jsonEventData, IServerCallback * in_callback = NULL);

		/**
		 * Updates an event in the user's incoming event mailbox.
		 * Returns the same data as updateIncomingEventData, but returns null instead of an error if none exists.
		 *
		 * Service Name - event
		 * Service Operation - UPDATE_EVENT_DATA
		 *
		 * @param in_evId The event id
		 * @param in_jsonEventData The user-defined data for this event encoded in JSON.
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void updateIncomingEventDataIfExists(const char * in_evId, const std::string& in_jsonEventData, IServerCallback * in_callback = NULL);

		/**
		 * Delete an event out of the user's incoming mailbox.
		 *
		 * Service Name - event
		 * Service Operation - DELETE_INCOMING
		 *
		 * @param in_evId The event id
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void deleteIncomingEvent(const char * in_evId, IServerCallback * in_callback = NULL);

		/**
		 * Delete a list of events out of the user's incoming mailbox.
		 *
		 * Service Name - event
		 * Service Operation - DELETE_INCOMING_EVENTS
		 *
		 * @param in_eventIds Collection of event ids
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void deleteIncomingEvents(const std::vector<std::string> & in_eventIds, IServerCallback * in_callback = NULL);

		/**
		 * Delete any events older than the given date out of the user's incoming mailbox.
		 *
		 * Service Name - event
		 * Service Operation - DELETE_INCOMING_EVENTS_OLDER_THAN
		 *
		 * @param in_dateMillis createdAt cut-off time whereby older events will be deleted (In UTC since Epoch)
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void deleteIncomingEventsOlderThan(int64_t in_dateMillis, IServerCallback * in_callback = NULL);

		/**
		 * Delete any events of the given type older than the given date out of the user's incoming mailbox.
		 *
		 * Service Name - event
		 * Service Operation - DELETE_INCOMING_EVENTS_BY_TYPE_OLDER_THAN
		 *
		 * @param in_eventType The user-defined type of the event
		 * @param in_dateMillis createdAt cut-off time whereby older events will be deleted (In UTC since Epoch)
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void deleteIncomingEventsByTypeOlderThan(const std::string & in_eventType, int64_t in_dateMillis, IServerCallback * in_callback = NULL);

		/**
		 * Get the events currently queued for the user.
		 *
		 * Service Name - event
		 * Service Operation - GET_EVENTS
		 *
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getEvents(IServerCallback * in_callback = NULL);

		/**
		 * @deprecated Use method with new signature - Removal after september 1 2021
		 */
		DEPRECATED void sendEvent(const char * in_toPlayerId, const char * in_eventType, const std::string& in_jsonEventData, bool in_recordLocally, IServerCallback * in_callback = NULL);
			
		/**
		 * @deprecated Use method with new signature - Removal after september 1 2021
		 */
		DEPRECATED void updateIncomingEventData(const char * in_fromPlayerId, uint64_t in_eventId, const std::string& in_jsonEventData, IServerCallback * in_callback = NULL);
			
		/**
		 * @deprecated Use method with new signature - Removal after september 1 2021
		 */
		DEPRECATED void deleteIncomingEvent(const char * in_fromPlayerId, uint64_t in_eventId, IServerCallback * in_callback = NULL);
			
		/**
		 * @deprecated Use method with new signature - Removal after september 1 2021
		 */
		DEPRECATED void deleteSentEvent(const char * in_toPlayerId, uint64_t in_eventId, IServerCallback * in_callback = NULL);
			
		/**
		 * @deprecated Use method with new signature - Removal after september 1 2021
		 */
		DEPRECATED void getEvents(bool in_includeIncomingEvents, bool in_includeSentEvents, IServerCallback * in_callback = NULL);


	private:
		BrainCloudClient * m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
