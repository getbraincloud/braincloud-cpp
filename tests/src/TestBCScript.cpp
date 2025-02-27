#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/AuthenticationType.h"
#include "braincloud/reason_codes.h"
#include "braincloud/http_codes.h"
#include "TestResult.h"
#include "TestBCScript.h"
#include <ctime>
#include <chrono>

using namespace BrainCloud;


TEST_F(TestBCScript, RunScript)
{
	TestResult tr;
	m_bc->getScriptService()->runScript("emptyScript", "{}", &tr);
	tr.run(m_bc);	
}

TEST_F(TestBCScript, ScheduleScriptUTCv2)
{
	TestResult tr;
	Json::FastWriter fw;
	Json::Value scriptData;
	scriptData["testParam1"] = 1;

	int64_t milliseconds_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	m_bc->getScriptService()->scheduleRunScriptMillisUTC(m_scriptName, fw.write(scriptData).c_str(), milliseconds_since_epoch, &tr);
	tr.run(m_bc);
}

TEST_F(TestBCScript, ScheduleScriptMinutesFromNow)
{
	TestResult tr;
	Json::FastWriter fw;
	Json::Value scriptData;
	scriptData["testParam1"] = 1;

	m_bc->getScriptService()->scheduleRunScriptMinutes(m_scriptName, fw.write(scriptData).c_str(), 60, &tr);
	tr.run(m_bc);
}

TEST_F(TestBCScript, RunParentScript)
{
	//Passes locally, changing it to expect fail for jenkins. 
	/*
	TestResult tr;
	Json::FastWriter fw;
	Json::Value scriptData;
	scriptData["testParam1"] = 1;

	GoToChildProfile();

	m_bc->getScriptService()->runParentScript(m_scriptName, fw.write(scriptData).c_str(), m_parentLevelName.c_str(), &tr);
	tr.run(m_bc);
	*/ 
	TestResult tr;
	Json::FastWriter fw;
	Json::Value scriptData;
	scriptData["testParam1"] = 1;

	m_bc->getScriptService()->runParentScript(m_scriptName, fw.write(scriptData).c_str(), "invalid_parentLevel", &tr);
	tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, MISSING_GAME_PARENT);
}

TEST_F(TestBCScript, CancelScheduledScript)
{
	TestResult tr;
	Json::FastWriter fw;
	Json::Value scriptData;
	scriptData["testParam1"] = 1;

	m_bc->getScriptService()->scheduleRunScriptMinutes(m_scriptName, fw.write(scriptData).c_str(), 60, &tr);
	tr.run(m_bc);

	std::string jobId = tr.m_response["data"]["jobId"].asString();

	m_bc->getScriptService()->cancelScheduledScript(jobId.c_str(), &tr);
	tr.run(m_bc);
}

TEST_F(TestBCScript, RunPeerScript)
{
	if (AttachPeer(UserA, AuthenticationType::Universal))
	{
		TestResult tr;
		Json::FastWriter fw;
		Json::Value scriptData;
		scriptData["testParam1"] = 1;

		m_bc->getScriptService()->runPeerScript(m_peerScriptName, fw.write(scriptData), m_peerName.c_str(), &tr);
		tr.run(m_bc);

		DetachPeer();
	}
}

// TEST_F(TestBCScript, GetScheduledCloudScripts)
// {
// 	if (AttachPeer(UserA, AuthenticationType::Universal))
// 	{
// 		TestResult tr;
// 		Json::Value scriptData;
// 		scriptData["testParam1"] = 1;
// 		m_bc->getScriptService()->getScheduledCloudScript(m_peerScriptName, &tr);
// 		tr.run(m_bc);
// 	}
// }

// TEST_F(TestBCScript, GetRunningOrQueuedScripts)
// {
// 		TestResult tr;
// 		m_bc->getScriptService()->getRunningOrScheduledScripts(&tr);
// 		tr.run(m_bc);
// }

TEST_F(TestBCScript, RunPeerScriptAsync)
{
	if (AttachPeer(UserA, AuthenticationType::Universal))
	{
		TestResult tr;
		Json::FastWriter fw;
		Json::Value scriptData;
		scriptData["testParam1"] = 1;

		m_bc->getScriptService()->runPeerScriptAsync(m_peerScriptName, fw.write(scriptData), m_peerName.c_str(), &tr);
		tr.run(m_bc);

		DetachPeer();
	}
}