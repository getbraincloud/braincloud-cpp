// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudDataStream.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"
#include "braincloud/internal/JsonUtil.h"

namespace BrainCloud
{
    BrainCloudDataStream::BrainCloudDataStream(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudDataStream::customPageEvent(const char *eventName, const char *jsonEventProperties, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::DataStreamEventName.getValue()] = eventName;

        if (StringUtil::IsOptionalParameterValid(jsonEventProperties))
        {
            message[OperationParam::DataStreamEventProperties.getValue()] = JsonUtil::jsonStringToValue(jsonEventProperties);
        }

        ServerCall *sc = new ServerCall(ServiceName::DataStream, ServiceOperation::CustomPageEvent, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudDataStream::customScreenEvent(const char *eventName, const char *jsonEventProperties, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::DataStreamEventName.getValue()] = eventName;

        if (StringUtil::IsOptionalParameterValid(jsonEventProperties))
        {
            message[OperationParam::DataStreamEventProperties.getValue()] = JsonUtil::jsonStringToValue(jsonEventProperties);
        }

        ServerCall *sc = new ServerCall(ServiceName::DataStream, ServiceOperation::CustomScreenEvent, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudDataStream::customTrackEvent(const char *eventName, const char *jsonEventProperties, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::DataStreamEventName.getValue()] = eventName;

        if (StringUtil::IsOptionalParameterValid(jsonEventProperties))
        {
            message[OperationParam::DataStreamEventProperties.getValue()] = JsonUtil::jsonStringToValue(jsonEventProperties);
        }

        ServerCall *sc = new ServerCall(ServiceName::DataStream, ServiceOperation::CustomTrackEvent, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudDataStream::submitCrashReport(const char *crashType, const char *errorMsg, const std::string &crashJson, const char *crashLog, const char *username, const char *userEmail, const char *userNotes, bool userSubmitted, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::DataStreamCrashType.getValue()] = crashType;
        message[OperationParam::DataStreamErrorMsg.getValue()] = errorMsg;
        message[OperationParam::DataStreamCrashJson.getValue()] = JsonUtil::jsonStringToValue(crashJson);
        message[OperationParam::DataStreamCrashLog.getValue()] = crashLog;
        message[OperationParam::DataStreamUsername.getValue()] = username;
        message[OperationParam::DataStreamUserEmail.getValue()] = userEmail;
        message[OperationParam::DataStreamUserNotes.getValue()] = userNotes;
        message[OperationParam::DataStreamUserSubmitted.getValue()] = userSubmitted;

        ServerCall *sc = new ServerCall(ServiceName::DataStream, ServiceOperation::SubmitCrashReport, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }
}