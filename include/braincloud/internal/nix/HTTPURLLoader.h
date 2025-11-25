#ifndef _HTTPURLLOADER_H_
#define _HTTPURLLOADER_H_

#include <atomic>
#include <memory>
#include <string>
#include <thread>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include "braincloud/internal/URLLoader.h"
#include "braincloud/internal/URLRequest.h"
#include "braincloud/internal/URLResponse.h"

namespace BrainCloud
{
    class HTTPURLLoader : virtual public URLLoader
    {
    public:
        
        virtual ~HTTPURLLoader();

        // URLLoader interface
        virtual void        close() override;
        virtual void        load(URLRequest const& request) override;
        virtual void        load(URLRequest const* r) override { if (r) load(*r); }
        virtual void        setTimeout(int milliseconds) { _timeoutMs = milliseconds; }
        virtual bool        isDone() override;

    protected:
        friend class URLLoader;
        HTTPURLLoader();

    private:
        // Thread worker
        void runRequest();

        // Cancellation / state
        std::atomic<bool> _cancelRequested{ false };
        std::atomic<bool> _threadRunning{ false };

        // Current request timeout (ms)
        int _timeoutMs{ 2000 };

        // Worker thread
        std::thread _thread;

        // HTTP client(s)
        std::unique_ptr<httplib::Client> _client;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        std::unique_ptr<httplib::SSLClient> _sslClient;
#endif
    };
}

#endif // _HTTPURLLOADER_H_
