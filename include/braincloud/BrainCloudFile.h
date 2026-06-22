// Copyright 2026 bitHeads, Inc. All Rights Reserved.

#pragma once
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#endif


#include <string>
#include "braincloud/BrainCloudTypes.h"

namespace BrainCloud
{
    class IServerCallback;
    class BrainCloudClient;

    class BrainCloudFile
    {
    public:
        BrainCloudFile(BrainCloudClient* client);

        /**
         * Prepares a user file upload. On success the file will begin uploading
         * to the brainCloud server. To be informed of success/failure of the upload
         * register an IFileUploadCallback with the BrainCloudClient class.
         *
         * Service Name - file
         * Service Operation - PREPARE_USER_UPLOAD
         *
         * @param cloudPath The desired cloud path of the file
         * @param cloudFilename The desired cloud filename of the file
         * @param shareable True if the file is shareable.
         * @param replaceIfExists Whether to replace file if it exists
         * @param localPath The path and filename of the local file
         * @param callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40429 - File maximum file size exceeded
         * 40430 - File exists, replaceIfExists not set
         */
        bool uploadFile(const char * cloudPath,
            const char * cloudFilename,
            bool shareable,
            bool replaceIfExists,
            const char * localPath,
            IServerCallback * callback = NULL);

        /**
         * List all user files
         *
         * Service Name - file
         * Service Operation - LIST_USER_FILES
         *
         * @param callback The method to be invoked when the server response is received
         */
        void listUserFiles(IServerCallback * callback = NULL);

        /**
         * List user files from the given cloud path
         *
         * Service Name - file
         * Service Operation - LIST_USER_FILES
         *
         * @param cloudPath File path
         * @param recurse Whether to recurse into sub-directories
         * @param callback The method to be invoked when the server response is received
         */
        void listUserFiles(const char * cloudPath, bool recurse, IServerCallback * callback = NULL);

        /**
         * Deletes a single user file.
         *
         * Service Name - file
         * Service Operation - DELETE_USER_FILES
         *
         * @param cloudPath File path
         * @param cloudFilename name of file
         * @param callback The method to be invoked when the server response is received
         *
         * Significant error codes:
         *
         * 40431 - Cloud storage service error
         * 40432 - File does not exist
         *
         */
        void deleteUserFile(const char * cloudPath, const char * cloudFilename, IServerCallback * callback = NULL);

        /**
         * Delete multiple user files
         *
         * Service Name - file
         * Service Operation - DELETE_USER_FILES
         *
         * @param cloudPath File path
         * @param recurse Whether to recurse into sub-directories
         * @param callback The method to be invoked when the server response is received
         */
        void deleteUserFiles(const char * cloudPath, bool recurse, IServerCallback * callback = NULL);

		/**
		* Returns the CDN url for a file object
		*
		* Service Name - file
		* Service Operation - GET_CDN_URL
		*
		* @param cloudPath File path
		* @param cloudFileName File name
		* @param callback The method to be invoked when the server response is received
		*/
		void getCDNUrl(const char * cloudPath, const char * cloudFileName, IServerCallback * callback = NULL);

        /**
         * Method cancels an upload. If an IFileUploadCallback has been registered with the BrainCloudClient class,
         * the fileUploadFailed callback method will be called once the upload has been canceled.
         *
         * @param uploadId The id of the upload
         */
        void cancelUpload(const char * uploadId);

        /**
         * Returns the progress of the given upload from 0.0 to 1.0
         * or -1 if upload not found.
         *
         * @param uploadId The id of the upload
         * @return A progress from 0.0 to 1.0 or -1 if upload not found.
         */
        double getUploadProgress(const char * uploadId);

        /**
         * Returns the number of bytes uploaded or -1 if upload not found.
         *
         * @param uploadId The id of the upload
         * @return The number of bytes uploaded or -1 if upload not found.
         */
        int64_t getUploadBytesTransferred(const char * uploadId);

        /**
         * Returns the total number of bytes that will be uploaded or -1 if upload not found.
         *
         * @param uploadId The id of the upload
         * @return The total number of bytes that will be uploaded or -1 if upload not found.
         */
        int64_t getUploadTotalBytesToTransfer(const char * uploadId);

    private:
        BrainCloudClient * m_client;
    };
}
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
