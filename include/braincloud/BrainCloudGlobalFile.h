// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif

#include <stddef.h>
#include <string>

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudGlobalFile
    {
    public:
        BrainCloudGlobalFile(BrainCloudClient *client);

        /**
         * Returns the complete info for the specified file given it’s fileId
         *
         * Service Name - GlobalFileV3
         * Service Operation - GetFileInfo
         *
         * @param fileId The fileId of the global file
         * @param callback The method to be invoked when the server response is received
         */
        void getFileInfo(const std::string &fileId, IServerCallback *callback = NULL);

        /**
         * Returns the complete info for the specified file, without having to look up the fileId first.
         *
         * Service Name - GlobalFileV3
         * Service Operation - GetFileInfoSimple
         *
         * @param folderPath The folder path of the file
         * @param filename The name of the file
         * @param callback The method to be invoked when the server response is received
         */
        void getFileInfoSimple(const std::string &folderPath, const std::string &filename, IServerCallback *callback = NULL);

        /**
         * Returns the CDN of the specified file.
         *
         * Service Name - GlobalFileV3
         * Service Operation - GetGlobalCDNUrl
         *
         * @param fileId The fileId of the global file
         * @param callback The method to be invoked when the server response is received
         */
        void getGlobalCDNUrl(const std::string &fileId, IServerCallback *callback = NULL);

        /**
         * Returns files at the current path.
         *
         * Service Name - GlobalFileV3
         * Service Operation - GetGlobalFileList
         *
         * @param folderPath The folder path to list files from
         * @param recurse Whether to recurse into subfolders
         * @param callback The method to be invoked when the server response is received
         */
        void getGlobalFileList(const std::string &folderPath, bool recurse, IServerCallback *callback = NULL);

    private:
        BrainCloudClient *m_client;
    };
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
