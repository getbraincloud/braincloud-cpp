#if (!defined(TARGET_OS_WATCH) || TARGET_OS_WATCH == 0)

#include "braincloud/internal/DefaultWebSocket.h"

#include <algorithm>
#include <iostream>
#include <cctype>

#define MAX_PAYLOAD (64 * 1024)

static std::mutex lwsContextMutex;

namespace BrainCloud
{
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
        lws_set_log_level(
            //LLL_DEBUG, NULL);
            LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE, NULL);

        InitializeSSLCertificates();

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
            #if(LWS_LIBRARY_VERSION_MAJOR >= 4)
                info.options |= LWS_SERVER_OPTION_DISABLE_OS_CA_CERTS;
                info.client_ssl_ca_mem = full_certs.front().c_str();
                info.client_ssl_ca_mem_len = static_cast<unsigned int>(full_certs.front().length());
            #endif
            std::unique_lock<std::mutex> lock(lwsContextMutex);
            _pLwsContext = lws_create_context(&info);
            if (!_pLwsContext)
            {
                std::cout << "Failed to create websocket context" << std::endl;
                return;
            }
        }

        // Start update thread
        _updateThread = std::thread([this]()
        {
            _mutex.lock();
            while (_isConnecting || _isValid)
            {
                _mutex.unlock();
                lws_callback_on_writable_all_protocol(_pLwsContext, &protocols[0]);
                lws_service(_pLwsContext, 0);
                _mutex.lock();
            }
            _mutex.unlock();
        });

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
        }
    }

    void DefaultWebSocket::InitializeSSLCertificates() const {
        // Go Daddy Class 2 CA
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIEADCCAuigAwIBAgIBADANBgkqhkiG9w0BAQUFADBjMQswCQYDVQQGEwJVUzEh\n"
                              "MB8GA1UEChMYVGhlIEdvIERhZGR5IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBE\n"
                              "YWRkeSBDbGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5MB4XDTA0MDYyOTE3\n"
                              "MDYyMFoXDTM0MDYyOTE3MDYyMFowYzELMAkGA1UEBhMCVVMxITAfBgNVBAoTGFRo\n"
                              "ZSBHbyBEYWRkeSBHcm91cCwgSW5jLjExMC8GA1UECxMoR28gRGFkZHkgQ2xhc3Mg\n"
                              "MiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0eTCCASAwDQYJKoZIhvcNAQEBBQADggEN\n"
                              "ADCCAQgCggEBAN6d1+pXGEmhW+vXX0iG6r7d/+TvZxz0ZWizV3GgXne77ZtJ6XCA\n"
                              "PVYYYwhv2vLM0D9/AlQiVBDYsoHUwHU9S3/Hd8M+eKsaA7Ugay9qK7HFiH7Eux6w\n"
                              "wdhFJ2+qN1j3hybX2C32qRe3H3I2TqYXP2WYktsqbl2i/ojgC95/5Y0V4evLOtXi\n"
                              "EqITLdiOr18SPaAIBQi2XKVlOARFmR6jYGB0xUGlcmIbYsUfb18aQr4CUWWoriMY\n"
                              "avx4A6lNf4DD+qta/KFApMoZFv6yyO9ecw3ud72a9nmYvLEHZ6IVDd2gWMZEewo+\n"
                              "YihfukEHU1jPEX44dMX4/7VpkI+EdOqXG68CAQOjgcAwgb0wHQYDVR0OBBYEFNLE\n"
                              "sNKR1EwRcbNhyz2h/t2oatTjMIGNBgNVHSMEgYUwgYKAFNLEsNKR1EwRcbNhyz2h\n"
                              "/t2oatTjoWekZTBjMQswCQYDVQQGEwJVUzEhMB8GA1UEChMYVGhlIEdvIERhZGR5\n"
                              "IEdyb3VwLCBJbmMuMTEwLwYDVQQLEyhHbyBEYWRkeSBDbGFzcyAyIENlcnRpZmlj\n"
                              "YXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8wDQYJKoZIhvcNAQEFBQAD\n"
                              "ggEBADJL87LKPpH8EsahB4yOd6AzBhRckB4Y9wimPQoZ+YeAEW5p5JYXMP80kWNy\n"
                              "OO7MHAGjHZQopDH2esRU1/blMVgDoszOYtuURXO1v0XJJLXVggKtI3lpjbi2Tc7P\n"
                              "TMozI+gciKqdi0FuFskg5YmezTvacPd+mSYgFFQlq25zheabIZ0KbIIOqPjCDPoQ\n"
                              "HmyW74cNxA9hi63ugyuV+I6ShHI56yDqg+2DzZduCLzrTia2cyvk0/ZM/iZx4mER\n"
                              "dEr/VxqHD3VILs9RaRegAhJhldXRQLIQTO7ErBBDpqWeCtWVYpoNz4iCxTIM5Cuf\n"
                              "ReYNnyicsbkqWletNw+vHX/bvZ8=\n"
                              "-----END CERTIFICATE-----");

        // Go Daddy Root Certificate Authority - G2
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIDxTCCAq2gAwIBAgIBADANBgkqhkiG9w0BAQsFADCBgzELMAkGA1UEBhMCVVMxEDAOBgNVBAgT\n"
                              "B0FyaXpvbmExEzARBgNVBAcTClNjb3R0c2RhbGUxGjAYBgNVBAoTEUdvRGFkZHkuY29tLCBJbmMu\n"
                              "MTEwLwYDVQQDEyhHbyBEYWRkeSBSb290IENlcnRpZmljYXRlIEF1dGhvcml0eSAtIEcyMB4XDTA5\n"
                              "MDkwMTAwMDAwMFoXDTM3MTIzMTIzNTk1OVowgYMxCzAJBgNVBAYTAlVTMRAwDgYDVQQIEwdBcml6\n"
                              "b25hMRMwEQYDVQQHEwpTY290dHNkYWxlMRowGAYDVQQKExFHb0RhZGR5LmNvbSwgSW5jLjExMC8G\n"
                              "A1UEAxMoR28gRGFkZHkgUm9vdCBDZXJ0aWZpY2F0ZSBBdXRob3JpdHkgLSBHMjCCASIwDQYJKoZI\n"
                              "hvcNAQEBBQADggEPADCCAQoCggEBAL9xYgjx+lk09xvJGKP3gElY6SKDE6bFIEMBO4Tx5oVJnyfq\n"
                              "9oQbTqC023CYxzIBsQU+B07u9PpPL1kwIuerGVZr4oAH/PMWdYA5UXvl+TW2dE6pjYIT5LY/qQOD\n"
                              "+qK+ihVqf94Lw7YZFAXK6sOoBJQ7RnwyDfMAZiLIjWltNowRGLfTshxgtDj6AozO091GB94KPutd\n"
                              "fMh8+7ArU6SSYmlRJQVhGkSBjCypQ5Yj36w6gZoOKcUcqeldHraenjAKOc7xiID7S13MMuyFYkMl\n"
                              "NAJWJwGRtDtwKj9useiciAF9n9T521NtYJ2/LOdYq7hfRvzOxBsDPAnrSTFcaUaz4EcCAwEAAaNC\n"
                              "MEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYwHQYDVR0OBBYEFDqahQcQZyi27/a9\n"
                              "BUFuIMGU2g/eMA0GCSqGSIb3DQEBCwUAA4IBAQCZ21151fmXWWcDYfF+OwYxdS2hII5PZYe096ac\n"
                              "vNjpL9DbWu7PdIxztDhC2gV7+AJ1uP2lsdeu9tfeE8tTEH6KRtGX+rcuKxGrkLAngPnon1rpN5+r\n"
                              "5N9ss4UXnT3ZJE95kTXWXwTrgIOrmgIttRD02JDHBHNA7XIloKmf7J6raBKZV8aPEjoJpL1E/QYV\n"
                              "N8Gb5DKj7Tjo2GTzLH4U/ALqn83/B2gX2yKQOC16jdFU8WnjXzPKej17CuPKf1855eJ1usV2GDPO\n"
                              "LPAvTK33sefOT6jEm0pUBsV/fdUID+Ic/n4XuKxe9tQWskMJDE32p2u0mYRlynqI4uJEvlz36hz1\n"
                              "-----END CERTIFICATE-----");

        // DigiCert Assured ID Root G2
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIDljCCAn6gAwIBAgIQC5McOtY5Z+pnI7/Dr5r0SzANBgkqhkiG9w0BAQsFADBlMQswCQYDVQQG\n"
                              "EwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3d3cuZGlnaWNlcnQuY29tMSQw\n"
                              "IgYDVQQDExtEaWdpQ2VydCBBc3N1cmVkIElEIFJvb3QgRzIwHhcNMTMwODAxMTIwMDAwWhcNMzgw\n"
                              "MTE1MTIwMDAwWjBlMQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQL\n"
                              "ExB3d3cuZGlnaWNlcnQuY29tMSQwIgYDVQQDExtEaWdpQ2VydCBBc3N1cmVkIElEIFJvb3QgRzIw\n"
                              "ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDZ5ygvUj82ckmIkzTz+GoeMVSAn61UQbVH\n"
                              "35ao1K+ALbkKz3X9iaV9JPrjIgwrvJUXCzO/GU1BBpAAvQxNEP4HteccbiJVMWWXvdMX0h5i89vq\n"
                              "bFCMP4QMls+3ywPgym2hFEwbid3tALBSfK+RbLE4E9HpEgjAALAcKxHad3A2m67OeYfcgnDmCXRw\n"
                              "VWmvo2ifv922ebPynXApVfSr/5Vh88lAbx3RvpO704gqu52/clpWcTs/1PPRCv4o76Pu2ZmvA9OP\n"
                              "YLfykqGxvYmJHzDNw6YuYjOuFgJ3RFrngQo8p0Quebg/BLxcoIfhG69Rjs3sLPr4/m3wOnyqi+Rn\n"
                              "lTGNAgMBAAGjQjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBTO\n"
                              "w0q5mVXyuNtgv6l+vVa1lzan1jANBgkqhkiG9w0BAQsFAAOCAQEAyqVVjOPIQW5pJ6d1Ee88hjZv\n"
                              "0p3GeDgdaZaikmkuOGybfQTUiaWxMTeKySHMq2zNixya1r9I0jJmwYrA8y8678Dj1JGG0VDjA9tz\n"
                              "d29KOVPt3ibHtX2vK0LRdWLjSisCx1BL4GnilmwORGYQRI+tBev4eaymG+g3NJ1TyWGqolKvSnAW\n"
                              "hsI6yLETcDbYz+70CjTVW0z9B5yiutkBclzzTcHdDrEcDcRjvq30FPuJ7KJBDkzMyFdA0G4Dqs0M\n"
                              "jomZmWzwPDCvON9vvKO+KSAnq3T/EyJ43pdSVR6DtVQgA+6uwE9W3jfMw3+qBCe703e4YtsXfJwo\n"
                              "IhNzbM8m9Yop5w==\n"
                              "-----END CERTIFICATE-----");

        // DigiCert Assured ID Root G3
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIICRjCCAc2gAwIBAgIQC6Fa+h3foLVJRK/NJKBs7DAKBggqhkjOPQQDAzBlMQswCQYDVQQGEwJV\n"
                              "UzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3d3cuZGlnaWNlcnQuY29tMSQwIgYD\n"
                              "VQQDExtEaWdpQ2VydCBBc3N1cmVkIElEIFJvb3QgRzMwHhcNMTMwODAxMTIwMDAwWhcNMzgwMTE1\n"
                              "MTIwMDAwWjBlMQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
                              "d3cuZGlnaWNlcnQuY29tMSQwIgYDVQQDExtEaWdpQ2VydCBBc3N1cmVkIElEIFJvb3QgRzMwdjAQ\n"
                              "BgcqhkjOPQIBBgUrgQQAIgNiAAQZ57ysRGXtzbg/WPuNsVepRC0FFfLvC/8QdJ+1YlJfZn4f5dwb\n"
                              "RXkLzMZTCp2NXQLZqVneAlr2lSoOjThKiknGvMYDOAdfVdp+CW7if17QRSAPWXYQ1qAk8C3eNvJs\n"
                              "KTmjQjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBTL0L2p4ZgF\n"
                              "UaFNN6KDec6NHSrkhDAKBggqhkjOPQQDAwNnADBkAjAlpIFFAmsSS3V0T8gj43DydXLefInwz5Fy\n"
                              "YZ5eEJJZVrmDxxDnOOlYJjZ91eQ0hjkCMHw2U/Aw5WJjOpnitqM7mzT6HtoQknFekROn3aRukswy\n"
                              "1vUhZscv6pZjamVFkpUBtA==\n"
                              "-----END CERTIFICATE-----");

        // DigiCert Global Root G2
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBhMQswCQYDVQQG\n"
                              "EwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3d3cuZGlnaWNlcnQuY29tMSAw\n"
                              "HgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBHMjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUx\n"
                              "MjAwMDBaMGExCzAJBgNVBAYTAlVTMRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3\n"
                              "dy5kaWdpY2VydC5jb20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkq\n"
                              "hkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI2/Ou8jqJ\n"
                              "kTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx1x7e/dfgy5SDN67sH0NO\n"
                              "3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQq2EGnI/yuum06ZIya7XzV+hdG82MHauV\n"
                              "BJVJ8zUtluNJbd134/tJS7SsVQepj5WztCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyM\n"
                              "UNGPHgm+F6HmIcr9g+UQvIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQAB\n"
                              "o0IwQDAPBgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV5uNu\n"
                              "5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY1Yl9PMWLSn/pvtsr\n"
                              "F9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4NeF22d+mQrvHRAiGfzZ0JFrabA0U\n"
                              "WTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NGFdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBH\n"
                              "QRFXGU7Aj64GxJUTFy8bJZ918rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/\n"
                              "iyK5S9kJRaTepLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n"
                              "MrY=\n"
                              "-----END CERTIFICATE-----");

        // DigiCert Global Root G3
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIICPzCCAcWgAwIBAgIQBVVWvPJepDU1w6QP1atFcjAKBggqhkjOPQQDAzBhMQswCQYDVQQGEwJV\n"
                              "UzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3d3cuZGlnaWNlcnQuY29tMSAwHgYD\n"
                              "VQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBHMzAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAw\n"
                              "MDBaMGExCzAJBgNVBAYTAlVTMRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5k\n"
                              "aWdpY2VydC5jb20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEczMHYwEAYHKoZIzj0C\n"
                              "AQYFK4EEACIDYgAE3afZu4q4C/sLfyHS8L6+c/MzXRq8NOrexpu80JX28MzQC7phW1FGfp4tn+6O\n"
                              "YwwX7Adw9c+ELkCDnOg/QW07rdOkFFk2eJ0DQ+4QE2xy3q6Ip6FrtUPOZ9wj/wMco+I+o0IwQDAP\n"
                              "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUs9tIpPmhxdiuNkHMEWNp\n"
                              "Yim8S8YwCgYIKoZIzj0EAwMDaAAwZQIxAK288mw/EkrRLTnDCgmXc/SINoyIJ7vmiI1Qhadj+Z4y\n"
                              "3maTD/HMsQmP3Wyr+mt/oAIwOWZbwmSNuJ5Q3KjVSaLtx9zRSX8XAbjIho9OjIgrqJqpisXRAL34\n"
                              "VOKa5Vt8sycX\n"
                              "-----END CERTIFICATE-----");

        // DigiCert Trusted Root G4
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIFkDCCA3igAwIBAgIQBZsbV56OITLiOQe9p3d1XDANBgkqhkiG9w0BAQwFADBiMQswCQYDVQQG\n"
                              "EwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3d3cuZGlnaWNlcnQuY29tMSEw\n"
                              "HwYDVQQDExhEaWdpQ2VydCBUcnVzdGVkIFJvb3QgRzQwHhcNMTMwODAxMTIwMDAwWhcNMzgwMTE1\n"
                              "MTIwMDAwWjBiMQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
                              "d3cuZGlnaWNlcnQuY29tMSEwHwYDVQQDExhEaWdpQ2VydCBUcnVzdGVkIFJvb3QgRzQwggIiMA0G\n"
                              "CSqGSIb3DQEBAQUAA4ICDwAwggIKAoICAQC/5pBzaN675F1KPDAiMGkz7MKnJS7JIT3yithZwuEp\n"
                              "pz1Yq3aaza57G4QNxDAf8xukOBbrVsaXbR2rsnnyyhHS5F/WBTxSD1Ifxp4VpX6+n6lXFllVcq9o\n"
                              "k3DCsrp1mWpzMpTREEQQLt+C8weE5nQ7bXHiLQwb7iDVySAdYyktzuxeTsiT+CFhmzTrBcZe7Fsa\n"
                              "vOvJz82sNEBfsXpm7nfISKhmV1efVFiODCu3T6cw2Vbuyntd463JT17lNecxy9qTXtyOj4DatpGY\n"
                              "QJB5w3jHtrHEtWoYOAMQjdjUN6QuBX2I9YI+EJFwq1WCQTLX2wRzKm6RAXwhTNS8rhsDdV14Ztk6\n"
                              "MUSaM0C/CNdaSaTC5qmgZ92kJ7yhTzm1EVgX9yRcRo9k98FpiHaYdj1ZXUJ2h4mXaXpI8OCiEhtm\n"
                              "mnTK3kse5w5jrubU75KSOp493ADkRSWJtppEGSt+wJS00mFt6zPZxd9LBADMfRyVw4/3IbKyEbe7\n"
                              "f/LVjHAsQWCqsWMYRJUadmJ+9oCw++hkpjPRiQfhvbfmQ6QYuKZ3AeEPlAwhHbJUKSWJbOUOUlFH\n"
                              "dL4mrLZBdd56rF+NP8m800ERElvlEFDrMcXKchYiCd98THU/Y+whX8QgUWtvsauGi0/C1kVfnSD8\n"
                              "oR7FwI+isX4KJpn15GkvmB0t9dmpsh3lGwIDAQABo0IwQDAPBgNVHRMBAf8EBTADAQH/MA4GA1Ud\n"
                              "DwEB/wQEAwIBhjAdBgNVHQ4EFgQU7NfjgtJxXWRM3y5nP+e6mK4cD08wDQYJKoZIhvcNAQEMBQAD\n"
                              "ggIBALth2X2pbL4XxJEbw6GiAI3jZGgPVs93rnD5/ZpKmbnJeFwMDF/k5hQpVgs2SV1EY+CtnJYY\n"
                              "ZhsjDT156W1r1lT40jzBQ0CuHVD1UvyQO7uYmWlrx8GnqGikJ9yd+SeuMIW59mdNOj6PWTkiU0Tr\n"
                              "yF0Dyu1Qen1iIQqAyHNm0aAFYF/opbSnr6j3bTWcfFqK1qI4mfN4i/RN0iAL3gTujJtHgXINwBQy\n"
                              "7zBZLq7gcfJW5GqXb5JQbZaNaHqasjYUegbyJLkJEVDXCLG4iXqEI2FCKeWjzaIgQdfRnGTZ6iah\n"
                              "ixTXTBmyUEFxPT9NcCOGDErcgdLMMpSEDQgJlxxPwO5rIHQw0uA5NBCFIRUBCOhVMt5xSdkoF1BN\n"
                              "5r5N0XWs0Mr7QbhDparTwwVETyw2m+L64kW4I1NsBm9nVX9GtUw/bihaeSbSpKhil9Ie4u1Ki7wb\n"
                              "/UdKDd9nZn6yW0HQO+T0O/QEY+nvwlQAUaCKKsnOeMzV6ocEGLPOr0mIr/OSmbaz5mEP0oUA51Aa\n"
                              "5BuVnRmhuZyxm7EAHu/QD09CbMkKvO5D+jpxpchNJqU1/YldvIViHTLSoCtU7ZpXwdv6EM8Zt4tK\n"
                              "G48BtieVU+i2iW1bvGjUI+iLUaJW+fCmgKDWHrO8Dw9TdSmq6hN35N6MgSGtBxBHEa2HPQfRdbzP\n"
                              "82Z+\n"
                              "-----END CERTIFICATE-----");

        // Amazon Root CA 1
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsFADA5MQswCQYD\n"
                              "VQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24gUm9vdCBDQSAxMB4XDTE1\n"
                              "MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTELMAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpv\n"
                              "bjEZMBcGA1UEAxMQQW1hem9uIFJvb3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\n"
                              "ggEBALJ4gHHKeNXjca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgH\n"
                              "FzZM9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qwIFAGbHrQ\n"
                              "gLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6VOujw5H5SNz/0egwLX0t\n"
                              "dHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L93FcXmn/6pUCyziKrlA4b9v7LWIbxcce\n"
                              "VOF34GfID5yHI9Y/QCB/IIDEgEw+OyQmjgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB\n"
                              "/zAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3\n"
                              "DQEBCwUAA4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDIU5PM\n"
                              "CCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUsN+gDS63pYaACbvXy\n"
                              "8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vvo/ufQJVtMVT8QtPHRh8jrdkPSHCa\n"
                              "2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2\n"
                              "xJNDd2ZhwLnoQdeXeGADbkpyrqXRfboQnoZsG4q5WTP468SQvvG5\n"
                              "-----END CERTIFICATE-----");

        // Amazon Root CA 2
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIFQTCCAymgAwIBAgITBmyf0pY1hp8KD+WGePhbJruKNzANBgkqhkiG9w0BAQwFADA5MQswCQYD\n"
                              "VQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24gUm9vdCBDQSAyMB4XDTE1\n"
                              "MDUyNjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpv\n"
                              "bjEZMBcGA1UEAxMQQW1hem9uIFJvb3QgQ0EgMjCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoC\n"
                              "ggIBAK2Wny2cSkxKgXlRmeyKy2tgURO8TW0G/LAIjd0ZEGrHJgw12MBvIITplLGbhQPDW9tK6Mj4\n"
                              "kHbZW0/jTOgGNk3Mmqw9DJArktQGGWCsN0R5hYGCrVo34A3MnaZMUnbqQ523BNFQ9lXg1dKmSYXp\n"
                              "N+nKfq5clU1Imj+uIFptiJXZNLhSGkOQsL9sBbm2eLfq0OQ6PBJTYv9K8nu+NQWpEjTj82R0Yiw9\n"
                              "AElaKP4yRLuH3WUnAnE72kr3H9rN9yFVkE8P7K6C4Z9r2UXTu/Bfh+08LDmG2j/e7HJV63mjrdvd\n"
                              "fLC6HM783k81ds8P+HgfajZRRidhW+mez/CiVX18JYpvL7TFz4QuK/0NURBs+18bvBt+xa47mAEx\n"
                              "kv8LV/SasrlX6avvDXbR8O70zoan4G7ptGmh32n2M8ZpLpcTnqWHsFcQgTfJU7O7f/aS0ZzQGPSS\n"
                              "btqDT6ZjmUyl+17vIWR6IF9sZIUVyzfpYgwLKhbcAS4y2j5L9Z469hdAlO+ekQiG+r5jqFoz7Mt0\n"
                              "Q5X5bGlSNscpb/xVA1wf+5+9R+vnSUeVC06JIglJ4PVhHvG/LopyboBZ/1c6+XUyo05f7O0oYtlN\n"
                              "c/LMgRdg7c3r3NunysV+Ar3yVAhU/bQtCSwXVEqY0VThUWcI0u1ufm8/0i2BWSlmy5A5lREedCf+\n"
                              "3euvAgMBAAGjQjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBSw\n"
                              "DPBMMPQFWAJI/TPlUq9LhONmUjANBgkqhkiG9w0BAQwFAAOCAgEAqqiAjw54o+Ci1M3m9Zh6O+oA\n"
                              "A7CXDpO8Wqj2LIxyh6mx/H9z/WNxeKWHWc8w4Q0QshNabYL1auaAn6AFC2jkR2vHat+2/XcycuUY\n"
                              "+gn0oJMsXdKMdYV2ZZAMA3m3MSNjrXiDCYZohMr/+c8mmpJ5581LxedhpxfL86kSk5Nrp+gvU5LE\n"
                              "YFiwzAJRGFuFjWJZY7attN6a+yb3ACfAXVU3dJnJUH/jWS5E4ywl7uxMMne0nxrpS10gxdr9HIcW\n"
                              "xkPo1LsmmkVwXqkLN1PiRnsn/eBG8om3zEK2yygmbtmlyTrIQRNg91CMFa6ybRoVGld45pIq2WWQ\n"
                              "gj9sAq+uEjonljYE1x2igGOpm/HlurR8FLBOybEfdF849lHqm/osohHUqS0nGkWxr7JOcQ3AWEbW\n"
                              "aQbLU8uz/mtBzUF+fUwPfHJ5elnNXkoOrJupmHN5fLT0zLm4BwyydFy4x2+IoZCn9Kr5v2c69BoV\n"
                              "Yh63n749sSmvZ6ES8lgQGVMDMBu4Gon2nL2XA46jCfMdiyHxtN/kHNGfZQIG6lzWE7OE76KlXIx3\n"
                              "KadowGuuQNKotOrN8I1LOJwZmhsoVLiJkO/KdYE+HvJkJMcYr07/R54H9jVlpNMKVv/1F2Rs76gi\n"
                              "JUmTtt8AF9pYfl3uxRuw0dFfIRDH+fO6AgonB8Xx1sfT4PsJYGw=\n"
                              "-----END CERTIFICATE-----");

        // Amazon Root CA 3
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIBtjCCAVugAwIBAgITBmyf1XSXNmY/Owua2eiedgPySjAKBggqhkjOPQQDAjA5MQswCQYDVQQG\n"
                              "EwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24gUm9vdCBDQSAzMB4XDTE1MDUy\n"
                              "NjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZ\n"
                              "MBcGA1UEAxMQQW1hem9uIFJvb3QgQ0EgMzBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABCmXp8ZB\n"
                              "f8ANm+gBG1bG8lKlui2yEujSLtf6ycXYqm0fc4E7O5hrOXwzpcVOho6AF2hiRVd9RFgdszflZwjr\n"
                              "Zt6jQjBAMA8GA1UdEwEB/wQFMAMBAf8wDgYDVR0PAQH/BAQDAgGGMB0GA1UdDgQWBBSrttvXBp43\n"
                              "rDCGB5Fwx5zEGbF4wDAKBggqhkjOPQQDAgNJADBGAiEA4IWSoxe3jfkrBqWTrBqYaGFy+uGh0Psc\n"
                              "eGCmQ5nFuMQCIQCcAu/xlJyzlvnrxir4tiz+OpAUFteMYyRIHN8wfdVoOw==\n"
                              "-----END CERTIFICATE-----");

        // Amazon Root CA 4
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIB8jCCAXigAwIBAgITBmyf18G7EEwpQ+Vxe3ssyBrBDjAKBggqhkjOPQQDAzA5MQswCQYDVQQG\n"
                              "EwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6b24gUm9vdCBDQSA0MB4XDTE1MDUy\n"
                              "NjAwMDAwMFoXDTQwMDUyNjAwMDAwMFowOTELMAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZ\n"
                              "MBcGA1UEAxMQQW1hem9uIFJvb3QgQ0EgNDB2MBAGByqGSM49AgEGBSuBBAAiA2IABNKrijdPo1MN\n"
                              "/sGKe0uoe0ZLY7Bi9i0b2whxIdIA6GO9mif78DluXeo9pcmBqqNbIJhFXRbb/egQbeOc4OO9X4Ri\n"
                              "83BkM6DLJC9wuoihKqB1+IGuYgbEgds5bimwHvouXKNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n"
                              "HQ8BAf8EBAMCAYYwHQYDVR0OBBYEFNPsxzplbszh2naaVvuc84ZtV+WBMAoGCCqGSM49BAMDA2gA\n"
                              "MGUCMDqLIfG9fhGt0O9Yli/W651+kI0rz2ZVwyzjKKlwCkcO8DdZEv8tmZQoTipPNU0zWgIxAOp1\n"
                              "AE47xDqUEpHJWEadIRNyp4iciuRMStuW1KyLa2tJElMzrdfkviT8tQp21KW8EA==\n"
                              "-----END CERTIFICATE-----");

        // ISRG Root X1 - Let's Encrypt
        full_certs.push_back( "-----BEGIN CERTIFICATE-----\n"
                              "MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAwTzELMAkGA1UE\n"
                              "BhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2VhcmNoIEdyb3VwMRUwEwYDVQQD\n"
                              "EwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQG\n"
                              "EwJVUzEpMCcGA1UEChMgSW50ZXJuZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMT\n"
                              "DElTUkcgUm9vdCBYMTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54r\n"
                              "Vygch77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+0TM8ukj1\n"
                              "3Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6UA5/TR5d8mUgjU+g4rk8K\n"
                              "b4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sWT8KOEUt+zwvo/7V3LvSye0rgTBIlDHCN\n"
                              "Aymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyHB5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ\n"
                              "4Q7e2RCOFvu396j3x+UCB5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf\n"
                              "1b0SHzUvKBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWnOlFu\n"
                              "hjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTnjh8BCNAw1FtxNrQH\n"
                              "usEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbwqHyGO0aoSCqI3Haadr8faqU9GY/r\n"
                              "OPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CIrU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4G\n"
                              "A1UdDwEB/wQEAwIBBjAPBgNVHRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY\n"
                              "9umbbjANBgkqhkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
                              "ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ3BebYhtF8GaV\n"
                              "0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KKNFtY2PwByVS5uCbMiogziUwt\n"
                              "hDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJw\n"
                              "TdwJx4nLCgdNbOhdjsnvzqvHu7UrTkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nx\n"
                              "e5AW0wdeRlN8NwdCjNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZA\n"
                              "JzVcoyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq4RgqsahD\n"
                              "YVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPAmRGunUHBcnWEvgJBQl9n\n"
                              "JEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57demyPxgcYxn/eR44/KJ4EBs+lVDR3veyJ\n"
                              "m+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
                              "-----END CERTIFICATE-----");

    }

    int DefaultWebSocket::libWebsocketsCallback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len)
    {
        DefaultWebSocket *pWebSocket = (DefaultWebSocket *)lws_wsi_user(wsi);

        switch (reason)
        {
            case LWS_CALLBACK_WSI_DESTROY:
            case LWS_CALLBACK_CLOSED_CLIENT_HTTP:
            case LWS_CALLBACK_CLOSED:
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
#if defined(BC_MBEDTLS_OFF)
            case LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS:
            {
                pWebSocket->addExtraRootCerts((SSL_CTX *)user);

                break;
            }
#endif
            default:
                break;
        }

        return 0;
    }

#if defined(BC_MBEDTLS_OFF)

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
