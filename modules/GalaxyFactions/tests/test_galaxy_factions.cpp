#include <gtest/gtest.h>
#include <fstream>
#include "ggh/modules/GalaxyFactions/GalaxyFactions.h"

namespace ggh::Galaxy::Factions {

class GalaxyFactionsTest : public ::testing::Test {
protected:
    void SetUp() override {
        galaxyFactions = new GalaxyFactions();
    }

    void TearDown() override {
        delete galaxyFactions;
    }

    GalaxyFactions* galaxyFactions;
};

TEST_F(GalaxyFactionsTest, CreateFaction) {
    auto faction = galaxyFactions->createFaction("Test Faction", "Description", "#FF0000");
    
    EXPECT_NE(faction, nullptr);
    EXPECT_EQ(faction->name(), "Test Faction");
    EXPECT_EQ(faction->description(), "Description");
    EXPECT_EQ(faction->color(), "#FF0000");
    EXPECT_EQ(galaxyFactions->getAllFactions().size(), 1);
}

TEST_F(GalaxyFactionsTest, ModifyFaction) {
    auto faction = galaxyFactions->createFaction("Original", "Original desc", "#FF0000");
    int factionId = faction->id();
    
    auto modifiedFaction = galaxyFactions->modifyFaction(factionId, "Modified", "Modified desc", "#00FF00");
    
    EXPECT_NE(modifiedFaction, nullptr);
    EXPECT_EQ(modifiedFaction->name(), "Modified");
    EXPECT_EQ(modifiedFaction->description(), "Modified desc");
    EXPECT_EQ(modifiedFaction->color(), "#00FF00");
}

TEST_F(GalaxyFactionsTest, DeleteFaction) {
    auto faction = galaxyFactions->createFaction("To Delete", "Description", "#FF0000");
    int factionId = faction->id();
    
    bool deleted = galaxyFactions->deleteFaction(factionId);
    
    EXPECT_TRUE(deleted);
    EXPECT_EQ(galaxyFactions->getAllFactions().size(), 0);
}

TEST_F(GalaxyFactionsTest, GetFaction) {
    auto faction = galaxyFactions->createFaction("Test Faction", "Description", "#FF0000");
    int factionId = faction->id();
    
    auto retrievedFaction = galaxyFactions->getFaction(factionId);
    
    EXPECT_NE(retrievedFaction, nullptr);
    EXPECT_EQ(retrievedFaction->id(), factionId);
    EXPECT_EQ(retrievedFaction->name(), "Test Faction");
}

TEST_F(GalaxyFactionsTest, GetNonExistentFaction) {
    auto faction = galaxyFactions->getFaction(999);
    
    EXPECT_EQ(faction, nullptr);
}

TEST_F(GalaxyFactionsTest, ExportFactionsToXml) {
    galaxyFactions->createFaction("Faction1", "Description1", "#FF0000");
    galaxyFactions->createFaction("Faction2", "Description2", "#00FF00");
    
    // Test export - should not crash and return valid XML
    std::string xmlOutput = galaxyFactions->toXml();
    EXPECT_FALSE(xmlOutput.empty());
    EXPECT_NE(xmlOutput.find("Faction1"), std::string::npos);
    EXPECT_NE(xmlOutput.find("Faction2"), std::string::npos);
}

TEST_F(GalaxyFactionsTest, ExportWithFormat) {
    galaxyFactions->createFaction("Faction1", "Description1", "#FF0000");
    
    // Test XML export using toXml method
    std::string xmlOutput = galaxyFactions->toXml();
    EXPECT_FALSE(xmlOutput.empty());
    EXPECT_NE(xmlOutput.find("Faction1"), std::string::npos);
}

TEST_F(GalaxyFactionsTest, MultipleFactionsUniqueIds) {
    auto faction1 = galaxyFactions->createFaction("Faction1", "Desc1", "#FF0000");
    auto faction2 = galaxyFactions->createFaction("Faction2", "Desc2", "#00FF00");
    auto faction3 = galaxyFactions->createFaction("Faction3", "Desc3", "#0000FF");
    
    EXPECT_NE(faction1->id(), faction2->id());
    EXPECT_NE(faction2->id(), faction3->id());
    EXPECT_NE(faction1->id(), faction3->id());
}
}
