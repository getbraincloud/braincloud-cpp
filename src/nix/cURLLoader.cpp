#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if (!defined(TARGET_OS_WATCH) || TARGET_OS_WATCH == 0)


#include "braincloud/internal/nix/cURLLoader.h"
#include "braincloud/internal/URLRequestMethod.h"
#include "braincloud/http_codes.h"


#include <iostream>
#include <vector>
#include <cstring>
#include <chrono>
#include <thread>


// RAII wrappers
namespace BrainCloud
{
	struct CurlHandle
	{
		CURL* h;
		CurlHandle(CURL* c = nullptr) : h(c) {}
		~CurlHandle() { if (h) curl_easy_cleanup(h); }
		operator CURL* () const { return h; }
	};


	struct CurlSList
	{
		curl_slist* list{ nullptr };
		~CurlSList() { if (list) curl_slist_free_all(list); }
		void append(const char* s) { list = curl_slist_append(list, s); }
	};
}

using namespace BrainCloud;


// static members
bool cURLLoader::_initialized = false;
long cURLLoader::_timeoutInterval = 5000; // default 5s


// Implementation
cURLLoader::cURLLoader()
{
#ifndef WIN32
	_socket = -1;
#else
	_socket = INVALID_SOCKET;
#endif
#if ( defined(GAMECLIENT_DEBUGLEVEL) && GAMECLIENT_DEBUGLEVEL > 0 )
	printCurlVersion();
#endif
}


cURLLoader::~cURLLoader()
{
	// Request cancellation and join thread to ensure safe destruction
	close();


#if !defined(USE_PTHREAD)
	if (_thread.joinable()) _thread.join();
#else
	// nothing extra here; close() handles pthread join
#endif
}

void cURLLoader::close()
{
	// Signal cancel
	_cancelRequested.store(true);


	// Wait for thread to clear _threadRunning (bounded wait)
	const int maxWaitMs = 5000;
	int waited = 0;
	while (_threadRunning.load() && waited < maxWaitMs)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		waited += 10;
	}


	// If we still have a running thread, join it (pthreads or std::thread)
#if defined(USE_PTHREAD)
	if (_threadRunning.load())
	{
		// try joining (blocks until thread exits); thread will check cancel flag
		pthread_join(_threadId, nullptr);
	}
#else
	if (_thread.joinable()) _thread.join();
#endif
}


void cURLLoader::load(URLRequest const& req)
{
	setRequest(req);


	if (!_initialized)
	{
		// use DEFAULT to avoid forcing signals
		curl_global_init(CURL_GLOBAL_DEFAULT);
		_initialized = true;
	}


	// Ensure any previous worker is asked to stop
	close();


	// Reset cancellation and start background thread
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
	// Create a socket normally; we don't store fd for manual closing
	(void)data; (void)purpose;
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
	std::string url = loader->getRequest().getUrl();
	std::string data = loader->getRequest().getData();
	if (!data.empty()) { url.append("?"); url.append(data); }
	curl_easy_setopt(curl.h, CURLOPT_URL, url.c_str());
	rc = curl_easy_perform(curl.h);
}
else if (loader->getRequest().getMethod() == URLRequestMethod::POST)
{
	curl_easy_setopt(curl.h, CURLOPT_URL, loader->getRequest().getUrl().c_str());
	curl_easy_setopt(curl.h, CURLOPT_POST, 1L);
	curl_easy_setopt(curl.h, CURLOPT_POSTFIELDSIZE, loader->getRequest().getData().length());
	curl_easy_setopt(curl.h, CURLOPT_COPYPOSTFIELDS, loader->getRequest().getData().c_str());
	rc = curl_easy_perform(curl.h);
	}
else if (loader->getRequest().getMethod() == URLRequestMethod::PUT)
{
	curl_easy_setopt(curl.h, CURLOPT_URL, loader->getRequest().getUrl().c_str());
	curl_easy_setopt(curl.h, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curl.h, CURLOPT_POSTFIELDSIZE, loader->getRequest().getData().length());
	curl_easy_setopt(curl.h, CURLOPT_COPYPOSTFIELDS, loader->getRequest().getData().c_str());
	rc = curl_easy_perform(curl.h);
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
		curl_easy_getinfo(curl.h, CURLINFO_RESPONSE_CODE, &statusCode);
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


	// If using pthreads, threadEntry will return, and the pthread should be joined by close()/destructor
}

#if ( defined(GAMECLIENT_DEBUGLEVEL) && GAMECLIENT_DEBUGLEVEL > 0 )
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
