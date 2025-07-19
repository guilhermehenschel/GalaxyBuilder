#include <gtest/gtest.h>
#include <QColor>
#include <QString>
#include "ggh/modules/GalaxyCore/viewmodels/Commons.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"

using namespace ggh::GalaxyCore::viewmodels::commons;
using namespace ggh::GalaxyCore::utilities;

class CommonsTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// Test starColor function with all StarType enum values
TEST_F(CommonsTest, StarColorReturnsCorrectColors) {
    // Test Red Dwarf
    QColor redDwarfColor = starColor(StarType::RedDwarf);
    EXPECT_EQ(redDwarfColor, QColor(255, 0, 0));
    
    // Test Yellow Star
    QColor yellowStarColor = starColor(StarType::YellowStar);
    EXPECT_EQ(yellowStarColor, QColor(255, 255, 0));
    
    // Test Blue Star
    QColor blueStarColor = starColor(StarType::BlueStar);
    EXPECT_EQ(blueStarColor, QColor(0, 0, 255));
    
    // Test White Dwarf
    QColor whiteDwarfColor = starColor(StarType::WhiteDwarf);
    EXPECT_EQ(whiteDwarfColor, QColor(255, 255, 255));
    
    // Test Red Giant
    QColor redGiantColor = starColor(StarType::RedGiant);
    EXPECT_EQ(redGiantColor, QColor(255, 0, 0));
    
    // Test Neutron Star
    QColor neutronColor = starColor(StarType::Neutron);
    EXPECT_EQ(neutronColor, QColor(0, 255, 255));
    
    // Test Black Hole
    QColor blackHoleColor = starColor(StarType::BlackHole);
    EXPECT_EQ(blackHoleColor, QColor(0, 0, 0));
}

TEST_F(CommonsTest, StarColorHandlesUnknownType) {
    // Test with an invalid/unknown star type (cast from invalid int)
    StarType unknownType = static_cast<StarType>(999);
    QColor unknownColor = starColor(unknownType);
    EXPECT_EQ(unknownColor, QColor(128, 128, 128)); // Gray for unknown
}

// Test planetColor function with all PlanetType enum values
TEST_F(CommonsTest, PlanetColorReturnsCorrectColors) {
    // Test Rocky planet
    QColor rockyColor = planetColor(PlanetType::Rocky);
    EXPECT_EQ(rockyColor, QColor(139, 69, 19)); // Brown
    
    // Test Gas Giant
    QColor gasGiantColor = planetColor(PlanetType::GasGiant);
    EXPECT_EQ(gasGiantColor, QColor(0, 0, 255)); // Blue
    
    // Test Ice Giant
    QColor iceGiantColor = planetColor(PlanetType::IceGiant);
    EXPECT_EQ(iceGiantColor, QColor(173, 216, 230)); // Light Blue
    
    // Test Desert planet
    QColor desertColor = planetColor(PlanetType::Desert);
    EXPECT_EQ(desertColor, QColor(210, 180, 140)); // Tan
    
    // Test Ocean planet
    QColor oceanColor = planetColor(PlanetType::Ocean);
    EXPECT_EQ(oceanColor, QColor(0, 191, 255)); // Deep Sky Blue
    
    // Test Frozen planet
    QColor frozenColor = planetColor(PlanetType::Frozen);
    EXPECT_EQ(frozenColor, QColor(240, 248, 255)); // Alice Blue
    
    // Test Volcanic planet
    QColor volcanicColor = planetColor(PlanetType::Volcanic);
    EXPECT_EQ(volcanicColor, QColor(255, 69, 0)); // Red-Orange
    
    // Test Toxic planet
    QColor toxicColor = planetColor(PlanetType::Toxic);
    EXPECT_EQ(toxicColor, QColor(0, 128, 0)); // Green
}

TEST_F(CommonsTest, PlanetColorHandlesUnknownType) {
    // Test with an invalid/unknown planet type
    PlanetType unknownType = static_cast<PlanetType>(999);
    QColor unknownColor = planetColor(unknownType);
    EXPECT_EQ(unknownColor, QColor(128, 128, 128)); // Gray for unknown
}

// Test planetTypeName function with all PlanetType enum values
TEST_F(CommonsTest, PlanetTypeNameReturnsCorrectStrings) {
    // Test Rocky planet
    QString rockyName = planetTypeName(PlanetType::Rocky);
    EXPECT_EQ(rockyName, QString("Rocky"));
    
    // Test Gas Giant
    QString gasGiantName = planetTypeName(PlanetType::GasGiant);
    EXPECT_EQ(gasGiantName, QString("Gas Giant"));
    
    // Test Ice Giant
    QString iceGiantName = planetTypeName(PlanetType::IceGiant);
    EXPECT_EQ(iceGiantName, QString("Ice Giant"));
    
    // Test Desert planet
    QString desertName = planetTypeName(PlanetType::Desert);
    EXPECT_EQ(desertName, QString("Desert"));
    
    // Test Ocean planet
    QString oceanName = planetTypeName(PlanetType::Ocean);
    EXPECT_EQ(oceanName, QString("Ocean"));
    
    // Test Frozen planet
    QString frozenName = planetTypeName(PlanetType::Frozen);
    EXPECT_EQ(frozenName, QString("Frozen"));
    
    // Test Volcanic planet
    QString volcanicName = planetTypeName(PlanetType::Volcanic);
    EXPECT_EQ(volcanicName, QString("Volcanic"));
    
    // Test Toxic planet
    QString toxicName = planetTypeName(PlanetType::Toxic);
    EXPECT_EQ(toxicName, QString("Toxic"));
}

TEST_F(CommonsTest, PlanetTypeNameHandlesUnknownType) {
    // Test with an invalid/unknown planet type
    PlanetType unknownType = static_cast<PlanetType>(999);
    QString unknownName = planetTypeName(unknownType);
    EXPECT_EQ(unknownName, QString("Unknown"));
}

// Test that all colors are valid
TEST_F(CommonsTest, AllColorsAreValid) {
    // Test that all star colors are valid QColor objects
    EXPECT_TRUE(starColor(StarType::RedDwarf).isValid());
    EXPECT_TRUE(starColor(StarType::YellowStar).isValid());
    EXPECT_TRUE(starColor(StarType::BlueStar).isValid());
    EXPECT_TRUE(starColor(StarType::WhiteDwarf).isValid());
    EXPECT_TRUE(starColor(StarType::RedGiant).isValid());
    EXPECT_TRUE(starColor(StarType::Neutron).isValid());
    EXPECT_TRUE(starColor(StarType::BlackHole).isValid());
    
    // Test that all planet colors are valid QColor objects
    EXPECT_TRUE(planetColor(PlanetType::Rocky).isValid());
    EXPECT_TRUE(planetColor(PlanetType::GasGiant).isValid());
    EXPECT_TRUE(planetColor(PlanetType::IceGiant).isValid());
    EXPECT_TRUE(planetColor(PlanetType::Desert).isValid());
    EXPECT_TRUE(planetColor(PlanetType::Ocean).isValid());
    EXPECT_TRUE(planetColor(PlanetType::Frozen).isValid());
    EXPECT_TRUE(planetColor(PlanetType::Volcanic).isValid());
    EXPECT_TRUE(planetColor(PlanetType::Toxic).isValid());
}

// Test that all planet type names are non-empty
TEST_F(CommonsTest, AllPlanetTypeNamesAreNonEmpty) {
    EXPECT_FALSE(planetTypeName(PlanetType::Rocky).isEmpty());
    EXPECT_FALSE(planetTypeName(PlanetType::GasGiant).isEmpty());
    EXPECT_FALSE(planetTypeName(PlanetType::IceGiant).isEmpty());
    EXPECT_FALSE(planetTypeName(PlanetType::Desert).isEmpty());
    EXPECT_FALSE(planetTypeName(PlanetType::Ocean).isEmpty());
    EXPECT_FALSE(planetTypeName(PlanetType::Frozen).isEmpty());
    EXPECT_FALSE(planetTypeName(PlanetType::Volcanic).isEmpty());
    EXPECT_FALSE(planetTypeName(PlanetType::Toxic).isEmpty());
}

// Test color consistency - same type should always return same color
TEST_F(CommonsTest, ColorConsistency) {
    // Test that calling the same function multiple times returns the same color
    QColor color1 = starColor(StarType::YellowStar);
    QColor color2 = starColor(StarType::YellowStar);
    EXPECT_EQ(color1, color2);
    
    QColor planetColor1 = planetColor(PlanetType::Rocky);
    QColor planetColor2 = planetColor(PlanetType::Rocky);
    EXPECT_EQ(planetColor1, planetColor2);
}

// Test string consistency - same type should always return same string
TEST_F(CommonsTest, StringConsistency) {
    QString name1 = planetTypeName(PlanetType::GasGiant);
    QString name2 = planetTypeName(PlanetType::GasGiant);
    EXPECT_EQ(name1, name2);
}
