#include <stdlib.h>
#include "gtest/gtest.h"
#include "braincloud/BrainCloudClient.h"
#include "braincloud/BrainCloudCampaign.h"
#include "TestResult.h"
#include "json/json.h"
#include "TestBCCampaign.h"

using namespace BrainCloud;

TEST_F(TestBCCampaign, GetMyCampaigns)
{
    TestResult tr;
    m_bc->getCampaignService()->getMyCampaigns("{}", &tr);
    tr.run(m_bc);
}
