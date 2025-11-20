#include "braincloud/internal/nix/cURLLoader.h"
#include "braincloud/internal/URLRequestMethod.h"
#include "braincloud/http_codes.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <thread>

namespace BrainCloud
{
    // RAII wrappers
    struct CurlHandle
    {
        CURL* h;
        CurlHandle(CURL* c = nullptr) : h(c) {}
        ~CurlHandle() { if (h) curl_easy_cleanup(h); }
    };

    struct CurlSList
    {
        curl_slist* list{ nullptr };
        ~CurlSList() { if (list) curl_slist_free_all(list); }
        void append(const char* s) { list = curl_slist_append(list, s); }
    };

    bool cURLLoader::_initialized = false;
    long cURLLoader::_timeoutInterval = 5000; // default 5s

    cURLLoader::cURLLoader()
    {
#ifndef WIN32
        _socket = -1;
#else
        _socket = INVALID_SOCKET;
#endif

#if ( defined(GAMECLIENT_DEBUGLEVEL)  &&  GAMECLIENT_DEBUGLEVEL > 0 )
        printCurlVersion();
#endif
    }

    cURLLoader::~cURLLoader()
    {
        // Signal cancel and wait/join thread to ensure safe destruction
        close();

#if !defined(USE_PTHREAD)
        if (_thread.joinable()) _thread.join();
#endif
    }

    void cURLLoader::close()
    {
        // Signal cancellation
        _cancelRequested.store(true);

        // Wait for a bounded time for the worker to finish
        const int maxWaitMs = 5000;
        int waited = 0;
        while (_threadRunning.load() && waited < maxWaitMs)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            waited += 10;
        }

        // Join thread if needed
#if defined(USE_PTHREAD)
        if (_threadRunning.load())
        {
            // If thread still running, join (it will observe cancel flag and exit)
            pthread_join(_threadId, nullptr);
        }
#else
        if (_thread.joinable())
        {
            _thread.join();
        }
#endif
    }

    void cURLLoader::load(URLRequest const& req)
    {
        setRequest(req);

        if (!_initialized)
        {
            // prefer DEFAULT so libcurl doesn't enable signals on Linux
            curl_global_init(CURL_GLOBAL_DEFAULT);
            _initialized = true;
        }

        // Ensure any previous worker is asked to stop and joined
        close();

        // Reset cancellation and start worker
        _cancelRequested.store(false);
        _threadRunning.store(true);

#if defined(USE_PTHREAD)
        pthread_attr_init(&_threadAttr);
        pthread_attr_setdetachstate(&_threadAttr, PTHREAD_CREATE_JOINABLE);
        pthread_create(&_threadId, &_threadAttr, &cURLLoader::threadEntry, this);
#else
        _thread = std::thread(&cURLLoader::threadEntry, this);
#endif
    }

    bool cURLLoader::isDone()
    {
        return !_threadRunning.load();
    }

    void* cURLLoader::threadEntry(void* arg)
    {
        cURLLoader* loader = static_cast<cURLLoader*>(arg);
        if (!loader) return nullptr;

        loadThreadCurl(loader);
        return nullptr;
    }

    curl_socket_t cURLLoader::openSocket(void* data, curlsocktype purpose, struct curl_sockaddr* addr)
    {
        // create socket normally; avoid storing raw fd for manual closing
        (void)data;
        (void)purpose;
        return socket(addr->family, addr->socktype, addr->protocol);
    }

    size_t cURLLoader::writeData(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        cURLLoader* loader = static_cast<cURLLoader*>(userdata);
        if (!loader) return 0;
        loader->_urlResponse.addData(std::string(ptr, size * nmemb));
        return size * nmemb;
    }

    size_t cURLLoader::writeHeader(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        cURLLoader* loader = static_cast<cURLLoader*>(userdata);
        if (!loader) return 0;
        std::string headerLine(ptr, size * nmemb);
        auto colon = headerLine.find(':');
        if (colon != std::string::npos)
        {
            URLRequestHeader header(headerLine.substr(0, colon));
            size_t valStart = colon + 1;
            while (valStart < headerLine.length() && isspace((unsigned char)headerLine[valStart])) ++valStart;
            header.setValue(headerLine.substr(valStart));
            loader->_urlResponse.addHeader(header);
        }
        return size * nmemb;
    }

    int cURLLoader::xfer_info_cb(void* clientp, curl_off_t, curl_off_t, curl_off_t, curl_off_t)
    {
        cURLLoader* loader = static_cast<cURLLoader*>(clientp);
        if (!loader) return 1; // abort
        if (loader->_cancelRequested.load()) return 1; // abort
        return 0; // continue
    }

    void cURLLoader::loadThreadCurl(cURLLoader* loader)
    {
        // Initialize CURL easy handle with RAII
        CurlHandle handle(curl_easy_init());
        if (!handle.h)
        {
            loader->_urlResponse.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
            loader->_urlResponse.setReasonPhrase("cURL Out of Memory");
            loader->_threadRunning.store(false);
            return;
        }

        CURL* easy = handle.h;

        char curlError[CURL_ERROR_SIZE] = { 0 };
        curl_easy_setopt(easy, CURLOPT_ERRORBUFFER, curlError);

        // Build headers (RAII)
        CurlSList headers;
        std::vector<URLRequestHeader> reqHeaders = loader->getRequest().getHeaders();
        for (size_t i = 0; i < reqHeaders.size(); ++i)
        {
            std::string line = reqHeaders[i].getName() + ": " + reqHeaders[i].getValue();
            headers.append(line.c_str());
        }
        std::string contentType = "Content-Type: " + loader->getRequest().getContentType();
        headers.append(contentType.c_str());

        if (!loader->getRequest().getUserAgent().empty())
        {
            std::string ua = "User-Agent: " + loader->getRequest().getUserAgent();
            headers.append(ua.c_str());
        }

        if (headers.list)
            curl_easy_setopt(easy, CURLOPT_HTTPHEADER, headers.list);

        // Safe options
#ifndef WIN32
        curl_easy_setopt(easy, CURLOPT_NOSIGNAL, 1L);
#endif
        curl_easy_setopt(easy, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);

        long effectiveTimeout = (loader->_timeoutInterval > 0) ? loader->_timeoutInterval : 10000;
        curl_easy_setopt(easy, CURLOPT_CONNECTTIMEOUT_MS, effectiveTimeout);
        curl_easy_setopt(easy, CURLOPT_TIMEOUT_MS, effectiveTimeout);

        // low speed guard
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_LIMIT, 1L);
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_TIME, 30L);

        // progress / cancellation callback
        curl_easy_setopt(easy, CURLOPT_XFERINFOFUNCTION, xfer_info_cb);
        curl_easy_setopt(easy, CURLOPT_XFERINFODATA, loader);
        curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 0L);

#if ( defined(GAMECLIENT_DEBUGLEVEL)  &&  GAMECLIENT_DEBUGLEVEL > 0 )
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 1L);
#else
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 0L);
#endif

        // keepalive enabled; do not forbid reuse
        curl_easy_setopt(easy, CURLOPT_TCP_KEEPALIVE, 1L);
        curl_easy_setopt(easy, CURLOPT_TCP_KEEPIDLE, 30L);
        curl_easy_setopt(easy, CURLOPT_TCP_KEEPINTVL, 30L);

        // socket callback
        curl_easy_setopt(easy, CURLOPT_OPENSOCKETFUNCTION, openSocket);
        curl_easy_setopt(easy, CURLOPT_OPENSOCKETDATA, loader);

        // write callbacks
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, loader);
        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(easy, CURLOPT_WRITEHEADER, loader);
        curl_easy_setopt(easy, CURLOPT_HEADERFUNCTION, writeHeader);

        // Perform the request based on method (method is a string in your URLRequest)
        CURLcode rc = CURLE_OK;
        const std::string method = loader->getRequest().getMethod();
        if (method == "GET")
        {
            std::string url = loader->getRequest().getUrl();
            std::string data = loader->getRequest().getData();
            if (!data.empty())
            {
                url.append("?");
                url.append(data);
            }
            curl_easy_setopt(easy, CURLOPT_URL, url.c_str());
            rc = curl_easy_perform(easy);
        }
        else if (method == "POST")
        {
            curl_easy_setopt(easy, CURLOPT_URL, loader->getRequest().getUrl().c_str());
            curl_easy_setopt(easy, CURLOPT_POST, 1L);
            curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, loader->getRequest().getData().length());
            curl_easy_setopt(easy, CURLOPT_COPYPOSTFIELDS, loader->getRequest().getData().c_str());
            rc = curl_easy_perform(easy);
        }
        else if (method == "PUT")
        {
            curl_easy_setopt(easy, CURLOPT_URL, loader->getRequest().getUrl().c_str());
            curl_easy_setopt(easy, CURLOPT_UPLOAD, 1L);
            curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, loader->getRequest().getData().length());
            curl_easy_setopt(easy, CURLOPT_COPYPOSTFIELDS, loader->getRequest().getData().c_str());
            rc = curl_easy_perform(easy);
        }
        else
        {
            rc = CURLE_FAILED_INIT;
        }

        if (rc == CURLE_ABORTED_BY_CALLBACK)
        {
            loader->_urlResponse.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
            loader->_urlResponse.setReasonPhrase(curlError[0] ? curlError : "aborted");
        }
        else if (rc == CURLE_OK)
        {
            long statusCode = 0;
            curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &statusCode);
            loader->_urlResponse.setStatusCode(static_cast<unsigned short>(statusCode));
        }
        else if (rc == CURLE_OPERATION_TIMEDOUT)
        {
            loader->_urlResponse.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
            loader->_urlResponse.setReasonPhrase("Operation timed out");
        }
        else
        {
            loader->_urlResponse.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
            loader->_urlResponse.setReasonPhrase(curlError[0] ? curlError : "cURL error");
        }

        // Mark done
        loader->_threadRunning.store(false);

        // Note: handle (CurlHandle) and headers (CurlSList) cleaned up by RAII
    }

#if ( defined(GAMECLIENT_DEBUGLEVEL)  &&  GAMECLIENT_DEBUGLEVEL > 0 )
    void cURLLoader::printCurlVersion()
    {
        curl_version_info_data* info = curl_version_info(CURLVERSION_NOW);
        std::cout << "libcurl version: " << info->version << std::endl;
        std::cout << "SSL version: " << (info->ssl_version ? info->ssl_version : "none") << std::endl;
        std::cout << "libz version: " << (info->libz_version ? info->libz_version : "none") << std::endl;
        if (info->protocols)
        {
            std::cout << "Supported protocols: ";
            for (const char* const* proto = info->protocols; *proto; ++proto) std::cout << *proto << " ";
            std::cout << std::endl;
        }
    }
#endif

} // namespace BrainCloud