#include "braincloud/internal/nix/HTTPURLLoader.h"
#include "braincloud/internal/URLRequestMethod.h"
#include "braincloud/internal/URLRequestHeader.h"
#include <chrono>

using namespace std::chrono;
namespace BrainCloud
{
    HTTPURLLoader::HTTPURLLoader() {}
    HTTPURLLoader::~HTTPURLLoader()
    {
        close();
        if (_thread.joinable()) _thread.join();
    }

    void HTTPURLLoader::close()
    {
        _cancelRequested.store(true);
        // stop client if running
        if (_client) {
            _client->stop();
            _client.reset();
        }
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        if (_sslClient) {
            _sslClient->stop();
            _sslClient.reset();
        }
#endif
        _threadRunning.store(false);
    }

    void HTTPURLLoader::load(URLRequest const & req)
    {
        setRequest(req);
        // ensure previous cleaned up
        close();
        _cancelRequested.store(false);
        _threadRunning.store(true);

        // start thread
        _thread = std::thread([this]() { runRequest(); });
        _thread.detach();
    }

    bool HTTPURLLoader::isDone()
    {
        return !_threadRunning.load();
    }

    // helper: parse scheme + host/path from full URL
    static void split_scheme_host_path(const std::string &url, std::string &scheme, std::string &host, std::string &path, int &port) {
        // very small parser (assumes well formed URLs). extend if needed.
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
            port = std::stoi(hostport.substr(colon+1));
        } else {
            host = hostport;
            port = (scheme == "https") ? 443 : 80;
        }
    }

    void HTTPURLLoader::runRequest()
    {
        const URLRequest & req = getRequest();
        URLResponse & res = getResponse();
        std::string scheme, host, path; int port;
        split_scheme_host_path(req.getUrl(), scheme, host, path, port);

        // prepare headers
        httplib::Headers headers;
        for (auto &h : req.getHeaders()) headers.emplace(h.getName(), h.getValue());
        if (!req.getUserAgent().empty()) headers.emplace("User-Agent", req.getUserAgent());
        if (!req.getContentType().empty()) headers.emplace("Content-Type", req.getContentType());

        // timeouts in seconds + microseconds
        std::chrono::seconds sec(_timeoutMs / 1000);
        std::chrono::microseconds usec((_timeoutMs % 1000) * 1000);

        std::shared_ptr<httplib::Response> response;

        try {
            if (scheme == "https") {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
                _sslClient = std::make_unique<httplib::SSLClient>(host.c_str(), port);
                _sslClient->set_read_timeout(sec.count(), usec.count());
                _sslClient->set_write_timeout(sec.count(), usec.count());
                if (req.getMethod() == URLRequestMethod::GET) {
                    std::string full = path;
                    if (!req.getData().empty()) full += std::string("?") + req.getData();
                    response = _sslClient->Get(full.c_str(), headers);
                } else if (req.getMethod() == URLRequestMethod::POST) {
                    response = _sslClient->Post(path.c_str(), headers, req.getData(), req.getContentType().c_str());
                } else if (req.getMethod() == URLRequestMethod::PUT) {
                    response = _sslClient->Put(path.c_str(), headers, req.getData(), req.getContentType().c_str());
                }
#else
                // no ssl support compiled in
                res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
                res.setReasonPhrase("SSL not supported in this build");
                _threadRunning.store(false);
                return;
#endif
            } else {
                _client = std::make_unique<httplib::Client>(host.c_str(), port);
                _client->set_read_timeout(sec.count(), usec.count());
                _client->set_write_timeout(sec.count(), usec.count());
                if (req.getMethod() == URLRequestMethod::GET) {
                    std::string full = path;
                    if (!req.getData().empty()) full += std::string("?") + req.getData();
                    response = _client->Get(full.c_str(), headers);
                } else if (req.getMethod() == URLRequestMethod::POST) {
                    response = _client->Post(path.c_str(), headers, req.getData(), req.getContentType().c_str());
                } else if (req.getMethod() == URLRequestMethod::PUT) {
                    response = _client->Put(path.c_str(), headers, req.getData(), req.getContentType().c_str());
                }
            }

            if (_cancelRequested.load()) {
                res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
                res.setReasonPhrase("Cancelled");
            } else if (response) {
                res.setStatusCode(static_cast<unsigned short>(response->status));
                res.setData(response->body);
                for (auto &h : response->headers) res.addHeader(URLRequestHeader(h.first, h.second));
            } else {
                res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
                res.setReasonPhrase("No response");
            }
        }
        catch (const std::exception & e) {
            res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
            res.setReasonPhrase(e.what());
        }

        // cleanup
        if (_client) { _client->stop(); _client.reset(); }
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
        if (_sslClient) { _sslClient->stop(); _sslClient.reset(); }
#endif
        _threadRunning.store(false);
    }
}
