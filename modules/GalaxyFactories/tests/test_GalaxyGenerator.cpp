#include "ggh/modules/GalaxyFactories/Types.h"

#include <gtest/gtest.h>
#include <memory>

using namespace ggh::GalaxyFactories;

// Simple tests for Types.h functionality
TEST(GenerationParametersTest, DefaultConstruction) {
    GenerationParameters params;
    
    // Test default values
    EXPECT_EQ(params.systemCount, 50);
    EXPECT_EQ(params.width, 1000);  // DEFAULT_GALAXY_WIDTH
    EXPECT_EQ(params.height, 1000); // DEFAULT_GALAXY_HEIGHT
    EXPECT_EQ(params.shape, GalaxyShape::Spiral);
    EXPECT_DOUBLE_EQ(params.spiralArms, 2.0);
    EXPECT_DOUBLE_EQ(params.spiralTightness, 0.5);
    EXPECT_DOUBLE_EQ(params.coreRadius, 0.2);
    EXPECT_DOUBLE_EQ(params.edgeRadius, 0.8);
    EXPECT_EQ(params.seed, 0);
}

TEST(GenerationParametersTest, ParameterAssignment) {
    GenerationParameters params;
    params.systemCount = 50;
    params.width = 1200;
    params.height = 800;
    params.shape = GalaxyShape::Elliptical;
    params.spiralArms = 3.0;
    params.spiralTightness = 0.7;
    params.coreRadius = 0.3;
    params.edgeRadius = 0.9;
    params.seed = 12345;

    EXPECT_EQ(params.systemCount, 50);
    EXPECT_EQ(params.width, 1200);
    EXPECT_EQ(params.height, 800);
    EXPECT_EQ(params.shape, GalaxyShape::Elliptical);
    EXPECT_DOUBLE_EQ(params.spiralArms, 3.0);
    EXPECT_DOUBLE_EQ(params.spiralTightness, 0.7);
    EXPECT_DOUBLE_EQ(params.coreRadius, 0.3);
    EXPECT_DOUBLE_EQ(params.edgeRadius, 0.9);
    EXPECT_EQ(params.seed, 12345);
}

// Test for GenerationParameters XML serialization
TEST(GenerationParametersTest, ToXmlSerialization) {
    GenerationParameters params;
    params.systemCount = 50;
    params.width = 1200;
    params.height = 800;
    params.shape = GalaxyShape::Spiral;
    params.spiralArms = 3.0;
    params.spiralTightness = 0.7;
    params.coreRadius = 0.3;
    params.edgeRadius = 0.9;
    params.seed = 12345;

    std::string xml = params.toXml();
    
    EXPECT_FALSE(xml.empty());
    EXPECT_NE(xml.find("<GalaxyParameters>"), std::string::npos);
    EXPECT_NE(xml.find("</GalaxyParameters>"), std::string::npos);
    EXPECT_NE(xml.find("<SystemCount>50</SystemCount>"), std::string::npos);
    EXPECT_NE(xml.find("<Width>1200</Width>"), std::string::npos);
    EXPECT_NE(xml.find("<Height>800</Height>"), std::string::npos);
    EXPECT_NE(xml.find("<Seed>12345</Seed>"), std::string::npos);
}

// Test enum values
TEST(GalaxyShapeTest, EnumValues) {
    EXPECT_NE(GalaxyShape::Spiral, GalaxyShape::Elliptical);
    EXPECT_NE(GalaxyShape::Elliptical, GalaxyShape::Ring);
    EXPECT_NE(GalaxyShape::Ring, GalaxyShape::Cluster);
}
