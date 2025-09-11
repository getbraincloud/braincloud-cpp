#if (!defined(TARGET_OS_WATCH) || TARGET_OS_WATCH == 0)

#include "braincloud/internal/DefaultWebSocket.h"
#include <openssl/ssl.h>
#include <openssl/x509_vfy.h> // for X509_STORE
#include <algorithm>
#include <iostream>
#include <cctype>

#define MAX_PAYLOAD (64 * 1024)

static std::mutex lwsContextMutex;

namespace BrainCloud
{
    // logging options include: LLL_DEBUG | LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE
    // call lws_set_log_level from main

    static struct lws_protocols protocols[] = {
        {
            "brainCloud",
            &DefaultWebSocket::libWebsocketsCallback,
            0,
            MAX_PAYLOAD,
            0, NULL, 0
        },
        { NULL, NULL, 0, 0 } /* terminator */
    };

    static const struct lws_extension exts[] = {
        {
            "permessage-deflate",
            lws_extension_callback_pm_deflate,
            "permessage-deflate; client_no_context_takeover"
        },
        {
            "deflate-frame",
            lws_extension_callback_pm_deflate,
            "deflate_frame"
        },
        { NULL, NULL, NULL /* terminator */ }
    };

    static std::vector<std::string> full_certs;
    static bool added = false;

    IWebSocket* IWebSocket::create(const std::string& address, int port, const std::map<std::string, std::string>& headers)
    {
        return new DefaultWebSocket(address, port, headers);
    }

    //struct lws_context* DefaultWebSocket::_pLwsContext = NULL;

    DefaultWebSocket::~DefaultWebSocket()
    {
        close();
    }

    DefaultWebSocket::DefaultWebSocket(const std::string& uri, int port, const std::map<std::string, std::string>& headers)
        : _isValid(false)
        , _pLwsContext(NULL)
        , _pLws(NULL)
        , _isConnecting(true)
        , _authHeaders(headers)
    {
        std::string uriCopy = uri;

        // Split address into host/addr/origin/protocol
        std::string protocol = uriCopy.substr(0, std::min<size_t>(uriCopy.size(), uriCopy.find_first_of(':')));
        size_t protocolSize = protocol.size() + 3;
        size_t argsPos = std::min<size_t>(uriCopy.size(), uriCopy.find_first_of('?', protocolSize));
        if (uriCopy[argsPos - 1] != '/')
        {
            uriCopy.insert(uriCopy.begin() + argsPos, '/');
        }
        size_t slashPos = std::min<size_t>(uriCopy.size(), uriCopy.find_first_of('/', protocolSize));
        std::string path = uriCopy.substr(slashPos);
        if (path.empty())
        {
            path = "/";
        }
        std::string addr = uriCopy.substr(protocolSize, slashPos - protocolSize);
        std::string host = addr;
        std::string origin = host + ":" + std::to_string(port);

        // Validate
        std::string protocolCaps = protocol;
        std::transform(protocol.begin(), protocol.end(), protocolCaps.begin(), [](unsigned char c){ return std::toupper(c); });
        if (protocolCaps != "WS" && protocolCaps != "WSS")
        {
            std::cout << "Invalid websocket protocol: " << protocol << std::endl;
            return;
        }
        bool useSSL = protocolCaps == "WSS";

        // Create context
        //if (!_pLwsContext)
        {
            struct lws_context_creation_info info;
            memset(&info, 0, sizeof info);

            info.port = CONTEXT_PORT_NO_LISTEN;
            info.protocols = protocols;
            info.gid = -1;
            info.uid = -1;
            //info.extensions = exts;
            info.options = LWS_SERVER_OPTION_VALIDATE_UTF8;
            info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
            #if(LWS_LIBRARY_VERSION_MAJOR >= 4) && !defined(BC_SSL_ALLOW_SELFSIGNED)
                //info.options |= LWS_SERVER_OPTION_DISABLE_OS_CA_CERTS;
                //info.client_ssl_ca_mem = full_certs.front().c_str();
                //info.client_ssl_ca_mem_len = static_cast<unsigned int>(full_certs.front().length());
            #endif
            std::unique_lock<std::mutex> lock(lwsContextMutex);
            _pLwsContext = lws_create_context(&info);
            if (!_pLwsContext)
            {
                std::cout << "Failed to create websocket context" << std::endl;
                return;
            }
        }

        // Create connection
        {
            struct lws_client_connect_info connectInfo;
            memset(&connectInfo, 0, sizeof(connectInfo));

            connectInfo.context = _pLwsContext;
            connectInfo.address = addr.c_str();
            connectInfo.port = port;
            if (useSSL)
            {
                connectInfo.ssl_connection = 
                    LCCSCF_USE_SSL
#if defined(BC_SSL_ALLOW_SELFSIGNED)
                    | LCCSCF_ALLOW_SELFSIGNED
#endif
                    // | LCCSCF_SKIP_SERVER_CERT_HOSTNAME_CHECK
                    // | LCCSCF_ALLOW_EXPIRED
                    ;
            }
            else
            {
                connectInfo.ssl_connection = 0;
            }
            connectInfo.path = path.c_str();
            connectInfo.host = host.c_str();
            connectInfo.origin = origin.c_str();
            connectInfo.protocol = protocols[0].name;
            connectInfo.ietf_version_or_minus_one = -1;
            connectInfo.userdata = this;

            _pLws = lws_client_connect_via_info(&connectInfo);

            if (!_pLws)
            {
                std::cout << "Failed to create websocket client" << std::endl;
                return;
            }

            // Start update thread
            _updateThread = std::thread([this]()
                {
                    _mutex.lock();
                    while (_isConnecting || _isValid)
                    {
                        _mutex.unlock();
                        lws_service(_pLwsContext, 0);
                        lws_callback_on_writable_all_protocol(_pLwsContext, &protocols[0]);
                        _mutex.lock();
                    }
                    _mutex.unlock();
                });
        }
    }

    int DefaultWebSocket::libWebsocketsCallback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
    {
        DefaultWebSocket *pWebSocket = (DefaultWebSocket *)lws_wsi_user(wsi);

        switch (reason)
        {
            case LWS_CALLBACK_WSI_DESTROY:
            case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
            case LWS_CALLBACK_CLOSED:
            case LWS_CALLBACK_CLIENT_CLOSED:
            {
                if (!pWebSocket)
                {
                    return -1;
                }
                pWebSocket->onClose();
                break;
            }
            case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
            {
                if (!pWebSocket || !in)
                {
                    return -1;
                }
                pWebSocket->onError((const char*)in);
                break;
            }
            case LWS_CALLBACK_CLIENT_ESTABLISHED:
            {
                if (!pWebSocket)
                {
                    return -1;
                }
                pWebSocket->onConnect();
                break;
            }
            case LWS_CALLBACK_CLIENT_RECEIVE:
            {
                if (!pWebSocket || !in)
                {
                    return -1;
                }
                pWebSocket->onRecv((const char*)in, (int)len);
                break;
            }
            case LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER:
            {
                if (!pWebSocket || !in)
                {
                    return -1;
                }

                unsigned char **p = (unsigned char **)in;
                unsigned char *end = (*p) + len;
                if (!pWebSocket->onProcessHeaders(p, end))
                {
                    return -1;
                }
                break;
            }
            case LWS_CALLBACK_CLIENT_WRITEABLE:
            {
                if (!pWebSocket)
                {
                    return -1;
                }
                pWebSocket->processSendQueue();
                break;
            }
#if defined(BC_MBEDTLS_OFF) && !defined(BC_SSL_ALLOW_SELFSIGNED)
            case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS:
            {
                if(!added)
                    pWebSocket->addExtraRootCerts((SSL_CTX *)user);
                added = true;

                break;
            }
#endif
            default:
                break;
        }

        return 0;
    }

#if defined(BC_MBEDTLS_OFF) && !defined(BC_SSL_ALLOW_SELFSIGNED)

    void DefaultWebSocket::addExtraRootCerts(SSL_CTX *ssl_ctx) {
        for (std::vector<std::string>::iterator cert = full_certs.begin();
             cert < full_certs.end(); ++cert){
            addCertString(*cert, ssl_ctx);
        }
    }

    void DefaultWebSocket::addCertString(std::string certString, SSL_CTX *ssl_ctx) {

        lws_filepos_t amount = 0;
        const uint8_t *up;
        uint8_t *up1;
        uint8_t *pem;
        size_t decodedlen;

        std::string stripped_cert = certString.replace(0, strlen("-----BEGIN CERTIFICATE-----\n"), "");

        size_t npos = stripped_cert.find("-----END CERTIFICATE-----");

        stripped_cert = stripped_cert.replace(npos, strlen("-----END CERTIFICATE-----"), "");

        const char *cacert = stripped_cert.c_str();
        size_t calen = stripped_cert.length();
        decodedlen = (calen * 3) / 4;
        pem = (uint8_t*)malloc(decodedlen);

        amount = lws_b64_decode_string_len((char *) cacert, calen,
                                           (char *) pem, (int) (long long) decodedlen);

        X509* client_CA = d2i_X509(NULL, (const unsigned char **)(&pem), (long)amount);

        if(client_CA != NULL){
            X509_STORE* x509_store = SSL_CTX_get_cert_store(ssl_ctx);
            X509_STORE_add_cert(x509_store, client_CA);
            X509_free(client_CA);
        }
        else{
            free(pem);
        }
    }
#endif

    bool DefaultWebSocket::isValid()
    {
        std::unique_lock<std::mutex> lock(_mutex);

        if (_isConnecting)
        {
            _connectionCondition.wait(lock, [this]() { return !_isConnecting; });
        }

        return _isValid;
    }

    void DefaultWebSocket::send(const std::string& message)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _sendQueue.push(message);
    }

    void DefaultWebSocket::processSendQueue()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while (!_sendQueue.empty())
        {
            const std::string& message = _sendQueue.front();

            _sendBuffer.resize(LWS_PRE + message.size());
            memcpy(_sendBuffer.data() + LWS_PRE, message.c_str(), message.size());
            lws_write(_pLws, _sendBuffer.data() + LWS_PRE, message.size(), LWS_WRITE_TEXT);

            _sendQueue.pop();
        }
    }

    std::string DefaultWebSocket::recv()
    {
        std::unique_lock<std::mutex> lock(_recvMutex);

        if (!_recvQueue.empty())
        {
            std::string message = _recvQueue.front();
            _recvQueue.pop();
            return message;
        }

        _recvCondition.wait(lock, [this]()
        {
            return !_isValid || !_recvQueue.empty();
        });

        if (_isValid && !_recvQueue.empty())
        {
            std::string message = _recvQueue.front();
            _recvQueue.pop();
            return message;
        }

        return "";
    }

    void DefaultWebSocket::close()
    {
        // Stop and clean send queue
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while (!_sendQueue.empty()) _sendQueue.pop();
            _isValid = false;
            _isConnecting = false;
            _connectionCondition.notify_all();
        }

        // Stop and clean recving
        {
            std::unique_lock<std::mutex> lock(_recvMutex);
            while (!_recvQueue.empty()) _recvQueue.pop();
            _recvCondition.notify_all();
        }

        // Join update thread
        if (_updateThread.joinable())
        {
            _updateThread.join();
        }

        // Destroy libWebSockets
        if (_pLws)
        {
            lws_set_timeout(_pLws, NO_PENDING_TIMEOUT, LWS_TO_KILL_SYNC);
            _pLws = NULL;
        }

        // Destroy the context
        if (_pLwsContext)
        {
            std::unique_lock<std::mutex> lock(lwsContextMutex);
            lws_context_destroy(_pLwsContext);
            _pLwsContext = NULL;
        }
    }

    void DefaultWebSocket::onClose()
    {
        std::cout << "WebSocket closed" << std::endl;

        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = false;
        _isConnecting = false;
        _connectionCondition.notify_all();
        _pLws = NULL; // This is now invalid
    }

    void DefaultWebSocket::onError(const char* msg)
    {
        std::cout << "WebSocket error: " << msg << std::endl;

        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = false;
        _isConnecting = false;
        _connectionCondition.notify_all();
        _pLws = NULL; // This is now invalid
    }

    void DefaultWebSocket::onConnect()
    {
        std::cout << "WebSocket Connected!" << std::endl;

        std::unique_lock<std::mutex> lock(_mutex);
        _isValid = true;
        _isConnecting = false;
        _connectionCondition.notify_all();
    }

    void DefaultWebSocket::onRecv(const char* buffer, int len)
    {
        std::unique_lock<std::mutex> lock(_recvMutex);
        std::string message(buffer, len);
        _recvQueue.push(message);
        _recvCondition.notify_all();
    }

    bool DefaultWebSocket::onProcessHeaders(unsigned char** ppBuffer, unsigned char* pEnd)
    {
        if (_authHeaders.empty())
        {
            return true;
        }

        std::string key;

        for (std::map<std::string, std::string>::iterator it = _authHeaders.begin(); it != _authHeaders.end(); ++it)
        {
            key = it->first;
            key += ":";
            const std::string& value = it->second;

            if (lws_add_http_header_by_name(_pLws, (const unsigned char *)key.c_str(), (const unsigned char *)value.c_str(), (int)value.size(), ppBuffer, pEnd))
            {
                return false;
            }
        }

        return true;
    }
};

#endif
