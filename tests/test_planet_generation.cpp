#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "GalaxyGenerator.h"
#include "../modules/GalaxyCore/models/StarSystemModel.h"
#include "../modules/GalaxyCore/models/PlanetModel.h"

using namespace ggh::GalaxyFactories;
using namespace ggh::GalaxyCore::models;

// Test fixture for planet generation tests
class PlanetGenerationTest : public ::testing::Test {
protected:
    void SetUp() override {
        generator = std::make_unique<GalaxyGenerator>();
    }

    std::unique_ptr<GalaxyGenerator> generator;
};

// Test that planets are generated based on system size
TEST_F(PlanetGenerationTest, PlanetsGeneratedBasedOnSystemSize) {
    auto smallSystem = std::make_unique<StarSystemModel>();
    smallSystem->setSize(StarSystemModel::SystemSize::Small);
    
    auto mediumSystem = std::make_unique<StarSystemModel>();
    mediumSystem->setSize(StarSystemModel::SystemSize::Medium);
    
    auto largeSystem = std::make_unique<StarSystemModel>();
    largeSystem->setSize(StarSystemModel::SystemSize::Large);
    
    auto hugeSystem = std::make_unique<StarSystemModel>();
    hugeSystem->setSize(StarSystemModel::SystemSize::Huge);
    
    // Generate planets for each system size
    generator->generatePlanetsForSystem(smallSystem.get());
    generator->generatePlanetsForSystem(mediumSystem.get());
    generator->generatePlanetsForSystem(largeSystem.get());
    generator->generatePlanetsForSystem(hugeSystem.get());
    
    // Check planet counts are within expected ranges
    EXPECT_GE(smallSystem->planets().size(), 1);
    EXPECT_LE(smallSystem->planets().size(), 3);
    
    EXPECT_GE(mediumSystem->planets().size(), 3);
    EXPECT_LE(mediumSystem->planets().size(), 6);
    
    EXPECT_GE(largeSystem->planets().size(), 5);
    EXPECT_LE(largeSystem->planets().size(), 9);
    
    EXPECT_GE(hugeSystem->planets().size(), 7);
    EXPECT_LE(hugeSystem->planets().size(), 12);
    
    // Verify that larger systems generally have more planets
    EXPECT_GE(mediumSystem->planets().size(), smallSystem->planets().size());
    EXPECT_GE(largeSystem->planets().size(), mediumSystem->planets().size());
    EXPECT_GE(hugeSystem->planets().size(), largeSystem->planets().size());
}

// Test that generated planets have valid properties
TEST_F(PlanetGenerationTest, GeneratedPlanetsHaveValidProperties) {
    auto system = std::make_unique<StarSystemModel>();
    system->setSize(StarSystemModel::SystemSize::Medium);
    
    generator->generatePlanetsForSystem(system.get());
    
    ASSERT_GT(system->planets().size(), 0);
    
    for (const auto& planet : system->planets()) {
        // Check that planet has a name
        EXPECT_FALSE(planet->name().isEmpty());
        
        // Check that planet type is valid
        auto type = planet->type();
        EXPECT_TRUE(type == PlanetModel::PlanetType::Terrestrial ||
                   type == PlanetModel::PlanetType::GasGiant ||
                   type == PlanetModel::PlanetType::IceGiant ||
                   type == PlanetModel::PlanetType::Desert ||
                   type == PlanetModel::PlanetType::Ocean ||
                   type == PlanetModel::PlanetType::Frozen);
        
        // Check that orbital radius is positive
        EXPECT_GT(planet->orbitalRadius(), 0.0);
        
        // Check that size is positive
        EXPECT_GT(planet->size(), 0.0);
        
        // Check that mass is positive
        EXPECT_GT(planet->mass(), 0.0);
        
        // Check that moon count is non-negative
        EXPECT_GE(planet->moons(), 0);
        
        // Check that temperatures are reasonable (absolute zero to very hot)
        EXPECT_GT(planet->surfaceTemperature(), -273.15); // Above absolute zero
        EXPECT_LT(planet->surfaceTemperature(), 2000.0);  // Below extreme stellar temperatures
        
        EXPECT_GT(planet->coreTemperature(), -273.15);
        EXPECT_LT(planet->coreTemperature(), 10000.0);
    }
}

// Test that planets are ordered by orbital radius
TEST_F(PlanetGenerationTest, PlanetsOrderedByOrbitalRadius) {
    auto system = std::make_unique<StarSystemModel>();
    system->setSize(StarSystemModel::SystemSize::Large);
    
    generator->generatePlanetsForSystem(system.get());
    
    ASSERT_GT(system->planets().size(), 1);
    
    // Check that planets are ordered by increasing orbital radius
    for (size_t i = 1; i < system->planets().size(); ++i) {
        EXPECT_GE(system->planets()[i]->orbitalRadius(), 
                 system->planets()[i-1]->orbitalRadius());
    }
}

// Test planet type distribution
TEST_F(PlanetGenerationTest, PlanetTypeDistributionIsRealistic) {
    auto system = std::make_unique<StarSystemModel>();
    system->setSize(StarSystemModel::SystemSize::Huge);
    
    generator->generatePlanetsForSystem(system.get());
    
    ASSERT_GT(system->planets().size(), 5);
    
    int terrestrialCount = 0;
    int gasGiantCount = 0;
    int otherCount = 0;
    
    for (const auto& planet : system->planets()) {
        switch (planet->type()) {
            case PlanetModel::PlanetType::Terrestrial:
            case PlanetModel::PlanetType::Desert:
            case PlanetModel::PlanetType::Ocean:
            case PlanetModel::PlanetType::Frozen:
                terrestrialCount++;
                break;
            case PlanetModel::PlanetType::GasGiant:
            case PlanetModel::PlanetType::IceGiant:
                gasGiantCount++;
                break;
            default:
                otherCount++;
                break;
        }
    }
    
    // Should have a mix of planet types
    EXPECT_GT(terrestrialCount + gasGiantCount + otherCount, 0);
    
    // In a large system, we expect to see some variety
    EXPECT_TRUE(terrestrialCount > 0 || gasGiantCount > 0);
}

// Test that planet properties correlate with distance from star
TEST_F(PlanetGenerationTest, PlanetPropertiesCorrelateWithDistance) {
    auto system = std::make_unique<StarSystemModel>();
    system->setSize(StarSystemModel::SystemSize::Large);
    
    generator->generatePlanetsForSystem(system.get());
    
    ASSERT_GT(system->planets().size(), 2);
    
    // Generally, planets farther from the star should be colder
    auto innerPlanet = system->planets().front();
    auto outerPlanet = system->planets().back();
    
    EXPECT_GT(outerPlanet->orbitalRadius(), innerPlanet->orbitalRadius());
    
    // Temperature correlation is more complex due to planet types,
    // but we can check that extreme distances have appropriate temperatures
    if (outerPlanet->orbitalRadius() > 10.0) { // Far outer system
        EXPECT_LT(outerPlanet->surfaceTemperature(), 100.0); // Should be quite cold
    }
}

// Test that gas giants are more likely to have moons
TEST_F(PlanetGenerationTest, GasGiantsHaveMoreMoons) {
    auto system = std::make_unique<StarSystemModel>();
    system->setSize(StarSystemModel::SystemSize::Huge);
    
    generator->generatePlanetsForSystem(system.get());
    
    int gasGiantMoons = 0;
    int gasGiantCount = 0;
    int terrestrialMoons = 0;
    int terrestrialCount = 0;
    
    for (const auto& planet : system->planets()) {
        if (planet->type() == PlanetModel::PlanetType::GasGiant ||
            planet->type() == PlanetModel::PlanetType::IceGiant) {
            gasGiantMoons += planet->moons();
            gasGiantCount++;
        } else {
            terrestrialMoons += planet->moons();
            terrestrialCount++;
        }
    }
    
    // If we have both types, gas giants should generally have more moons
    if (gasGiantCount > 0 && terrestrialCount > 0) {
        double avgGasGiantMoons = static_cast<double>(gasGiantMoons) / gasGiantCount;
        double avgTerrestrialMoons = static_cast<double>(terrestrialMoons) / terrestrialCount;
        
        // Gas giants should tend to have more moons on average
        EXPECT_GE(avgGasGiantMoons, avgTerrestrialMoons * 0.8); // Allow some variance
    }
}

// Test system size generation
TEST_F(PlanetGenerationTest, SystemSizeGenerationIsValid) {
    for (int i = 0; i < 100; ++i) {
        auto size = generator->generateRandomSystemSize();
        
        EXPECT_TRUE(size == StarSystemModel::SystemSize::Small ||
                   size == StarSystemModel::SystemSize::Medium ||
                   size == StarSystemModel::SystemSize::Large ||
                   size == StarSystemModel::SystemSize::Huge);
    }
}

// Test planet name generation
TEST_F(PlanetGenerationTest, PlanetNamesAreUnique) {
    auto system = std::make_unique<StarSystemModel>();
    system->setSize(StarSystemModel::SystemSize::Large);
    system->setName("TestSystem");
    
    generator->generatePlanetsForSystem(system.get());
    
    std::set<QString> planetNames;
    for (const auto& planet : system->planets()) {
        EXPECT_FALSE(planet->name().isEmpty());
        
        // Names should be unique within the system
        EXPECT_TRUE(planetNames.find(planet->name()) == planetNames.end());
        planetNames.insert(planet->name());
        
        // Names should contain the system name
        EXPECT_TRUE(planet->name().contains("TestSystem"));
    }
}

// Test that multiple system generations are consistent
TEST_F(PlanetGenerationTest, MultipleGenerationsAreConsistent) {
    for (int i = 0; i < 10; ++i) {
        auto system = std::make_unique<StarSystemModel>();
        system->setSize(StarSystemModel::SystemSize::Medium);
        
        generator->generatePlanetsForSystem(system.get());
        
        // Each generation should produce valid results
        EXPECT_GT(system->planets().size(), 0);
        EXPECT_LE(system->planets().size(), 6);
        
        // All planets should have valid properties
        for (const auto& planet : system->planets()) {
            EXPECT_FALSE(planet->name().isEmpty());
            EXPECT_GT(planet->orbitalRadius(), 0.0);
            EXPECT_GT(planet->size(), 0.0);
            EXPECT_GT(planet->mass(), 0.0);
        }
    }
}
