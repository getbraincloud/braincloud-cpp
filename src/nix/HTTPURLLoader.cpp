#include "braincloud/internal/nix/HTTPURLLoader.h"
#include "braincloud/internal/URLRequestMethod.h"
#include "braincloud/internal/URLRequestHeader.h"
#include "braincloud/http_codes.h"

#include <chrono>
#include <stdexcept>
#include <httplib.h>

using namespace std::chrono;

namespace BrainCloud
{
    HTTPURLLoader::HTTPURLLoader() {}

	URLLoader* URLLoader::create()
	{
		return new HTTPURLLoader();
	}

    HTTPURLLoader::~HTTPURLLoader()
    {
        close();
        if (_thread.joinable())
            _thread.join();
    }

    void HTTPURLLoader::close()
    {
        _cancelRequested.store(true);

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

    void HTTPURLLoader::load(URLRequest const& req)
    {
        setRequest(req);
        close();

        _cancelRequested.store(false);
        _threadRunning.store(true);

        _thread = std::thread([this]() { runRequest(); });
        _thread.detach();
    }

    bool HTTPURLLoader::isDone()
    {
        return !_threadRunning.load();
    }

    // --- small URL splitter ---
    static void split_scheme_host_path(const std::string& url,
                                       std::string& scheme,
                                       std::string& host,
                                       std::string& path,
                                       int& port)
    {
        scheme = "http";
        host = "";
        path = "/";
        port = 0;

        size_t p = url.find("://");
        size_t pos = 0;
        if (p != std::string::npos) {
            scheme = url.substr(0, p);
            pos = p + 3;
        }

        size_t slash = url.find('/', pos);
        std::string hostport = (slash == std::string::npos)
                                   ? url.substr(pos)
                                   : url.substr(pos, slash - pos);

        if (slash != std::string::npos)
            path = url.substr(slash);

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

    void HTTPURLLoader::runRequest()
	{
		const URLRequest& req = getRequest();
		URLResponse& res = getResponse();

		std::string scheme, host, path;
		int port;
		split_scheme_host_path(req.getUrl(), scheme, host, path, port);

		// headers
		httplib::Headers headers;
		for (auto& h : req.getHeaders())
			headers.emplace(h.getName(), h.getValue());

		if (!req.getUserAgent().empty())
			headers.emplace("User-Agent", req.getUserAgent());

		if (!req.getContentType().empty())
			headers.emplace("Content-Type", req.getContentType());

		// timeouts
		auto sec = std::chrono::seconds(_timeoutMs / 1000);
		auto usec = std::chrono::microseconds((_timeoutMs % 1000) * 1000);

		// This is the correct type
		httplib::Result result;

		try {
			// HTTPS
			if (scheme == "https") {
	#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
				_sslClient = std::make_unique<httplib::SSLClient>(host.c_str(), port);
				_sslClient->set_read_timeout(sec.count(), usec.count());
				_sslClient->set_write_timeout(sec.count(), usec.count());

				if (req.getMethod() == URLRequestMethod::GET) {
					std::string full = path;
					if (!req.getData().empty())
						full += "?" + req.getData();
					result = _sslClient->Get(full.c_str(), headers);
				}
				else if (req.getMethod() == URLRequestMethod::POST) {
					result = _sslClient->Post(path.c_str(),
						headers, req.getData(), req.getContentType().c_str());
				}
				else if (req.getMethod() == URLRequestMethod::PUT) {
					result = _sslClient->Put(path.c_str(),
						headers, req.getData(), req.getContentType().c_str());
				}
	#else
				res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
				res.setReasonPhrase("HTTPS request attempted but SSL disabled");
				_threadRunning.store(false);
				return;
	#endif
			}

			// HTTP
			else {
				_client = std::make_unique<httplib::Client>(host.c_str(), port);
				_client->set_read_timeout(sec.count(), usec.count());
				_client->set_write_timeout(sec.count(), usec.count());

				if (req.getMethod() == URLRequestMethod::GET) {
					std::string full = path;
					if (!req.getData().empty())
						full += "?" + req.getData();
					result = _client->Get(full.c_str(), headers);
				}
				else if (req.getMethod() == URLRequestMethod::POST) {
					result = _client->Post(path.c_str(),
						headers, req.getData(), req.getContentType().c_str());
				}
				else if (req.getMethod() == URLRequestMethod::PUT) {
					result = _client->Put(path.c_str(),
						headers, req.getData(), req.getContentType().c_str());
				}
			}

			// Cancellation
			if (_cancelRequested.load()) {
				res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
				res.setReasonPhrase("Cancelled");
			}

			// Network failure
			else if (!result) {
				res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
				res.setReasonPhrase(httplib::to_string(result.error()));
			}

			// Success
			else {
				// result.value() -> shared_ptr<httplib::Response>
				auto response = result.value();

				res.setStatusCode(static_cast<unsigned short>(response->status));
				res.setData(response->body);

				for (auto& h : response->headers)
					res.addHeader(URLRequestHeader(h.first, h.second));
			}
		}
		catch (const std::exception& e) {
			res.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
			res.setReasonPhrase(e.what());
		}

		// Cleanup
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

}
