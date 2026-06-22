#include "braincloud/internal/nix/cURLPinger.h"

#include "curl/curl.h"

#include <chrono>

namespace BrainCloud
{
    IPinger* IPinger::create(BrainCloudClient* pClient)
    {
        return new cURLPinger();
    }

    cURLPinger::cURLPinger() {}

    int cURLPinger::ping(const std::string& url)
    {
        CURL *curl = curl_easy_init();
        if (!curl)
            return 999;

        // curl requires a scheme to parse the URL correctly.
        // GameLift targets are plain hostnames (curl defaults to http://:80).
        // EdgeGap targets are "ip:port" with no scheme — prepend http:// so curl
        // resolves the host and port correctly for the TCP-only handshake.
        std::string curlUrl = (url.find("://") == std::string::npos) ? "http://" + url : url;
        curl_easy_setopt(curl, CURLOPT_URL, curlUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2);
        // CONNECT_ONLY: perform only the TCP handshake and return.
        // This avoids sending an HTTP request to a relay server that doesn't speak HTTP,
        // and gives an accurate RTT measurement regardless of what's listening on the port.
        curl_easy_setopt(curl, CURLOPT_CONNECT_ONLY, 1L);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        auto startTime = std::chrono::high_resolution_clock::now();
        CURLcode res = curl_easy_perform(curl);
        auto doneTime = std::chrono::high_resolution_clock::now();

        curl_easy_cleanup(curl);

        if (res != CURLE_OK)
            return 999;

        int pingResult = (int)(std::chrono::duration_cast<std::chrono::milliseconds>(doneTime - startTime).count());
        return pingResult > 999 ? 999 : pingResult;
    }
}
