#ifdef __APPLE__
#include <TargetConditionals.h>
#endif 

#if (!defined(TARGET_OS_WATCH) || TARGET_OS_WATCH == 0)// necessary as cocoapods doesn't allow per platform source files


#if ( defined(GAMECLIENT_DEBUGLEVEL)  &&  GAMECLIENT_DEBUGLEVEL > 0 )
#   include <iostream>
#endif
#include <cctype>

#ifdef HG_PLATFORM_BB
    #include <stdio.h>
    #include <ctype.h>
#endif

#include "curl/curl.h"
#include "braincloud/internal/nix/cURLLoader.h"
#include "braincloud/internal/URLRequestMethod.h"
#include "braincloud/http_codes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if !defined(USE_PTHREAD)
#include <thread>
#endif

namespace BrainCloud
{
    URLLoader* URLLoader::create()
    {
        return new cURLLoader();
    }

    bool cURLLoader::_initialized = false;
    long cURLLoader::_timeoutInterval = 0;

    /**
     * Constructor
     *   - NOTE:  If a URLRequest is given, the object will try to contact the
     *            server immediately by calling the load() method.
     */

    cURLLoader::cURLLoader()
        : _threadRunning(false)
    {
#ifndef WIN32
        _socket = -1;
#else
        _socket = INVALID_SOCKET;
#endif

#if defined(USE_PTHREAD)
        memset(&_threadId, 0, sizeof(pthread_t));
        memset(&_threadAttributes, 0, sizeof(_threadAttributes));
#endif
    }

    cURLLoader::~cURLLoader()
    {
        close();
    }

    /**
     * Close a currently running load operation, if in progress.
     */
    void cURLLoader::close()
    {
        // We can stop loading the page by killing its thread.
        if (_threadRunning)
        {
            // close socket directly to kill the curl request more quickly (WAT !??)
#ifndef WIN32
            if (_socket >= 0)
            {
                ::close(_socket);
            }
#else
            if (_socket != INVALID_SOCKET)
            {
                shutdown(_socket, SD_BOTH);
                closesocket(_socket);
            }
#endif
#if defined(USE_PTHREAD)
            pthread_attr_destroy(&_threadAttributes);
#else
#endif
        }
    }


    /**
     * Issue an HTTP Request to the remote server, and load the response.
     *
     * @param urlRequest - HTTP Request
     */
    void cURLLoader::load(URLRequest const & urlRequest)
    {
        // Assume the specified URL in the request is valid.
        setRequest(urlRequest);

        if (!_initialized)
        {
            curl_global_init(CURL_GLOBAL_ALL);
            _initialized = true;
        }

        // load is sometimes called while prev thread is processing
        // so make sure to cleanup beforehand...
        close();

#if defined(USE_PTHREAD)
        // Start up a thread to load the web page.
        // Set up a new thread to handle the request.
        int rc = 0;
        rc = pthread_attr_init(&_threadAttributes);

        if (rc == 0) {
            // Set the thread to detach when launched.
            rc = pthread_attr_setdetachstate(&_threadAttributes,
                PTHREAD_CREATE_DETACHED);
            if (rc == 0) {
                // Launch the thread.
                _threadRunning = true;
                rc = pthread_create(&_threadId, &_threadAttributes, &loadThread,
                    this);
                if (rc == 0) {
                    // Launched
                }
                else {
                    // Error
                    close();
                }  // end if
            }
            else {
                // Error
                pthread_attr_destroy(&_threadAttributes);
            }  // end if
        }
        else {
            // Error
        }  // end if
#else
        _threadRunning = true;
        auto thread = std::thread(loadThread, this);
        thread.detach();
#endif
    }

    bool cURLLoader::isDone()
    {
        return !_threadRunning;
    }


    /**
     * This is the writer call back function used by curl
     *
     * @param toWrite - data received from the remote server
     * @param size - size of a character (?)
     * @param nmemb - number of characters
     * @param data - pointer to the curlloader
     *
     * @return int - number of characters received (should equal size * nmemb)
     */
    size_t cURLLoader::writeData(
        char * toWrite,
        size_t size,
        size_t nmemb,
        void * data)
    {
        cURLLoader * loader = (cURLLoader*)data;

        // What we will return
        size_t result = 0;

        // Check for a valid response object.
        if (loader != NULL) {
            // Append the data to the buffer
            loader->_urlResponse.addData(std::string(toWrite, size * nmemb));

            // How much did we write?
            result = size * nmemb;
        }

        return result;
    }


    /**
     * This static method is called once per HTTP response header line.
     *
     * NOTE:  The initial HTTP status line is also sent to this function.
     *
     * @param line - string containing the header line
     * @param size - size of a character (?)
     * @param nmemb - number of characters
     * @param response - pointer to the object to receive the response
     */
    size_t cURLLoader::writeHeader(
        char * line,
        size_t size,
        size_t nmemb,
        void * data)
    {
        cURLLoader * loader = (cURLLoader*)data;
        size_t num_processed = 0;   // Number of bytes processed for this header
                                    // -1 means terminate URL request with an error

        // Check for a valid response object.
        if (loader != NULL)
        {
            // Parse the header line for Header: Value
            std::string headerLine(line, size * nmemb);
            std::string::size_type colon = headerLine.find(':');

            // Make sure the line contains a colon.
            if (colon != std::string::npos)
            {
                // The header name consists of everything up to the colon.
                URLRequestHeader header(headerLine.substr(0, colon));

                // The value starts at the first non-space after the colon.
                for (colon += 1; colon < headerLine.length(); colon += 1)
                {
                    if (!isspace(headerLine[colon]))
                    {
                        header.setValue(headerLine.substr(colon));
                        break;
                    }
                }

                // Add the new header to the response.
                loader->_urlResponse.addHeader(header);
            }

            // We have processed the entire header message.
            num_processed = size * nmemb;
        }

        return num_processed;
    }


    /**
     * This static method is the target of the thread creation.
     * Load up the proper reference to the object, and complete loading the page.
     *
     * @param urlLoader - pointer to the object which is loading the web page
     */
    void * cURLLoader::loadThread(void * urlLoader)
    {
        // This is the starting point of a new thread.
        // A pointer to the object should have been passed as urlLoader.
        // Verify the pointer.
        if (urlLoader != NULL)
        {
            cURLLoader * loader = reinterpret_cast<cURLLoader *>(urlLoader);
            loadThreadCurl(loader);
        }

        return NULL;
    }

    /*
    static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size)
    {
        size_t i;
        size_t c;
        unsigned int width=0x10;

        fprintf(stream, "%s, %10.10ld bytes (0x%8.8lx)\n",
                text, (long)size, (long)size);

        for(i=0; i<size; i+= width) {
            fprintf(stream, "%4.4lx: ", (long)i);

            // show hex to the left
            for(c = 0; c < width; c++) {
                if(i+c < size)
                    fprintf(stream, "%02x ", ptr[i+c]);
                else
                    fputs("   ", stream);
            }

            // show data on the right
            for(c = 0; (c < width) && (i+c < size); c++)
                fputc((ptr[i+c]>=0x20) && (ptr[i+c]<0x80)?ptr[i+c]:'.', stream);

            fputc('\n', stream); // newline
        }
    }
    */

    /*
    static int my_trace(CURL *handle, curl_infotype type, char *data, size_t size, void *userp)
    {
        const char *text;
        (void)handle; // prevent compiler warning

        switch (type) {
            case CURLINFO_TEXT:
                fprintf(stderr, "== Info: %s", data);
            default: // in case a new one is introduced to shock us
                return 0;

            case CURLINFO_HEADER_OUT:
                text = "=> Send header";
                break;
            case CURLINFO_DATA_OUT:
                text = "=> Send data";
                break;
            case CURLINFO_SSL_DATA_OUT:
                text = "=> Send SSL data";
                break;
            case CURLINFO_HEADER_IN:
                text = "<= Recv header";
                break;
            case CURLINFO_DATA_IN:
                text = "<= Recv data";
                break;
            case CURLINFO_SSL_DATA_IN:
                text = "<= Recv SSL data";
                break;
        }

        dump(text, stderr, (unsigned char *)data, size);
        return 0;
    }
    */

    curl_socket_t cURLLoader::openSocket(void *data, curlsocktype purpose, struct curl_sockaddr *addr)
    {
        cURLLoader * loader = (cURLLoader*)data;
        loader->_socket = socket(addr->family, addr->socktype, addr->protocol);
        return loader->_socket;
    }

    /*
    size_t abort_payload(void *ptr, size_t size, size_t nmemb, SOCKET *curl_socket) {
        SOCKET l_socket = INVALID_SOCKET;
        swap(l_socket, *curl_socket);
        if (l_socket != INVALID_SOCKET) {
            shutdown(l_socket, SD_BOTH);
            closesocket(l_socket);
        }
        return CURL_READFUNC_ABORT;
    }
    */

    /**
     * Use libCurl to load up the web page.
     * @param loader - pointer to the object which is loading the web page
     */
    void cURLLoader::loadThreadCurl(cURLLoader * loader)
    {
        CURL * curl = curl_easy_init();

        if (curl != NULL)
        {
            CURLcode rc = CURLE_OK;
            char curlError[CURL_ERROR_SIZE];

            // Use an error buffer to store the description of any errors.
            curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlError);

            // Set the headers.
            struct curl_slist * headers = NULL;

            std::vector<URLRequestHeader> h = loader->getRequest().getHeaders();
            for (std::string::size_type i = 0; i < h.size(); i++)
            {
                std::string headerLine = h[i].getName();
                headerLine.append(": ");
                headerLine.append(h[i].getValue());
                headers = curl_slist_append(headers, headerLine.c_str());
            }  // end for


            // Content Type (special header)
            std::string contentType = "Content-Type: ";
            contentType.append(loader->getRequest().getContentType());
            headers = curl_slist_append(headers, contentType.c_str());

            // User Agent (special header)
            if (loader->getRequest().getUserAgent().length() > 0)
            {
                std::string userAgent = "User-Agent: ";
                userAgent.append(loader->getRequest().getUserAgent());
                headers = curl_slist_append(headers, userAgent.c_str());
            }

            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, openSocket);
            curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, loader);

            // Set up the object to store the content of the response.
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, loader);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);

            // Set up the object to store the response headers.
            curl_easy_setopt(curl, CURLOPT_WRITEHEADER, loader);
            curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeHeader);

            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, (long)0);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, (long)0);

            //curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, my_trace);
            //curl_easy_setopt(curl, CURLOPT_VERBOSE, (long)1);

            // Only set timeout if it's not 0.
            if (_timeoutInterval)
            {
                curl_easy_setopt(curl, CURLOPT_NOSIGNAL, (long)1);
                curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, _timeoutInterval);
            }

            // Determine the type of request being made.
            if (loader->getRequest().getMethod() == URLRequestMethod::GET)
            {
                std::string url = loader->getRequest().getUrl();
                std::string data = loader->getRequest().getData();

                if (data.length() > 0)
                {
                    // Append the request data to the end of the URL.
                    url.append("?");
                    url.append(data);
                }

                // Set the URL for the request.
                curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

                rc = curl_easy_perform(curl);
            }
            else if (loader->getRequest().getMethod() == URLRequestMethod::POST)
            {
                // Set the base URL for the request.
                curl_easy_setopt(curl, CURLOPT_URL, loader->getRequest().getUrl().c_str());

                // Create all of the form data.
                curl_easy_setopt(curl, CURLOPT_POST, 1);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, loader->getRequest().getData().length());
                curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, loader->getRequest().getData().c_str());

                rc = curl_easy_perform(curl);
            }
            else if (loader->getRequest().getMethod() == URLRequestMethod::PUT)
            {
                // Set the base URL for the request.
                curl_easy_setopt(curl, CURLOPT_URL, loader->getRequest().getUrl().c_str());

                // Create all of the form data.
                curl_easy_setopt(curl, CURLOPT_UPLOAD, 1);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, loader->getRequest().getData().length());
                curl_easy_setopt(curl, CURLOPT_COPYPOSTFIELDS, loader->getRequest().getData().c_str());

                rc = curl_easy_perform(curl);
            }
            else
            {
                // Method type not supported.
                rc = CURLE_FAILED_INIT;
            }


            if (rc == CURLE_ABORTED_BY_CALLBACK)
            {
                // aborted by caller
                loader->_urlResponse.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
                loader->_urlResponse.setReasonPhrase(curlError);
            }
            else if (rc == CURLE_OK)
            {
                long statusCode;
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
                loader->_urlResponse.setStatusCode(static_cast<unsigned short>(statusCode));
            }
            else if (rc == CURLE_OPERATION_TIMEDOUT)
            {
                loader->_urlResponse.setStatusCode(HTTP_CLIENT_NETWORK_ERROR/*408*/);
                loader->_urlResponse.setReasonPhrase("Operation timed out");
            }
            else
            {
                loader->_urlResponse.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
                loader->_urlResponse.setReasonPhrase(curlError);
            }

            // Clean up memory.
            if (headers != NULL)
            {
                curl_slist_free_all(headers);
            }
            curl_easy_cleanup(curl);

            // We're done.  Fire the appropriate event.
            /*
            if (loader->getClient() != NULL)
            {
                loader->getClient()->handleResult(loader->_urlResponse);
            }*/
        }
        else
        {
            URLResponse response;
            response.setStatusCode(HTTP_CLIENT_NETWORK_ERROR);
            response.setReasonPhrase("cURL Out of Memory");
            //loader->getClient()->handleError(response);
        }
        loader->_threadRunning = false; // This is not even atomic !!!
    }
}

#endif
