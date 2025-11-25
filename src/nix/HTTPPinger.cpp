#include "braincloud/internal/nix/HTTPPinger.h"
#include <array>
#include <cstdio>
#include <memory>
#include <regex>

using namespace BrainCloud;

IPinger* IPinger::create(BrainCloudClient* pClient)
{
    return new LinuxPinger();
}

int LinuxPinger::ping(const std::string& url)
{
    // Build ping command:
    // -c 1 : send exactly 1 ICMP packet
    // -W 1 : timeout after 1 second
    // 2>&1 : redirect stderr to stdout for consistent parsing
    std::string cmd = "ping -c 1 -W 1 " + url + " 2>&1";

    std::array<char, 256> buffer{};
    std::string result;

    // Open pipe
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return -1;

    // Read command output
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    pclose(pipe);

    // Example output format to extract:
    // "64 bytes from google.com: icmp_seq=1 ttl=113 time=22.3 ms"
    std::regex timeRegex(R"(time=([0-9]*\.?[0-9]+)\s*ms)");
    std::smatch match;

    if (std::regex_search(result, match, timeRegex))
    {
        float ms = std::stof(match[1].str());
        // Clamp result similar to Windows implementation (0-999ms)
        if (ms >= 0 && ms <= 999)
            return static_cast<int>(ms);
    }

    // If no "time=" found, it timed out or failed
    return -1;
}
