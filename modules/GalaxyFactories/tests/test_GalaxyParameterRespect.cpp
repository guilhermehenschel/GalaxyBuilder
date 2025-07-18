#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactories/GalaxyGenerator.h"
#include "ggh/modules/GalaxyFactories/Types.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"

using namespace ggh::GalaxyFactories;
using namespace ggh::GalaxyCore::utilities;

class GalaxyParameterRespectTest : public ::testing::Test {
protected:
    void SetUp() override {
        generator = std::make_unique<GalaxyGenerator>(12345); // Fixed seed for reproducible tests
    }

    std::unique_ptr<GalaxyGenerator> generator;
};

TEST_F(GalaxyParameterRespectTest, SystemCountIsRespected) {
    std::vector<int> testCounts = {10, 25, 50, 100, 200};
    
    for (int expectedCount : testCounts) {
        GenerationParameters params;
        params.systemCount = expectedCount;
        params.width = 1000;
        params.height = 1000;
        params.shape = GalaxyShape::Spiral;
        params.seed = 12345;
        
        generator->setParameters(params);
        auto galaxy = generator->generateGalaxy();
        
        ASSERT_NE(galaxy, nullptr) << "Galaxy generation failed for " << expectedCount << " systems";
        
        auto systems = galaxy->getAllStarSystems();
        
        // Allow small tolerance for edge cases where position validation might prevent some systems
        EXPECT_GE(systems.size(), expectedCount * 0.9) 
            << "Generated " << systems.size() << " systems, expected at least " << (expectedCount * 0.9) << " for target " << expectedCount;
        EXPECT_LE(systems.size(), expectedCount * 1.1) 
            << "Generated " << systems.size() << " systems, expected at most " << (expectedCount * 1.1) << " for target " << expectedCount;
    }
}

TEST_F(GalaxyParameterRespectTest, SpiralArmsAffectDistribution) {
    GenerationParameters params;
    params.systemCount = 100;
    params.width = 1000;
    params.height = 1000;
    params.shape = GalaxyShape::Spiral;
    params.spiralArms = 2;
    params.seed = 12345;
    
    generator->setParameters(params);
    auto galaxy2Arms = generator->generateGalaxy();
    
    params.spiralArms = 4;
    generator->setParameters(params);
    auto galaxy4Arms = generator->generateGalaxy();
    
    ASSERT_NE(galaxy2Arms, nullptr);
    ASSERT_NE(galaxy4Arms, nullptr);
    
    auto systems2Arms = galaxy2Arms->getAllStarSystems();
    auto systems4Arms = galaxy4Arms->getAllStarSystems();
    
    // Both should have similar number of systems
    EXPECT_NEAR(systems2Arms.size(), systems4Arms.size(), 10) 
        << "Different spiral arm counts should produce similar system counts";
    
    // The distributions should be different (we can't easily verify arm structure, but systems should be different)
    bool positionsDifferent = false;
    for (size_t i = 0; i < std::min(systems2Arms.size(), systems4Arms.size()); ++i) {
        auto pos2 = systems2Arms[i]->getPosition();
        auto pos4 = systems4Arms[i]->getPosition();
        if (std::abs(pos2.x - pos4.x) > 10 || std::abs(pos2.y - pos4.y) > 10) {
            positionsDifferent = true;
            break;
        }
    }
    EXPECT_TRUE(positionsDifferent) << "Different spiral arm counts should produce different distributions";
}

TEST_F(GalaxyParameterRespectTest, GalaxyShapeAffectsGeneration) {
    GenerationParameters params;
    params.systemCount = 50;
    params.width = 1000;
    params.height = 1000;
    params.seed = 12345;
    
    std::vector<GalaxyShape> shapes = {
        GalaxyShape::Spiral,
        GalaxyShape::Elliptical,
        GalaxyShape::Ring,
        GalaxyShape::Cluster
    };
    
    std::vector<std::unique_ptr<ggh::GalaxyCore::models::GalaxyModel>> galaxies;
    
    for (auto shape : shapes) {
        params.shape = shape;
        generator->setParameters(params);
        auto galaxy = generator->generateGalaxy();
        
        ASSERT_NE(galaxy, nullptr) << "Failed to generate galaxy for shape " << static_cast<int>(shape);
        
        auto systems = galaxy->getAllStarSystems();
        EXPECT_GE(systems.size(), params.systemCount * 0.8) 
            << "Shape " << static_cast<int>(shape) << " generated too few systems: " << systems.size();
        
        // Verify systems are within bounds
        for (const auto& system : systems) {
            auto pos = system->getPosition();
            EXPECT_GE(pos.x, 0) << "System X position out of bounds for shape " << static_cast<int>(shape);
            EXPECT_LT(pos.x, params.width) << "System X position out of bounds for shape " << static_cast<int>(shape);
            EXPECT_GE(pos.y, 0) << "System Y position out of bounds for shape " << static_cast<int>(shape);
            EXPECT_LT(pos.y, params.height) << "System Y position out of bounds for shape " << static_cast<int>(shape);
        }
        
        galaxies.push_back(std::move(galaxy));
    }
    
    // Verify that different shapes produce different distributions
    auto systems1 = galaxies[0]->getAllStarSystems();
    auto systems2 = galaxies[1]->getAllStarSystems();
    
    bool distributionsDifferent = false;
    for (size_t i = 0; i < std::min(systems1.size(), systems2.size()); ++i) {
        auto pos1 = systems1[i]->getPosition();
        auto pos2 = systems2[i]->getPosition();
        if (std::abs(pos1.x - pos2.x) > 50 || std::abs(pos1.y - pos2.y) > 50) {
            distributionsDifferent = true;
            break;
        }
    }
    EXPECT_TRUE(distributionsDifferent) << "Different galaxy shapes should produce different distributions";
}

TEST_F(GalaxyParameterRespectTest, GalaxySizeIsRespected) {
    std::vector<std::pair<int, int>> sizes = {
        {500, 500},
        {1000, 500},
        {1500, 1000},
        {2000, 1500}
    };
    
    for (auto [width, height] : sizes) {
        GenerationParameters params;
        params.systemCount = 50;
        params.width = width;
        params.height = height;
        params.shape = GalaxyShape::Spiral;
        params.seed = 12345;
        
        generator->setParameters(params);
        auto galaxy = generator->generateGalaxy();
        
        ASSERT_NE(galaxy, nullptr) << "Failed to generate galaxy for size " << width << "x" << height;
        EXPECT_EQ(galaxy->getWidth(), width) << "Galaxy width not set correctly";
        EXPECT_EQ(galaxy->getHeight(), height) << "Galaxy height not set correctly";
        
        auto systems = galaxy->getAllStarSystems();
        
        // Verify all systems are within the specified bounds
        for (const auto& system : systems) {
            auto pos = system->getPosition();
            EXPECT_GE(pos.x, 0) << "System X position negative";
            EXPECT_LT(pos.x, width) << "System X position exceeds galaxy width";
            EXPECT_GE(pos.y, 0) << "System Y position negative";  
            EXPECT_LT(pos.y, height) << "System Y position exceeds galaxy height";
        }
    }
}

TEST_F(GalaxyParameterRespectTest, SpiralTightnessAffectsDistribution) {
    GenerationParameters params;
    params.systemCount = 80;
    params.width = 1000;
    params.height = 1000;
    params.shape = GalaxyShape::Spiral;
    params.spiralArms = 2;
    params.seed = 12345;
    
    // Test loose spiral
    params.spiralTightness = 0.2;
    generator->setParameters(params);
    auto galaxyLoose = generator->generateGalaxy();
    
    // Test tight spiral
    params.spiralTightness = 1.5;
    generator->setParameters(params);
    auto galaxyTight = generator->generateGalaxy();
    
    ASSERT_NE(galaxyLoose, nullptr);
    ASSERT_NE(galaxyTight, nullptr);
    
    auto systemsLoose = galaxyLoose->getAllStarSystems();
    auto systemsTight = galaxyTight->getAllStarSystems();
    
    // Both should have similar system counts
    EXPECT_NEAR(systemsLoose.size(), systemsTight.size(), 10);
    
    // Distributions should be different
    bool positionsDifferent = false;
    for (size_t i = 0; i < std::min(systemsLoose.size(), systemsTight.size()); ++i) {
        auto posLoose = systemsLoose[i]->getPosition();
        auto posTight = systemsTight[i]->getPosition();
        if (std::abs(posLoose.x - posTight.x) > 20 || std::abs(posLoose.y - posTight.y) > 20) {
            positionsDifferent = true;
            break;
        }
    }
    EXPECT_TRUE(positionsDifferent) << "Different spiral tightness should produce different distributions";
}

TEST_F(GalaxyParameterRespectTest, CoreAndEdgeRadiusAffectDistribution) {
    GenerationParameters params;
    params.systemCount = 60;
    params.width = 1000;
    params.height = 1000;
    params.shape = GalaxyShape::Spiral;
    params.seed = 12345;
    
    // Test large core
    params.coreRadius = 0.5;
    params.edgeRadius = 0.8;
    generator->setParameters(params);
    auto galaxyLargeCore = generator->generateGalaxy();
    
    // Test small core
    params.coreRadius = 0.1;
    params.edgeRadius = 0.9;
    generator->setParameters(params);
    auto galaxySmallCore = generator->generateGalaxy();
    
    ASSERT_NE(galaxyLargeCore, nullptr);
    ASSERT_NE(galaxySmallCore, nullptr);
    
    auto systemsLargeCore = galaxyLargeCore->getAllStarSystems();
    auto systemsSmallCore = galaxySmallCore->getAllStarSystems();
    
    // Both should have similar system counts
    EXPECT_NEAR(systemsLargeCore.size(), systemsSmallCore.size(), 10);
    
    // Calculate average distance from center for each galaxy
    double centerX = params.width / 2.0;
    double centerY = params.height / 2.0;
    
    double avgDistLargeCore = 0.0;
    for (const auto& system : systemsLargeCore) {
        auto pos = system->getPosition();
        double dist = std::sqrt((pos.x - centerX) * (pos.x - centerX) + (pos.y - centerY) * (pos.y - centerY));
        avgDistLargeCore += dist;
    }
    avgDistLargeCore /= systemsLargeCore.size();
    
    double avgDistSmallCore = 0.0;
    for (const auto& system : systemsSmallCore) {
        auto pos = system->getPosition();
        double dist = std::sqrt((pos.x - centerX) * (pos.x - centerX) + (pos.y - centerY) * (pos.y - centerY));
        avgDistSmallCore += dist;
    }
    avgDistSmallCore /= systemsSmallCore.size();
    
    // Large core should have systems spread out more (larger minimum radius)
    // Small core should have systems more concentrated near center
    EXPECT_GT(avgDistLargeCore, avgDistSmallCore) 
        << "Large core galaxy should have systems spread out more than small core galaxy";
}

TEST_F(GalaxyParameterRespectTest, SeedProducesReproducibleResults) {
    GenerationParameters params;
    params.systemCount = 40;
    params.width = 1000;
    params.height = 1000;
    params.shape = GalaxyShape::Spiral;
    params.seed = 98765;
    
    // Generate first galaxy
    generator->setParameters(params);
    auto galaxy1 = generator->generateGalaxy();
    
    // Generate second galaxy with same seed
    auto generator2 = std::make_unique<GalaxyGenerator>(98765);
    generator2->setParameters(params);
    auto galaxy2 = generator2->generateGalaxy();
    
    ASSERT_NE(galaxy1, nullptr);
    ASSERT_NE(galaxy2, nullptr);
    
    auto systems1 = galaxy1->getAllStarSystems();
    auto systems2 = galaxy2->getAllStarSystems();
    
    // Should have same number of systems
    EXPECT_EQ(systems1.size(), systems2.size()) << "Same seed should produce same number of systems";
    
    // Systems should be in same positions
    for (size_t i = 0; i < std::min(systems1.size(), systems2.size()); ++i) {
        auto pos1 = systems1[i]->getPosition();
        auto pos2 = systems2[i]->getPosition();
        EXPECT_NEAR(pos1.x, pos2.x, 0.1) << "System " << i << " X position should be identical with same seed";
        EXPECT_NEAR(pos1.y, pos2.y, 0.1) << "System " << i << " Y position should be identical with same seed";
    }
}

TEST_F(GalaxyParameterRespectTest, TravelLanesAreGenerated) {
    GenerationParameters params;
    params.systemCount = 30;
    params.width = 1000;
    params.height = 1000;
    params.shape = GalaxyShape::Spiral;
    params.seed = 12345;
    
    generator->setParameters(params);
    auto galaxy = generator->generateGalaxy();
    
    ASSERT_NE(galaxy, nullptr);
    
    auto systems = galaxy->getAllStarSystems();
    auto lanes = galaxy->getAllTravelLanes();
    
    EXPECT_GT(systems.size(), 0) << "Should generate some systems";
    
    if (systems.size() > 1) {
        EXPECT_GT(lanes.size(), 0) << "Should generate travel lanes when there are multiple systems";
        
        // Verify all travel lanes connect valid systems
        for (const auto& lane : lanes) {
            auto fromSystem = lane->getFromSystem();
            auto toSystem = lane->getToSystem();
            
            EXPECT_NE(fromSystem, nullptr) << "Travel lane has invalid from system";
            EXPECT_NE(toSystem, nullptr) << "Travel lane has invalid to system";
            EXPECT_NE(fromSystem, toSystem) << "Travel lane connects system to itself";
            
            if (fromSystem && toSystem) {
                auto fromId = fromSystem->getId();
                auto toId = toSystem->getId();
                
                bool fromFound = false, toFound = false;
                for (const auto& system : systems) {
                    if (system->getId() == fromId) fromFound = true;
                    if (system->getId() == toId) toFound = true;
                }
                
                EXPECT_TRUE(fromFound) << "Travel lane references non-existent from system " << fromId;
                EXPECT_TRUE(toFound) << "Travel lane references non-existent to system " << toId;
            }
        }
    }
}

TEST_F(GalaxyParameterRespectTest, SpiralShapeIsWellFormed) {
    GenerationParameters params;
    params.systemCount = 60;
    params.width = 1000;
    params.height = 1000;
    params.shape = GalaxyShape::Spiral;
    params.spiralArms = 4;
    params.spiralTightness = 1.0;
    params.coreRadius = 0.2;
    params.edgeRadius = 0.8;
    params.seed = 12345;
    
    generator->setParameters(params);
    auto galaxy = generator->generateGalaxy();
    
    ASSERT_NE(galaxy, nullptr);
    
    auto systems = galaxy->getAllStarSystems();
    EXPECT_GE(systems.size(), params.systemCount * 0.9);
    
    // Verify systems form distinct spiral arms
    double centerX = params.width / 2.0;
    double centerY = params.height / 2.0;
    
    // Group systems by their approximate arm angle
    std::vector<std::vector<int>> armGroups(params.spiralArms);
    
    for (size_t i = 0; i < systems.size(); ++i) {
        auto pos = systems[i]->getPosition();
        double angle = std::atan2(pos.y - centerY, pos.x - centerX);
        if (angle < 0) angle += 2.0 * ggh::GalaxyCore::utilities::PI;
        
        // Determine which arm this system likely belongs to
        int closestArm = 0;
        double minAngleDiff = std::numeric_limits<double>::max();
        
        for (int arm = 0; arm < static_cast<int>(params.spiralArms); ++arm) {
            double armBaseAngle = (2.0 * ggh::GalaxyCore::utilities::PI * arm) / params.spiralArms;
            double angleDiff = std::abs(angle - armBaseAngle);
            if (angleDiff > ggh::GalaxyCore::utilities::PI) {
                angleDiff = 2.0 * ggh::GalaxyCore::utilities::PI - angleDiff;
            }
            
            if (angleDiff < minAngleDiff) {
                minAngleDiff = angleDiff;
                closestArm = arm;
            }
        }
        
        armGroups[closestArm].push_back(i);
    }
    
    // Each arm should have some systems
    for (int arm = 0; arm < static_cast<int>(params.spiralArms); ++arm) {
        EXPECT_GT(armGroups[arm].size(), 0) 
            << "Spiral arm " << arm << " should have some systems";
    }
    
    // Systems should be distributed across different radii (not all clumped at center or edge)
    std::vector<double> radii;
    for (const auto& system : systems) {
        auto pos = system->getPosition();
        double radius = std::sqrt((pos.x - centerX) * (pos.x - centerX) + (pos.y - centerY) * (pos.y - centerY));
        radii.push_back(radius);
    }
    
    std::sort(radii.begin(), radii.end());
    double minRadius = radii.front();
    double maxRadius = radii.back();
    double radiusRange = maxRadius - minRadius;
    
    EXPECT_GT(radiusRange, 100.0) << "Systems should be distributed across a reasonable radius range";
    
    // Check that systems are roughly distributed along radius (not all at center or edge)
    double medianRadius = radii[radii.size() / 2];
    double expectedMidRadius = minRadius + radiusRange / 2.0;
    EXPECT_NEAR(medianRadius, expectedMidRadius, radiusRange * 0.3) 
        << "Median system radius should be reasonably close to middle of range";
}
