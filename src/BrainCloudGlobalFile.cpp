// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudGlobalFile.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

namespace BrainCloud
{
    BrainCloudGlobalFile::BrainCloudGlobalFile(BrainCloudClient *client) : m_client(client) {}

    void BrainCloudGlobalFile::getFileInfo(const std::string &fileId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GlobalFileV3ServiceFileId.getValue()] = fileId;

        ServerCall *sc = new ServerCall(ServiceName::GlobalFileV3, ServiceOperation::GetFileInfo, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudGlobalFile::getFileInfoSimple(const std::string &folderPath, const std::string &filename, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GlobalFileV3ServiceFolderPath.getValue()] = folderPath;
        message[OperationParam::GlobalFileV3ServiceFileName.getValue()] = filename;

        ServerCall *sc = new ServerCall(ServiceName::GlobalFileV3, ServiceOperation::GetFileInfoSimple, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudGlobalFile::getGlobalCDNUrl(const std::string &fileId, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GlobalFileV3ServiceFileId.getValue()] = fileId;

        ServerCall *sc = new ServerCall(ServiceName::GlobalFileV3, ServiceOperation::GetGlobalCDNUrl, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudGlobalFile::getGlobalFileList(const std::string &folderPath, bool recurse, IServerCallback *callback)
    {
        Json::Value message;
        message[OperationParam::GlobalFileV3ServiceFolderPath.getValue()] = folderPath;
        message[OperationParam::GlobalFileV3ServiceRecurse.getValue()] = recurse;

        ServerCall *sc = new ServerCall(ServiceName::GlobalFileV3, ServiceOperation::GetGlobalFileList, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }
}
