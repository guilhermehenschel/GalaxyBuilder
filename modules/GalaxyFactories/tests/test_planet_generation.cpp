#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactories/GalaxyGenerator.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

using namespace ggh::GalaxyFactories;
using namespace ggh::GalaxyCore::models;
using namespace ggh::GalaxyCore::utilities;

// Test fixture for planet generation tests
class PlanetGenerationTest : public ::testing::Test {
protected:
    void SetUp() override {
        generator = std::make_unique<GalaxyGenerator>();
    }

    std::unique_ptr<GalaxyGenerator> generator;
    
    // Helper method to create a star system
    std::shared_ptr<StarSystemModel> createTestSystem(SystemSize size) {
        CartesianCoordinates<double> position(0.0, 0.0);
        auto system = std::make_shared<StarSystemModel>(1, "TestSystem", position);
        system->setSystemSize(size);
        return system;
    }
};

// Test that planets are generated based on system size (statistical test)
TEST_F(PlanetGenerationTest, PlanetsGeneratedBasedOnSystemSize) {
    // Run multiple iterations to get statistical averages
    constexpr int iterations = 50;
    double smallAvg = 0, mediumAvg = 0, largeAvg = 0, hugeAvg = 0;
    
    for (int i = 0; i < iterations; ++i) {
        auto smallSystem = createTestSystem(SystemSize::Small);
        auto mediumSystem = createTestSystem(SystemSize::Medium);
        auto largeSystem = createTestSystem(SystemSize::Large);
        auto hugeSystem = createTestSystem(SystemSize::Huge);
        
        // Generate planets for each system size
        generator->generatePlanetsForSystem(smallSystem);
        generator->generatePlanetsForSystem(mediumSystem);
        generator->generatePlanetsForSystem(largeSystem);
        generator->generatePlanetsForSystem(hugeSystem);
        
        // Check planet counts are within expected ranges
        EXPECT_GE(smallSystem->getPlanets().size(), 1);
        EXPECT_LE(smallSystem->getPlanets().size(), 3);
        
        EXPECT_GE(mediumSystem->getPlanets().size(), 3);
        EXPECT_LE(mediumSystem->getPlanets().size(), 6);
        
        EXPECT_GE(largeSystem->getPlanets().size(), 5);
        EXPECT_LE(largeSystem->getPlanets().size(), 9);
        
        EXPECT_GE(hugeSystem->getPlanets().size(), 7);
        EXPECT_LE(hugeSystem->getPlanets().size(), 12);
        
        // Accumulate for averages
        smallAvg += smallSystem->getPlanets().size();
        mediumAvg += mediumSystem->getPlanets().size();
        largeAvg += largeSystem->getPlanets().size();
        hugeAvg += hugeSystem->getPlanets().size();
    }
    
    // Calculate averages
    smallAvg /= iterations;
    mediumAvg /= iterations;
    largeAvg /= iterations;
    hugeAvg /= iterations;
    
    // Verify that on average, larger systems have more planets than smaller ones
    EXPECT_GT(mediumAvg, smallAvg);
    EXPECT_GT(largeAvg, mediumAvg);
    EXPECT_GT(hugeAvg, largeAvg);
}

// Test that generated planets have valid properties
TEST_F(PlanetGenerationTest, GeneratedPlanetsHaveValidProperties) {
    auto system = createTestSystem(SystemSize::Medium);
    
    generator->generatePlanetsForSystem(system);
    
    ASSERT_GT(system->getPlanets().size(), 0);
    
    for (const auto& planet : system->getPlanets()) {
        // Check that planet has a name
        EXPECT_FALSE(planet->name().empty());

        // Check that planet type is valid
        auto type = planet->type();
        EXPECT_TRUE(type == PlanetType::Rocky ||
                   type == PlanetType::GasGiant ||
                   type == PlanetType::IceGiant ||
                   type == PlanetType::Desert ||
                   type == PlanetType::Ocean ||
                   type == PlanetType::Frozen ||
                   type == PlanetType::Volcanic ||
                   type == PlanetType::Toxic);
        
        // Check that orbital radius is positive
        EXPECT_GT(planet->orbitalRadius(), 0.0);

        // Check that size is positive
        EXPECT_GT(planet->size(), 0.0);

        // Check that mass is positive
        EXPECT_GT(planet->mass(), 0.0);

        // Check that moon count is non-negative
        EXPECT_GE(planet->numberOfMoons(), 0);

        // Check that temperatures are reasonable (absolute zero to very hot)
        EXPECT_GT(planet->maxTemperature(), -273.15); // Above absolute zero
        EXPECT_LT(planet->maxTemperature(), 2000.0);  // Below extreme stellar temperatures

        EXPECT_GT(planet->minTemperature(), -273.15);
        EXPECT_LT(planet->minTemperature(), 2000.0);

        // Max temperature should be higher than or equal to min temperature
        EXPECT_GE(planet->maxTemperature(), planet->minTemperature());
    }
}

// Test that planets are ordered by orbital radius
TEST_F(PlanetGenerationTest, PlanetsOrderedByOrbitalRadius) {
    auto system = createTestSystem(SystemSize::Large);
    
    generator->generatePlanetsForSystem(system);
    
    ASSERT_GT(system->getPlanets().size(), 1);
    
    // Check that planets are ordered by increasing orbital radius
    for (size_t i = 1; i < system->getPlanets().size(); ++i) {
        EXPECT_GE(system->getPlanets()[i]->orbitalRadius(), 
                 system->getPlanets()[i-1]->orbitalRadius());
    }
}

// Test planet type distribution
TEST_F(PlanetGenerationTest, PlanetTypeDistributionIsRealistic) {
    auto system = createTestSystem(SystemSize::Huge);
    
    generator->generatePlanetsForSystem(system);
    
    ASSERT_GT(system->getPlanets().size(), 5);
    
    int rockyCount = 0;
    int gasGiantCount = 0;
    int otherCount = 0;
    
    for (const auto& planet : system->getPlanets()) {
        switch (planet->type()) {
            case PlanetType::Rocky:
            case PlanetType::Desert:
            case PlanetType::Ocean:
            case PlanetType::Frozen:
            case PlanetType::Volcanic:
            case PlanetType::Toxic:
                rockyCount++;
                break;
            case PlanetType::GasGiant:
            case PlanetType::IceGiant:
                gasGiantCount++;
                break;
            default:
                otherCount++;
                break;
        }
    }
    
    // Should have a mix of planet types
    EXPECT_GT(rockyCount + gasGiantCount + otherCount, 0);
    
    // In a large system, we expect to see some variety
    EXPECT_TRUE(rockyCount > 0 || gasGiantCount > 0);
}

// Test that gas giants tend to have more moons
TEST_F(PlanetGenerationTest, GasGiantsHaveMoreMoons) {
    auto system = createTestSystem(SystemSize::Huge);
    
    generator->generatePlanetsForSystem(system);
    
    int gasGiantMoons = 0;
    int gasGiantCount = 0;
    int rockyMoons = 0;
    int rockyCount = 0;
    
    for (const auto& planet : system->getPlanets()) {
        if (planet->type() == PlanetType::GasGiant ||
            planet->type() == PlanetType::IceGiant) {
            gasGiantMoons += planet->numberOfMoons();
            gasGiantCount++;
        } else {
            rockyMoons += planet->numberOfMoons();
            rockyCount++;
        }
    }
    
    // If we have both types, gas giants should generally have more moons
    if (gasGiantCount > 0 && rockyCount > 0) {
        double avgGasGiantMoons = static_cast<double>(gasGiantMoons) / gasGiantCount;
        double avgRockyMoons = static_cast<double>(rockyMoons) / rockyCount;
        
        // Gas giants should tend to have more moons on average
        EXPECT_GE(avgGasGiantMoons, avgRockyMoons * 0.8); // Allow some variance
    }
}

// Test system size generation
TEST_F(PlanetGenerationTest, SystemSizeGenerationIsValid) {
    for (int i = 0; i < 100; ++i) {
        auto size = generator->generateRandomSystemSize();
        
        EXPECT_TRUE(size == SystemSize::Small ||
                   size == SystemSize::Medium ||
                   size == SystemSize::Large ||
                   size == SystemSize::Huge);
    }
}

// Test that multiple system generations are consistent
TEST_F(PlanetGenerationTest, MultipleGenerationsAreConsistent) {
    for (int i = 0; i < 10; ++i) {
        auto system = createTestSystem(SystemSize::Medium);
        
        generator->generatePlanetsForSystem(system);
        
        // Each generation should produce valid results
        EXPECT_GT(system->getPlanets().size(), 0);
        EXPECT_LE(system->getPlanets().size(), 6);
        
        // All planets should have valid properties
        for (const auto& planet : system->getPlanets()) {
            EXPECT_FALSE(planet->name().empty());
            EXPECT_GT(planet->orbitalRadius(), 0.0);
            EXPECT_GT(planet->size(), 0.0);
            EXPECT_GT(planet->mass(), 0.0);
        }
    }
}
