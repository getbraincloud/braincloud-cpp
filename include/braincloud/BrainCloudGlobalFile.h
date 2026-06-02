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
        BrainCloudGlobalFile(BrainCloudClient* client);

        /**
         * Returns the complete info for the specified file given it’s fileId
         *
         * Service Name - globalFileV3
         * Service Operation - GET_FILE_INFO
         *
         * @param callback The method to be invoked when the server response is received
         */
        void getFileInfo(const std::string& fileId, IServerCallback* callback = NULL);

        /**
         * Returns the complete info for the specified file, without having to look up the fileId first.
         *
         * Service Name - globalFileV3
         * Service Operation - GET_FILE_INFO_SIMPLE
         *
         * @param callback The method to be invoked when the server response is received
         */
        void getFileInfoSimple(const std::string& folderPath, const std::string& filename, IServerCallback* callback = NULL);

        /**
         * Returns the CDN of the specified file.
         *
         * Service Name - globalFileV3
         * Service Operation - GET_GLOBAL_CDN_URL
         *
         * @param callback The method to be invoked when the server response is received
         */
        void getGlobalCDNUrl(const std::string& fileId, IServerCallback* callback = NULL);

        /**
         * Returns files at the current path.
         *
         * Service Name - globalFileV3
         * Service Operation - GET_GLOBAL_FILE_LIST
         *
         * @param callback The method to be invoked when the server response is received
         */
        void getGlobalFileList(const std::string& folderPath, bool recurse, IServerCallback* callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
