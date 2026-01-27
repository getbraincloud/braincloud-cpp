// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudS3Handling.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include "braincloud/internal/StringUtil.h"

namespace BrainCloud
{
    BrainCloudS3Handling::BrainCloudS3Handling(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudS3Handling::getUpdatedFiles(const char *category, const char *fileDetails, IServerCallback *callback)
    {
        Json::Value message;
        Json::Reader reader;

        if (StringUtil::IsOptionalParameterValid(category))
        {
            message[OperationParam::S3HandlingServiceCategory.getValue()] = category;
        }

        Json::Value parsedDetails;
        reader.parse(fileDetails, parsedDetails);

        message[OperationParam::S3HandlingServiceFileDetails.getValue()] = parsedDetails;
        ServerCall *sc = new ServerCall(ServiceName::S3Handling, ServiceOperation::GetUpdatedFiles, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudS3Handling::getFileList(const char *category, IServerCallback *callback)
    {
        Json::Value message;

        if (StringUtil::IsOptionalParameterValid(category))
        {
            message[OperationParam::S3HandlingServiceCategory.getValue()] = category;
        }

        ServerCall *sc = new ServerCall(ServiceName::S3Handling, ServiceOperation::GetFileList, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudS3Handling::getCDNUrl(const char *fileId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::S3HandlingServiceFileId.getValue()] = fileId;

        ServerCall *sc = new ServerCall(ServiceName::S3Handling, ServiceOperation::GetCdnUrl, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }
}