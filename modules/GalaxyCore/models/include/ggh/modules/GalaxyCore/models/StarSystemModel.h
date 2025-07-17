#ifndef GGH_GALAXYCORE_MODELS_STAR_SYSTEM_MODEL_H
#define GGH_GALAXYCORE_MODELS_STAR_SYSTEM_MODEL_H

#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"

namespace ggh::GalaxyCore::models {
using SystemId = ggh::GalaxyCore::utilities::SystemId;
using StarType = ggh::GalaxyCore::utilities::StarType;
using SystemSize = ggh::GalaxyCore::utilities::SystemSize;
/**
 * @class StarSystemModel
 * @brief Represents a star system with properties such as position, type, size, and name.
 */
class StarSystemModel {
public:
    StarSystemModel(SystemId id, std::string_view name, const utilities::CartesianCoordinates<double>& position, StarType type = StarType::YellowStar);

    // Getters
    SystemId getId() const noexcept;
    const utilities::CartesianCoordinates<double>& getPosition() const noexcept;
    StarType getStarType() const noexcept;
    SystemSize getSystemSize() const noexcept;
    const std::string& getName() const noexcept;
    const std::vector<Planet>& getPlanets() const noexcept;

    std::string toXml() const;

    // Setters
    void setPosition(const utilities::CartesianCoordinates<double>& position);
    void setStarType(StarType type);
    void setSystemSize(SystemSize size);
    void setName(const std::string& name);

    // Planet management
    void addPlanet(const Planet& planet);
    bool removePlanet(const Planet& planet);

        /**
     * @brief Gets the XML tag name for the model.
     * @return A string containing the XML tag name.
     */
    static std::string xmlTag() noexcept {
        return "StarSystem";
    }

private:
    SystemId m_id;  // Unique identifier for the star system
    utilities::CartesianCoordinates<double> m_position;  // Position of the star system in Cartesian coordinates
    StarType m_starType;  // Type of the star in the system
    SystemSize m_systemSize;  // Size category of the star system
    std::string m_name;  // Name of the star system
    std::vector<Planet> m_planets;  // List of planets in the star system
};
}

#endif // !GGH_GALAXYCORE_MODELS_STAR_SYSTEM_MODEL_H