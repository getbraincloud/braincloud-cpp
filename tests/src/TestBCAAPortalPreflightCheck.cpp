#include "gtest/gtest.h"
#include "TestBCAAPortalPreflightCheck.h"
#include "TestResult.h"
#include "braincloud/BrainCloudClient.h"
#include <string>
#include <vector>

using namespace BrainCloud;

// Runs first (alphabetically before all other TestBC* fixtures) to verify that all
// required portal configurations exist. A single failure here means the environment
// is missing portal setup — fix those before investigating other test failures.
TEST_F(TestBCAAPortalPreflightCheck, PortalPreflightCheck)
{
    std::vector<std::string> missing;

    // -------------------------------------------------------------------------
    // Leaderboards
    // -------------------------------------------------------------------------
    {
        const char* ids[] = { "testLeaderboard", "testSocialLeaderboard", "testTournamentLeaderboard", "groupLeaderboardConfig" };
        for (const char* lbId : ids)
        {
            TestResult tr;
            m_bc->getLeaderboardService()->getGlobalLeaderboardEntryCount(lbId, &tr);
            if (!tr.run(m_bc, true))
                missing.push_back(std::string("leaderboard: ") + lbId);
        }
    }

    // -------------------------------------------------------------------------
    // Item catalog
    // -------------------------------------------------------------------------
    {
        const char* ids[] = { "sword001", "equipmentBundle" };
        for (const char* itemId : ids)
        {
            TestResult tr;
            m_bc->getItemCatalogService()->getCatalogItemDefinition(itemId, &tr);
            if (!tr.run(m_bc, true))
                missing.push_back(std::string("catalog item: ") + itemId);
        }
    }

    // -------------------------------------------------------------------------
    // Global properties
    // -------------------------------------------------------------------------
    {
        TestResult tr;
        m_bc->getGlobalAppService()->readSelectedProperties({ "prop1", "prop2", "prop3" }, &tr);
        if (tr.run(m_bc, true))
        {
            const Json::Value& props = tr.m_response["data"]["response"];
            for (const char* name : { "prop1", "prop2", "prop3" })
            {
                if (!props.isMember(name))
                    missing.push_back(std::string("global property: ") + name);
            }
        }
        else
        {
            missing.push_back("global properties: prop1, prop2, prop3");
        }
    }

    // -------------------------------------------------------------------------
    // Achievements
    // -------------------------------------------------------------------------
    {
        TestResult tr;
        m_bc->getGamificationService()->readAchievements(false, &tr);
        if (tr.run(m_bc, true))
        {
            const Json::Value& achs = tr.m_response["data"]["achievements"];
            bool found01 = false, found02 = false;
            for (const auto& ach : achs)
            {
                std::string id = ach["id"].asString();
                if (id == "testAchievement01") found01 = true;
                if (id == "testAchievement02") found02 = true;
            }
            if (!found01) missing.push_back("achievement: testAchievement01");
            if (!found02) missing.push_back("achievement: testAchievement02");
        }
        else
        {
            missing.push_back("achievement: testAchievement01");
            missing.push_back("achievement: testAchievement02");
        }
    }

    // -------------------------------------------------------------------------
    // Milestone and quest category: Experience
    // -------------------------------------------------------------------------
    {
        TestResult tr;
        m_bc->getGamificationService()->readMilestonesByCategory("Experience", false, &tr);
        if (tr.run(m_bc, true))
        {
            if (tr.m_response["data"]["milestones"].size() == 0)
                missing.push_back("milestone category: Experience (no milestones defined)");
        }
        else
        {
            missing.push_back("milestone category: Experience");
        }
    }
    {
        TestResult tr;
        m_bc->getGamificationService()->readQuestsByCategory("Experience", false, &tr);
        if (tr.run(m_bc, true))
        {
            if (tr.m_response["data"]["quests"].size() == 0)
                missing.push_back("quest category: Experience (no quests defined)");
        }
        else
        {
            missing.push_back("quest category: Experience");
        }
    }

    // -------------------------------------------------------------------------
    // Virtual currency type: credits
    // -------------------------------------------------------------------------
    {
        TestResult tr;
        m_bc->getVirtualCurrencyService()->getCurrency(NULL, &tr);
        if (tr.run(m_bc, true))
        {
            if (!tr.m_response["data"]["currency"].isMember("credits"))
                missing.push_back("virtual currency type: credits");
        }
        else
        {
            missing.push_back("virtual currency type: credits");
        }
    }

    // -------------------------------------------------------------------------
    // Custom entity type: athletes
    // -------------------------------------------------------------------------
    {
        TestResult tr;
        m_bc->getCustomEntityService()->getEntityPage(
            "athletes",
            "{\"pagination\":{\"rowsPerPage\":1,\"pageNumber\":1},\"searchCriteria\":{}}",
            &tr);
        if (!tr.run(m_bc, true))
            missing.push_back("custom entity type: athletes");
    }

    // -------------------------------------------------------------------------
    // Tournament division set: testDivSetId
    // -------------------------------------------------------------------------
    {
        TestResult tr;
        m_bc->getTournamentService()->getDivisionInfo("testDivSetId", &tr);
        if (!tr.run(m_bc, true))
            missing.push_back("tournament division set: testDivSetId");
    }

    // -------------------------------------------------------------------------
    // Lobby type: MATCH_UNRANKED
    // -------------------------------------------------------------------------
    {
        TestResult tr;
        m_bc->getLobbyService()->getRegionsForLobbies({ "MATCH_UNRANKED" }, &tr);
        if (!tr.run(m_bc, true))
            missing.push_back("lobby type: MATCH_UNRANKED");
    }

    // -------------------------------------------------------------------------
    // Report
    // -------------------------------------------------------------------------
    if (!missing.empty())
    {
        std::string message = "\nPORTAL PREFLIGHT CHECK FAILED - the following items are not configured on the portal:\n";
        for (const auto& item : missing)
            message += "  - " + item + "\n";
        message += "\nSet these up in the portal before running the full test suite.\n";
        FAIL() << message;
    }
}
