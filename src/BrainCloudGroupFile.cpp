// Copyright 2023 bitHeads, Inc. All Rights Reserved.

#include "braincloud/BrainCloudGroupFile.h"

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
	BrainCloudGroupFile::BrainCloudGroupFile(BrainCloudClient* in_client) : m_client(in_client) { }

void BrainCloudGroupFile::checkFilenameExists(std::string groupId, std::string folderPath, std::string fileName, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FileName.getValue()] = fileName;
    message[OperationParam::FolderPath.getValue()] = folderPath;
    
    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::CheckFilenameExists, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::checkFullpathFilenameExists(std::string groupId, std::string fullPathFilename, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FullPathFilename.getValue()] = fullPathFilename;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::CheckFullpathFilenameExists, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::copyFile(std::string groupId, std::string fileId, int version, std::string newTreeId, int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FileId.getValue()] = fileId;
    message[OperationParam::GroupFileVersion.getValue()] = version;
    message[OperationParam::NewTreeId.getValue()] = newTreeId;
    message[OperationParam::TreeVersion.getValue()] = treeVersion;
    message[OperationParam::NewFilename.getValue()] = newFilename;
    message[OperationParam::OverwriteIfPresent.getValue()] = overwriteIfPresent;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::CopyFile, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::deleteFile(std::string groupId, std::string fileId, int version, std::string filename, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FileId.getValue()] = fileId;
    message[OperationParam::GroupFileVersion.getValue()] = version;
    message[OperationParam::FileName.getValue()] = filename;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::DeleteFile, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::getCDNUrl(std::string groupId, std::string fileId, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FileId.getValue()] = fileId;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::GetCdnUrl, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::getFileInfo(std::string groupId, std::string fileId, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FileId.getValue()] = fileId;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::GetFileInfo, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::getFileInfoSimple(std::string groupId, std::string folderPath, std::string filename, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FolderPath.getValue()] = folderPath;
    message[OperationParam::FileName.getValue()] = filename;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::GetFileInfoSimple, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::getFileList(std::string groupId, std::string folderPath, bool recurse, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FolderPath.getValue()] = folderPath;
    message[OperationParam::Recurse.getValue()] = recurse;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::GetFileList, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::moveFile( std::string groupId,  std::string fileId, int version,  std::string newTreeId,  int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FileId.getValue()] = fileId;
    message[OperationParam::GroupFileVersion.getValue()] = version;
    message[OperationParam::NewTreeId.getValue()] = newTreeId;
    message[OperationParam::TreeVersion.getValue()] = treeVersion;
    message[OperationParam::NewFilename.getValue()] = newFilename;
    message[OperationParam::OverwriteIfPresent.getValue()] = overwriteIfPresent;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::MoveFile, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::moveUserToGroupFile(std::string userCloudPath, std::string userCloudFilename, std::string groupId, std::string groupTreeId, std::string groupFilename, const std::string& groupFileAcl, bool overwriteIfPresent, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::UserCloudPath.getValue()] = userCloudPath;
    message[OperationParam::UserCloudFilename.getValue()] = userCloudFilename;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::GroupTreeId.getValue()] = groupTreeId;
    message[OperationParam::GroupFilename.getValue()] = groupFilename;
    
    if (StringUtil::IsOptionalParameterValid(groupFileAcl))
        message[OperationParam::GroupFileACL.getValue()] = JsonUtil::jsonStringToValue(groupFileAcl);
    
    message[OperationParam::OverwriteIfPresent.getValue()] = overwriteIfPresent;


    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::MoveUserToGroupFile, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

void BrainCloudGroupFile::updateFileInfo(std::string groupId, std::string fileId, int version, std::string newFilename, const std::string& newACL, IServerCallback* in_callback){
    Json::Value message;
    message[OperationParam::GroupId.getValue()] = groupId;
    message[OperationParam::FileId.getValue()] = fileId;
    message[OperationParam::GroupFileVersion.getValue()] = version;
    message[OperationParam::NewFilename.getValue()] = newFilename;

    ServerCall * sc = new ServerCall(ServiceName::GroupFile, ServiceOperation::UpdateFileInfo, message, in_callback);
    m_client->getBrainCloudComms()->addToQueue(sc);
}

}
