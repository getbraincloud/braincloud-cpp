#include <stdlib.h>
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "braincloud/reason_codes.h"
#include "braincloud/http_codes.h"


#include <iostream>
#include <string>
#include <sstream>

#if __cplusplus < 201103L

#ifdef WIN32
#include <WinBase.h>

#else
#include <unistd.h>

#endif

#else

#include <chrono>
#include <thread>

#endif

#ifdef WIN32

#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")

#else

#include <execinfo.h>
#include <cstdlib>
#include <cxxabi.h>
#include <cstdio>

#endif

#define MAX_WAIT_SECS 5

TestResult::TestResult()
{
    m_maxWaitMillis = 0;
    reset();
}


void TestResult::reset()
{
    m_done = false;
    m_result = false;
    m_apiCountExpected = 0;

    m_response = Json::Value();

    m_statusCode = 0;
    m_reasonCode = 0;
    m_statusMessage.clear();

    m_networkErrorCount = 0;
    m_globalErrorCount = 0;
}

void TestResult::sleep(int millis)
{
#if __cplusplus >= 201103L
    auto sleep = std::chrono::milliseconds(millis);
    std::this_thread::sleep_for(sleep);
#else
#ifdef WIN32
    Sleep(millis);
#else
    usleep(millis * 1000);
#endif
#endif
}

void TestResult::sleepAndUpdate(BrainCloudClient * in_bc)
{
    long maxWaitMs = m_maxWaitMillis > 0 ? m_maxWaitMillis : MAX_WAIT_SECS * 1000;
    long sleepSliceMs = 500;
    while (!m_done && maxWaitMs > 0)
    {
        in_bc->runCallbacks();
        sleep(static_cast<int>(sleepSliceMs));
        maxWaitMs -= sleepSliceMs;
    }
}

void TestResult::printStackTrace()
{
#ifdef WIN32
    void* stack[64];
    HANDLE process = GetCurrentProcess();

    // Capture the stack
    USHORT frames = CaptureStackBackTrace(0, 64, stack, nullptr);

    // Initialize symbol handler
    SymInitialize(process, nullptr, TRUE);

    SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

    std::cout << "Stack trace (" << frames << " frames):\n";

    for (USHORT i = 0; i < frames; i++)
    {
        SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
        std::cout << i << ": " << symbol->Name << " - 0x" << std::hex << symbol->Address << std::dec << "\n";
    }

    free(symbol);
#else
    const int maxFrames = 64;
    void* frames[maxFrames];
    int frameCount = backtrace(frames, maxFrames);

    std::cout << "Stack trace (" << frameCount << " frames):\n";
    for (int i = 0; i < frameCount; ++i)
    {
        std::ostringstream cmd;
        cmd << "addr2line -e ./bctests -f -p " << frames[i];
        FILE* fp = popen(cmd.str().c_str(), "r");
        if (!fp) continue;

        char buffer[512];
        if (fgets(buffer, sizeof(buffer), fp))
            std::cout << buffer;

        pclose(fp);
    }
#endif
}

std::string TestResult::demangle(const char* name)
{
#ifdef WIN32
    return std::string();
#else
    int status = 0;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    std::string result = (status == 0 && demangled) ? demangled : name;
    free(demangled);
    return result;
#endif
}

bool TestResult::run(BrainCloudClient * in_bc, bool in_noAssert)
{
    return runExpectCount(in_bc, 1, in_noAssert);
}

bool TestResult::runExpectCount(BrainCloudClient * in_bc, int in_apiCountExpected, bool in_noAssert)
{
    reset(); //reset before run
    m_apiCountExpected = in_apiCountExpected;
    sleepAndUpdate(in_bc);

    if (!m_done)
    {
        // we timed out so try to reset the comms and throw the error
        in_bc->resetCommunication();
        m_result = false;
        m_statusCode = 999;
        m_reasonCode = 999;
        m_statusMessage = "TEST TIMEOUT EXCEEDED";
        long maxWaitMs = m_maxWaitMillis > 0 ? m_maxWaitMillis : MAX_WAIT_SECS * 1000;
        printf("\n [TIMEOUT EXCEEDED]: Timeout exceeded %ld - expected count: %d  \n", maxWaitMs, in_apiCountExpected);
        printStackTrace();
        if (!in_noAssert) EXPECT_TRUE(m_done);
    }
    else
    {
        if (!in_noAssert) EXPECT_TRUE(m_result);
        if (!m_result)
        {
            if (!in_noAssert) EXPECT_EQ(200, m_statusCode);
            if (!in_noAssert) EXPECT_EQ(0, m_reasonCode);
            if (!in_noAssert) EXPECT_EQ("", m_statusMessage);
        }
    }

    return m_result;
}


bool TestResult::runExpectFail(BrainCloudClient * in_bc, int expectedStatus, int expectedReasonCode)
{
    reset(); //reset before run
    sleepAndUpdate(in_bc);

    if (!m_done)
    {
        // bc lib should have sent a timeout
        EXPECT_TRUE(m_done);
        return false;
    }
    bool ret = true;
    EXPECT_FALSE(m_result);
    if (expectedStatus != -1)
    {
        EXPECT_EQ(expectedStatus, m_statusCode);
        if (m_statusCode != expectedStatus)
        {
            ret = false;
        }
    }
    if (expectedReasonCode != -1)
    {
        EXPECT_EQ(expectedReasonCode, m_reasonCode);
        if (m_reasonCode != expectedReasonCode)
        {
            ret = false;
        }
    }
    if (!ret)
    {
        // to log the status message
        EXPECT_EQ("", m_statusMessage);
    }

    return ret;
}

void TestResult::rttConnectSuccess()
{
    m_response.clear();
    m_result = true;
    --m_apiCountExpected;
    if (m_apiCountExpected <= 0)
    {
        m_done = true;
    }
}

void TestResult::rttConnectFailure(const std::string& errorMessage)
{
    m_statusMessage = errorMessage;

    m_result = false;
    --m_apiCountExpected;
    if (m_apiCountExpected <= 0)
    {
        m_done = true;
    }
}

void TestResult::serverCallback(ServiceName serviceName, ServiceOperation serviceOperation, std::string const & jsonData)
{
    Json::Value value;
    Json::Reader reader;
    m_response.clear();
    reader.parse(jsonData, m_response);

    printf("\n [From Request Service: %s Operation: %s] \n", serviceName.getValue().c_str(), serviceOperation.getValue().c_str());

    printf("\n [RESPONSE]: %s \n", jsonData.c_str());

    m_result = true;
    --m_apiCountExpected;
    if (m_apiCountExpected <= 0)
    {
        m_done = true;
    }
}

void TestResult::serverError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string & statusMessage)
{
    m_statusCode = statusCode;
    m_reasonCode = reasonCode;
    m_statusMessage = statusMessage;

    printf("\n [Server ERROR]: %s \n", statusMessage.c_str());

    m_result = false;
    --m_apiCountExpected;
    if (m_apiCountExpected <= 0)
    {
        m_done = true;
    }
}

void TestResult::globalError(ServiceName serviceName, ServiceOperation serviceOperation, int statusCode, int reasonCode, const std::string & jsonError)
{
    m_statusCode = statusCode;
    m_reasonCode = reasonCode;
    m_statusMessage = jsonError;

    m_result = false;
    --m_apiCountExpected;
    if (m_apiCountExpected <= 0)
    {
        m_done = true;
    }
    ++m_globalErrorCount;
}

void TestResult::networkError()
{
    m_statusCode = HTTP_CLIENT_NETWORK_ERROR;
    m_reasonCode = CLIENT_NETWORK_ERROR_TIMEOUT;
    m_statusMessage = "Network Error";

    m_result = false;
    --m_apiCountExpected;
    if (m_apiCountExpected <= 0)
    {
        m_done = true;
    }
    ++m_networkErrorCount;
}

void TestResult::setMaxWaitSecs(int secs)
{
    m_maxWaitMillis = secs * 1000;
}
