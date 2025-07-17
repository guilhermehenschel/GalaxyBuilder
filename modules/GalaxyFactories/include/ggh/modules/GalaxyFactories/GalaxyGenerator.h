#ifndef GGH_GALAXYGENERATOR_GALAXYGENERATOR_H
#define GGH_GALAXYGENERATOR_GALAXYGENERATOR_H

#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "ggh/modules/GalaxyFactories/AbstractGalaxyFactory.h"
#include "ggh/modules/GalaxyFactories/Types.h"
#include "galaxyfactories_global.h"
#include <memory>
#include <random>

using GalaxySize = ggh::GalaxyCore::utilities::GalaxySize;
using GalaxyShape = ggh::GalaxyCore::utilities::GalaxyShape;
using StarType = ggh::GalaxyCore::utilities::StarType;
using SystemId = ggh::GalaxyCore::utilities::SystemId;
using LaneId = ggh::GalaxyCore::utilities::LaneId;
using GalaxyModel = ggh::GalaxyCore::models::GalaxyModel;

namespace ggh::GalaxyFactories {
class GALAXYFACTORIES_EXPORT GalaxyGenerator : public AbstractGalaxyFactory {
public:
    explicit GalaxyGenerator(std::uint32_t seed = 0);
    ~GalaxyGenerator() = default;

    // Generate a complete galaxy
    std::unique_ptr<GalaxyModel> generateGalaxy() override;

    void setParameters(const GenerationParameters& params) override;
    GenerationParameters getParameters() const override;

    // Setters
    void setSeed(std::uint32_t seed);

private:
    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_realDist;
    std::uniform_int_distribution<int> m_intDist;
    GenerationParameters m_params;

    // Generation methods for different galaxy shapes
    void generateSpiralGalaxy(GalaxyModel& galaxy, const GenerationParameters& params);
    void generateEllipticalGalaxy(GalaxyModel& galaxy, const GenerationParameters& params);
    void generateRingGalaxy(GalaxyModel& galaxy, const GenerationParameters& params);
    void generateClusterGalaxy(GalaxyModel& galaxy, const GenerationParameters& params);

    // Helper method for galaxy generation with parameters
    std::unique_ptr<GalaxyModel> generateGalaxy(const GenerationParameters& params);

    // Generate systems only
    void generateSystems(GalaxyModel& galaxy, const GenerationParameters& params);
    
    // Generate travel lanes based on existing systems
    void generateTravelLanes(GalaxyModel& galaxy, const GenerationParameters& params);

    // Utility methods
    ggh::GalaxyCore::utilities::CartesianCoordinates<double> generateSpiralPosition(double angle, double radius, double armOffset, 
                                   const GenerationParameters& params);
    StarType generateRandomStarType();
    bool isValidSystemPosition(const GalaxyModel& galaxy, const ggh::GalaxyCore::utilities::CartesianCoordinates<double>& position);
    void connectNearestSystems(GalaxyModel& galaxy, const GenerationParameters& params);
};
}
#endif // !GGH_GALAXYGENERATOR_GALAXYGENERATOR_H
