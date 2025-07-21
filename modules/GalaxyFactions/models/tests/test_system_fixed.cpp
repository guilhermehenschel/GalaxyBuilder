#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/models/System.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/CartesianCoordinates.h"

namespace ggh::Galaxy::Factions {

class SystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto coordinates = std::make_shared<GalaxyCore::models::CartesianCoordinates>(100.0, 200.0, 300.0);
        starSystemModel = std::make_shared<GalaxyCore::models::StarSystemModel>(1, "Test System", coordinates);
        system = std::make_shared<System>(starSystemModel);
    }

    void TearDown() override {
        system.reset();
        starSystemModel.reset();
    }

    std::shared_ptr<GalaxyCore::models::StarSystemModel> starSystemModel;
    std::shared_ptr<System> system;
};

TEST_F(SystemTest, ConstructorInitialization) {
    EXPECT_EQ(system->id(), 1);
    EXPECT_EQ(system->name(), "Test System");
}

TEST_F(SystemTest, ResourceBonusManagement) {
    ResourceType energyType;
    energyType.setName("Energy");
    
    system->addResourceBonus(energyType, 50);
    auto bonuses = system->getResourceBonuses();
    
    EXPECT_EQ(bonuses.size(), 1);
    EXPECT_EQ(bonuses[0].resourceType().name(), "Energy");
    EXPECT_EQ(bonuses[0].bonusAmount(), 50);
}

TEST_F(SystemTest, SettersAndGetters) {
    system->setDescription("A test system description");
    EXPECT_EQ(system->description(), "A test system description");
}

} // namespace ggh::Galaxy::Factions
