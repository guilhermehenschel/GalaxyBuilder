#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"

namespace ggh::GalaxyCore::models
{
    StarSystemModel::StarSystemModel(SystemId id, std::string_view name, const utilities::CartesianCoordinates<double> &position, StarType type)
        : m_id(id), m_name(name), m_position(position), m_starType(type), m_systemSize(SystemSize::Medium)
    {
    }

    void StarSystemModel::setPosition(const utilities::CartesianCoordinates<double> &position)
    {
        m_position = position;
    }
    void StarSystemModel::setStarType(StarType type)
    {
        m_starType = type;
    }
    void StarSystemModel::setSystemSize(SystemSize size)
    {
        m_systemSize = size;
    }
    void StarSystemModel::setName(const std::string &name)
    {
        m_name = name;
    }

    void StarSystemModel::addPlanet(Planet&& planet)
    {
        m_planets.push_back(std::make_shared<Planet>(std::move(planet)));
    }

    bool StarSystemModel::removePlanet(std::shared_ptr<Planet> planet)
    {
        auto it = std::remove_if(m_planets.begin(), m_planets.end(),
                                 [&planet](std::shared_ptr<Planet> p) { return p->name() == planet->name(); });
        if (it != m_planets.end()) {
            m_planets.erase(it, m_planets.end());
            return true;
        }
        return false;
    }

    std::string StarSystemModel::toXml() const
    {
        // Convert star system data to XML format
        std::string xml{std::format("<StarSystem id=\"{}\" name=\"{}\" positionX=\"{}\" positionY=\"{}\" starType=\"{}\" systemSize=\"{}\">",
                          m_id, m_name, m_position.x, m_position.y, static_cast<int>(m_starType), static_cast<int>(m_systemSize))};
        for (auto &planet : m_planets)
        {
            xml += planet->toXml();
        }
        xml += "</StarSystem>";
        return xml;
    }

    SystemId StarSystemModel::getId() const noexcept { return m_id; }
    const utilities::CartesianCoordinates<double> &StarSystemModel::getPosition() const noexcept { return m_position; }
    StarType StarSystemModel::getStarType() const noexcept { return m_starType; }
    SystemSize StarSystemModel::getSystemSize() const noexcept { return m_systemSize; }
    const std::string &StarSystemModel::getName() const noexcept { return m_name; }
    const std::vector<std::shared_ptr<Planet>> &StarSystemModel::getPlanets() const noexcept { return m_planets; }
} // namespace ggh::GalaxyCore::models
