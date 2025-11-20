#pragma once

#ifndef _CURLLOADER_H_
#define _CURLLOADER_H_

#ifndef WIN32
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <unistd.h>
#else
#  include <WinSock2.h>
#  include <Windows.h>
#endif

#include <atomic>
#include <string>

#include "braincloud/internal/URLLoader.h" // your existing base class
#include "curl/curl.h"

#if defined(USE_PTHREAD)
#  include <pthread.h>
#else
#  include <thread>
#endif

namespace BrainCloud
{

    class cURLLoader : virtual public URLLoader
    {
    public:
        virtual ~cURLLoader();

        // Async, non-blocking: starts background request and returns immediately.
        virtual void load(URLRequest const& req);
        virtual void load(URLRequest const* r) { if (r) load(*r); }

        // Request cancellation / close
        virtual void close();

        // Polled by runCallbacks()
        virtual bool isDone();

        // Timeout in milliseconds (0 means no timeout)
        virtual void setTimeout(int milliseconds) { _timeoutInterval = milliseconds; }

    protected:
        friend class URLLoader;
        cURLLoader();

    private:
        // thread entry (pthreads or std::thread compatible)
        static void* threadEntry(void* arg);
        static void loadThreadCurl(cURLLoader* loader);

        // libcurl callbacks
        static curl_socket_t openSocket(void* data, curlsocktype purpose, struct curl_sockaddr* addr);
        static size_t writeData(char* ptr, size_t size, size_t nmemb, void* userdata);
        static size_t writeHeader(char* ptr, size_t size, size_t nmemb, void* userdata);
        static int xfer_info_cb(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

        // helpers / state
        static bool _initialized;
        static long _timeoutInterval; // ms

        std::atomic<bool> _threadRunning{ false };
        std::atomic<bool> _cancelRequested{ false };

#if defined(USE_PTHREAD)
        pthread_t _threadId;
        pthread_attr_t _threadAttr;
#else
        std::thread _thread;
#endif

#if ( defined(GAMECLIENT_DEBUGLEVEL)  &&  GAMECLIENT_DEBUGLEVEL > 0 )
        void printCurlVersion();
#endif

#ifndef WIN32
        int _socket{ -1 };
#else
        SOCKET _socket{ INVALID_SOCKET };
#endif
    };

} // namespace BrainCloud

#endif // _CURLLOADER_H_