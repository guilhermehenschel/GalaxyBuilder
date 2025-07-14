#include "ggh/modules/GalaxyCore/models/PlanetModel.h"

namespace ggh::GalaxyCore::models {

using PlanetType = ggh::GalaxyCore::utilities::PlanetType;

Planet::Planet(std::string_view name, PlanetType type, double size,
           double mass, int numberOfMoons, double orbitalRadius, double maxTemperature,
           double minTemperature)
    : m_name(name), m_type(type), m_size(size), m_mass(mass),
      m_numberOfMoons(numberOfMoons), m_orbitalRadius(orbitalRadius),
      m_maxTemperature(maxTemperature), m_minTemperature(minTemperature) {

}

const std::string& Planet::name() const { return m_name; }
PlanetType Planet::type() const { return m_type; }
double Planet::size() const { return m_size; }
double Planet::mass() const { return m_mass; }
int Planet::numberOfMoons() const { return m_numberOfMoons; }
double Planet::orbitalRadius() const { return m_orbitalRadius; }
double Planet::maxTemperature() const { return m_maxTemperature; }
double Planet::minTemperature() const { return m_minTemperature; }

void Planet::setName(const std::string& name) { m_name = name; }
void Planet::setType(PlanetType type) { m_type = type; }
void Planet::setSize(double size) { m_size = size; }
void Planet::setMass(double mass) { m_mass = mass; }
void Planet::setNumberOfMoons(int numberOfMoons) { m_numberOfMoons = numberOfMoons; }
void Planet::setOrbitalRadius(double orbitalRadius) { m_orbitalRadius = orbitalRadius; }
void Planet::setMaxTemperature(double maxTemperature) { m_maxTemperature = maxTemperature; }
void Planet::setMinTemperature(double minTemperature) { m_minTemperature = minTemperature; }

std::string Planet::toXml() const {
    // Convert planet data to XML format
    return "<Planet name=\"" + m_name + "\" type=\"" + std::to_string(static_cast<int>(m_type)) +
            "\" size=\"" + std::to_string(m_size) + "\" mass=\"" + std::to_string(m_mass) +
            "\" moonCount=\"" + std::to_string(m_numberOfMoons) +
            "\" orbitDistance=\"" + std::to_string(m_orbitalRadius) +
            "\" minTemperature=\"" + std::to_string(m_minTemperature) +
            "\" maxTemperature=\"" + std::to_string(m_maxTemperature) + "\" />";
}
}