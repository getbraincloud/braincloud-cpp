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

TestBCGroupFile::TestBCGroupFile():
groupFileId ("d2dd646a-f1af-4a96-90a7-a0310246f5a2"),
groupID("a7ff751c-3251-407a-b2fd-2bd1e9bca64a"),
version(-1),
_returnCount(0),
_failCount(0),
filename("testingGroupFile.dat"),
newFileName("testCopiedFile.dat"),
tempFilename("deleteThisFileAfter.dat"),
updatedName("UpdatedGroupFile.dat")
{
}

TEST_F(TestBCGroupFile, testCheckFilenameExists)
{
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
    printf("testGetFileInfoSimple...\n");
    
    Authenticate();
    TestResult tr;
    
    m_bc->getGroupFileService()->getFileInfoSimple(groupID, "", filename, &tr);
    
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCGroupFile, testGetFileList)
{
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
    printf("testMoveUserToGroupFile...\n");
    
    Authenticate();
    TestResult tr;
    
    Json::FastWriter fw;
    Json::Value acl;
    acl["member"] = 2; // can be 0, 1 or 2
    acl["other"] = 0;
    
    m_bc->registerFileUploadCallback(this);
    
    // create new file and wait for upload
    std::string uploadId;
    if (!simpleUpload(5, "TestFolder", tempFilename.c_str(), uploadId))
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
    m_bc->getAuthenticationService()->authenticateEmailPassword(
                                                                "cpp-tester",
                                                                "cpp-tester",
                                                                false,
                                                                &tr);
    tr.run(m_bc);
}

void TestBCGroupFile::fileUploadCompleted(const char * in_fileUploadId, const std::string & in_jsonResponse)
{
    std::cout << "FileUploadCompleted callback hit" << std::endl;
    
    UploadCompletedDetails details;
    details._fileUploadId = in_fileUploadId;
    details._jsonResponse = in_jsonResponse;
    _completedUploadDetails.push_back(details);
}

void TestBCGroupFile::fileUploadFailed(const char * in_fileUploadId, int in_statusCode, int in_reasonCode, const std::string & in_jsonResponse)
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

bool TestBCGroupFile::simpleUpload(int mb, const std::string & cloudPath, const std::string & cloudFilename, std::string & out_uploadId)
{
    std::string localPath = cloudFilename;
    createFile(localPath.c_str(), mb * 1024 * 1024);
    
    TestResult tr;
    m_bc->getFileService()->uploadFile(cloudPath.c_str(), cloudFilename.c_str(), true, true, localPath.c_str(), &tr);
    if (!tr.run(m_bc))
    {
        return false;
    }
    out_uploadId = tr.m_response["data"]["fileDetails"]["uploadId"].asString();
    
    m_bc->registerFileUploadCallback(this);
    while (_completedUploadDetails.size() == 0 && _failedUploadDetails.size() == 0)
    {
        m_bc->runCallbacks();
        TestResult::sleep(SLEEP_INTERVAL_MS);
        
        int64_t transferred = m_bc->getFileService()->getUploadBytesTransferred(out_uploadId.c_str());
        int64_t total = m_bc->getFileService()->getUploadTotalBytesToTransfer(out_uploadId.c_str());
        double progress = m_bc->getFileService()->getUploadProgress(out_uploadId.c_str());
        printf("%lld transfered %lld total %f progress\n", transferred, total, progress);        
    }
    m_bc->deregisterFileUploadCallback();
    return true;
}
