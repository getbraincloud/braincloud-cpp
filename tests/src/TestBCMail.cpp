#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "TestResult.h"
#include "TestBCMail.h"
#include <time.h>
#include <vector>

using namespace BrainCloud;


TEST_F(TestBCMail, SendBasicEmail)
{
	TestResult tr;
	m_bc->getMailService()->sendBasicEmail(
		GetUser(UserA)->m_profileId,
		"Test Subject",
		"Test body content text form unit test",
		&tr);
	tr.run(m_bc);
}

TEST_F(TestBCMail, SendAdvancedEmail)
{
	TestResult tr;
	Json::FastWriter fw;

	Json::Value data;
	data["subject"] = "Test Subject";
	data["body"] = "Test body";

	std::string dataStr = fw.write(data);

	m_bc->getMailService()->sendAdvancedEmail(GetUser(UserB)->m_profileId, dataStr, &tr);
	tr.run(m_bc);
}

TEST_F(TestBCMail, SendAdvancedEmailByAddress)
{
	TestResult tr;
	Json::FastWriter fw;

	Json::Value data;
	data["subject"] = "Test Subject";
	data["body"] = "Test body";

	std::string dataStr = fw.write(data);

	m_bc->getMailService()->sendAdvancedEmailByAddress(GetUser(UserB)->m_email, dataStr, &tr);
	tr.run(m_bc);
}

TEST_F(TestBCMail, SendAdvancedEmailByAddresses)
{
	TestResult tr;
	Json::FastWriter fw;
	std::vector<std::string> emailAddresses;
	emailAddresses.push_back("test@email.com");
	emailAddresses.push_back("anothertest@email.com");
	Json::Value data;
	data["subject"] = "Test Subject";
	data["body"] = "Test body";
	data["fromAddress"] = "testemail@email.com";
	data["fromName"] = "James Reece";
	std::string dataStr = fw.write(data);

	m_bc->getMailService()->sendAdvancedEmailByAddresses(emailAddresses, dataStr, &tr);
	tr.run(m_bc);
}