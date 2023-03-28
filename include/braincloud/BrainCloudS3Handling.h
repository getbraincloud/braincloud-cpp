// Copyright 2016 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <stddef.h>

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudS3Handling
    {
    public:
        BrainCloudS3Handling(BrainCloudClient* in_client);

        /*
         * Sends an array of file details and returns
         * the details of any of those files that have changed
         *
         * Service Name - S3Handling
         * Service Operation - GetUpdatedFiles
         *
         * @param in_category  Category of files on server to compare against
         * @param in_fileDetailsJson  An array of file details
         * @param in_callback  Instance of IServerCallback to call when the server response is received
         */
        void getUpdatedFiles(const char * in_category, const char * in_fileDetails, IServerCallback * in_callback = NULL);

        /*
         * Retrieves the details of custom files stored on the server
         *
         * Service Name - S3Handling
         * Service Operation - GetFileList
         *
         * @param in_category  Category of files to retrieve
         * @param in_callback  Instance of IServerCallback to call when the server response is receieved
         */
        void getFileList(const char * in_category, IServerCallback * in_callback = NULL);

		/**
		* Returns the CDN url for a file
		*
		* @param in_fileId ID of file
		* @param in_callback The method to be invoked when the server response is received
		*/
		void getCDNUrl(const char * in_fileId, IServerCallback * in_callback = NULL);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
