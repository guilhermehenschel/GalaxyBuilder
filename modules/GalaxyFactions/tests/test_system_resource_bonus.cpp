#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/SystemResourceBonus.h"

namespace ggh::Galaxy::Factions {

class SystemResourceBonusTest : public ::testing::Test {
protected:
    void SetUp() override {
        ResourceType testResourceType;
        testResourceType.setName("Test Resource");
        bonus = std::make_shared<SystemResourceBonus>(testResourceType, 100);
    }

    void TearDown() override {
        bonus.reset();
    }

    std::shared_ptr<SystemResourceBonus> bonus;
};

TEST_F(SystemResourceBonusTest, ConstructorInitialization) {
    EXPECT_EQ(bonus->resourceType().name(), "Test Resource");
    EXPECT_EQ(bonus->bonusAmount(), 100);
}

TEST_F(SystemResourceBonusTest, ParameterizedConstructor) {
    ResourceType energyType;
    energyType.setName("Energy");
    auto testBonus = std::make_shared<SystemResourceBonus>(energyType, 50);
    
    EXPECT_EQ(testBonus->resourceType().name(), "Energy");
    EXPECT_EQ(testBonus->bonusAmount(), 50);
}

TEST_F(SystemResourceBonusTest, SettersAndGetters) {
    ResourceType mineralType;
    mineralType.setName("Minerals");
    
    bonus->setResourceType(mineralType);
    EXPECT_EQ(bonus->resourceType().name(), "Minerals");
    
    bonus->setBonusAmount(200);
    EXPECT_EQ(bonus->bonusAmount(), 200);
}

} // namespace ggh::Galaxy::Factions
