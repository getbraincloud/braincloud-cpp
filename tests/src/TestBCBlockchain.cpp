#include <stdlib.h>
#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/BrainCloudBlockchain.h"
#include "TestResult.h"
#include "json/json.h"
#include "TestBCBlockchain.h"
#include "braincloud/reason_codes.h"
#include "braincloud/http_codes.h"


using namespace BrainCloud;


TEST_F(TestBCBlockchain, GetBlockchainItems)
{
	TestResult tr;

	Json::FastWriter fw;
	m_bc->getBlockchainService()->GetBlockchainItems(_defaultIntegrationId, _defaultContextJson, &tr);
    tr.runExpectFail(m_bc, 400, 40678);
}

TEST_F(TestBCBlockchain, GetUniqs)
{
	TestResult tr;
    Json::Value context;
	Json::FastWriter fw;

    m_bc->getBlockchainService()->GetUniqs(_defaultIntegrationId, _defaultContextJson, &tr);

    tr.runExpectFail(m_bc, 400, 40678);
}
