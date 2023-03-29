// Copyright 2020 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include "braincloud/BrainCloudTypes.h"
#include "braincloud/RelayChannel.h"
#include "braincloud/RelayConnectionType.h"
#include "braincloud/internal/JsonUtil.h"

#include <string>


namespace BrainCloud
{
	class BrainCloudClient;
    class IRelayConnectCallback;
    class IRelayCallback;
    class IRelaySystemCallback;
    class RelayComms;

    static const uint64_t TO_ALL_PLAYERS = 0x000000FFFFFFFFFF;

	class BrainCloudRelay
	{
	public:
		BrainCloudRelay(RelayComms* in_commsLayer, BrainCloudClient* in_client);

        /**
         * Start a connection, based on connection type to 
         * brainClouds Relay Servers. Connect options come in
         * from ROOM_ASSIGNED lobby callback.
         * 
         * @param connectionType
         * @param host
         * @param port
         * @param passcode
         * @param lobbyId
         * @param callback Callback objects that report Success or Failure|Disconnect.
         *
         * @note SSL option will only work with WEBSOCKET connetion type.
         */
        void connect(eRelayConnectionType in_connectionType, const std::string& host, int port, const std::string& passcode, const std::string& lobbyId, IRelayConnectCallback* in_callback);

        /**
         * Disconnects from the relay server
         */
        void disconnect();

        /**
         * Requests to end the current match on the relay server
         */
        void endMatch(const std::string&  jsonPayload);

        /**
         * Returns whether or not we have a successful connection with
         * the relay server.
         */
        bool isConnected() const;

        /**
         * Get the current ping for our user.
         * Note: Pings are not distributed among other members. Your game will
         * have to bundle it inside a packet and distribute to other peers.
         */
        int getPing() const;

        /**
         * Set the ping interval. Ping allows to keep the connection
         * alive, but also inform the player of his current ping.
         * The default is 1000 miliseconds interval. (1 seconds)
         */
        void setPingInterval(int in_intervalMS);

        /**
         * Get the lobby's owner profile Id.
         */
        const std::string& getOwnerProfileId() const;

        /**
         * Get the lobby's owner Connection Id.
         */
        const std::string& getOwnerCxId() const;

        /**
         * Returns the profileId associated with a netId.
         */
        const std::string& getProfileIdForNetId(int in_netId) const;

        /**
         * Returns the netId associated with a profileId.
         */
        int getNetIdForProfileId(const std::string& in_profileId) const;

        /**
         * Returns the connection Id associated with a netId.
         */
        const std::string& getCxIdForNetId(int in_netId) const;

        /**
         * Returns the netId associated with a connection Id.
         */
        int getNetIdForCxId(const std::string& in_cxId) const;

        /**
         * Register callback for relay messages coming from peers.
         *
         * @param callback Called whenever a relay message was received.
         */
        void registerRelayCallback(IRelayCallback* in_callback);
        void deregisterRelayCallback();

        /**
         * Register callback for RelayServer system messages.
         *
         * @param callback Called whenever a system message was received. function(json)
         *
         * # CONNECT
         * Received when a new member connects to the server.
         * {
         *   op: "CONNECT",
         *   profileId: "...",
         *   ownerId: "...",
         *   netId: #
         * }
         *
         * # NET_ID
         * Receive the Net Id assossiated with a profile Id. This is
         * sent for each already connected members once you
         * successfully connected.
         * {
         *   op: "NET_ID",
         *   profileId: "...",
         *   netId: #
         * }
         *
         * # DISCONNECT
         * Received when a member disconnects from the server.
         * {
         *   op: "DISCONNECT",
         *   profileId: "..."
         * }
         *
         * # MIGRATE_OWNER
         * If the owner left or never connected in a timely manner,
         * the relay-server will migrate the role to the next member
         * with the best ping. If no one else is currently connected
         * yet, it will be transferred to the next member in the
         * lobby members' list. This last scenario can only occur if
         * the owner connected first, then quickly disconnected.
         * Leaving only unconnected lobby members.
         * {
         *   op: "MIGRATE_OWNER",
         *   profileId: "..."
         * }
         */
        void registerSystemCallback(IRelaySystemCallback* in_callback);
        void deregisterSystemCallback();

        /**
         * Send a packet to peer(s)
         *
         * @param data Byte array for the data to send
         * @param size Size of data in bytes
         * @param toNetId The net id to send to, TO_ALL_PLAYERS to relay to all.
         * @param reliable Send this reliable or not.
         * @param ordered Receive this ordered or not.
         * @param channel One of: (CHANNEL_HIGH_PRIORITY_1, CHANNEL_HIGH_PRIORITY_2, CHANNEL_NORMAL_PRIORITY, CHANNEL_LOW_PRIORITY)
         */
        void send(const uint8_t* in_data, int in_size, uint64_t toNetId, bool in_reliable, bool in_ordered, eRelayChannel in_channel);

        /**
         * Send a packet to any players by using a mask
         *
         * @param data Byte array for the data to send
         * @param size Size of data in bytes
         * @param playerMask Mask of the players to send to. 0001 = netId 0, 0010 = netId 1, etc. If you pass ALL_PLAYER_MASK you will be included and you will get an echo for your message. Use sendToAll instead, you will be filtered out. You can manually filter out by : ALL_PLAYER_MASK &= ~(1 << myNetId)
         * @param reliable Send this reliable or not.
         * @param ordered Receive this ordered or not.
         * @param channel One of: (CHANNEL_HIGH_PRIORITY_1, CHANNEL_HIGH_PRIORITY_2, CHANNEL_NORMAL_PRIORITY, CHANNEL_LOW_PRIORITY)
         */
        void sendToPlayers(const uint8_t* in_data, int in_size, uint64_t in_playerMask, bool in_reliable, bool in_ordered, eRelayChannel in_channel);

        /**
         * Send a packet to all except yourself
         *
         * @param data Byte array for the data to send
         * @param size Size of data in bytes
         * @param reliable Send this reliable or not.
         * @param ordered Receive this ordered or not.
         * @param channel One of: (CHANNEL_HIGH_PRIORITY_1, CHANNEL_HIGH_PRIORITY_2, CHANNEL_NORMAL_PRIORITY, CHANNEL_LOW_PRIORITY)
         */
        void sendToAll(const uint8_t* in_data, int in_size, bool in_reliable, bool in_ordered, eRelayChannel in_channel);

	private:
        RelayComms* m_commsLayer;
        BrainCloudClient* m_client;
	};
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
