#include "ggh/modules/GalaxyCore/models/PlanetModel.h"

#include <gtest/gtest.h>

namespace ggh::GalaxyCore::models {

using PlanetType = ggh::GalaxyCore::utilities::PlanetType;

TEST(PlanetModelTest, DefaultConstructor) {
    Planet planet("", PlanetType::Rocky, 0.0, 0.0, 0, 0.0, 0.0, 0.0);
    EXPECT_EQ(planet.name(), "");
    EXPECT_EQ(planet.type(), PlanetType::Rocky);
    EXPECT_EQ(planet.numberOfMoons(), 0);
    EXPECT_DOUBLE_EQ(planet.size(), 0.0);
    EXPECT_DOUBLE_EQ(planet.mass(), 0.0);
    EXPECT_DOUBLE_EQ(planet.maxTemperature(), 0.0);
    EXPECT_DOUBLE_EQ(planet.minTemperature(), 0.0);
    EXPECT_DOUBLE_EQ(planet.orbitalRadius(), 0.0);
}

TEST(PlanetModelTest, ParameterizedConstructor) {
    Planet planet("Earth", PlanetType::Rocky, 1.0, 5.972e24, 1, 1.496e11, 288.0, 255.0);
    EXPECT_EQ(planet.name(), "Earth");
    EXPECT_EQ(planet.type(), PlanetType::Rocky);
    EXPECT_EQ(planet.numberOfMoons(), 1);
    EXPECT_DOUBLE_EQ(planet.size(), 1.0);
    EXPECT_DOUBLE_EQ(planet.mass(), 5.972e24);
    EXPECT_DOUBLE_EQ(planet.maxTemperature(), 288.0);
    EXPECT_DOUBLE_EQ(planet.minTemperature(), 255.0);
    EXPECT_DOUBLE_EQ(planet.orbitalRadius(), 1.496e11);
}

TEST(PlanetModelTest, SettersAndGetters) {
    Planet planet("", PlanetType::Rocky, 0.0, 0.0, 0, 0.0, 0.0, 0.0);
    planet.setName("Mars");
    planet.setType(PlanetType::Rocky);
    planet.setSize(0.532);
    planet.setMass(6.4171e23);
    planet.setNumberOfMoons(2);
    planet.setOrbitalRadius(227.9e9);
    planet.setMaxTemperature(20.0);
    planet.setMinTemperature(-125.0);
    
    EXPECT_EQ(planet.name(), "Mars");
    EXPECT_EQ(planet.type(), PlanetType::Rocky);
    EXPECT_DOUBLE_EQ(planet.size(), 0.532);
    EXPECT_DOUBLE_EQ(planet.mass(), 6.4171e23);
    EXPECT_EQ(planet.numberOfMoons(), 2);
    EXPECT_DOUBLE_EQ(planet.orbitalRadius(), 227.9e9);
    EXPECT_DOUBLE_EQ(planet.maxTemperature(), 20.0);
    EXPECT_DOUBLE_EQ(planet.minTemperature(), -125.0);
}

TEST(PlanetModelTest, ToXml) {
    Planet planet("Venus", PlanetType::Rocky, 0.949, 4.8675e24, 0, 1.082e11, 737.0, 462.0);
    std::string xml = planet.toXml();
    
    EXPECT_TRUE(xml.find("<Planet name=\"Venus\"") != std::string::npos);
    EXPECT_TRUE(xml.find("type=\"0\"") != std::string::npos); // Rocky type
    EXPECT_TRUE(xml.find("size=\"0.949000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("mass=\"4867500000000000349175808.000000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("moonCount=\"0\"") != std::string::npos);
    EXPECT_TRUE(xml.find("orbitDistance=\"108200000000.000000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("minTemperature=\"462.000000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("maxTemperature=\"737.000000\"") != std::string::npos);
}

}