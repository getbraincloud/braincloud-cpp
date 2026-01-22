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
		BrainCloudLobby(BrainCloudClient *in_client);

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
		 * Service Name - Lobby
		 * Service Operation - GetRegionsForLobbies
		 *
		 * @param in_roomTypes Ids of the lobby types
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getRegionsForLobbies(const std::vector<std::string> &in_roomTypes, IServerCallback *in_callback = NULL);

		/**
		 * Retrieves associated ping data averages to be used with all
		 * Lobby APIs that support ping data.
		 * Must be called after getRegionsForLobbies completes successfully.
		 *
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void pingRegions(IServerCallback *in_callback);

		/**
		 * Returns the ping data collected after calling pingRegions.
		 *
		 * @return A map keyed by region containing average ping values
		 */
		const std::map<std::string, int> &getPingData() const;

		/**
		 * Creates a new lobby.
		 *
		 * Service Name - Lobby
		 * Service Operation - CreateLobby
		 *
		 * @param in_lobbyType The type of lobby to create
		 * @param in_rating The skill rating used for matchmaking
		 * @param in_otherUserCxIds Other users to add to the lobby
		 * @param in_isReady Initial ready state of this user
		 * @param in_extraJson Initial extra data for this user
		 * @param in_teamCode Preferred team code, or empty for auto assignment
		 * @param in_jsonSettings Configuration data for the lobby
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void createLobby(const std::string &in_lobbyType, int in_rating, const std::vector<std::string> &in_otherUserCxIds, bool in_isReady, const std::string &in_extraJson, const std::string &in_teamCode, const std::string &in_jsonSettings, IServerCallback *in_callback = NULL);

		/**
		 * Creates a new lobby using collected ping data to select the best region.
		 *
		 * Service Name - Lobby
		 * Service Operation - CreateLobbyWithPingData
		 *
		 * @param in_lobbyType The type of lobby to create
		 * @param in_rating The skill rating used for matchmaking
		 * @param in_otherUserCxIds Other users to add to the lobby
		 * @param in_isReady Initial ready state of this user
		 * @param in_extraJson Initial extra data for this user
		 * @param in_teamCode Preferred team code, or empty for auto assignment
		 * @param in_jsonSettings Configuration data for the lobby
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void createLobbyWithPingData(const std::string &in_lobbyType, int in_rating, const std::vector<std::string> &in_otherUserCxIds, bool in_isReady, const std::string &in_extraJson, const std::string &in_teamCode, const std::string &in_jsonSettings, IServerCallback *in_callback = NULL);

		/**
		 * Begins matchmaking to find a lobby matching the given parameters.
		 *
		 * Service Name - Lobby
		 * Service Operation - FindLobby
		 *
		 * @param in_lobbyType The type of lobby to search for
		 * @param in_rating The skill rating used for matchmaking
		 * @param in_maxSteps Maximum number of matchmaking steps
		 * @param in_jsonAlgo Matchmaking algorithm configuration
		 * @param in_jsonFilter Matchmaking filter criteria
		 * @param in_otherUserCxIds Other users to include in the lobby
		 * @param in_isReady Initial ready state of this user
		 * @param in_extraJson Initial extra data for this user
		 * @param in_teamCode Preferred team code, or empty for auto assignment
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void findLobby(const std::string &in_lobbyType, int in_rating, int in_maxSteps, const std::string &in_jsonAlgo, const std::string &in_jsonFilter, const std::vector<std::string> &in_otherUserCxIds, bool in_isReady, const std::string &in_extraJson, const std::string &in_teamCode, IServerCallback *in_callback = NULL);

		/**
		 * Begins matchmaking using ping data to select the best region.
		 *
		 * Service Name - Lobby
		 * Service Operation - FindLobbyWithPingData
		 *
		 * @param in_lobbyType The type of lobby to search for
		 * @param in_rating The skill rating used for matchmaking
		 * @param in_maxSteps Maximum number of matchmaking steps
		 * @param in_jsonAlgo Matchmaking algorithm configuration
		 * @param in_jsonFilter Matchmaking filter criteria
		 * @param in_otherUserCxIds Other users to include in the lobby
		 * @param in_isReady Initial ready state of this user
		 * @param in_extraJson Initial extra data for this user
		 * @param in_teamCode Preferred team code, or empty for auto assignment
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void findLobbyWithPingData(const std::string &in_lobbyType, int in_rating, int in_maxSteps, const std::string &in_jsonAlgo, const std::string &in_jsonFilter, const std::vector<std::string> &in_otherUserCxIds, bool in_isReady, const std::string &in_extraJson, const std::string &in_teamCode, IServerCallback *in_callback = NULL);

		/**
		 * Finds or creates a lobby if none are available.
		 *
		 * Service Name - Lobby
		 * Service Operation - FindOrCreateLobby
		 *
		 * @param in_lobbyType The type of lobby
		 * @param in_rating The skill rating used for matchmaking
		 * @param in_maxSteps Maximum number of matchmaking steps
		 * @param in_jsonAlgo Matchmaking algorithm configuration
		 * @param in_jsonFilter Matchmaking filter criteria
		 * @param in_otherUserCxIds Other users to include in the lobby
		 * @param in_jsonSettings Configuration data for the lobby
		 * @param in_isReady Initial ready state of this user
		 * @param in_extraJson Initial extra data for this user
		 * @param in_teamCode Preferred team code, or empty for auto assignment
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void findOrCreateLobby(const std::string &in_lobbyType, int in_rating, int in_maxSteps, const std::string &in_jsonAlgo, const std::string &in_jsonFilter, const std::vector<std::string> &in_otherUserCxIds, const std::string &in_jsonSettings, bool in_isReady, const std::string &in_extraJson, const std::string &in_teamCode, IServerCallback *in_callback = NULL);

		/**
		 * Finds or creates a lobby using ping data.
		 *
		 * Service Name - Lobby
		 * Service Operation - FindOrCreateLobbyWithPingData
		 *
		 * @param in_lobbyType The type of lobby
		 * @param in_rating The skill rating used for matchmaking
		 * @param in_maxSteps Maximum number of matchmaking steps
		 * @param in_jsonAlgo Matchmaking algorithm configuration
		 * @param in_jsonFilter Matchmaking filter criteria
		 * @param in_otherUserCxIds Other users to include in the lobby
		 * @param in_jsonSettings Configuration data for the lobby
		 * @param in_isReady Initial ready state of this user
		 * @param in_extraJson Initial extra data for this user
		 * @param in_teamCode Preferred team code, or empty for auto assignment
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void findOrCreateLobbyWithPingData(const std::string &in_lobbyType, int in_rating, int in_maxSteps, const std::string &in_jsonAlgo, const std::string &in_jsonFilter, const std::vector<std::string> &in_otherUserCxIds, const std::string &in_jsonSettings, bool in_isReady, const std::string &in_extraJson, const std::string &in_teamCode, IServerCallback *in_callback = NULL);

		/**
		 * Retrieves full lobby data for the specified lobby.
		 *
		 * Service Name - Lobby
		 * Service Operation - GetLobbyData
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getLobbyData(const std::string &in_lobbyId, IServerCallback *in_callback = NULL);

		/**
		 * Leaves the specified lobby.
		 *
		 * Service Name - Lobby
		 * Service Operation - LeaveLobby
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void leaveLobby(const std::string &in_lobbyId, IServerCallback *in_callback = NULL);

		/**
		 * Removes a member from the lobby. Caller must be the lobby owner.
		 *
		 * Service Name - Lobby
		 * Service Operation - RemoveMember
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_cxId The cxId of the member to remove
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void removeMember(const std::string &in_lobbyId, const std::string &in_cxId, IServerCallback *in_callback = NULL);

		/**
		 * Sends a signal to all lobby members.
		 *
		 * Service Name - Lobby
		 * Service Operation - SendSignal
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_jsonSignalData Signal payload to send
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void sendSignal(const std::string &in_lobbyId, const std::string &in_jsonSignalData, IServerCallback *in_callback = NULL);

		/**
		 * Switches the caller to a different team within the lobby.
		 *
		 * Service Name - Lobby
		 * Service Operation - SwitchTeam
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_toTeamCode Target team code
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void switchTeam(const std::string &in_lobbyId, const std::string &in_toTeamCode, IServerCallback *in_callback = NULL);

		/**
		 * Updates the ready state and extra data for the caller.
		 *
		 * Service Name - Lobby
		 * Service Operation - UpdateReady
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_isReady Updated ready state
		 * @param in_extraJson Updated extra data
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void updateReady(const std::string &in_lobbyId, bool in_isReady, const std::string &in_extraJson, IServerCallback *in_callback = NULL);

		/**
		 * Updates the lobby settings.
		 *
		 * Service Name - Lobby
		 * Service Operation - UpdateSettings
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_jsonSettings Updated lobby settings
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void updateSettings(const std::string &in_lobbyId, const std::string &in_jsonSettings, IServerCallback *in_callback = NULL);

		/**
		 * Joins the specified lobby.
		 *
		 * Service Name - Lobby
		 * Service Operation - JoinLobby
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_isReady Initial ready state
		 * @param in_extraJson Initial extra data
		 * @param in_teamCode Preferred team code
		 * @param in_otherUserCxIds Other users to include
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void joinLobby(const std::string in_lobbyId, bool in_isReady, const std::string &in_extraJson, std::string in_teamCode, const std::vector<std::string> &in_otherUserCxIds, IServerCallback *in_callback);

		/**
		 * Joins the specified lobby using ping data.
		 *
		 * Service Name - Lobby
		 * Service Operation - JoinLobbyWithPingData
		 *
		 * @param in_lobbyId The lobby identifier
		 * @param in_isReady Initial ready state
		 * @param in_extraJson Initial extra data
		 * @param in_teamCode Preferred team code
		 * @param in_otherUserCxIds Other users to include
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void joinLobbyWithPingData(const std::string in_lobbyId, bool in_isReady, const std::string &in_extraJson, std::string in_teamCode, const std::vector<std::string> &in_otherUserCxIds, IServerCallback *in_callback);

		/**
		 * Cancels an active find, join, or search request for lobbies.
		 *
		 * @param in_lobbyType The lobby type associated with the request
		 * @param in_entryId The entry identifier returned from matchmaking
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void cancelFindRequest(const std::string &in_lobbyType, std::string in_entryId, IServerCallback *in_callback);

		/**
		 * Executes pending ping callbacks.
		 * Called automatically by BrainCloudClient::runCallbacks.
		 */
		void runPingCallbacks();

		/**
		 * Retrieves visible lobby instances matching the given criteria.
		 *
		 * Service Name - Lobby
		 * Service Operation - GET_LOBBY_INSTANCES
		 *
		 * @param in_lobbyType The type of lobby
		 * @param in_criteriaJson JSON filter criteria
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getLobbyInstances(const std::string &in_lobbyType, const std::string &in_criteriaJson, IServerCallback *in_callback);

		/**
		 * Retrieves visible lobby instances matching the given criteria using ping data.
		 *
		 * Service Name - Lobby
		 * Service Operation - GET_LOBBY_INSTANCES_WITH_PING_DATA
		 *
		 * @param in_lobbyType The type of lobby
		 * @param in_criteriaJson JSON filter criteria
		 * @param in_callback The method to be invoked when the server response is received
		 */
		void getLobbyInstancesWithPingData(const std::string &in_lobbyType, const std::string &in_criteriaJson, IServerCallback *in_callback);

	private:
		class GetRegionsForLobbiesCallback final : public IServerCallback
		{
		public:
			GetRegionsForLobbiesCallback(BrainCloudLobby *pBrainCloudLobby);
			void setExternalCallback(IServerCallback *in_callback);

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
