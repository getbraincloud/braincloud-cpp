#pragma once

#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestFixtureBase.h"
#include "json/json.h"

using namespace BrainCloud;

struct UploadCompletedDetails
{
public:
    std::string _fileUploadId;
    std::string _jsonResponse;
};

struct UploadFailedDetails
{
public:
    std::string _fileUploadId;
    int _statusCode;
    int _reasonCode;
    std::string _jsonResponse;

    UploadFailedDetails()
    {
        _statusCode = 0;
        _reasonCode = 0;
    }
};
class GroupFileUploadCallback:  public IFileUploadCallback
{
    friend class TestBCGroupFile;
protected:
    std::vector <UploadCompletedDetails> _completedUploadDetails;
    std::vector <UploadFailedDetails> _failedUploadDetails;

public:
    GroupFileUploadCallback()
    {
    }

    virtual void fileUploadCompleted(const char * in_fileUploadId, const std::string & in_jsonResponse);
    virtual void fileUploadFailed(const char * in_fileUploadId, int in_statusCode, int in_reasonCode, const std::string & in_jsonResponse);

};

class TestBCGroupFile : public TestFixtureBase
{
protected:
    TestBCGroupFile();

    virtual bool ShouldSkipAuthenticate() {
        return true;
    }

    void Authenticate();
    static int createFile(const char * in_path, int in_size);
    static bool simpleUpload(BrainCloudClient* client, int mb, const std::string & cloudPath, const std::string & cloudFilename, std::string & out_uploadId);
    static void SetUpTestCase();

    static std::string groupFileId;
    static std::string groupID;
    static int version;
    static std::string filename;
    static std::string newFileName;
    static std::string tempFilename;
    static std::string updatedName;

    static BrainCloudWrapper* wrapper;
    static GroupFileUploadCallback* uploadCallback;

};

