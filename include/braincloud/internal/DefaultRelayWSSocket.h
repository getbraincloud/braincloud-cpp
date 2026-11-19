#pragma once

#if __APPLE__
    #include "TargetConditionals.h"
#endif

#if (!defined(TARGET_OS_WATCH) || TARGET_OS_WATCH == 0)

#include "braincloud/internal/IRelayWSSocket.h"

#include <libwebsockets.h>
#include <lws_config.h>

#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace BrainCloud
{
    class DefaultRelayWSSocket : public IRelayWSSocket
    {
    public:
        static int libWebsocketsCallback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len);

        virtual ~DefaultRelayWSSocket();

        virtual bool isConnected() override;
        virtual bool isValid() override;
        virtual void updateConnection() override;
        virtual void send(const uint8_t* pData, int size) override;
        virtual const uint8_t* peek(int& size) override;
        virtual void close() override;

    protected:
        friend class IRelayWSSocket;

        DefaultRelayWSSocket(const std::string& address, int port, int maxPacketSize, bool useSSL);

    private:
        void onClose();
        void onError(const char* msg);
        void onConnect();
        void onRecv(const uint8_t* buffer, int len);
        void processSendQueue();

        // State (protected by _mutex)
        bool _isValid;
        bool _isConnecting;

        // Send side
        std::mutex _mutex;
        std::queue<std::vector<uint8_t>> _sendQueue;
        std::vector<uint8_t> _sendBuffer;
        std::thread _updateThread;

        // Receive side (protected by _recvMutex)
        std::mutex _recvMutex;
        std::queue<std::vector<uint8_t>> _recvQueue;
        std::vector<uint8_t> _currentPacket;  // Holds the packet returned by peek()

        // libwebsockets
        struct lws_context* _pLwsContext;
        struct lws* _pLws;

        // Kept alive for lws, which holds these pointers during the handshake
        std::string _host;
        std::string _path;
    };
};

#endif /* TARGET_OS_WATCH */
