#ifndef _HTTPREQUESTFILEUPLOADER_H_
#define _HTTPREQUESTFILEUPLOADER_H_

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <mutex>

#include "httplib.h"
#include "braincloud/internal/IFileUploader.h"

namespace BrainCloud
{
    class HTTPFileUploader : public IFileUploader
    {
    public:
        virtual ~HTTPFileUploader();

        // IFileUploader interface
        virtual void enableLogging(bool in_loggingEnabled) override;

        // Starts upload on a background thread. Returns true if upload started ok.
        virtual bool uploadFile(std::string& in_sessionId,
            std::string& in_fileUploadId,
            std::string& in_fileName,
            int64_t in_fileSize,
            std::string& in_uploadUrl) override;

        virtual void setUploadLowTransferRateTimeout(int in_timeoutSecs) override;
        virtual void setUploadLowTransferRateThreshold(int in_bytesPerSec) override;

        virtual void cancelUpload() override;
        virtual int64_t getBytesTransferred() override;
        virtual int64_t getTotalBytesToTransfer() override;
        virtual double getProgress() override;

        virtual eFileUploaderStatus getStatus() override;
        virtual const std::string& getHttpResponse() override;
        virtual int getHttpStatus() override;
        virtual int getErrorReasonCode() override;

        virtual bool isThreadRunning() override;

    private:
        // worker
        void workerUpload(const std::string sessionId,
            const std::string fileUploadId,
            const std::string fileName,
            int64_t fileSize,
            const std::string uploadUrl);

        // helper (parses URL into scheme/host/path/port)
        static void split_scheme_host_path(const std::string& url,
            std::string& scheme,
            std::string& host,
            std::string& path,
            int& port);

    protected:
        friend class IFileUploader;
        HTTPFileUploader();

    private:
        std::atomic<eFileUploaderStatus> _status{ UPLOAD_STATUS_NONE };

        std::atomic<bool> _loggingEnabled{ false };

        // cancellation / state
        std::atomic<bool> _cancelRequested{ false };
        std::atomic<bool> _threadRunning{ false };

        // progress counters
        std::atomic<int64_t> _bytesTransferred{ 0 };
        std::atomic<int64_t> _totalBytes{ 0 };

        // low transfer rate monitor settings
        std::atomic<int> _lowTransferTimeoutSecs{ 0 };
        std::atomic<int> _lowTransferBytesPerSec{ 0 };

        std::thread _thread;
        std::mutex _responseMutex;

        // last HTTP response
        std::string _httpResponseBody;
        int _httpStatus{ 0 };
        int _errorReasonCode{ 0 };

        // underlying HTTP client
        std::unique_ptr<httplib::Client> _client;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        std::unique_ptr<httplib::SSLClient> _sslClient;
#endif
    };
}

#endif /* _HTTPREQUESTFILEUPLOADER_H_ */