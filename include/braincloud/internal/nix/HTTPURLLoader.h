#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include "braincloud/internal/URLLoader.h"
#include "httplib.h"

namespace BrainCloud
{
    class HTTPURLLoader : virtual public URLLoader
    {
    public:
        HTTPURLLoader();
        virtual ~HTTPURLLoader();

        virtual void close() override;
        virtual void load(URLRequest const& req) override;
        virtual void load(URLRequest const* r) override { if (r) load(*r); }
        virtual bool isDone() override;
        virtual void setTimeout(int milliseconds) override { _timeoutMs = milliseconds; }

    private:
        void runRequest();

        std::thread _thread;
        std::atomic<bool> _threadRunning{ false };
        std::atomic<bool> _cancelRequested{ false };
        int _timeoutMs{ 2000 };

        // client stored while request in flight
        std::unique_ptr<httplib::Client> _client;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        std::unique_ptr<httplib::SSLClient> _sslClient;
#endif
    };
}
