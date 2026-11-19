// Copyright 2026 bitHeads, Inc. All Rights Reserved.
#if (!defined(TARGET_OS_WATCH) || TARGET_OS_WATCH == 0)

#include "braincloud/internal/DefaultRelayWSSocket.h"

#include <cstring>
#include <iostream>

#define RELAY_WS_MAX_PAYLOAD (64 * 1024)

static std::mutex relayWsContextMutex;

namespace BrainCloud
{
    static struct lws_protocols relayWsProtocols[] = {
        {
            "bcrs",
            &DefaultRelayWSSocket::libWebsocketsCallback,
            0,
            RELAY_WS_MAX_PAYLOAD,
            0, NULL, 0
        },
        { NULL, NULL, 0, 0 } /* terminator */
    };

    IRelayWSSocket* IRelayWSSocket::create(const std::string& address, int port, int maxPacketSize, bool useSSL)
    {
        return new DefaultRelayWSSocket(address, port, maxPacketSize, useSSL);
    }

    DefaultRelayWSSocket::~DefaultRelayWSSocket()
    {
        close();
    }

    DefaultRelayWSSocket::DefaultRelayWSSocket(const std::string& address, int port, int maxPacketSize, bool useSSL)
        : _isValid(false)
        , _isConnecting(true)
        , _pLwsContext(NULL)
        , _pLws(NULL)
        , _host(address)
        , _path("/")
    {
        // A full URL (secureUrl) is used as given; port is ignored.
        if (address.find("://") != std::string::npos)
        {
            std::vector<char> buf(address.begin(), address.end());
            buf.push_back('\0');
            const char *prot, *host, *path;
            if (lws_parse_uri(buf.data(), &prot, &host, &port, &path))
            {
                std::cout << "Relay WS: Invalid url " << address << std::endl;
                _isConnecting = false;
                return;
            }
            useSSL = strcmp(prot, "wss") == 0;
            _host = host;
            _path = std::string("/") + path; // lws strips the leading '/'
        }

        // Create lws context
        {
            struct lws_context_creation_info info;
            memset(&info, 0, sizeof(info));

            info.port = CONTEXT_PORT_NO_LISTEN;
            info.protocols = relayWsProtocols;
            info.gid = -1;
            info.uid = -1;
            info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
            if (useSSL)
            {
                #if (LWS_LIBRARY_VERSION_MAJOR >= 4) && !defined(BC_SSL_ALLOW_SELFSIGNED) && defined(LWS_WITH_MBEDTLS)
                    info.client_ssl_ca_filepath = CACERTS_FILE_PATH;
                #endif
            }

            std::unique_lock<std::mutex> lock(relayWsContextMutex);
            _pLwsContext = lws_create_context(&info);
            if (!_pLwsContext)
            {
                std::cout << "Relay WS: Failed to create websocket context" << std::endl;
                _isConnecting = false;
                return;
            }
        }

        // Create connection
        {
            struct lws_client_connect_info connectInfo;
            memset(&connectInfo, 0, sizeof(connectInfo));

            connectInfo.context = _pLwsContext;
            connectInfo.address = _host.c_str();
            connectInfo.port = port;
            if (useSSL)
            {
                connectInfo.ssl_connection =
                    LCCSCF_USE_SSL
#if defined(BC_SSL_ALLOW_SELFSIGNED)
                    | LCCSCF_ALLOW_SELFSIGNED
#endif
                    ;
            }
            else
            {
                connectInfo.ssl_connection = 0;
            }
            connectInfo.path = _path.c_str();
            connectInfo.host = _host.c_str();
            connectInfo.origin = _host.c_str();
            connectInfo.protocol = relayWsProtocols[0].name;
            connectInfo.ietf_version_or_minus_one = -1;
            connectInfo.userdata = this;

            _pLws = lws_client_connect_via_info(&connectInfo);
            if (!_pLws)
            {
                std::cout << "Relay WS: Failed to create websocket client" << std::endl;
                _isConnecting = false;
                return;
            }

            // Start background service thread
            _updateThread = std::thread([this]()
            {
                _mutex.lock();
                while (_isConnecting || _isValid)
                {
                    _mutex.unlock();
                    lws_service(_pLwsContext, 0);
                    lws_callback_on_writable_all_protocol(_pLwsContext, &relayWsProtocols[0]);
                    _mutex.lock();
                }
                _mutex.unlock();
            });
        }
    }

    int DefaultRelayWSSocket::libWebsocketsCallback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
    {
        DefaultRelayWSSocket* pSocket = (DefaultRelayWSSocket*)lws_wsi_user(wsi);

        switch (reason)
        {
            case LWS_CALLBACK_WSI_DESTROY:
            case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
            case LWS_CALLBACK_CLOSED:
            case LWS_CALLBACK_CLIENT_CLOSED:
            {
                if (!pSocket) return -1;
                pSocket->onClose();
                break;
            }
            case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            {
                if (!pSocket) return -1;
                pSocket->onError(in ? (const char*)in : "connection error");
                break;
            }
            case LWS_CALLBACK_CLIENT_ESTABLISHED:
            {
                if (!pSocket) return -1;
                pSocket->onConnect();
                break;
            }
            case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                if (!pSocket || !in) return -1;
                pSocket->onRecv((const uint8_t*)in, (int)len);
                break;
            }
            case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                if (!pSocket) return -1;
                pSocket->processSendQueue();
                break;
            }
            default:
                break;
        }

        return 0;
    }

    bool DefaultRelayWSSocket::isConnected()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return !_isConnecting && _isValid;
    }

    bool DefaultRelayWSSocket::isValid()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        return _isConnecting || _isValid;
    }

    void DefaultRelayWSSocket::updateConnection()
    {
        // lws background thread drives the connection asynchronously
    }

    void DefaultRelayWSSocket::send(const uint8_t* pData, int size)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (!_isValid && !_isConnecting) return;
        _sendQueue.push(std::vector<uint8_t>(pData, pData + size));
    }

    void DefaultRelayWSSocket::processSendQueue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (!_sendQueue.empty())
        {
            const auto& packet = _sendQueue.front();
            _sendBuffer.resize(LWS_PRE + packet.size());
            memcpy(_sendBuffer.data() + LWS_PRE, packet.data(), packet.size());
            lws_write(_pLws, _sendBuffer.data() + LWS_PRE, packet.size(), LWS_WRITE_BINARY);
            _sendQueue.pop();
        }
    }

    const uint8_t* DefaultRelayWSSocket::peek(int& size)
    {
        size = 0;
        std::unique_lock<std::mutex> lock(_recvMutex);
        if (_recvQueue.empty()) return nullptr;
        _currentPacket = std::move(_recvQueue.front());
        _recvQueue.pop();
        size = (int)_currentPacket.size();
        return _currentPacket.data();
    }

    void DefaultRelayWSSocket::close()
    {
        // Signal thread to stop
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while (!_sendQueue.empty()) _sendQueue.pop();
            _isValid = false;
            _isConnecting = false;
        }

        // Unblock lws_service() so the update thread can exit
        if (_pLwsContext)
        {
            lws_cancel_service(_pLwsContext);
        }

        // Join update thread
        if (_updateThread.joinable())
        {
            _updateThread.join();
        }

        // Kill the lws connection
        if (_pLws)
        {
            lws_set_timeout(_pLws, NO_PENDING_TIMEOUT, LWS_TO_KILL_SYNC);
            _pLws = NULL;
        }

        // Destroy context
        if (_pLwsContext)
        {
            std::unique_lock<std::mutex> lock(relayWsContextMutex);
            lws_context_destroy(_pLwsContext);
            _pLwsContext = NULL;
        }
    }

    void DefaultRelayWSSocket::onClose()
    {
        std::cout << "Relay WS: closed" << std::endl;
        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = false;
        _isConnecting = false;
        _pLws = NULL;
    }

    void DefaultRelayWSSocket::onError(const char* msg)
    {
        std::cout << "Relay WS error: " << msg << std::endl;
        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = false;
        _isConnecting = false;
        _pLws = NULL;
    }

    void DefaultRelayWSSocket::onConnect()
    {
        std::cout << "Relay WS: connected" << std::endl;
        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = true;
        _isConnecting = false;
    }

    void DefaultRelayWSSocket::onRecv(const uint8_t* buffer, int len)
    {
        std::unique_lock<std::mutex> lock(_recvMutex);
        _recvQueue.push(std::vector<uint8_t>(buffer, buffer + len));
    }
};

#endif /* TARGET_OS_WATCH */
