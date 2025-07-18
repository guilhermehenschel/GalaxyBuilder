#include "ggh/modules/GalaxyFactories/GalaxyGenerator.h"
#include <cmath>
#include <algorithm>
#include <chrono>

using namespace ggh::GalaxyFactories;
using GalaxyModel = ggh::GalaxyCore::models::GalaxyModel;
using StarSystemModel = ggh::GalaxyCore::models::StarSystemModel;
using CartesianCoordinates = ggh::GalaxyCore::utilities::CartesianCoordinates<double>;

namespace ggh::GalaxyFactories {
GalaxyGenerator::GalaxyGenerator(std::uint32_t seed)
    : m_rng(seed == 0 ? std::chrono::steady_clock::now().time_since_epoch().count() : seed),
      m_realDist(0.0, 1.0), m_intDist(0, 100) {
}

std::unique_ptr<GalaxyModel> GalaxyGenerator::generateGalaxy() {
    return generateGalaxy(m_params);
}

void GalaxyGenerator::setParameters(const GenerationParameters& params) {
    m_params = params;
    setSeed(params.seed);
}

GenerationParameters GalaxyGenerator::getParameters() const {
    return m_params;
}

std::unique_ptr<GalaxyModel> GalaxyGenerator::generateGalaxy(const GenerationParameters& params) {
    auto galaxy = std::make_unique<GalaxyModel>(params.width, params.height);
    // Note: GalaxyModel doesn't have setShape method, shape is handled by generation algorithm
    
    generateSystems(*galaxy, params);
    generateTravelLanes(*galaxy, params);
    
    return galaxy;
} 

void GalaxyGenerator::generateSystems(GalaxyModel& galaxy, const GenerationParameters& params) {
    // Note: GalaxyModel doesn't have clear method - it starts empty
    
    switch (params.shape) {
        case GalaxyShape::Spiral:
            generateSpiralGalaxy(galaxy, params);
            break;
        case GalaxyShape::Elliptical:
            generateEllipticalGalaxy(galaxy, params);
            break;
        case GalaxyShape::Ring:
            generateRingGalaxy(galaxy, params);
            break;
        case GalaxyShape::Cluster:
            generateClusterGalaxy(galaxy, params);
            break;
    }
}

void GalaxyGenerator::generateTravelLanes(GalaxyModel& galaxy, const GenerationParameters& params) {
    connectNearestSystems(galaxy, params);
}

void GalaxyGenerator::setSeed(std::uint32_t seed) {
    m_rng.seed(seed == 0 ? std::chrono::steady_clock::now().time_since_epoch().count() : seed);
}

void GalaxyGenerator::generateSpiralGalaxy(GalaxyModel& galaxy, const GenerationParameters& params) {
    const double centerX = params.width / 2.0;
    const double centerY = params.height / 2.0;
    const double maxRadius = std::min(params.width, params.height) / 2.0 * params.edgeRadius;
    const double minRadius = maxRadius * params.coreRadius;
    
    SystemId systemId = 1;
    int systemsGenerated = 0;
    int maxAttempts = static_cast<int>(params.systemCount) * 10;
    int attempts = 0;
    
    // Calculate systems per arm for even distribution
    int systemsPerArm = params.systemCount / static_cast<int>(params.spiralArms);
    int remainingSystems = params.systemCount % static_cast<int>(params.spiralArms);
    
    for (int arm = 0; arm < static_cast<int>(params.spiralArms) && systemsGenerated < static_cast<int>(params.systemCount); ++arm) {
        int systemsInThisArm = systemsPerArm + (arm < remainingSystems ? 1 : 0);
        
        for (int systemInArm = 0; systemInArm < systemsInThisArm && attempts < maxAttempts; ++systemInArm) {
            // Progress along this arm (0.0 = center, 1.0 = edge)
            double armProgress = static_cast<double>(systemInArm) / static_cast<double>(systemsInThisArm - 1);
            if (systemsInThisArm == 1) armProgress = 0.5; // Single system goes in middle
            
            // Calculate radius from center (logarithmic distribution looks more natural)
            double radius = minRadius + (maxRadius - minRadius) * std::pow(armProgress, 0.8);
            
            // Calculate angle - logarithmic spiral formula
            double baseAngle = (2.0 * GalaxyCore::utilities::PI * arm) / params.spiralArms;
            double spiralAngle = baseAngle + armProgress * params.spiralTightness * 2.0 * GalaxyCore::utilities::PI;
            
            // Add small random offset for natural look (much less than before)
            double randomAngleOffset = (m_realDist(m_rng) - 0.5) * 0.2;
            double randomRadiusOffset = (m_realDist(m_rng) - 0.5) * (maxRadius * 0.05);
            
            double finalAngle = spiralAngle + randomAngleOffset;
            double finalRadius = radius + randomRadiusOffset;
            
            // Convert to Cartesian coordinates
            double x = centerX + finalRadius * std::cos(finalAngle);
            double y = centerY + finalRadius * std::sin(finalAngle);
            
            CartesianCoordinates pos(x, y);
            
            // Check bounds and validity
            if (pos.x >= 0 && pos.x < params.width && pos.y >= 0 && pos.y < params.height) {
                if (isValidSystemPosition(galaxy, pos) || attempts > static_cast<int>(params.systemCount) * 3) {
                    std::string systemName = "System_" + std::to_string(systemId);
                    galaxy.addStarSystem(systemId++, systemName, pos, generateRandomStarType());
                    systemsGenerated++;
                } else {
                    systemInArm--; // Try this position again
                }
            } else {
                systemInArm--; // Try this position again
            }
            attempts++;
        }
    }
}

void GalaxyGenerator::generateEllipticalGalaxy(GalaxyModel& galaxy, const GenerationParameters& params) {
    const double centerX = params.width / 2.0;
    const double centerY = params.height / 2.0;
    const double maxRadiusX = params.width / 2.0 * params.edgeRadius;
    const double maxRadiusY = params.height / 2.0 * params.edgeRadius * 0.6; // Make it elliptical
    
    SystemId systemId = 1;
    int systemsGenerated = 0;
    int maxAttempts = static_cast<int>(params.systemCount) * 10; // Maximum attempts to avoid infinite loops
    int attempts = 0;
    
    while (systemsGenerated < static_cast<int>(params.systemCount) && attempts < maxAttempts) {
        double angle = m_realDist(m_rng) * 2.0 * GalaxyCore::utilities::PI;
        double r = std::sqrt(m_realDist(m_rng)); // Square root for uniform distribution
        
        double x = centerX + r * maxRadiusX * std::cos(angle);
        double y = centerY + r * maxRadiusY * std::sin(angle);
        
        CartesianCoordinates pos(x, y);
        
        // Check bounds
        if (pos.x >= 0 && pos.x < params.width && pos.y >= 0 && pos.y < params.height) {
            if (isValidSystemPosition(galaxy, pos) || attempts > static_cast<int>(params.systemCount) * 5) {
                std::string systemName = "System_" + std::to_string(systemId);
                galaxy.addStarSystem(systemId++, systemName, pos, generateRandomStarType());
                systemsGenerated++;
            }
        }
        attempts++;
    }
}

void GalaxyGenerator::generateRingGalaxy(GalaxyModel& galaxy, const GenerationParameters& params) {
    const double centerX = params.width / 2.0;
    const double centerY = params.height / 2.0;
    const double innerRadius = std::min(params.width, params.height) / 2.0 * 0.3;
    const double outerRadius = std::min(params.width, params.height) / 2.0 * params.edgeRadius;
    
    SystemId systemId = 1;
    int systemsGenerated = 0;
    int maxAttempts = static_cast<int>(params.systemCount) * 10; // Maximum attempts to avoid infinite loops
    int attempts = 0;
    
    while (systemsGenerated < static_cast<int>(params.systemCount) && attempts < maxAttempts) {
        double angle = m_realDist(m_rng) * 2.0 * GalaxyCore::utilities::PI;
        double r = innerRadius + (outerRadius - innerRadius) * m_realDist(m_rng);
        
        double x = centerX + r * std::cos(angle);
        double y = centerY + r * std::sin(angle);
        
        CartesianCoordinates pos(x, y);
        
        // Check bounds
        if (pos.x >= 0 && pos.x < params.width && pos.y >= 0 && pos.y < params.height) {
            if (isValidSystemPosition(galaxy, pos) || attempts > static_cast<int>(params.systemCount) * 5) {
                std::string systemName = "System_" + std::to_string(systemId);
                galaxy.addStarSystem(systemId++, systemName, pos, generateRandomStarType());
                systemsGenerated++;
            }
        }
        attempts++;
    }
}

void GalaxyGenerator::generateClusterGalaxy(GalaxyModel& galaxy, const GenerationParameters& params) {
    const int numClusters = 3 + m_intDist(m_rng) % 4; // 3-6 clusters
    const int systemsPerCluster = (static_cast<int>(params.systemCount) + numClusters - 1) / numClusters; // Round up division
    
    SystemId systemId = 1;
    int totalSystemsGenerated = 0;
    
    for (int cluster = 0; cluster < numClusters && totalSystemsGenerated < static_cast<int>(params.systemCount); ++cluster) {
        double clusterX = m_realDist(m_rng) * params.width;
        double clusterY = m_realDist(m_rng) * params.height;
        double clusterRadius = 50 + m_realDist(m_rng) * 100;
        
        int systemsInThisCluster = std::min(systemsPerCluster, static_cast<int>(params.systemCount) - totalSystemsGenerated);
        int systemsGenerated = 0;
        int maxAttempts = systemsInThisCluster * 10;
        int attempts = 0;
        
        while (systemsGenerated < systemsInThisCluster && attempts < maxAttempts) {
            double angle = m_realDist(m_rng) * 2.0 * GalaxyCore::utilities::PI;
            double r = m_realDist(m_rng) * clusterRadius;
            
            double x = clusterX + r * std::cos(angle);
            double y = clusterY + r * std::sin(angle);
            
            CartesianCoordinates pos(x, y);
            if (pos.x >= 0 && pos.x < params.width && 
                pos.y >= 0 && pos.y < params.height) {
                if (isValidSystemPosition(galaxy, pos) || attempts > systemsInThisCluster * 5) {
                    std::string systemName = "System_" + std::to_string(systemId);
                    galaxy.addStarSystem(systemId++, systemName, pos, generateRandomStarType());
                    systemsGenerated++;
                    totalSystemsGenerated++;
                }
            }
            attempts++;
        }
    }
}

CartesianCoordinates GalaxyGenerator::generateSpiralPosition(double angle, double radius, double armOffset,
                                                 const GenerationParameters& params) {
    Q_UNUSED(armOffset)
    Q_UNUSED(params)
    
    // Simple conversion from polar to Cartesian coordinates
    // Randomness is now handled in the main spiral generation function
    double x = radius * std::cos(angle);
    double y = radius * std::sin(angle);
    
    return CartesianCoordinates(x, y);
}

StarType GalaxyGenerator::generateRandomStarType() {
    int rand = m_intDist(m_rng);
    
    if (rand < 50) return StarType::RedDwarf;      // 50%
    else if (rand < 75) return StarType::YellowStar; // 25%
    else if (rand < 85) return StarType::BlueStar;   // 10%
    else if (rand < 92) return StarType::WhiteDwarf; // 7%
    else if (rand < 97) return StarType::RedGiant;   // 5%
    else if (rand < 99) return StarType::Neutron;    // 2%
    else return StarType::BlackHole;                  // 1%
}

bool GalaxyGenerator::isValidSystemPosition(const GalaxyModel& galaxy, const CartesianCoordinates& position) {
    auto systems = galaxy.getAllStarSystems();
    for (const auto& system : systems) {
        // Calculate distance between two points
        double dx = system->getPosition().x - position.x;
        double dy = system->getPosition().y - position.y;
        double distance = std::sqrt(dx * dx + dy * dy);
        
        if (distance < GalaxyCore::utilities::MIN_SYSTEM_DISTANCE) {
            return false;
        }
    }
    return true;
}

void GalaxyGenerator::connectNearestSystems(GalaxyModel& galaxy, const GenerationParameters& params) {
    Q_UNUSED(params) // Parameters might be used for future enhancements
    
    auto systems = galaxy.getAllStarSystems();
    LaneId laneId = 1;
    
    for (std::size_t i = 0; i < systems.size(); ++i) {
        std::vector<std::pair<double, std::size_t>> distances;
        
        // Calculate distances to all other systems
        for (std::size_t j = 0; j < systems.size(); ++j) {
            if (i != j) {
                // Calculate distance between two systems
                double dx = systems[i]->getPosition().x - systems[j]->getPosition().x;
                double dy = systems[i]->getPosition().y - systems[j]->getPosition().y;
                double distance = std::sqrt(dx * dx + dy * dy);
                
                if (distance <= GalaxyCore::utilities::MAX_TRAVEL_LANE_DISTANCE) {
                    distances.emplace_back(distance, j);
                }
            }
        }
        
        // Sort by distance and connect to 2-4 nearest systems
        std::sort(distances.begin(), distances.end());
        int connectionsToMake = std::min(static_cast<int>(distances.size()), 2 + m_intDist(m_rng) % 3);
        
        for (int k = 0; k < connectionsToMake; ++k) {
            galaxy.addTravelLane(laneId++, systems[i]->getId(), systems[distances[k].second]->getId());
        }
    }
}
}