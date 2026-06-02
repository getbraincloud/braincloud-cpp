// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include "braincloud/BrainCloudTypes.h"
#include <string>

#include "braincloud/IServerCallback.h"

namespace BrainCloud
{
	class IServerCallback;
	class BrainCloudClient;

	class BrainCloudScript
	{
	public:
		BrainCloudScript(BrainCloudClient* client);

		/**
		 * Executes a script on the server.
		 *
		 * Service Name - Script
		 * Service Operation - RUN
		 *
		 * @param scriptName The name of the script to be run
		 * @param jsonScriptData Data to be sent to the script in json format
		 * @param callback The method to be invoked when the server response is received
		 */
		void runScript(const char * scriptName, const std::string & jsonScriptData, IServerCallback * callback = NULL);

		/**
		 * Allows cloud script executions to be scheduled - UTC time
		 *
		 * Service Name - Script
		 * Service Operation - SCHEDULE_CLOUD_SCRIPT
		 *
		 * @param scriptName The name of the script to be run
		 * @param jsonScriptData Data to be sent to the script in json format
		 * @param startDateInUTC The start date in UTC
		 * @param callback The method to be invoked when the server response is received
		 */
		void scheduleRunScriptMillisUTC(const char * scriptName, const char * jsonScriptData, int64_t startDateInUTC, IServerCallback * callback = NULL);

		/**
		 * Allows cloud script executions to be scheduled
		 *
		 * Service Name - Script
		 * Service Operation - SCHEDULE_CLOUD_SCRIPT
		 *
		 * @param scriptName The name of the script to be run
		 * @param jsonScriptData Data to be sent to the script in json format
		 * @param minutesFromNow Number of minutes from now to run script
		 * @param callback The method to be invoked when the server response is received
		 */
		void scheduleRunScriptMinutes(const char * scriptName, const char * jsonScriptData, int minutesFromNow, IServerCallback * callback = NULL);

		/**
		 * Run a cloud script in a parent app
		 *
		 * Service Name - Script
		 * Service Operation - RUN_PARENT_SCRIPT
		 *
		 * @param scriptName The name of the script to be run
		 * @param scriptData Data to be sent to the script in json format
		 * @param parentLevel The level name of the parent to run the script from
		 * @param callback The method to be invoked when the server response is received
		 */
		void runParentScript(const char * scriptName, const char * jsonScriptData, const char * parentLevel, IServerCallback * callback = NULL);

		/**
		* Cancels a scheduled cloud code script
		*
		* Service Name - Script
		* Service Operation - CANCEL_SCHEDULED_SCRIPT
		*
		* @param jobId ID of script job to cancel
		* @param callback The method to be invoked when the server response is received
		*/
		void cancelScheduledScript(const char * jobId, IServerCallback * callback = NULL);

		/**
		* Cancels a scheduled cloud code script
		*
		* Service Name - Script
		* Service Operation - CANCEL_SCHEDULED_SCRIPT
		*
		* @param callback The method to be invoked when the server response is received
		*/
		void getRunningOrQueuedCloudScripts(IServerCallback * callback = NULL);

		/**
		* Cancels a scheduled cloud code script
		*
		* Service Name - Script
		* Service Operation - CANCEL_SCHEDULED_SCRIPT
		*
		* @param jobId ID of script job to cancel
		* @param callback The method to be invoked when the server response is received
		*/
		void getScheduledCloudScripts(const struct tm* startDateInUTC, IServerCallback * callback = NULL);

		/**
		* Runs a script from the context of a peer
		*
		* Service Name - Script
		* Service Operation - RUN_PEER_SCRIPT
		*
		* @param scriptName The name of the script to be run
		* @param jsonScriptData Data to be sent to the script in json format
		* @param callback The method to be invoked when the server response is received
		*/
		void runPeerScript(const char * scriptName, const std::string & jsonScriptData, const char * peer, IServerCallback * callback = NULL);

		/**
		* Runs a script asynchronously from the context of a peer
		* This method does not wait for the script to complete before returning
		*
		* Service Name - Script
		* Service Operation - RUN_PEER_SCRIPT_ASYNC
		*
		* @param scriptName The name of the script to be run
		* @param jsonScriptData Data to be sent to the script in json format
		* @param callback The method to be invoked when the server response is received
		*/
		void runPeerScriptAsync(const char * scriptName, const std::string & jsonScriptData, const char * peer, IServerCallback * callback = NULL);

	private:
		BrainCloudClient * m_client;
	};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
