#pragma once

#include "Common.h"
#include "Galaxy.h"
#include <memory>
#include <random>

class GalaxyGenerator {
public:
    struct GenerationParameters {
        GalaxySize systemCount = 50;
        GalaxyShape shape = GalaxyShape::Spiral;
        int width = DEFAULT_GALAXY_WIDTH;
        int height = DEFAULT_GALAXY_HEIGHT;
        double spiralArms = 2.0;
        double spiralTightness = 0.5;
        double coreRadius = 0.2;
        double edgeRadius = 0.8;
        int seed = 0; // 0 for random seed
    };

    explicit GalaxyGenerator(std::uint32_t seed = 0);
    ~GalaxyGenerator() = default;

    // Generate a complete galaxy
    std::unique_ptr<Galaxy> generateGalaxy(const GenerationParameters& params);

    // Generate systems only
    void generateSystems(Galaxy& galaxy, const GenerationParameters& params);
    
    // Generate travel lanes based on existing systems
    void generateTravelLanes(Galaxy& galaxy, const GenerationParameters& params);

    // Setters
    void setSeed(std::uint32_t seed);

private:
    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_realDist;
    std::uniform_int_distribution<int> m_intDist;

    // Generation methods for different galaxy shapes
    void generateSpiralGalaxy(Galaxy& galaxy, const GenerationParameters& params);
    void generateEllipticalGalaxy(Galaxy& galaxy, const GenerationParameters& params);
    void generateRingGalaxy(Galaxy& galaxy, const GenerationParameters& params);
    void generateClusterGalaxy(Galaxy& galaxy, const GenerationParameters& params);

    // Utility methods
    QPointF generateSpiralPosition(double angle, double radius, double armOffset, 
                                   const GenerationParameters& params);
    StarType generateRandomStarType();
    bool isValidSystemPosition(const Galaxy& galaxy, const QPointF& position);
    void connectNearestSystems(Galaxy& galaxy, const GenerationParameters& params);
};
