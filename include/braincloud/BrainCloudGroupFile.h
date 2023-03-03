// Copyright 2023 bitHeads, Inc. All Rights Reserved.

#pragma once
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include <vector>
#include <string>
#include "json/json.h"
#include "BrainCloudTypes.h"

namespace BrainCloud
{

class IServerCallback;
class BrainCloudClient;

class BrainCloudGroupFile
{
public:
    BrainCloudGroupFile(BrainCloudClient* in_client);
    
    void checkFilenameExists(std::string groupId, std::string folderPath, std::string fileName, IServerCallback* in_callback = NULL);

    void checkFullpathFilenameExists(std::string groupId, std::string fullPathFilename, IServerCallback* in_callback = NULL);

    void copyFile(std::string groupId, std::string fileId, int version, std::string newTreeId, int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback* in_callback = NULL);

    void deleteFile(std::string groupId, std::string fileId, int version, std::string filename, IServerCallback* in_callback = NULL);

    void getCDNUrl(std::string groupId, std::string fileId, IServerCallback* in_callback = NULL);
    
    void getFileInfo(std::string groupId, std::string fileId, IServerCallback* in_callback = NULL);
    
    void getFileInfoSimple(std::string groupId, std::string folderPath, std::string filename, IServerCallback* in_callback = NULL);
    
    void getFileList(std::string groupId, std::string folderPath, bool recurse, IServerCallback* in_callback = NULL);
    
    void moveFile( std::string groupId,  std::string fileId, int version,  std::string newTreeId,  int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback* in_callback = NULL);
    
    void moveUserToGroupFile(std::string userCloudPath, std::string userCloudFilename, std::string groupId, std::string groupTreeId, std::string groupFilename, const std::string& groupFileAcl, bool overwriteIfPresent, IServerCallback* in_callback = NULL);
    
    void updateFileInfo(std::string groupId, std::string fileId, int version, std::string newFilename, const std::string& newACL, IServerCallback* in_callback = NULL);

private:
    BrainCloudClient * m_client;
    
};
}
#pragma clang diagnostic pop
