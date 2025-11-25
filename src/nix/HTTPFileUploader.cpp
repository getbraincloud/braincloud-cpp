#include "braincloud/internal/nix/HTTPFileUploader.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

namespace BrainCloud
{
    IFileUploader* IFileUploader::create()
    {
        return new HTTPFileUploader();
    }

    HTTPFileUploader::HTTPFileUploader()
    {
        _status.store(UPLOAD_STATUS_NONE);
        _bytesTransferred.store(0);
        _totalBytes.store(0);
    }

    HTTPFileUploader::~HTTPFileUploader()
    {
        cancelUpload();
        if (_thread.joinable())
            _thread.join();
    }

    void HTTPFileUploader::enableLogging(bool in_loggingEnabled)
    {
        _loggingEnabled.store(in_loggingEnabled ? true : false);
    }

    void HTTPFileUploader::setUploadLowTransferRateTimeout(int in_timeoutSecs)
    {
        _lowTransferTimeoutSecs.store(in_timeoutSecs);
    }

    void HTTPFileUploader::setUploadLowTransferRateThreshold(int in_bytesPerSec)
    {
        _lowTransferBytesPerSec.store(in_bytesPerSec);
    }

    bool HTTPFileUploader::uploadFile(std::string& in_sessionId,
        std::string& in_fileUploadId,
        std::string& in_fileName,
        int64_t in_fileSize,
        std::string& in_uploadUrl)
    {
        // Basic validation
        if (in_uploadUrl.empty() || in_fileName.empty())
            return false;

        if (_threadRunning.load())
        {
            // already running
            return false;
        }

        _cancelRequested.store(false);
        _bytesTransferred.store(0);
        _totalBytes.store(in_fileSize);
        _httpResponseBody.clear();
        _httpStatus = 0;
        _errorReasonCode = 0;
        _status.store(UPLOAD_STATUS_PENDING);

        // Launch worker thread (detached-like behavior but we join in destructor)
        _thread = std::thread(&HTTPFileUploader::workerUpload, this,
            in_sessionId, in_fileUploadId, in_fileName, in_fileSize, in_uploadUrl);

        // detach semantics (we still keep thread handle to join on destruction if needed)
        _thread.detach();
        _threadRunning.store(true);

        return true;
    }

    void HTTPFileUploader::cancelUpload()
    {
        _cancelRequested.store(true);

        // If client exists, ask it to stop; this should interrupt an in-flight operation
        if (_client)
        {
            _client->stop();
        }
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        if (_sslClient)
        {
            _sslClient->stop();
        }
#endif

        _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
    }

    int64_t HTTPFileUploader::getBytesTransferred()
    {
        return _bytesTransferred.load();
    }

    int64_t HTTPFileUploader::getTotalBytesToTransfer()
    {
        return _totalBytes.load();
    }

    double HTTPFileUploader::getProgress()
    {
        int64_t total = _totalBytes.load();
        if (total <= 0) return 0.0;
        double bytes = static_cast<double>(_bytesTransferred.load());
        return bytes / static_cast<double>(total);
    }

    HTTPFileUploader::eFileUploaderStatus HTTPFileUploader::getStatus()
    {
        return _status.load();
    }

    const std::string& HTTPFileUploader::getHttpResponse()
    {
        std::lock_guard<std::mutex> lock(_responseMutex);
        return _httpResponseBody;
    }

    int HTTPFileUploader::getHttpStatus()
    {
        return _httpStatus;
    }

    int HTTPFileUploader::getErrorReasonCode()
    {
        return _errorReasonCode;
    }

    bool HTTPFileUploader::isThreadRunning()
    {
        return _threadRunning.load();
    }

    void HTTPFileUploader::split_scheme_host_path(const std::string& url,
        std::string& scheme,
        std::string& host,
        std::string& path,
        int& port)
    {
        scheme = "http"; host = ""; path = "/"; port = 0;
        size_t p = url.find("://");
        size_t pos = 0;
        if (p != std::string::npos) {
            scheme = url.substr(0, p);
            pos = p + 3;
        }
        size_t slash = url.find('/', pos);
        std::string hostport = (slash == std::string::npos) ? url.substr(pos) : url.substr(pos, slash - pos);
        if (slash != std::string::npos) path = url.substr(slash);
        size_t colon = hostport.find(':');
        if (colon != std::string::npos) {
            host = hostport.substr(0, colon);
            port = std::stoi(hostport.substr(colon + 1));
        }
        else {
            host = hostport;
            port = (scheme == "https") ? 443 : 80;
        }
    }

    void HTTPFileUploader::workerUpload(const std::string sessionId,
        const std::string fileUploadId,
        const std::string fileName,
        int64_t fileSize,
        const std::string uploadUrl)
    {
        _status.store(UPLOAD_STATUS_UPLOADING);

        // parse URL
        std::string scheme, host, path;
        int port = 0;
        split_scheme_host_path(uploadUrl, scheme, host, path, port);

        try
        {
            // Read file into memory (simple approach)
            std::ifstream ifs(fileName, std::ios::binary);
            if (!ifs)
            {
                _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
                _errorReasonCode = -1; // file open failed
                _threadRunning.store(false);
                return;
            }

            std::ostringstream oss;
            oss << ifs.rdbuf();
            std::string fileContents = oss.str();

            // Update bytes transferred = file loaded into memory (approximation)
            _bytesTransferred.store(static_cast<int64_t>(fileContents.size()));

            // prepare multipart items
            httplib::MultipartFormDataItems items;

            // Add any fields server expects (sessionId and fileUploadId commonly required)
            if (!sessionId.empty())
                items.push_back({ "sessionId", sessionId, "", "" });

            if (!fileUploadId.empty())
                items.push_back({ "fileUploadId", fileUploadId, "", "" });

            // File item: cpp-httplib wants content as string
            // third param is filename, fourth param is content-type
            std::string contentType = "application/octet-stream";
            items.push_back({ "file", fileContents, fileName, contentType });

            // Create client
            if (scheme == "https")
            {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
                _sslClient = std::make_unique<httplib::SSLClient>(host.c_str(), port);
                _sslClient->set_follow_location(true);
                // timeouts: optional, can be set
#else
                // SSL not supported in this build
                std::lock_guard<std::mutex> lock(_responseMutex);
                _httpResponseBody = "SSL not supported in this build";
                _httpStatus = 0;
                _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
                _threadRunning.store(false);
                return;
#endif
            }
            else
            {
                _client = std::make_unique<httplib::Client>(host.c_str(), port);
                _client->set_follow_location(true);
            }

            // If cancellation requested before starting, abort
            if (_cancelRequested.load())
            {
                _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
                _threadRunning.store(false);
                return;
            }

            // Perform multipart post
            std::shared_ptr<httplib::Response> response;
            if (scheme == "https")
            {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
                response = _sslClient->Post(path.c_str(), items);
#endif
            }
            else
            {
                response = _client->Post(path.c_str(), items);
            }

            // If upload was cancelled via stop(), response may be null
            if (_cancelRequested.load())
            {
                _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
                _threadRunning.store(false);
                return;
            }

            if (response)
            {
                // success or server error - inspect HTTP status
                {
                    std::lock_guard<std::mutex> lock(_responseMutex);
                    _httpResponseBody = response.body;
                    _httpStatus = response.status;
                }

                if (response.status >= 200 && response.status < 300)
                {
                    _status.store(UPLOAD_STATUS_COMPLETE_SUCCESS);
                    _bytesTransferred.store(_totalBytes.load());
                }
                else
                {
                    _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
                    _errorReasonCode = response.status;
                }
            }
            else
            {
                // response null -> network error or aborted
                _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
                _errorReasonCode = -2; // indicate network/aborted
            }
        }
        catch (const std::exception& e)
        {
            std::lock_guard<std::mutex> lock(_responseMutex);
            _httpResponseBody = e.what();
            _status.store(UPLOAD_STATUS_COMPLETE_FAILED);
            _errorReasonCode = -3;
        }

        _threadRunning.store(false);
    }
}
