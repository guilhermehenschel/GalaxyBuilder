#ifndef GHH_GALAXYCORE_MODELS_PLANETMODEL_H
#define GHH_GALAXYCORE_MODELS_PLANETMODEL_H

#include "ggh/modules/GalaxyCore/utilities/Common.h"

namespace ggh::GalaxyCore::models {

using PlanetType = ggh::GalaxyCore::utilities::PlanetType;

class Planet {
public:
    Planet() = default;
    Planet(std::string_view name, PlanetType type, double size,
           double mass, int numberOfMoons, double orbitalRadius, double maxTemperature,
           double minTemperature);

    // Getters
    const std::string& name() const;
    PlanetType type() const;
    double size() const;
    double mass() const;
    int numberOfMoons() const;
    double orbitalRadius() const;
    double maxTemperature() const;
    double minTemperature() const ;

    // Setters
    void setName(const std::string& name);
    void setType(PlanetType type);
    void setSize(double size);
    void setMass(double mass);
    void setNumberOfMoons(int numberOfMoons);
    void setOrbitalRadius(double orbitalRadius);
    void setMaxTemperature(double maxTemperature);
    void setMinTemperature(double minTemperature);

    // Serialization methods
    std::string toXml() const;

    private:
    std::string m_name;
    PlanetType m_type;
    double m_size;
    double m_mass;
    int m_numberOfMoons;
    double m_orbitalRadius;
    double m_maxTemperature;
    double m_minTemperature;
};
}

#endif // !GHH_GALAXYCORE_MODELS_PLANETMODEL_H