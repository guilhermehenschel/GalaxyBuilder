#include "GalaxyGenerator.h"
#include <cmath>
#include <algorithm>
#include <chrono>

GalaxyGenerator::GalaxyGenerator(std::uint32_t seed)
    : m_rng(seed == 0 ? std::chrono::steady_clock::now().time_since_epoch().count() : seed),
      m_realDist(0.0, 1.0), m_intDist(0, 100) {
}

std::unique_ptr<Galaxy> GalaxyGenerator::generateGalaxy(const GenerationParameters& params) {
    auto galaxy = std::make_unique<Galaxy>(params.width, params.height, nullptr);
    galaxy->setShape(params.shape);
    
    generateSystems(*galaxy, params);
    generateTravelLanes(*galaxy, params);
    
    return galaxy;
}

void GalaxyGenerator::generateSystems(Galaxy& galaxy, const GenerationParameters& params) {
    galaxy.clear();
    
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

void GalaxyGenerator::generateTravelLanes(Galaxy& galaxy, const GenerationParameters& params) {
    connectNearestSystems(galaxy, params);
}

void GalaxyGenerator::setSeed(std::uint32_t seed) {
    m_rng.seed(seed == 0 ? std::chrono::steady_clock::now().time_since_epoch().count() : seed);
}

void GalaxyGenerator::generateSpiralGalaxy(Galaxy& galaxy, const GenerationParameters& params) {
    const double centerX = params.width / 2.0;
    const double centerY = params.height / 2.0;
    const double maxRadius = std::min(params.width, params.height) / 2.0 * params.edgeRadius;
    
    int systemsPerArm = params.systemCount / static_cast<int>(params.spiralArms);
    int remainingSystems = params.systemCount % static_cast<int>(params.spiralArms);
    
    for (int arm = 0; arm < static_cast<int>(params.spiralArms); ++arm) {
        int systemsInThisArm = systemsPerArm + (arm < remainingSystems ? 1 : 0);
        double armOffset = (2.0 * PI * arm) / params.spiralArms;
        
        for (int i = 0; i < systemsInThisArm; ++i) {
            double t = static_cast<double>(i) / systemsInThisArm;
            double radius = params.coreRadius * maxRadius + 
                           (maxRadius - params.coreRadius * maxRadius) * t;
            double angle = armOffset + t * params.spiralTightness * 4.0 * PI;
            
            QPointF pos = generateSpiralPosition(angle, radius, armOffset, params);
            pos.setX(pos.x() + centerX);
            pos.setY(pos.y() + centerY);
            
            if (isValidSystemPosition(galaxy, pos)) {
                galaxy.addSystem(pos, generateRandomStarType());
            }
        }
    }
}

void GalaxyGenerator::generateEllipticalGalaxy(Galaxy& galaxy, const GenerationParameters& params) {
    const double centerX = params.width / 2.0;
    const double centerY = params.height / 2.0;
    const double maxRadiusX = params.width / 2.0 * params.edgeRadius;
    const double maxRadiusY = params.height / 2.0 * params.edgeRadius * 0.6; // Make it elliptical
    
    for (GalaxySize i = 0; i < params.systemCount; ++i) {
        double angle = m_realDist(m_rng) * 2.0 * PI;
        double r = std::sqrt(m_realDist(m_rng)); // Square root for uniform distribution
        
        double x = centerX + r * maxRadiusX * std::cos(angle);
        double y = centerY + r * maxRadiusY * std::sin(angle);
        
        QPointF pos(x, y);
        if (isValidSystemPosition(galaxy, pos)) {
            galaxy.addSystem(pos, generateRandomStarType());
        }
    }
}

void GalaxyGenerator::generateRingGalaxy(Galaxy& galaxy, const GenerationParameters& params) {
    const double centerX = params.width / 2.0;
    const double centerY = params.height / 2.0;
    const double innerRadius = std::min(params.width, params.height) / 2.0 * 0.3;
    const double outerRadius = std::min(params.width, params.height) / 2.0 * params.edgeRadius;
    
    for (GalaxySize i = 0; i < params.systemCount; ++i) {
        double angle = m_realDist(m_rng) * 2.0 * PI;
        double r = innerRadius + (outerRadius - innerRadius) * m_realDist(m_rng);
        
        double x = centerX + r * std::cos(angle);
        double y = centerY + r * std::sin(angle);
        
        QPointF pos(x, y);
        if (isValidSystemPosition(galaxy, pos)) {
            galaxy.addSystem(pos, generateRandomStarType());
        }
    }
}

void GalaxyGenerator::generateClusterGalaxy(Galaxy& galaxy, const GenerationParameters& params) {
    const int numClusters = 3 + m_intDist(m_rng) % 4; // 3-6 clusters
    const int systemsPerCluster = params.systemCount / numClusters;
    
    for (int cluster = 0; cluster < numClusters; ++cluster) {
        double clusterX = m_realDist(m_rng) * params.width;
        double clusterY = m_realDist(m_rng) * params.height;
        double clusterRadius = 50 + m_realDist(m_rng) * 100;
        
        for (int i = 0; i < systemsPerCluster; ++i) {
            double angle = m_realDist(m_rng) * 2.0 * PI;
            double r = m_realDist(m_rng) * clusterRadius;
            
            double x = clusterX + r * std::cos(angle);
            double y = clusterY + r * std::sin(angle);
            
            QPointF pos(x, y);
            if (pos.x() >= 0 && pos.x() < params.width && 
                pos.y() >= 0 && pos.y() < params.height &&
                isValidSystemPosition(galaxy, pos)) {
                galaxy.addSystem(pos, generateRandomStarType());
            }
        }
    }
}

QPointF GalaxyGenerator::generateSpiralPosition(double angle, double radius, double armOffset,
                                                 const GenerationParameters& params) {
    Q_UNUSED(armOffset)
    Q_UNUSED(params)
    
    // Add some randomness to make it look more natural
    double randomAngle = angle + (m_realDist(m_rng) - 0.5) * 0.5;
    double randomRadius = radius + (m_realDist(m_rng) - 0.5) * 30.0;
    
    double x = randomRadius * std::cos(randomAngle);
    double y = randomRadius * std::sin(randomAngle);
    
    return QPointF(x, y);
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

bool GalaxyGenerator::isValidSystemPosition(const Galaxy& galaxy, const QPointF& position) {
    for (const auto& system : galaxy.getSystems()) {
        if (system->distanceTo(StarSystem(0, position)) < MIN_SYSTEM_DISTANCE) {
            return false;
        }
    }
    return true;
}

void GalaxyGenerator::connectNearestSystems(Galaxy& galaxy, const GenerationParameters& params) {
    Q_UNUSED(params) // Parameters might be used for future enhancements
    
    const auto& systems = galaxy.getSystems();
    
    for (std::size_t i = 0; i < systems.size(); ++i) {
        std::vector<std::pair<double, std::size_t>> distances;
        
        // Calculate distances to all other systems
        for (std::size_t j = 0; j < systems.size(); ++j) {
            if (i != j) {
                double distance = systems[i]->distanceTo(*systems[j]);
                if (distance <= MAX_TRAVEL_LANE_DISTANCE) {
                    distances.emplace_back(distance, j);
                }
            }
        }
        
        // Sort by distance and connect to 2-4 nearest systems
        std::sort(distances.begin(), distances.end());
        int connectionsToMake = std::min(static_cast<int>(distances.size()), 2 + m_intDist(m_rng) % 3);
        
        for (int k = 0; k < connectionsToMake; ++k) {
            galaxy.addTravelLane(systems[i]->getId(), systems[distances[k].second]->getId());
        }
    }
}
