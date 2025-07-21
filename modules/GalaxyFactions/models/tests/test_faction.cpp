#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/models/Faction.h"
#include "ggh/modules/GalaxyFactions/models/System.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

namespace ggh::Galaxy::Factions::models {

class FactionTest : public ::testing::Test {
protected:
    void SetUp() override {
        faction = std::make_shared<Faction>(1, "Test Faction", "A test faction", "#FF0000");
    }

    void TearDown() override {
        faction.reset();
    }

    std::shared_ptr<Faction> faction;
};

TEST_F(FactionTest, ConstructorInitialization) {
    EXPECT_EQ(faction->id(), 1);
    EXPECT_EQ(faction->name(), "Test Faction");
    EXPECT_EQ(faction->description(), "A test faction");
    EXPECT_EQ(faction->color(), "#FF0000");
}

TEST_F(FactionTest, SettersAndGetters) {
    faction->setName("Modified Faction");
    EXPECT_EQ(faction->name(), "Modified Faction");
    
    faction->setDescription("Modified description");
    EXPECT_EQ(faction->description(), "Modified description");
    
    faction->setColor("#00FF00");
    EXPECT_EQ(faction->color(), "#00FF00");
}

TEST_F(FactionTest, SystemManagement) {
    // Create a test system
    GalaxyCore::utilities::CartesianCoordinates<double> coordinates(100.0, 200.0);
    auto starSystemModel = std::make_shared<GalaxyCore::models::StarSystemModel>(
        1, "Test System", coordinates
    );
    auto system = std::make_shared<System>(starSystemModel);
    
    // Add system
    faction->addSystem(system);
    EXPECT_EQ(faction->systems().size(), 1);
    EXPECT_EQ(faction->systems()[0]->name(), "Test System");
    
    // Remove system
    faction->removeSystem(system->id());
    EXPECT_EQ(faction->systems().size(), 0);
}

TEST_F(FactionTest, XMLExport) {
    auto xmlString = faction->toXml();
    EXPECT_FALSE(xmlString.empty());
    EXPECT_NE(xmlString.find("Test Faction"), std::string::npos);
}

} // namespace ggh::Galaxy::Factions
