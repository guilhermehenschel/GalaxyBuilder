#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"

#include <gtest/gtest.h>

namespace ggh::GalaxyCore::models {

TEST(StarSystemModelTest, ParameterizedConstructor) {
    StarSystemModel system(1, "Test System", {100.0, 200.0}, StarType::RedGiant);
    EXPECT_EQ(system.getId(), 1);
    EXPECT_EQ(system.getName(), "Test System");
    EXPECT_DOUBLE_EQ(system.getPosition().x, 100.0);
    EXPECT_DOUBLE_EQ(system.getPosition().y, 200.0);
    EXPECT_EQ(system.getStarType(), StarType::RedGiant);
    EXPECT_EQ(system.getSystemSize(), SystemSize::Medium);
    EXPECT_TRUE(system.getPlanets().empty());
}

TEST(StarSystemModelTest, AddPlanet) {
    StarSystemModel system(2, "Planetary System", {300.0, 400.0}, StarType::BlueStar);
    Planet planet("Test Planet", PlanetType::GasGiant, 1.0, 5.972e24, 2, 1.496e11, 288.0, 255.0);
    
    system.addPlanet(planet);
    
    EXPECT_EQ(system.getPlanets().size(), 1);
    EXPECT_EQ(system.getPlanets()[0].name(), "Test Planet");
    EXPECT_EQ(system.getPlanets()[0].type(), PlanetType::GasGiant);
    EXPECT_DOUBLE_EQ(system.getPlanets()[0].size(), 1.0);
    EXPECT_DOUBLE_EQ(system.getPlanets()[0].mass(), 5.972e24);
    EXPECT_EQ(system.getPlanets()[0].numberOfMoons(), 2);
    EXPECT_DOUBLE_EQ(system.getPlanets()[0].orbitalRadius(), 1.496e11);
    EXPECT_DOUBLE_EQ(system.getPlanets()[0].maxTemperature(), 288.0);
    EXPECT_DOUBLE_EQ(system.getPlanets()[0].minTemperature(), 255.0);
}

TEST(StarSystemModelTest, ToXml) {
    StarSystemModel system(3, "XML System", {500.0, 600.0}, StarType::WhiteDwarf);
    Planet planet("XML Planet", PlanetType::Rocky, 0.5, 5.972e24, 1, 1.496e11, 288.0, 255.0);
    
    system.addPlanet(planet);
    
    std::string xml = system.toXml();
    
    EXPECT_TRUE(xml.find("<StarSystem") != std::string::npos);
    EXPECT_TRUE(xml.find("id=\"3\"") != std::string::npos);
    EXPECT_TRUE(xml.find("name=\"XML System\"") != std::string::npos);
    EXPECT_TRUE(xml.find("<Planet name=\"XML Planet\"") != std::string::npos);
}

TEST(StarSystemModelTest, SettersAndGetters) {
    StarSystemModel system(4, "Setter System", {700.0, 800.0}, StarType::YellowStar);
    
    system.setName("Updated System");
    system.setPosition({900.0, 1000.0});
    system.setStarType(StarType::BlackHole);
    system.setSystemSize(SystemSize::Large);
    
    EXPECT_EQ(system.getName(), "Updated System");
    EXPECT_DOUBLE_EQ(system.getPosition().x, 900.0);
    EXPECT_DOUBLE_EQ(system.getPosition().y, 1000.0);
    EXPECT_EQ(system.getStarType(), StarType::BlackHole);
    EXPECT_EQ(system.getSystemSize(), SystemSize::Large);
}
TEST(StarSystemModelTest, PlanetXmlConversion) {
    StarSystemModel system(5, "Planet XML System", {1100.0, 1200.0}, StarType::Neutron);
    Planet planet("XML Test Planet", PlanetType::IceGiant, 2.0, 1.898e27, 79, 778.5e9, 130.0, 90.0);
    
    system.addPlanet(planet);
    
    std::string xml = system.toXml();
    
    EXPECT_TRUE(xml.find("<Planet name=\"XML Test Planet\"") != std::string::npos);
    EXPECT_TRUE(xml.find("type=\"2\"") != std::string::npos); // IceGiant type
    EXPECT_TRUE(xml.find("size=\"2.000000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("mass=\"1897999999999999949903233024.000000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("moonCount=\"79\"") != std::string::npos);
    EXPECT_TRUE(xml.find("minTemperature=\"90.000000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("maxTemperature=\"130.000000\"") != std::string::npos);
    EXPECT_TRUE(xml.find("orbitDistance=\"778500000000.000000\"") != std::string::npos);
}


}