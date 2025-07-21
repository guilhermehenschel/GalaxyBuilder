#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/models/System.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

namespace ggh::Galaxy::Factions::models {

class SystemTest : public ::testing::Test {
protected:
    void SetUp() override {
        GalaxyCore::utilities::CartesianCoordinates<double> coordinates(100.0, 200.0);
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
    // Test that the system correctly retrieves data from the underlying StarSystemModel
    EXPECT_EQ(system->name(), "Test System");
    EXPECT_EQ(system->id(), 1);
}

} // namespace ggh::Galaxy::Factions
