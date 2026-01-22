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
		BrainCloudPresence(BrainCloudClient *in_client);

		/**
		 * Force an RTT presence update to all listeners of the caller.
		 *
		 * Service Name - Presence
		 * Service Operation - ForcePush
		 *
		 * @param in_callback The callback invoked when the server response is received.
		 */
		void forcePush(IServerCallback *in_callback = nullptr);

		/**
		 * Retrieves the presence data for friends on the specified platform.
		 *
		 * @param in_platform One of "all", "brainCloud", or "facebook".
		 * @param in_includeOffline If true, includes offline profiles.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void getPresenceOfFriends(const std::string &in_platform, bool in_includeOffline, IServerCallback *in_callback = nullptr);

		/**
		 * Retrieves the presence data for members of a given group.
		 *
		 * @param in_groupId Group ID to query.
		 * @param in_includeOffline If true, includes offline profiles.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void getPresenceOfGroup(const std::string &in_groupId, bool in_includeOffline, IServerCallback *in_callback = nullptr);

		/**
		 * Retrieves the presence data for the specified users.
		 *
		 * @param in_profileIds Vector of profile IDs to query.
		 * @param in_includeOffline If true, includes offline profiles.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void getPresenceOfUsers(const std::vector<std::string> &in_profileIds, bool in_includeOffline, IServerCallback *in_callback = nullptr);

		/**
		 * Registers the caller for RTT presence updates from friends on a given platform.
		 *
		 * @param in_platform One of "all", "brainCloud", or "facebook".
		 * @param in_bidirectional If true, also registers targeted users for updates from the caller.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void registerListenersForFriends(const std::string &in_platform, bool in_bidirectional, IServerCallback *in_callback = nullptr);

		/**
		 * Registers the caller for RTT presence updates from members of a given group.
		 *
		 * @param in_groupId Group ID to listen to. Caller must be a member.
		 * @param in_bidirectional If true, also registers targeted users for updates from the caller.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void registerListenersForGroup(const std::string &in_groupId, bool in_bidirectional, IServerCallback *in_callback = nullptr);

		/**
		 * Registers the caller for RTT presence updates from specific profiles.
		 *
		 * @param in_profileIds Vector of profile IDs to listen to.
		 * @param in_bidirectional If true, also registers targeted users for updates from the caller.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void registerListenersForProfiles(const std::vector<std::string> &in_profileIds, bool in_bidirectional, IServerCallback *in_callback = nullptr);

		/**
		 * Updates the visibility field of the caller's presence data.
		 *
		 * @param in_visible True to make the caller visible, false to hide.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void setVisibility(bool in_visible, IServerCallback *in_callback = nullptr);

		/**
		 * Stops the caller from receiving RTT presence updates.
		 * Does not affect broadcasting of the caller's own presence updates.
		 *
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void stopListening(IServerCallback *in_callback = nullptr);

		/**
		 * Updates the activity field of the caller's presence data.
		 *
		 * @param in_jsonActivity JSON string representing activity information.
		 * @param in_callback Callback invoked when the server response is received.
		 */
		void updateActivity(const std::string &in_jsonActivity, IServerCallback *in_callback = nullptr);

	private:
		BrainCloudClient *m_client;
	};
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
