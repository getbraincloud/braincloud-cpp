#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "TestBCGlobalFile.h"
#include <vector>
using namespace BrainCloud;

static const std::string testfileName = "png1.png";
static const std::string testFileId = "34802251-0da0-419e-91b5-59d91790af15";
static const std::string testFolderPath = "/existingfolder/";

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
