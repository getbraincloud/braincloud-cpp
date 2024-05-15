#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "TestBCGroupFile.h"
#include <vector>
#include <chrono>

//#define UPLOAD_TIMEOUT_SECS 30
#define SLEEP_INTERVAL_MS 300

using namespace BrainCloud;
using namespace std::chrono;
BrainCloudWrapper* TestBCGroupFile::wrapper = nullptr;
GroupFileUploadCallback* TestBCGroupFile::uploadCallback = nullptr;
std::string TestBCGroupFile::filename = "testingGroupFile.dat";
std::string TestBCGroupFile::newFileName = "testCopiedFile.dat";
std::string TestBCGroupFile::tempFilename = "deleteThisFileAfter.dat";
std::string TestBCGroupFile::updatedName = "UpdatedGroupFile.dat";
std::string TestBCGroupFile::groupFileId = "d2dd646a-f1af-4a96-90a7-a0310246f5a2";
std::string TestBCGroupFile::groupID = "a7ff751c-3251-407a-b2fd-2bd1e9bca64a";
int TestBCGroupFile::version = -1;

TestBCGroupFile::TestBCGroupFile()
{
}

TEST_F(TestBCGroupFile, testCheckFilenameExists)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testCheckFilenameExists...\n");
    
    Authenticate();
    TestResult tr;
    
    m_bc->getGroupFileService()->checkFilenameExists(groupID, "", filename, &tr);
    
    tr.run(m_bc);
    
    ASSERT_TRUE(tr.m_response["data"]["exists"].asBool());
    
    Logout();
}

TEST_F(TestBCGroupFile, testCheckFilenameNoExists)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testCheckFilenameNoExists...\n");
    
    Authenticate();
    TestResult tr;
    
    m_bc->getGroupFileService()->checkFilenameExists(groupID, "", "nosuchfile.dat", &tr);
    
    tr.run(m_bc);
    
    ASSERT_TRUE(!tr.m_response["data"]["exists"].asBool());
    
    Logout();
}

TEST_F(TestBCGroupFile, testCheckFullpathFilenameExists)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testCheckFullpathFilenameExists...\n");
    
    Authenticate();
    TestResult tr;
    
    m_bc->getGroupFileService()->checkFullpathFilenameExists(groupID, filename, &tr);
    
    tr.run(m_bc);
    
    ASSERT_TRUE(tr.m_response["data"]["exists"].asBool());
    
    Logout();
}

TEST_F(TestBCGroupFile, testGetFileInfo)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testGetFileInfo...\n");
    
    Authenticate();
    TestResult tr;
    
    m_bc->getGroupFileService()->getFileInfo(groupID, groupFileId, &tr);
    tr.run(m_bc);
    
    m_bc->getGroupFileService()->getCDNUrl(groupID, groupFileId, &tr);
    tr.run(m_bc);
    
    Logout();
}


TEST_F(TestBCGroupFile, testGetFileInfoSimple)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testGetFileInfoSimple...\n");
    
    Authenticate();
    TestResult tr;
    
    m_bc->getGroupFileService()->getFileInfoSimple(groupID, "", filename, &tr);
    
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCGroupFile, testGetFileList)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testGetFileList...\n");
    
    Authenticate();
    TestResult tr;
    
    
    bool recurse = true;
    m_bc->getGroupFileService()->getFileList(groupID, "", recurse, &tr);
    
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCGroupFile, testMoveFile)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testMoveFile...\n");
    
    Authenticate();
    TestResult tr;
    
    m_bc->getGroupFileService()->moveFile(groupID, groupFileId, version, "", 0, newFileName, true, &tr);
    
    tr.run(m_bc);
    
    // reverting back
    m_bc->getGroupFileService()->moveFile(groupID, groupFileId, version, "", 0, filename, true, &tr);
    
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCGroupFile, testMoveUserToGroupFile)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testMoveUserToGroupFile...\n");
    
    Authenticate();
    TestResult tr;
    
    Json::FastWriter fw;
    Json::Value acl;
    acl["member"] = 2; // can be 0, 1 or 2
    acl["other"] = 0;
    
    // create new file and wait for upload
    std::string uploadId;
    if (!simpleUpload(m_bc, 5, "TestFolder", tempFilename.c_str(), uploadId))
    {
        return;
    }
    
    m_bc->getGroupFileService()->moveUserToGroupFile("TestFolder/", tempFilename, groupID, "", tempFilename, fw.write(acl).c_str(), true, &tr);
    
    tr.run(m_bc);
    
    std::string newFileId = tr.m_response["data"]["fileDetails"]["fileId"].asString();

    // deleting new file
    m_bc->getGroupFileService()->deleteFile(groupID, newFileId, version, tempFilename, &tr);
    
    tr.run(m_bc);
    
    m_bc->deregisterFileUploadCallback();
    
    Logout();
}

TEST_F(TestBCGroupFile, testCopyDeleteFile)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testCopyDeleteFile...\n");
    
    Authenticate();
    TestResult tr;
    
    // copy a file
    m_bc->getGroupFileService()->copyFile(groupID, groupFileId, version, "", 0, newFileName, true, &tr);
    
    tr.run(m_bc);
    
    std::string newFileId = tr.m_response["data"]["fileDetails"]["fileId"].asString();
    
    // check exists
    m_bc->getGroupFileService()->checkFilenameExists(groupID, "", newFileName, &tr);
    
    tr.run(m_bc);
    
    ASSERT_TRUE(tr.m_response["data"]["exists"].asBool());
    
    // deleting new file
    m_bc->getGroupFileService()->deleteFile(groupID, newFileId, version, newFileName, &tr);
    
    tr.run(m_bc);
    
    // check no exists
    m_bc->getGroupFileService()->checkFilenameExists(groupID, "", newFileName, &tr);
    
    tr.run(m_bc);
    
    ASSERT_TRUE(!tr.m_response["data"]["exists"].asBool());

    Logout();
}

TEST_F(TestBCGroupFile, testUpdateFileInfo)
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
    TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
    {
        printf("This env doesn't support the file set up for GroupFile testing..\n");
        return;
    }
    printf("testUpdateFileInfo...\n");
    
    Authenticate();
    TestResult tr;
    
    Json::FastWriter fw;
    Json::Value acl;
    acl["member"] = 2; // can be 0, 1 or 2
    acl["other"] = 0;
    
    m_bc->getGroupFileService()->updateFileInfo(groupID, groupFileId, version, updatedName, fw.write(acl).c_str(), &tr);
    
    tr.run(m_bc);
    
    // reverting back
    m_bc->getGroupFileService()->updateFileInfo(groupID, groupFileId, version, filename, fw.write(acl).c_str(), &tr);
    
    tr.run(m_bc);
    
    Logout();
}

/*
 * Helpers
 */
void TestBCGroupFile::Authenticate()
{
    TestResult tr;
    m_bcWrapper->authenticateEmailPassword(
                                                                "cpp-tester",
                                                                "cpp-tester",
                                                                true,
                                                                &tr);
    tr.run(m_bc);
}

void GroupFileUploadCallback::fileUploadCompleted(const char * in_fileUploadId, const std::string & in_jsonResponse)
{
    std::cout << "FileUploadCompleted callback hit" << std::endl;

    UploadCompletedDetails details;
    details._fileUploadId = in_fileUploadId;
    details._jsonResponse = in_jsonResponse;
    _completedUploadDetails.push_back(details);
}

void GroupFileUploadCallback::fileUploadFailed(const char * in_fileUploadId, int in_statusCode, int in_reasonCode, const std::string & in_jsonResponse)
{
    std::cout << "FileUploadFailed callback hit status(" << in_statusCode << ") reason(" << in_reasonCode << ") id(" << in_fileUploadId << ")" << std::endl;

    UploadFailedDetails details;
    details._fileUploadId = in_fileUploadId;
    details._statusCode = in_statusCode;
    details._reasonCode = in_reasonCode;
    details._jsonResponse = in_jsonResponse;
    _failedUploadDetails.push_back(details);
}

int TestBCGroupFile::createFile(const char * in_path, int in_size)
{
    FILE* fp = NULL;
    fp = fopen(in_path, "w");
    if (fp == NULL)
    {
        return -1;
    }
    for (int i = 0; i < in_size; ++i)
    {
        fputc('!', fp);
    }
    fseek(fp, 0, SEEK_END);
    int fileLen = static_cast<int>(ftell(fp));
    fclose(fp);
    fp = NULL;
    return fileLen;
}

bool TestBCGroupFile::simpleUpload(BrainCloudClient* client, int mb, const std::string & cloudPath, const std::string & cloudFilename, std::string & out_uploadId)
{
    std::string localPath = cloudFilename;

    uploadCallback = new GroupFileUploadCallback();
    createFile(localPath.c_str(), mb * 1024 * 1024);

    TestResult tr;
    client->getFileService()->uploadFile(cloudPath.c_str(), cloudFilename.c_str(), true, true, localPath.c_str(), &tr);
    if (!tr.run(client))
    {
        return false;
    }
    out_uploadId = tr.m_response["data"]["fileDetails"]["uploadId"].asString();

    client->registerFileUploadCallback(uploadCallback);

    while (uploadCallback->_completedUploadDetails.size() == 0 && uploadCallback->_failedUploadDetails.size() == 0)
    {
        client->runCallbacks();
        TestResult::sleep(SLEEP_INTERVAL_MS);

        int64_t transferred = client->getFileService()->getUploadBytesTransferred(out_uploadId.c_str());
        int64_t total = client->getFileService()->getUploadTotalBytesToTransfer(out_uploadId.c_str());
        double progress = client->getFileService()->getUploadProgress(out_uploadId.c_str());
        printf("%lld transfered %lld total %f progress\n", transferred, total, progress);
    }
    client->deregisterFileUploadCallback();
    delete uploadCallback;
    uploadCallback = nullptr;
    return true;
}

void TestBCGroupFile::SetUpTestCase()
{
    if (TestFixtureBase::getServerUrl().find("internala.braincloudservers.com") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.internalg.braincloudservers.com") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.braincloudservers.com") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.dev.bc.talespin.io") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.bc.talespin.io") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.ultracloud.ultra.io") == std::string::npos ||
        TestFixtureBase::getServerUrl().find("api.bc.bigfishgames.com") == std::string::npos)
        {
            printf("This env doesn't support the file set up for GroupFile testing..\n");
            return;
        }
    printf("Setting up files for TestGroupFile\n");
    wrapper = new BrainCloudWrapper("_wrapperName");

    LoadIds();

    std::map<std::string, std::string> secretMap;
    secretMap[m_appId] = m_secret;
    secretMap[m_childAppId] = m_childSecret;
    wrapper->initializeWithApps(m_serverUrl.c_str(), m_appId.c_str(), secretMap, m_version.c_str(), "", "");

    TestResult tr;
    wrapper->authenticateEmailPassword(
            "cpp-tester",
            "cpp-tester",
            false,
            &tr);
    tr.run(wrapper->getBCClient());

    Json::FastWriter fw;
    Json::Value acl;
    acl["member"] = 2; // can be 0, 1 or 2
    acl["other"] = 0;

    // create new file and wait for upload
    std::string uploadId;
    if (!simpleUpload(wrapper->getBCClient(), 5, "TestFolder", filename.c_str(), uploadId))
    {
        wrapper->logout(true, &tr);
        tr.run(wrapper->client);
        wrapper->getBCClient()->resetCommunication();
        delete wrapper;
        wrapper = nullptr;
        return;
    }

    /* Add user to test group */
    wrapper->getGroupService()->joinGroup(groupID.c_str(), &tr);
    tr.run(wrapper->getBCClient());

    /* moveUserToGroupFile Test */
    wrapper->getBCClient()->getGroupFileService()->moveUserToGroupFile("TestFolder/", filename, groupID, "", filename, fw.write(acl).c_str(), true, &tr);
    tr.run(wrapper->getBCClient());

    /* Save group file ID for tests */
    groupFileId = tr.m_response["data"]["fileDetails"]["fileId"].asString();

    wrapper->logout(true, &tr);
    tr.run(wrapper->client);
    wrapper->getBCClient()->resetCommunication();
    delete wrapper;
    wrapper = nullptr;
}
