#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/models/FactionHandover.h"
#include "ggh/modules/GalaxyCore/models/GalacticDate.h"

namespace ggh::Galaxy::Factions::models {

class FactionHandoverTest : public ::testing::Test {
protected:
    void SetUp() override {
        GalaxyCore::models::GalacticDate testDate(2400, 1, 1); // Provide proper constructor parameters
        handover = std::make_shared<FactionHandover>(1, 10, 5, 20, 2, testDate, "Test handover");
    }

    void TearDown() override {
        handover.reset();
    }

    std::shared_ptr<FactionHandover> handover;
};

TEST_F(FactionHandoverTest, ConstructorInitialization) {
    EXPECT_EQ(handover->id(), 1);
    EXPECT_EQ(handover->factionToId(), 10);
    EXPECT_EQ(handover->systemId(), 5);
    EXPECT_EQ(handover->factionFromId(), 20);
    EXPECT_EQ(handover->handoverTypeId(), 2);
    EXPECT_EQ(handover->notes(), "Test handover");
}

TEST_F(FactionHandoverTest, ParameterizedConstructor) {
    GalaxyCore::models::GalacticDate testDate(2401, 5, 15);
    auto testHandover = std::make_shared<FactionHandover>(2, 11, 6, 21, 3, testDate, "Conquest completed");
    
    EXPECT_EQ(testHandover->id(), 2);
    EXPECT_EQ(testHandover->factionToId(), 11);
    EXPECT_EQ(testHandover->systemId(), 6);
    EXPECT_EQ(testHandover->factionFromId(), 21);
    EXPECT_EQ(testHandover->handoverTypeId(), 3);
    EXPECT_EQ(testHandover->notes(), "Conquest completed");
}

TEST_F(FactionHandoverTest, SettersAndGetters) {
    GalaxyCore::models::GalacticDate testDate(2402, 12, 31);
    
    handover->setId(99);
    handover->setFactionToId(100);
    handover->setSystemId(200);
    handover->setFactionFromId(300);
    handover->setHandoverTypeId(3);
    handover->setHandoverDate(testDate);
    handover->setNotes("Diplomatic agreement");
    
    EXPECT_EQ(handover->id(), 99);
    EXPECT_EQ(handover->factionToId(), 100);
    EXPECT_EQ(handover->systemId(), 200);
    EXPECT_EQ(handover->factionFromId(), 300);
    EXPECT_EQ(handover->handoverTypeId(), 3);
    EXPECT_EQ(handover->handoverDate(), testDate);
    EXPECT_EQ(handover->notes(), "Diplomatic agreement");
}
}
