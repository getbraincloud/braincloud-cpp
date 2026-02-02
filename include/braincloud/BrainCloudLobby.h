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
		BrainCloudLobby(BrainCloudClient *client);

		/**
		 * Enables or disables logging of ping requests to standard output.
		 *
		 * @param shouldEnable Set true to enable logging, false to disable
		 */
		void enableLogging(bool shouldEnable);

		/**
		 * Retrieves the region settings for each of the given lobby types.
		 * Upon success, pingRegions should be called to collect ping data.
		 *
		 * Service Name - lobby
		 * Service Operation - GET_REGIONS_FOR_LOBBIES
		 *
		 * @param roomTypes Ids of the lobby types
		 * @param callback The method to be invoked when the server response is received
		 */
		void getRegionsForLobbies(const std::vector<std::string> &roomTypes, IServerCallback *callback = NULL);

		/**
		 * Retrieves associated ping data averages to be used with all
		 * Lobby APIs that support ping data.
		 * Must be called after getRegionsForLobbies completes successfully.
		 *
		 * @param callback The method to be invoked when the server response is received
		 */
		void pingRegions(IServerCallback *callback);

		/**
		 * Returns the ping data collected after calling pingRegions.
		 *
		 * @return A map keyed by region containing average ping values
		 */
		const std::map<std::string, int> &getPingData() const;

		/**
		 * Creates a new lobby.
		 *
		 * Service Name - lobby
		 * Service Operation - CREATE_LOBBY
		 *
		 * @param lobbyType The type of lobby to create
		 * @param rating The skill rating used for matchmaking
		 * @param otherUserCxIds Other users to add to the lobby
		 * @param isReady Initial ready state of this user
		 * @param extraJson Initial extra data for this user
		 * @param teamCode Preferred team code, or empty for auto assignment
		 * @param jsonSettings Configuration data for the lobby
		 * @param callback The method to be invoked when the server response is received
		 */
		void createLobby(const std::string &lobbyType, int rating, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, const std::string &jsonSettings, IServerCallback *callback = NULL);

		/**
		 * Creates a new lobby using collected ping data to select the best region.
		 *
		 * Service Name - lobby
		 * Service Operation - CREATE_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby to create
		 * @param rating The skill rating used for matchmaking
		 * @param otherUserCxIds Other users to add to the lobby
		 * @param isReady Initial ready state of this user
		 * @param extraJson Initial extra data for this user
		 * @param teamCode Preferred team code, or empty for auto assignment
		 * @param jsonSettings Configuration data for the lobby
		 * @param callback The method to be invoked when the server response is received
		 */
		void createLobbyWithPingData(const std::string &lobbyType, int rating, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, const std::string &jsonSettings, IServerCallback *callback = NULL);

		/**
		 * Begins matchmaking to find a lobby matching the given parameters.
		 *
		 * Service Name - lobby
		 * Service Operation - FindLobby
		 *
		 * @param lobbyType The type of lobby to search for
		 * @param rating The skill rating used for matchmaking
		 * @param maxSteps Maximum number of matchmaking steps
		 * @param jsonAlgo Matchmaking algorithm configuration
		 * @param jsonFilter Matchmaking filter criteria
		 * @param otherUserCxIds Other users to include in the lobby
		 * @param isReady Initial ready state of this user
		 * @param extraJson Initial extra data for this user
		 * @param teamCode Preferred team code, or empty for auto assignment
		 * @param callback The method to be invoked when the server response is received
		 */
		void findLobby(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback = NULL);

		/**
		 * Begins matchmaking using ping data to select the best region.
		 *
		 * Service Name - lobby
		 * Service Operation - FIND_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby to search for
		 * @param rating The skill rating used for matchmaking
		 * @param maxSteps Maximum number of matchmaking steps
		 * @param jsonAlgo Matchmaking algorithm configuration
		 * @param jsonFilter Matchmaking filter criteria
		 * @param otherUserCxIds Other users to include in the lobby
		 * @param isReady Initial ready state of this user
		 * @param extraJson Initial extra data for this user
		 * @param teamCode Preferred team code, or empty for auto assignment
		 * @param callback The method to be invoked when the server response is received
		 */
		void findLobbyWithPingData(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback = NULL);

		/**
		 * Finds or creates a lobby if none are available.
		 *
		 * Service Name - lobby
		 * Service Operation - FIND_OR_CREATE_LOBBY
		 *
		 * @param lobbyType The type of lobby
		 * @param rating The skill rating used for matchmaking
		 * @param maxSteps Maximum number of matchmaking steps
		 * @param jsonAlgo Matchmaking algorithm configuration
		 * @param jsonFilter Matchmaking filter criteria
		 * @param otherUserCxIds Other users to include in the lobby
		 * @param jsonSettings Configuration data for the lobby
		 * @param isReady Initial ready state of this user
		 * @param extraJson Initial extra data for this user
		 * @param teamCode Preferred team code, or empty for auto assignment
		 * @param callback The method to be invoked when the server response is received
		 */
		void findOrCreateLobby(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, const std::string &jsonSettings, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback = NULL);

		/**
		 * Finds or creates a lobby using ping data.
		 *
		 * Service Name - lobby
		 * Service Operation - FIND_OR_CREATE_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby
		 * @param rating The skill rating used for matchmaking
		 * @param maxSteps Maximum number of matchmaking steps
		 * @param jsonAlgo Matchmaking algorithm configuration
		 * @param jsonFilter Matchmaking filter criteria
		 * @param otherUserCxIds Other users to include in the lobby
		 * @param jsonSettings Configuration data for the lobby
		 * @param isReady Initial ready state of this user
		 * @param extraJson Initial extra data for this user
		 * @param teamCode Preferred team code, or empty for auto assignment
		 * @param callback The method to be invoked when the server response is received
		 */
		void findOrCreateLobbyWithPingData(const std::string &lobbyType, int rating, int maxSteps, const std::string &jsonAlgo, const std::string &jsonFilter, const std::vector<std::string> &otherUserCxIds, const std::string &jsonSettings, bool isReady, const std::string &extraJson, const std::string &teamCode, IServerCallback *callback = NULL);

		/**
		 * Retrieves full lobby data for the specified lobby.
		 *
		 * Service Name - lobby
		 * Service Operation - GET_LOBBY_DATA
		 *
		 * @param lobbyId The lobby identifier
		 * @param callback The method to be invoked when the server response is received
		 */
		void getLobbyData(const std::string &lobbyId, IServerCallback *callback = NULL);

		/**
		 * Leaves the specified lobby.
		 *
		 * Service Name - lobby
		 * Service Operation - LEAVE_LOBBY
		 *
		 * @param lobbyId The lobby identifier
		 * @param callback The method to be invoked when the server response is received
		 */
		void leaveLobby(const std::string &lobbyId, IServerCallback *callback = NULL);

		/**
		 * Removes a member from the lobby. Caller must be the lobby owner.
		 *
		 * Service Name - lobby
		 * Service Operation - REMOVE_MEMBER
		 *
		 * @param lobbyId The lobby identifier
		 * @param cxId The cxId of the member to remove
		 * @param callback The method to be invoked when the server response is received
		 */
		void removeMember(const std::string &lobbyId, const std::string &cxId, IServerCallback *callback = NULL);

		/**
		 * Sends a signal to all lobby members.
		 *
		 * Service Name - lobby
		 * Service Operation - SEND_SIGNAL
		 *
		 * @param lobbyId The lobby identifier
		 * @param jsonSignalData Signal payload to send
		 * @param callback The method to be invoked when the server response is received
		 */
		void sendSignal(const std::string &lobbyId, const std::string &jsonSignalData, IServerCallback *callback = NULL);

		/**
		 * Switches the caller to a different team within the lobby.
		 *
		 * Service Name - lobby
		 * Service Operation - SWITCH_TEAM
		 *
		 * @param lobbyId The lobby identifier
		 * @param toTeamCode Target team code
		 * @param callback The method to be invoked when the server response is received
		 */
		void switchTeam(const std::string &lobbyId, const std::string &toTeamCode, IServerCallback *callback = NULL);

		/**
		 * Updates the ready state and extra data for the caller.
		 *
		 * Service Name - lobby
		 * Service Operation - UPDATE_READY
		 *
		 * @param lobbyId The lobby identifier
		 * @param isReady Updated ready state
		 * @param extraJson Updated extra data
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateReady(const std::string &lobbyId, bool isReady, const std::string &extraJson, IServerCallback *callback = NULL);

		/**
		 * Updates the lobby settings.
		 *
		 * Service Name - lobby
		 * Service Operation - UPDATE_SETTINGS
		 *
		 * @param lobbyId The lobby identifier
		 * @param jsonSettings Updated lobby settings
		 * @param callback The method to be invoked when the server response is received
		 */
		void updateSettings(const std::string &lobbyId, const std::string &jsonSettings, IServerCallback *callback = NULL);

		/**
		 * Joins the specified lobby.
		 *
		 * Service Name - lobby
		 * Service Operation - JOIN_LOBBY
		 *
		 * @param lobbyId The lobby identifier
		 * @param isReady Initial ready state
		 * @param extraJson Initial extra data
		 * @param teamCode Preferred team code
		 * @param otherUserCxIds Other users to include
		 * @param callback The method to be invoked when the server response is received
		 */
		void joinLobby(const std::string lobbyId, bool isReady, const std::string &extraJson, std::string teamCode, const std::vector<std::string> &otherUserCxIds, IServerCallback *callback);

		/**
		 * Joins the specified lobby using ping data.
		 *
		 * Service Name - lobby
		 * Service Operation - JOIN_LOBBY_WITH_PING_DATA
		 *
		 * @param lobbyId The lobby identifier
		 * @param isReady Initial ready state
		 * @param extraJson Initial extra data
		 * @param teamCode Preferred team code
		 * @param otherUserCxIds Other users to include
		 * @param callback The method to be invoked when the server response is received
		 */
		void joinLobbyWithPingData(const std::string lobbyId, bool isReady, const std::string &extraJson, std::string teamCode, const std::vector<std::string> &otherUserCxIds, IServerCallback *callback);

		/**
		 * Cancels an active find, join, or search request for lobbies.
		 *
		 * @param lobbyType The lobby type associated with the request
		 * @param entryId The entry identifier returned from matchmaking
		 * @param callback The method to be invoked when the server response is received
		 */
		void cancelFindRequest(const std::string &lobbyType, std::string entryId, IServerCallback *callback);

		/**
		 * Executes pending ping callbacks.
		 * Called automatically by BrainCloudClient::runCallbacks.
		 */
		void runPingCallbacks();

		/**
		 * Retrieves visible lobby instances matching the given criteria.
		 *
		 * Service Name - lobby
		 * Service Operation - GET_LOBBY_INSTANCES
		 *
		 * @param lobbyType The type of lobby
		 * @param criteriaJson JSON filter criteria
		 * @param callback The method to be invoked when the server response is received
		 */
		void getLobbyInstances(const std::string &lobbyType, const std::string &criteriaJson, IServerCallback *callback);

		/**
		 * Retrieves visible lobby instances matching the given criteria using ping data.
		 *
		 * Service Name - lobby
		 * Service Operation - GET_LOBBY_INSTANCES_WITH_PING_DATA
		 *
		 * @param lobbyType The type of lobby
		 * @param criteriaJson JSON filter criteria
		 * @param callback The method to be invoked when the server response is received
		 */
		void getLobbyInstancesWithPingData(const std::string &lobbyType, const std::string &criteriaJson, IServerCallback *callback);

	private:
		class GetRegionsForLobbiesCallback final : public IServerCallback
		{
		public:
			GetRegionsForLobbiesCallback(BrainCloudLobby *pBrainCloudLobby);
			void setExternalCallback(IServerCallback *callback);

		private:
			void serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const &jsonData) override;
			void serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string &jsonError) override;

			BrainCloudLobby *m_pBrainCloudLobby;
			IServerCallback *m_pExternalCallback;
		};

		class PingRegionsThread final
		{
		public:
			PingRegionsThread(BrainCloudLobby *pBrainCloudLobby);
			~PingRegionsThread();

			bool isRunning() const;
			void stop();
			void start(const std::map<std::string, std::string> &pingRegions);

		private:
			std::atomic<bool> m_isRunning;
			BrainCloudLobby *m_pBrainCloudLobby;
			std::thread m_thread;
			std::mutex m_mutex;
			std::condition_variable m_condition;
		};

		struct ErrorCallbackEvent
		{
			IServerCallback *callback;
			ServiceName serviceName;
			ServiceOperation serviceOperation;
			int statusCode;
			int reasonCode;
			std::string jsonError;
		};

		friend class GetRegionsForLobbiesCallback;
		friend class PingRegionsThread;

		void attachPingDataAndSend(ServiceName serviceName, ServiceOperation serviceOperation, Json::Value &data, IServerCallback *callback);

		std::atomic<bool> _loggingEnabled;
		BrainCloudClient *m_client;
		GetRegionsForLobbiesCallback m_getRegionsForLobbiesCallback;
		PingRegionsThread m_pingRegionsThread;
		std::map<std::string, int> m_pingData;
		std::map<std::string, std::string> m_pingRegions;
		IServerCallback *m_pingCallback;
		std::vector<ErrorCallbackEvent> m_errorCallbackQueue;
	};
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
