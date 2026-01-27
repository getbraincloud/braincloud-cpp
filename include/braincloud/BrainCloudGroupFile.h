// Copyright 2026 bitHeads, Inc. All Rights Reserved.

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
        BrainCloudGroupFile(BrainCloudClient *client);

        /**
         * Check if filename exists for provided path and name
         *
         * Service Name GroupFile
         * Service Operation  CheckFilenameExists
         *
         * @param groupId ID of the group.
         * @param folderPath The path of the file
         * @param filename The filename of the file
         * @param callback The method to be invoked when the server response is received
         */
        void checkFilenameExists(std::string groupId, std::string folderPath, std::string fileName, IServerCallback *callback = NULL);

        /**
         * Check if filename exists for provided full path name
         *
         * Service Name GroupFile
         * Service Operation CheckFullpathFilenameExists
         *
         * @param groupId ID of the group.
         * @param fullPathFilename The full path of the file
         * @param callback The method to be invoked when the server response is received
         */
        void checkFullpathFilenameExists(std::string groupId, std::string fullPathFilename, IServerCallback *callback = NULL);

        /**
         *  Copy a file.
         *
         * Service Name GroupFile
         * Service Operation CopyFile
         *
         * @param groupId ID of the group
         * @param fileId ID of the file
         * @param version Target version of the file
         * @param newTreeId ID of the destination folder
         * @param treeVersion Target version of the folder tree
         * @param newFilename Optional new file name
         * @param overwriteIfPresent Whether to allow overwrite of an existing file if present
         * @param callback The method to be invoked when the server response is received
         */
        void copyFile(std::string groupId, std::string fileId, int version, std::string newTreeId, int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback *callback = NULL);

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
         * @param callback The method to be invoked when the server response is received

         */
        void deleteFile(std::string groupId, std::string fileId, int version, std::string filename, IServerCallback *callback = NULL);

        /**
         * Return CDN url for file for clients that cannot handle redirect.
         *
         * Service Name GroupFile
         * Service Operation GetCdnUrl
         *
         * @param groupId the groupId
         * @param fileId the fileId
         * @param callback The method to be invoked when the server response is received

         */
        void getCDNUrl(std::string groupId, std::string fileId, IServerCallback *callback = NULL);

        /**
         * Returns information on a file using fileId.
         *
         * Service Name GroupFile
         * Service Operation GetFileInfo
         *
         * @param groupId the groupId
         * @param fileId the fileId
         * @param callback The method to be invoked when the server response is received
         */
        void getFileInfo(std::string groupId, std::string fileId, IServerCallback *callback = NULL);

        /**
         * Returns information on a file using path and name.
         *
         * Service Name GroupFile
         * Service Operation GetFileInfoSimple
         *
         * @param groupId the groupId
         * @param folderPath the folderPath
         * @param fileName the fileName
         * @param callback The method to be invoked when the server response is received
         */
        void getFileInfoSimple(std::string groupId, std::string folderPath, std::string filename, IServerCallback *callback = NULL);

        /**
         * Returns a list of files.
         *
         * Service Name GroupFile
         * Service Operation GetFileList
         *
         * @param groupId the groupId
         * @param folderPath the folderPath
         * @param recurse true to recurse
         * @param callback The method to be invoked when the server response is received
         */
        void getFileList(std::string groupId, std::string folderPath, bool recurse, IServerCallback *callback = NULL);

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
         * @param callback The method to be invoked when the server response is received
         */
        void moveFile(std::string groupId, std::string fileId, int version, std::string newTreeId, int treeVersion, std::string newFilename, bool overwriteIfPresent, IServerCallback *callback = NULL);

        /**
         * Move a file from user space to group space.
         *
         * Service Name GroupFile
         * Service Operation MoveUserToGroupFile
         *
         * @param userCloudPath User file folder
         * @param userCloudFilename User file name
         * @param groupId ID of the group
         * @param groupTreeId ID of the destination folder
         * @param groupFileName Group file name
         * @param groupFileAcl Acl of the new group file
         * @param overwriteIfPresent Whether to allow overwrite of an existing file if present
         * @param callback The method to be invoked when the server response is received @param callback The method to be invoked when the server response is received
         */
        void moveUserToGroupFile(std::string userCloudPath, std::string userCloudFilename, std::string groupId, std::string groupTreeId, std::string groupFilename, const std::string &groupFileAcl, bool overwriteIfPresent, IServerCallback *callback = NULL);

        /**
         * Rename or edit permissions of an uploaded file. Does not change the contents of the file.
         *
         * Service Name GroupFile
         * Service Operation UpdateFileInfo
         *
         * @param groupId ID of the group
         * @param fileId ID of the file
         * @param version Target version of the file
         * @param newFilename Optional new file name
         * @param newACL Optional new acl
         * @param callback The method to be invoked when the server response is received
         * */
        void updateFileInfo(std::string groupId, std::string fileId, int version, std::string newFilename, const std::string &newACL, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
