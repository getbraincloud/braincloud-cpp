// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudFile.h"

#include "braincloud/BrainCloudClient.h"
#include "braincloud/IServerCallback.h"
#include "braincloud/ServerCall.h"
#include "braincloud/ServiceName.h"
#include "braincloud/ServiceOperation.h"
#include "braincloud/OperationParam.h"
#include "json/json.h"

#include <stdio.h>

namespace BrainCloud
{
    BrainCloudFile::BrainCloudFile(BrainCloudClient *client) : m_client(client) {}

    bool BrainCloudFile::uploadFile(const char *cloudPath,
                                    const char *cloudFilename,
                                    bool shareable,
                                    bool replaceIfExists,
                                    const char *localPath,
                                    IServerCallback *callback)
    {
        int64_t fileSize = 0;

        // todo - improve the method of retrieving file size
        // as this has to scan the whole file
        int rc = 0;
        FILE *fp = NULL;
#if defined(WINAPI_FAMILY)
        fopen_s(&fp, localPath, "rb");
#else
        fp = fopen(localPath, "rb");
#endif
        if (fp == NULL)
        {
            return false;
        }
        if ((rc = fseek(fp, 0, SEEK_END)) != 0)
        {
            fclose(fp);
            return false;
        }
        fileSize = ftell(fp);
        fclose(fp);
        fp = NULL;
        if (fileSize <= 0)
        {
            return false;
        }

        Json::Value message;

        message[OperationParam::UploadCloudPath.getValue()] = cloudPath;
        message[OperationParam::UploadCloudFilename.getValue()] = cloudFilename;
        message[OperationParam::UploadShareable.getValue()] = shareable;
        message[OperationParam::UploadReplaceIfExists.getValue()] = replaceIfExists;
        message[OperationParam::UploadFileSize.getValue()] = (Json::Int64)fileSize;
        message[OperationParam::UploadLocalPath.getValue()] = localPath;

        ServerCall *sc = new ServerCall(ServiceName::File, ServiceOperation::PrepareUserUpload, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
        return true;
    }

    void BrainCloudFile::listUserFiles(IServerCallback *callback)
    {
        Json::Value message;

        ServerCall *sc = new ServerCall(ServiceName::File, ServiceOperation::ListUserFiles, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudFile::listUserFiles(const char *cloudPath, bool recurse, IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::UploadCloudPath.getValue()] = cloudPath;
        message[OperationParam::UploadRecurse.getValue()] = recurse;

        ServerCall *sc = new ServerCall(ServiceName::File, ServiceOperation::ListUserFiles, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudFile::deleteUserFile(const char *cloudPath, const char *cloudFilename, IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::UploadCloudPath.getValue()] = cloudPath;
        message[OperationParam::UploadCloudFilename.getValue()] = cloudFilename;

        ServerCall *sc = new ServerCall(ServiceName::File, ServiceOperation::DeleteUserFiles, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudFile::deleteUserFiles(const char *cloudPath, bool recurse, IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::UploadCloudPath.getValue()] = cloudPath;
        message[OperationParam::UploadRecurse.getValue()] = recurse;

        ServerCall *sc = new ServerCall(ServiceName::File, ServiceOperation::DeleteUserFiles, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudFile::getCDNUrl(const char *cloudPath, const char *cloudFileName, IServerCallback *callback)
    {
        Json::Value message;

        message[OperationParam::UploadCloudPath.getValue()] = cloudPath;
        message[OperationParam::UploadCloudFilename.getValue()] = cloudFileName;

        ServerCall *sc = new ServerCall(ServiceName::File, ServiceOperation::GetCdnUrl, message, callback);
        m_client->getBrainCloudComms()->addToQueue(sc);
    }

    void BrainCloudFile::cancelUpload(const char *uploadFileId)
    {
        m_client->getBrainCloudComms()->cancelUpload(uploadFileId);
    }

    double BrainCloudFile::getUploadProgress(const char *uploadFileId)
    {
        return m_client->getBrainCloudComms()->getUploadProgress(uploadFileId);
    }

    int64_t BrainCloudFile::getUploadBytesTransferred(const char *uploadFileId)
    {
        return m_client->getBrainCloudComms()->getUploadBytesTransferred(uploadFileId);
    }

    int64_t BrainCloudFile::getUploadTotalBytesToTransfer(const char *uploadFileId)
    {
        return m_client->getBrainCloudComms()->getUploadTotalBytesToTransfer(uploadFileId);
    }
}
