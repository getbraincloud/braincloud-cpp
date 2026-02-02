// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudScript.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"
#include <time.h>
#include "braincloud/internal/timegm.h"

namespace BrainCloud
{
	BrainCloudScript::BrainCloudScript(BrainCloudClient *client) : m_client(client) {}

	void BrainCloudScript::runScript(const char *scriptName, const std::string &jsonScriptData, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceRunScriptName.getValue()] = scriptName;

		if (StringUtil::IsOptionalParameterValid(jsonScriptData))
		{
			Json::Value jsonData = JsonUtil::jsonStringToValue(jsonScriptData);
			message[OperationParam::ScriptServiceRunScriptData.getValue()] = jsonData;
		}

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::Run, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::scheduleRunScriptMillisUTC(const char *scriptName, const char *jsonScriptData, int64_t startDateInUTC, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceRunScriptName.getValue()] = scriptName;

		if (StringUtil::IsOptionalParameterValid(jsonScriptData))
		{
			Json::Value jsonData = JsonUtil::jsonStringToValue(jsonScriptData);
			message[OperationParam::ScriptServiceRunScriptData.getValue()] = jsonData;
		}

		message[OperationParam::ScriptServiceStartDateUTC.getValue()] = (Json::Int64)startDateInUTC;

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::ScheduleCloudScript, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::scheduleRunScriptMinutes(const char *scriptName, const char *jsonScriptData, int minutesFromNow, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceRunScriptName.getValue()] = scriptName;

		if (StringUtil::IsOptionalParameterValid(jsonScriptData))
		{
			Json::Value jsonData = JsonUtil::jsonStringToValue(jsonScriptData);
			message[OperationParam::ScriptServiceRunScriptData.getValue()] = jsonData;
		}

		message[OperationParam::ScriptServiceStartMinutesFromNow.getValue()] = minutesFromNow;

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::ScheduleCloudScript, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::runParentScript(const char *scriptName, const char *jsonScriptData, const char *parentLevel, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceRunScriptName.getValue()] = scriptName;

		if (StringUtil::IsOptionalParameterValid(jsonScriptData))
		{
			Json::Value jsonData = JsonUtil::jsonStringToValue(jsonScriptData);
			message[OperationParam::ScriptServiceRunScriptData.getValue()] = jsonData;
		}

		message[OperationParam::ScriptServiceParentLevelName.getValue()] = parentLevel;

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::RunParentScript, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::cancelScheduledScript(const char *jobId, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceJobId.getValue()] = jobId;

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::CancelScheduledScript, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::getScheduledCloudScripts(const tm *startDateInUTC, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceJobId.getValue()] = startDateInUTC;

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::GetScheduledCloudScripts, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::getRunningOrQueuedCloudScripts(IServerCallback *callback)
	{
		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::GetRunningOrQueuedCloudScripts, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::runPeerScript(const char *scriptName, const std::string &jsonScriptData, const char *peer, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceRunScriptName.getValue()] = scriptName;
		message[OperationParam::Peer.getValue()] = peer;

		if (StringUtil::IsOptionalParameterValid(jsonScriptData))
		{
			Json::Value jsonData = JsonUtil::jsonStringToValue(jsonScriptData);
			message[OperationParam::ScriptServiceRunScriptData.getValue()] = jsonData;
		}

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::RunPeerScript, message, callback);
		m_client->sendRequest(sc);
	}

	void BrainCloudScript::runPeerScriptAsync(const char *scriptName, const std::string &jsonScriptData, const char *peer, IServerCallback *callback)
	{
		Json::Value message;
		message[OperationParam::ScriptServiceRunScriptName.getValue()] = scriptName;
		message[OperationParam::Peer.getValue()] = peer;

		if (StringUtil::IsOptionalParameterValid(jsonScriptData))
		{
			Json::Value jsonData = JsonUtil::jsonStringToValue(jsonScriptData);
			message[OperationParam::ScriptServiceRunScriptData.getValue()] = jsonData;
		}

		ServerCall *sc = new ServerCall(ServiceName::Script, ServiceOperation::RunPeerScriptAsync, message, callback);
		m_client->sendRequest(sc);
	}
}