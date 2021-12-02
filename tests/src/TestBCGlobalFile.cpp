#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "TestBCGlobalFile.h"

#if defined(BC_TEST_LINUX)
#include "curl/curl.h"
#include <mutex>
#include <condition_variable>
#include <thread>
#endif

#include <vector>
using namespace BrainCloud;

static const std::string testfileName = "testGlobalFile.png";
static const std::string testFileId = "ed2d2924-4650-4a88-b095-94b75ce9aa18";
static const std::string testFolderPath = "/fname/";

TEST_F(TestBCGlobalFile, GetFileInfo)
{
	TestResult tr;
	m_bc->getGlobalFileService()->getFileInfo(testFileId, &tr);
	tr.run(m_bc);
}

TEST_F(TestBCGlobalFile, GetFileInfoSimple)
{
	TestResult tr;
	m_bc->getGlobalFileService()->getFileInfoSimple(testFolderPath, testfileName, &tr);
	tr.run(m_bc);
}

TEST_F(TestBCGlobalFile, GetGlobalCDNUrl)
{
	TestResult tr;
	m_bc->getGlobalFileService()->getGlobalCDNUrl(testFileId, &tr);
	tr.run(m_bc);
}

TEST_F(TestBCGlobalFile, GetGlobalFileList)
{
	TestResult tr;
	m_bc->getGlobalFileService()->getGlobalFileList(testFolderPath, true, &tr);
	tr.run(m_bc);
}

#if defined(BC_TEST_LINUX)
static size_t writeData(char * toWrite, size_t size, size_t nmemb, void * data)
{
    auto result = size * nmemb;

    auto downloaded = (int *)data;
    *downloaded += result;
    printf("Downloaded: %i B\n", *downloaded);

    return result;
}

TEST_F(TestBCGlobalFile, DownloadFile)
{
	TestResult tr;
	m_bc->getGlobalFileService()->getGlobalCDNUrl(testFileId, &tr);
	tr.run(m_bc);

    // Download the file using cURL, on Linux only
    const int DOWNLOAD_FILE_TIMEOUT_MS = 2 * 60 * 1000; // 2 mins
    bool done = false;
    bool success = false;
    std::string url = tr.m_response["data"]["cdnUrl"].asString();

    curl_global_init(CURL_GLOBAL_ALL); // Already done inside the BC lib, but doesn't hurt to redo it...
    CURL *curl = curl_easy_init();
    EXPECT_TRUE(curl);

    char curlError[CURL_ERROR_SIZE];
    size_t downloaded = 0;

    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curlError);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &downloaded);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, (long)0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, (long)0);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, (long)1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, DOWNLOAD_FILE_TIMEOUT_MS);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    CURLcode rc = CURLE_OK;
    rc = curl_easy_perform(curl);
    
    long statusCode;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &statusCode);
    EXPECT_EQ(200, statusCode);

    curl_easy_cleanup(curl);

    if (rc != CURLE_OK)
        printf("curl_easy_perform failed with: %i, %s\n", (int)rc, curlError);

    EXPECT_EQ(CURLE_OK, rc);
}
#endif
