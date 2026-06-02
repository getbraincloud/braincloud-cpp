// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include "braincloud/BrainCloudTypes.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "json/json.h"

#include <atomic>
#include <condition_variable>
#include <map>
#include <string>
#include <thread>
#include <vector>

namespace BrainCloud
{
	class BrainCloudClient;

	class BrainCloudLobby
	{
	public:
		BrainCloudLobby(BrainCloudClient* client);

		/**
		 * Set to true to enable logging ping requests to std::out
		 */
		void enableLogging(bool shouldEnable);

		/* Retrieves the region settings for each of the given lobby types. Upon success or afterwards, call pingRegions to start retrieving appropriate data.
		 *
		 * Service Name - Lobby
		 * Service Operation - GET_REGIONS_FOR_LOBBIES
		 *
		 * @param roomTypes Ids of the lobby types.
		 */
		void getRegionsForLobbies(const std::vector<std::string> &roomTypes, IServerCallback *callback = NULL);

		/* Retrieves associated Ping Data averages to be used with all associated <>WithPingData APIs.
		 * Call anytime after GetRegionsForLobbies before proceeding.
		 * Once that completes, the associated region Ping Data is retrievable via getPingData and all associated <>WithPingData APIs are useable
		 */
		void pingRegions(IServerCallback* callback);

		/* Returns the ping data collected after (or during) a pingRegions call.
		 * Thread-safe: acquires an internal mutex so it is safe to call while
		 * pingRegions is still in progress — partial results are returned as
		 * regions complete. Regions not yet done are absent from the returned map.
		 * Returns a copy so the caller holds a stable snapshot.
		 */
		std::map<std::string, int> getPingData() const;

		/**
		 * Creates a new lobby.
		 * 
		 * Sends LOBBY_JOSUCCESS message to the user, with full copy of lobby data Sends LOBBY_MEMBER_JOINED to all lobby members, with copy of member data
		 *
		 * Service Name - Lobby
		 * Service Operation - CREATE_LOBBY
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
		 * @param settings Configuration data for the room.
		 */
		void createLobby(const std::string& lobbyType, int rating, const std::vector<std::string>& otherUserCxIds, bool isReady, const std::string& extraJson, const std::string& teamCode, const std::string& jsonSettings, IServerCallback* callback = NULL);

		/**
		 * Creates a new lobby. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
		 * 
		 * Sends LOBBY_JOSUCCESS message to the user, with full copy of lobby data Sends LOBBY_MEMBER_JOINED to all lobby members, with copy of member data
		 *
		 * Service Name - Lobby
		 * Service Operation - CREATE_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
		 * @param settings Configuration data for the room.
		 */
		void createLobbyWithPingData(const std::string& lobbyType, int rating, const std::vector<std::string>& otherUserCxIds, bool isReady, const std::string& extraJson, const std::string& teamCode, const std::string& jsonSettings, IServerCallback* callback = NULL);

		/**
		 * Creates a new lobby with server config overrides.
		 *
		 * Service Name - Lobby
		 * Service Operation - CREATE_LOBBY_WITH_CONFIG
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
		 * @param settings Configuration data for the room.
		 * @param jsonConfigOverrides Server config overrides for the lobby.
		 */
		void createLobbyWithConfig(const std::string& lobbyType, int rating, const std::vector<std::string>& otherUserCxIds, bool isReady, const std::string& extraJson, const std::string& teamCode, const std::string& jsonSettings, const std::string& jsonConfigOverrides, IServerCallback* callback = NULL);

		/**
		 * Creates a new lobby with server config overrides. Uses attached ping data to resolve best location.
		 *
		 * Service Name - Lobby
		 * Service Operation - CREATE_LOBBY_WITH_CONFIG_AND_PING_DATA
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
		 * @param settings Configuration data for the room.
		 * @param jsonConfigOverrides Server config overrides for the lobby.
		 */
		void createLobbyWithConfigAndPingData(const std::string& lobbyType, int rating, const std::vector<std::string>& otherUserCxIds, bool isReady, const std::string& extraJson, const std::string& teamCode, const std::string& jsonSettings, const std::string& jsonConfigOverrides, IServerCallback* callback = NULL);

		/**
		 * Finds a lobby matching the specified parameters. Asynchronous - returns 200 to indicate that matchmaking has started.
		 *
		 * Service Name - Lobby
		 * Service Operation - FIND_LOBBY
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
		 * @param maxSteps The maximum number of steps to wait when looking for an applicable lobby. Each step is ~5 seconds.
		 * @param algo The algorithm to use for increasing the search scope.
		 * @param filterJson Used to help filter the list of rooms to consider. Passed to the matchmaking filter, if configured.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment
		 */
		void findLobby(const std::string& lobbyType, int rating, int maxSteps, const std::string& jsonAlgo, const std::string& jsonFilter, const std::vector<std::string>& otherUserCxIds, bool isReady, const std::string& extraJson, const std::string& teamCode, IServerCallback* callback = NULL);

		/**
		 * Finds a lobby matching the specified parameters. Asynchronous - returns 200 to indicate that matchmaking has started. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
		 *
		 * Service Name - Lobby
		 * Service Operation - FIND_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
		 * @param maxSteps The maximum number of steps to wait when looking for an applicable lobby. Each step is ~5 seconds.
		 * @param algo The algorithm to use for increasing the search scope.
		 * @param filterJson Used to help filter the list of rooms to consider. Passed to the matchmaking filter, if configured.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment
		 */
		void findLobbyWithPingData(const std::string& lobbyType, int rating, int maxSteps, const std::string& jsonAlgo, const std::string& jsonFilter, const std::vector<std::string>& otherUserCxIds, bool isReady, const std::string& extraJson, const std::string& teamCode, IServerCallback* callback = NULL);

		/**
		 * Adds the caller to the lobby entry queue and will create a lobby if none are found.
		 *
		 * Service Name - Lobby
		 * Service Operation - FIND_OR_CREATE_LOBBY
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
		 * @param maxSteps The maximum number of steps to wait when looking for an applicable lobby. Each step is ~5 seconds.
		 * @param algo The algorithm to use for increasing the search scope.
		 * @param filterJson Used to help filter the list of rooms to consider. Passed to the matchmaking filter, if configured.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
		 * @param settings Configuration data for the room.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
		 */
		void findOrCreateLobby(const std::string& lobbyType, int rating, int maxSteps, const std::string& jsonAlgo, const std::string& jsonFilter, const std::vector<std::string>& otherUserCxIds, const std::string& jsonSettings, bool isReady, const std::string& extraJson, const std::string& teamCode, IServerCallback* callback = NULL);

		/**
		 * Adds the caller to the lobby entry queue and will create a lobby if none are found. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
		 *
		 * Service Name - Lobby
		 * Service Operation - FIND_OR_CREATE_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby to look for. Lobby types are defined in the portal.
		 * @param rating The skill rating to use for finding the lobby. Provided as a separate parameter because it may not exactly match the user's rating (especially in cases where parties are involved).
		 * @param maxSteps The maximum number of steps to wait when looking for an applicable lobby. Each step is ~5 seconds.
		 * @param algo The algorithm to use for increasing the search scope.
		 * @param filterJson Used to help filter the list of rooms to consider. Passed to the matchmaking filter, if configured.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.
		 * @param settings Configuration data for the room.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param teamCode Preferred team for this user, if applicable. Send "" or null for automatic assignment.
		 */
		void findOrCreateLobbyWithPingData(const std::string& lobbyType, int rating, int maxSteps, const std::string& jsonAlgo, const std::string& jsonFilter, const std::vector<std::string>& otherUserCxIds, const std::string& jsonSettings, bool isReady, const std::string& extraJson, const std::string& teamCode, IServerCallback* callback = NULL);

		/**
		 * Returns the data for the specified lobby, including member data.
		 *
		 * Service Name - Lobby
		 * Service Operation - GET_LOBBY_DATA
		 *
		 * @param lobbyId Id of chosen lobby.
		 */
		void getLobbyData(const std::string& lobbyId, IServerCallback* callback = NULL);

		/**
		 * Causes the caller to leave the specified lobby. If the user was the owner, a new owner will be chosen. If user was the last member, the lobby will be deleted.
		 *
		 * Service Name - Lobby
		 * Service Operation - LEAVE_LOBBY
		 *
		 * @param lobbyId Id of chosen lobby.
		 */
		void leaveLobby(const std::string& lobbyId, IServerCallback* callback = NULL);

		/**
		 * Evicts the specified user from the specified lobby. The caller must be the owner of the lobby.
		 *
		 * Service Name - Lobby
		 * Service Operation - REMOVE_MEMBER
		 *
		 * @param lobbyId Id of chosen lobby.
		 * @param cxId Specified member to be removed from the lobby.
		 */
		void removeMember(const std::string& lobbyId, const std::string& cxId, IServerCallback* callback = NULL);

		/**
		 * Sends LOBBY_SIGNAL_DATA message to all lobby members.
		 *
		 * Service Name - Lobby
		 * Service Operation - SEND_SIGNAL
		 *
		 * @param lobbyId Id of chosen lobby.
		 * @param signalData Signal data to be sent.
		 */
		void sendSignal(const std::string& lobbyId, const std::string& jsonSignalData, IServerCallback* callback = NULL);

		/**
		 * Switches to the specified team (if allowed.)
		 * 
		 * Sends LOBBY_MEMBER_UPDATED to all lobby members, with copy of member data
		 *
		 * Service Name - Lobby
		 * Service Operation - SWITCH_TEAM
		 *
		 * @param lobbyId Id of chosen lobby.
		 * @param toTeamCode Specified team code.
		 */
		void switchTeam(const std::string& lobbyId, const std::string& toTeamCode, IServerCallback* callback = NULL);

		/**
		 * Updates the ready status and extra json for the given lobby member.
		 *
		 * Service Name - Lobby
		 * Service Operation - UPDATE_READY
		 *
		 * @param lobbyId The type of lobby to look for. Lobby types are defined in the portal.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 */
		void updateReady(const std::string& lobbyId, bool isReady, const std::string& extraJson, IServerCallback* callback = NULL);

		/**
		 * Updates the ready status and extra json for the given lobby member.
		 *
		 * Service Name - Lobby
		 * Service Operation - UPDATE_SETTINGS
		 *
		 * @param lobbyId Id of the specfified lobby.
		 * @param settings Configuration data for the room.
		 */
		void updateSettings(const std::string& lobbyId, const std::string& jsonSettings, IServerCallback* callback = NULL);
				
		/**
		 * Join specified lobby
		 *
		 * Service Name - Lobby
		 * Service Operation - JOIN_LOBBY
		 *
		 * @param lobbyId Id of the specfified lobby.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param toTeamCode Specified team code.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.

		 */
		void joinLobby(const std::string lobbyId, bool isReady, const std::string& extraJson, std::string teamCode, const std::vector<std::string>& otherUserCxIds, IServerCallback* callback);
				
		/**
		 * Join specified lobby. Uses attached ping data to resolve best location. GetRegionsForLobbies and PingRegions must be successfully responded to.
		 *
		 * Service Name - Lobby
		 * Service Operation - JOIN_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyId Id of the specfified lobby.
		 * @param isReady Initial ready-status of this user.
		 * @param extraJson Initial extra-data about this user.
		 * @param toTeamCode Specified team code.
		 * @param otherUserCxIds Array of other users (i.e. party members) to add to the lobby as well. Will constrain things so that only lobbies with room for all players will be considered.

		 */
		void joinLobbyWithPingData(const std::string lobbyId, bool isReady, const std::string& extraJson, std::string teamCode, const std::vector<std::string>& otherUserCxIds, IServerCallback* callback);

		/// <summary>
		/// Cancel this members Find, Join and Searching of Lobbies
		/// </summary>
		void cancelFindRequest(const std::string& lobbyType, std::string entryId, IServerCallback* callback);

		/* Runs ping callbacks if pingRegions was called.
		 * Note: This is already called by BrainCloudClient::runCallbacks
		 */
		void runPingCallbacks();

		/**
		 * Gets a map keyed by rating of the visible lobby instances matching the given type and rating range.
		 *
		 * Service Name - Lobby
		 * Service Operation - GET_LOBBY_INSTANCES
		 *
		 * @param lobbyType The type of lobby to look for.
		 * @param criteriaJson A JSON string used to describe filter criteria.
		 */
		void getLobbyInstances(const std::string &lobbyType, const std::string &criteriaJson, IServerCallback* callback);

		/**
		 * Gets a map keyed by rating of the visible lobby instances matching the given type and rating range.
		 * Only lobby instances in the regions that satisfy the ping portion of the criteriaJson (based on the values provided in pingData) will be returned.
		 *
		 * Service Name - Lobby
		 * Service Operation - GET_LOBBY_INSTANCES_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby to look for.
		 * @param criteriaJson A JSON string used to describe filter criteria.
		 */
		void getLobbyInstancesWithPingData(const std::string &lobbyType, const std::string &criteriaJson, IServerCallback* callback);

	private:
		class GetRegionsForLobbiesCallback final : public IServerCallback
		{
		public:
			GetRegionsForLobbiesCallback(BrainCloudLobby* pBrainCloudLobby);
			void setExternalCallback(IServerCallback* callback);

		private:
			void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const& jsonData) override;
			void serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string& jsonError) override;

			BrainCloudLobby* m_pBrainCloudLobby;
			IServerCallback* m_pExternalCallback;
		};

		class PingRegionsThread final
		{
		public:
			PingRegionsThread(BrainCloudLobby* pBrainCloudLobby);
			~PingRegionsThread();

			bool isRunning() const;
			void stop();
			void start(const std::map<std::string, std::string>& pingRegions);

		private:
			std::atomic<bool> m_isRunning;
			BrainCloudLobby* m_pBrainCloudLobby;
			std::thread m_thread;
			std::mutex m_mutex;
			std::condition_variable m_condition;
		};

		struct ErrorCallbackEvent
		{
			IServerCallback* callback;
			ServiceName serviceName;
			ServiceOperation serviceOperation;
			int statusCode;
			int reasonCode;
			std::string jsonError;
		};

		friend class GetRegionsForLobbiesCallback;
		friend class PingRegionsThread;

		void attachPingDataAndSend(ServiceName serviceName, ServiceOperation serviceOperation, Json::Value& data, IServerCallback* callback);

		std::atomic<bool> _loggingEnabled;
		BrainCloudClient* m_client;
		GetRegionsForLobbiesCallback m_getRegionsForLobbiesCallback;
		PingRegionsThread m_pingRegionsThread;
		std::map<std::string, int> m_pingData;
		mutable std::mutex m_pingDataMutex; /* protects m_pingData for incremental cross-thread reads */
		std::map<std::string, std::string> m_pingRegions;
		IServerCallback* m_pingCallback;
		std::vector<ErrorCallbackEvent> m_errorCallbackQueue;
	};
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
