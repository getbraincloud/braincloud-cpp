// Copyright 2023 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

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
    /**
     * Initializes the brainCloudService
     */
    BrainCloudGroupFile(BrainCloudClient* in_client);
    
    /**
     * Check if filename exists for provided path and name
     *
     * Service Name GroupFile
     * Service Operation  CheckFilenameExists
     *
     * @param groupId ID of the group.
     * @param folderPath The path of the file
     * @param filename The filename of the file
     * @param in_callback Block to call on return of  server response
     */
    void checkFilenameExists(std::string groupId, std::string folderPath, std::string fileName, IServerCallback* in_callback = NULL);
    
    /**
     * Check if filename exists for provided full path name
     *
     * Service Name GroupFile
     * Service Operation CheckFullpathFilenameExists
     *
     * @param groupId ID of the group.
     * @param fullPathFilename The full path of the file
     * @param in_callback Block to call on return of  server response
     */
    void checkFullpathFilenameExists(std::string groupId, std::string fullPathFilename, IServerCallback* in_callback = NULL);
    
    /**
     *  Copy a file.
     *
     * Service Name GroupFile
     * Service Operation CopyFile
     *
     * @param groupId the groupId
     * @param fileId the fileId
     * @param version the version
     * @param newTreeId thenewTreeId
     * @param treeVersion the treeVersion
     * @param newFilename the newFilename
     * @param in_callback Block to call on return of  server response
     */
    void copyFile(std::string groupId, std::string fileId, int version, std::string newTreeId, int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback* in_callback = NULL);
    
    /**
     * Delete a file.
     *
     * Service Name GroupFile
     * Service Operation DeleteFile
     *
     * @param groupId the groupId
     * @param fileId the fileId
     * @param version the version
     * @param newFilename the newFilename
     * @param in_callback Block to call on return of  server response
     
     */
    void deleteFile(std::string groupId, std::string fileId, int version, std::string filename, IServerCallback* in_callback = NULL);
    
    /**
     * Return CDN url for file for clients that cannot handle redirect.
     *
     * Service Name GroupFile
     * Service Operation GetCdnUrl
     *
     * @param groupId the groupId
     * @param fileId the fileId
     * @param in_callback Block to call on return of  server response
     
     */
    void getCDNUrl(std::string groupId, std::string fileId, IServerCallback* in_callback = NULL);
    
    /**
     * Returns information on a file using fileId.
     *
     * Service Name GroupFile
     * Service Operation GetFileInfo
     *
     * @param groupId the groupId
     * @param fileId the fileId
     * @param in_callback Block to call on return of  server response
     */
    void getFileInfo(std::string groupId, std::string fileId, IServerCallback* in_callback = NULL);
    
    /**
     * Returns information on a file using path and name.
     *
     * Service Name GroupFile
     * Service Operation GetFileInfoSimple
     *
     * @param groupId the groupId
     * @param folderPath the folderPath
     * @param fileName the fileName
     * @param in_callback Block to call on return of  server response
     
     */
    void getFileInfoSimple(std::string groupId, std::string folderPath, std::string filename, IServerCallback* in_callback = NULL);
    
    /**
     * Returns a list of files.
     *
     * Service Name GroupFile
     * Service Operation GetFileList
     *
     * @param groupId the groupId
     * @param folderPath the folderPath
     * @param recurse true to recurse
     * @param in_callback Block to call on return of  server response
     */
    void getFileList(std::string groupId, std::string folderPath, bool recurse, IServerCallback* in_callback = NULL);
    
    /**
     *  Move a file.
     *
     * Service Name GroupFile
     * Service Operation MoveFile
     *
     * @param groupId the groupId
     * @param fileId the fileId
     * @param version the version
     * @param newTreeId the newTreeId
     * @param newFilename the newFilename
     * @param in_callback Block to call on return of  server response
     */
    void moveFile( std::string groupId,  std::string fileId, int version,  std::string newTreeId,  int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback* in_callback = NULL);
    
    /**
     * Move a file from user space to group space.
     *
     * Service Name GroupFile
     * Service Operation MoveUserToGroupFile
     *
     * @param userCloudPath the userCloudPath
     * @param userCloudFilename the userCloudFilename
     * @param groupId the groupId
     * @param groupTreeId the groupTreeId
     * @param groupFilename the groupFilename
     * @param groupFileAcl the groupFileAcl
     * @param overwriteIfPresent the overwriteIfPresent
     * @param in_callback Block to call on return of  server response
     */
    void moveUserToGroupFile(std::string userCloudPath, std::string userCloudFilename, std::string groupId, std::string groupTreeId, std::string groupFilename, const std::string& groupFileAcl, bool overwriteIfPresent, IServerCallback* in_callback = NULL);
    
    /**
     * updates information on a file given fileId.
     *
     * Service Name GroupFile
     * Service Operation UpdateFileInfo
     *
     * @param groupId the groupId
     * @param fileId the fileId
     * @param version the version
     * @param newFilename the newFilename
     * @param newAcl the newAcl
     * @param in_callback Block to call on return of  server response
     */
    void updateFileInfo(std::string groupId, std::string fileId, int version, std::string newFilename, const std::string& newACL, IServerCallback* in_callback = NULL);
    
private:
    BrainCloudClient * m_client;
    
};
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
