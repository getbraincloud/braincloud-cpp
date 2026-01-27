// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include "braincloud/BrainCloudTypes.h"

#include <string>
#include <vector>

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;

	/**
	 * The BrainCloudPresence service allows tracking and updating user presence
	 * information in real-time (RTT). This includes friends, groups, and arbitrary
	 * profiles, and supports bidirectional listener registration.
	 */
	class BrainCloudPresence
	{
	public:
		BrainCloudPresence(BrainCloudClient *client);

		/**
		 * Force an RTT presence update to all listeners of the caller.
		 *
		 * Service Name - presence
		 * Service Operation - FORCE_PUSH
		 *
		 * @param callback The callback invoked when the server response is received.
		 */
		void forcePush(IServerCallback *callback = nullptr);

		/**
		 * Retrieves the presence data for friends on the specified platform.
		 *
		 * Service Name - presence
		 * Service Operation - GET_PRESENCE_OF_FRIENDS
		 *
		 * @param platform One of "all", "brainCloud", or "facebook".
		 * @param includeOffline If true, includes offline profiles.
		 * @param callback Callback invoked when the server response is received.
		 */
		void getPresenceOfFriends(const std::string &platform, bool includeOffline, IServerCallback *callback = nullptr);

		/**
		 * Retrieves the presence data for members of a given group.
		 *
		 * Service Name - presence
		 * Service Operation - GET_PRESENCE_OF_GROUP
		 *
		 * @param groupId Group ID to query.
		 * @param includeOffline If true, includes offline profiles.
		 * @param callback Callback invoked when the server response is received.
		 */
		void getPresenceOfGroup(const std::string &groupId, bool includeOffline, IServerCallback *callback = nullptr);

		/**
		 * Retrieves the presence data for the specified users.
		 *
		 * Service Name - presence
		 * Service Operation - GET_PRESENCE_OF_USERS
		 *
		 * @param profileIds Vector of profile IDs to query.
		 * @param includeOffline If true, includes offline profiles.
		 * @param callback Callback invoked when the server response is received.
		 */
		void getPresenceOfUsers(const std::vector<std::string> &profileIds, bool includeOffline, IServerCallback *callback = nullptr);

		/**
		 * Registers the caller for RTT presence updates from friends on a given platform.
		 *
		 * Service Name - presence
		 * Service Operation - REGISTER_LISTENERS_FOR_FRIENDS
		 *
		 * @param platform One of "all", "brainCloud", or "facebook".
		 * @param bidirectional If true, also registers targeted users for updates from the caller.
		 * @param callback Callback invoked when the server response is received.
		 */
		void registerListenersForFriends(const std::string &platform, bool bidirectional, IServerCallback *callback = nullptr);

		/**
		 * Registers the caller for RTT presence updates from members of a given group.
		 *
		 * Service Name - presence
		 * Service Operation - REGISTER_LISTENERS_FOR_GROUP
		 *
		 * @param groupId Group ID to listen to. Caller must be a member.
		 * @param bidirectional If true, also registers targeted users for updates from the caller.
		 * @param callback Callback invoked when the server response is received.
		 */
		void registerListenersForGroup(const std::string &groupId, bool bidirectional, IServerCallback *callback = nullptr);

		/**
		 * Registers the caller for RTT presence updates from specific profiles.
		 *
		 * Service Name - presence
		 * Service Operation - REGISTER_LISTENERS_FOR_PROFILES
		 *
		 * @param profileIds Vector of profile IDs to listen to.
		 * @param bidirectional If true, also registers targeted users for updates from the caller.
		 * @param callback Callback invoked when the server response is received.
		 */
		void registerListenersForProfiles(const std::vector<std::string> &profileIds, bool bidirectional, IServerCallback *callback = nullptr);

		/**
		 * Updates the visibility field of the caller's presence data.
		 *
		 * Service Name - presence
		 * Service Operation - SET_VISIBILITY
		 *
		 * @param visible True to make the caller visible, false to hide.
		 * @param callback Callback invoked when the server response is received.
		 */
		void setVisibility(bool visible, IServerCallback *callback = nullptr);

		/**
		 * Stops the caller from receiving RTT presence updates.
		 * Does not affect broadcasting of the caller's own presence updates.
		 *
		 * Service Name - presence
		 * Service Operation - STOP_LISTENING
		 *
		 * @param callback Callback invoked when the server response is received.
		 */
		void stopListening(IServerCallback *callback = nullptr);

		/**
		 * Updates the activity field of the caller's presence data.
		 *
		 * Service Name - presence
		 * Service Operation - UPDATE_ACTIVITY
		 *
		 * @param jsonActivity JSON string representing activity information.
		 * @param callback Callback invoked when the server response is received.
		 */
		void updateActivity(const std::string &jsonActivity, IServerCallback *callback = nullptr);

	private:
		BrainCloudClient *m_client;
	};
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
