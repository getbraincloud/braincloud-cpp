// Copyright 2018 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>

namespace BrainCloud
{
	class BrainCloudClient;
	class RTTComms;
	class IRTTConnectCallback;
	class IRTTCallback;
	class IServerCallback;
	
	class BrainCloudRTT
	{
	public:
		BrainCloudRTT(RTTComms* in_comms, BrainCloudClient* in_client);
		
		enum RTTConnectionStatus
		{
			Connected,
			Disconnected,
			Connecting,
			Disconnecting
		};

		/**
		 * Requests the event server address
		 *
		 * @param callback The callback.
		 */
		void requestClientConnection(IServerCallback* in_callback);

		/**
		 * Enables Real Time event for this session.
		 * Real Time events are disabled by default. Usually events
		 * need to be polled using GET_EVENTS. By enabling this, events will
		 * be received instantly when they happen through a TCP connection to an Event Server.
		 *
		 * This function will first call requestClientConnection, then connect to the address
		 *
		 * @param callback The callback.
		 * @param useWebSocket Use web sockets instead of TCP for the internal connections. Default is true
		 */
		void enableRTT(IRTTConnectCallback* in_callback, bool in_useWebSocket = true);

		/**
		 * Disables Real Time event for this session.
		 */
		void disableRTT();

		/**
		 *returns true if RTT is enabled 
		 */
		bool getRTTEnabled();

		/**
		 *returns rtt connection Status
		 */
		BrainCloudRTT::RTTConnectionStatus getConnectionStatus();

		const std::string& getRTTConnectionId() const;

		/**
		 * Listen to real time events.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one event callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
		void registerRTTEventCallback(IRTTCallback* in_callback);
		void deregisterRTTEventCallback();

		/**
		 * Listen to real time chat messages.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one chat callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
		void registerRTTChatCallback(IRTTCallback* in_callback);
		void deregisterRTTChatCallback();

		/**
		 * Listen to real time messaging.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one messaging callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
		void registerRTTMessagingCallback(IRTTCallback* in_callback);
		void deregisterRTTMessagingCallback();

		/**
		 * Listen to real time lobby events.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one lobby callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
		void registerRTTLobbyCallback(IRTTCallback* in_callback);
		void deregisterRTTLobbyCallback();

		/**
		 * Listen to real time presence events.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one presence callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
		void registerRTTPresenceCallback(IRTTCallback* in_callback);
		void deregisterRTTPresenceCallback();

		/**
		 * Listen to real time blockchain events.
		 * 
		 * Notes: RTT must be enabled for this app, and enableRTT must have been successfully called.
		 * Only one presence callback can be registered at a time. Calling this a second time will override the previous callback.
		 */
		void registerRTTBlockchainRefresh(IRTTCallback* in_callback);
		void deregisterRTTBlockchainRefresh();

		/**
		 * Clear all set RTT callbacks
		 */
		void deregisterAllRTTCallbacks();

	private:
		BrainCloudClient* m_client;
		RTTComms* m_commsLayer;
	};
};
#if defined(__clang__)
#pragma clang diagnostic pop
#endif

