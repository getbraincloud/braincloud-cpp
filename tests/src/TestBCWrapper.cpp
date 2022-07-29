#include "TestBCWrapper.h"
#include "braincloud/http_codes.h"
#include "braincloud/reason_codes.h"
#include "braincloud/BrainCloudWrapper.h"

// These tests are normally commented out for apple as the wrapper
// manipulates the keychain which asks the user for confirmation.
// Build machines dislike this :-)
// However you can uncomment to verify the wrapper is working.

#ifndef __APPLE__

TEST_F(TestBCWrapper, AaaRunFirst)
{
    // test that should be run before all others
#if defined (USE_IMMEDIATE_RETRY)
	m_bcWrapper->client->setImmediateRetryOnError(true);
#else
	m_bcWrapper->client->setImmediateRetryOnError(false);
#endif

    // this forces us to create a new anonymous account
	m_bcWrapper->setStoredAnonymousId("");
	m_bcWrapper->setStoredProfileId("");
}

TEST_F(TestBCWrapper, AuthenticateAnonymous)
{
    m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");

    TestResult tr;
	m_bcWrapper->authenticateAnonymous(&tr);
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCWrapper, ManualRedirect) // Redirects to the same environement, different app id
{
    m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_redirectAppId.c_str(), m_version.c_str(), "wrapper", "unittest");

    TestResult tr;
 	m_bcWrapper->authenticateAnonymous(&tr);
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCWrapper, AuthenticateEmailPassword)
{
	m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");
    
    std::string email = GetUser(UserA)->m_email;
    email.append("_wrapper");
    
    TestResult tr;
	m_bcWrapper->authenticateEmailPassword(email.c_str(), GetUser(UserA)->m_password, true, &tr);
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCWrapper, AuthenticateUniversal)
{
    m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");
    
    TestResult tr;
    std::string uid = GetUser(UserA)->m_id;
    uid.append("_wrapper");
    m_bcWrapper->authenticateUniversal(uid.c_str(), GetUser(UserA)->m_password, true, &tr);
    tr.run(m_bc);
    
    Logout();
}

TEST_F(TestBCWrapper, VerifyAlwaysAllowProfileFalse)
{
    m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");
    m_bcWrapper->setAlwaysAllowProfileSwitch(false);

    // this forces us to create a new anonymous account
    m_bcWrapper->setStoredAnonymousId("");
    m_bcWrapper->setStoredProfileId("");

    TestResult tr;
    m_bcWrapper->authenticateAnonymous(&tr);
    tr.run(m_bc);

    std::string anonId = m_bcWrapper->getStoredAnonymousId();
    std::string profileId = m_bcWrapper->getStoredProfileId();
    std::string uid = GetUser(UserA)->m_id;
    uid.append("_wrapperVerifyAlwaysAllowProfileFalse");

	m_bcWrapper->getIdentityService()->attachUniversalIdentity(uid.c_str(), GetUser(UserA)->m_password, &tr);
    tr.run(m_bc);

    Logout();
	m_bcWrapper->getBCClient()->getAuthenticationService()->clearSavedProfileId();

    m_bcWrapper->authenticateUniversal(uid.c_str(), GetUser(UserA)->m_password, true, &tr);
    tr.run(m_bc);

    EXPECT_TRUE(anonId.compare(m_bcWrapper->getStoredAnonymousId()) == 0);
    EXPECT_TRUE(profileId.compare(m_bcWrapper->getStoredProfileId()) == 0);

    m_bcWrapper->setAlwaysAllowProfileSwitch(true);
    Logout();
}

TEST_F(TestBCWrapper, ResetEmailPassword)
{
    const char* email = "braincloudunittest@gmail.com";

    TestResult tr;
    m_bcWrapper->resetEmailPassword(email, &tr);
    tr.run(m_bc);
}

TEST_F(TestBCWrapper, ResetEmailPasswordAdvanced)
{
    const char* email = "braincloudunittest@gmail.com";
    std::string content = "{\"fromAddress\": \"fromAddress\",\"fromName\": \"fromName\",\"replyToAddress\": \"replyToAddress\",\"replyToName\": \"replyToName\", \"templateId\": \"8f14c77d-61f4-4966-ab6d-0bee8b13d090\",\"subject\": \"subject\",\"body\": \"Body goes here\", \"substitutions\": { \":name\": \"John Doe\",\":resetLink\": \"www.dummuyLink.io\"}, \"categories\": [\"category1\",\"category2\" ]}";

    TestResult tr;
    m_bcWrapper->resetEmailPasswordAdvanced(email, content, &tr);
    tr.runExpectFail(m_bc, HTTP_BAD_REQUEST, INVALID_FROM_ADDRESS);
}

TEST_F(TestBCWrapper, Reconnect)
{
	TestResult tr;

	m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");

	m_bcWrapper->reconnect(&tr);
	tr.runExpectFail(m_bc, HTTP_ACCEPTED, MISSING_PROFILE_ERROR);

	Logout();
}

TEST_F(TestBCWrapper, SmartSwitchAnonToUniversal)
{
	//need to separate these tests for windows and linux for the time being. 
	#if defined(_WIN32)
	
	std::string uid = GetUser(UserA)->m_id;
	uid.append("_wrapper");

	/*
	std::string anonIdtest =  "\n STORED ID: " + m_bcWrapper->getStoredAnonymousId() + "\n";
	std::cout<< anonIdtest;

	//after testing and test cases it's noticeable that this fucntion does not work for linux. 
	m_bcWrapper->setStoredAnonymousId(m_bcWrapper->client->getAuthenticationService()->generateAnonymousId().c_str());
	
	std::string anonIdtest1 =  "\n STORED ID: " + m_bcWrapper->getStoredAnonymousId() + "\n";
	std::cout<< anonIdtest1;
	*/
	//CONFIRMED : setStoredAnonymousId uses a pure virtual method save data helper, which is only implemented for Windows. Due to this it will not work on linux. 

	m_bcWrapper->setStoredAnonymousId(m_bcWrapper->client->getAuthenticationService()->generateAnonymousId().c_str());
	m_bcWrapper->resetStoredProfileId();

	TestResult tr;

	m_bcWrapper->authenticateAnonymous(&tr);
	tr.run(m_bc);

	std::string profileId = m_bcWrapper->getStoredProfileId();

	m_bcWrapper->smartSwitchAuthenticateUniversal(uid.c_str(), GetUser(UserA)->m_password, true, &tr);
	tr.run(m_bc);

	EXPECT_TRUE(profileId.compare(m_bcWrapper->getStoredAnonymousId()) != 0);

	#elif defined (UNIX)

	std::string uid = GetUser(UserA)->m_id;
	uid.append("_wrapper");

	m_bcWrapper->setStoredAnonymousId(m_bcWrapper->client->getAuthenticationService()->generateAnonymousId().c_str());
	m_bcWrapper->resetStoredProfileId();

	TestResult tr;

	m_bcWrapper->authenticateAnonymous(&tr);
	tr.run(m_bc);

	std::string profileId = m_bcWrapper->getStoredProfileId();

	m_bcWrapper->smartSwitchAuthenticateUniversal(uid.c_str(), GetUser(UserA)->m_password, true, &tr);
	tr.run(m_bc);

	//WE WILL NOT HAVE A SAVED ANON ID, <<BUT>> the switchauthentication call works. 
	EXPECT_FALSE(profileId.compare(m_bcWrapper->getStoredAnonymousId()) != 0);

	#endif
}

TEST_F(TestBCWrapper, SmartSwitchUniversalToEmail)
{
	//same reason, we have no way to properly store an anon id in linux yet
	#if defined(_WIN32)

	std::string email = GetUser(UserA)->m_email;
	email.append("_wrapper");

	m_bcWrapper->setStoredAnonymousId(m_bcWrapper->client->getAuthenticationService()->generateAnonymousId().c_str());
	m_bcWrapper->resetStoredProfileId();

	TestResult tr;
	std::string uid = GetUser(UserA)->m_id;
	uid.append("_wrapper");
	m_bcWrapper->authenticateUniversal(uid.c_str(), GetUser(UserA)->m_password, true, &tr);
	tr.run(m_bc);

	std::string profileId = m_bcWrapper->getStoredProfileId();

	m_bcWrapper->smartSwitchAuthenticateEmailPassword(email.c_str(), GetUser(UserA)->m_password, true, &tr);
	tr.run(m_bc);

	EXPECT_TRUE(profileId.compare(m_bcWrapper->getStoredProfileId()) != 0);

	#elif defined(UNIX)

	std::string email = GetUser(UserA)->m_email;
	email.append("_wrapper");

	m_bcWrapper->setStoredAnonymousId(m_bcWrapper->client->getAuthenticationService()->generateAnonymousId().c_str());
	m_bcWrapper->resetStoredProfileId();

	TestResult tr;
	std::string uid = GetUser(UserA)->m_id;
	uid.append("_wrapper");
	m_bcWrapper->authenticateUniversal(uid.c_str(), GetUser(UserA)->m_password, true, &tr);
	tr.run(m_bc);

	std::string profileId = m_bcWrapper->getStoredProfileId();

	m_bcWrapper->smartSwitchAuthenticateEmailPassword(email.c_str(), GetUser(UserA)->m_password, true, &tr);
	tr.run(m_bc);

	//makes it through until failure here. 
	EXPECT_FAIL(profileId.compare(m_bcWrapper->getStoredProfileId()) != 0);

	#endif
}

TEST_F(TestBCWrapper, ReInit)
{
	//case 1 multiple inits
	int initCounter = 1;

	m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");
	assert(initCounter == 1);
	initCounter++;

	m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");
	assert(initCounter == 2);
	initCounter++;

	m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");
	assert(initCounter == 3);
	initCounter++;

	//case 2 
	//auth
	TestResult tr1;
    m_bcWrapper->authenticateAnonymous(&tr1);
    tr1.run(m_bc);

	//call
	TestResult tr2;
    m_bc->getTimeService()->readServerTime(&tr2);
    tr2.run(m_bc);

	//reinit
	m_bcWrapper->initialize(m_serverUrl.c_str(), m_secret.c_str(), m_appId.c_str(), m_version.c_str(), "wrapper", "unittest");

	//call - expect failure no session
	TestResult tr3;
    m_bc->getTimeService()->readServerTime(&tr3);
    tr3.runExpectFail(m_bc, HTTP_FORBIDDEN, NO_SESSION);
}


#endif

