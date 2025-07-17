#ifndef GGH_GALAXYFACTORIES_ABSTRACT_GALAXY_FACTORY_H
#define GGH_GALAXYFACTORIES_ABSTRACT_GALAXY_FACTORY_H

#include <memory>
#include "ggh/modules/GalaxyFactories/Types.h"
#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "galaxyfactories_global.h"

using GalaxyModel = ggh::GalaxyCore::models::GalaxyModel;

namespace ggh::GalaxyFactories {
/**
 * @class AbstractGalaxyFactory
 * @brief An abstract base class for creating galaxy factories.
 *
 * This class defines the interface for galaxy factories, which are responsible for generating
 * and managing galaxies in the GalaxyBuilder application.
 */
class GALAXYFACTORIES_EXPORT AbstractGalaxyFactory {
public:
    virtual ~AbstractGalaxyFactory() = default;

    /**
     * @brief Generates a galaxy based on the factory's parameters.
     * @return A unique pointer to the generated galaxy model.
     */
    virtual std::unique_ptr<GalaxyModel> generateGalaxy() = 0;

    /**
     * @brief Sets the parameters for galaxy generation.
     * @param params The parameters to use for generating the galaxy.
     */
    virtual void setParameters(const GenerationParameters& params) = 0;

    /**
     * @brief Gets the current parameters used by the factory.
     * @return The current galaxy parameters.
     */
    virtual GenerationParameters getParameters() const = 0;
};
}

#endif // !GGH_GALAXYFACTORIES_ABSTRACT_GALAXY_FACTORY_H