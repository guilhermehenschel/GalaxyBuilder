/**
 * @file Faction.cpp
 * @brief Implementation of the Faction class for GalaxyFactions module
 */

#include "ggh/modules/GalaxyFactions/models/Faction.h"
#include <algorithm>
#include <sstream>

namespace ggh::Galaxy::Factions::models {

/**
 * @brief Constructor for Faction with all parameters
 */
/**
 * @brief Constructs a Faction with specified parameters
 * @param id The faction ID
 * @param name The faction name
 * @param description The faction description
 * @param color The faction color
 */
Faction::Faction(int id, const std::string& name, const std::string& description, const std::string& color)
    : m_id(id), m_name(name), m_description(description), m_color(color)
{
}

/**
 * @brief Sets the faction ID
 * @param id The new faction ID
 */
void Faction::setId(int id)
{
    m_id = id;
}

/**
 * @brief Sets the faction name
 * @param name The new faction name
 */
void Faction::setName(const std::string& name)
{
    m_name = name;
}

/**
 * @brief Sets the faction description
 * @param description The new faction description
 */
void Faction::setDescription(const std::string& description)
{
    m_description = description;
}

/**
 * @brief Sets the faction color
 * @param color The new faction color
 */
void Faction::setColor(const std::string& color)
{
    m_color = color;
}

/**
 * @brief Adds a system to the faction
 * @param system Shared pointer to the system to add
 */
void Faction::addSystem(std::shared_ptr<System> system)
{
    m_systems.push_back(system);
}

/**
 * @brief Removes a system from the faction by ID
 * @param systemId The ID of the system to remove
 */
void Faction::removeSystem(int systemId)
{
    m_systems.erase(
        std::remove_if(m_systems.begin(), m_systems.end(),
            [systemId](const std::shared_ptr<System>& s) { return s->id() == systemId; }),
        m_systems.end());
}

/**
 * @brief Gets all systems controlled by this faction
 * @return Vector of system shared pointers
 */
std::vector<std::shared_ptr<System>> Faction::getSystems() const
{
    return m_systems;
}

/**
 * @brief Gets detailed information about the faction
 * @return String containing faction details
 */
std::string Faction::getFactionDetails() const
{
    std::ostringstream details;
    details << "Faction: " << m_name << "\n";
    details << "Description: " << m_description << "\n";
    details << "Color: " << m_color << "\n";
    details << "Systems count: " << m_systems.size() << "\n";
    return details.str();
}

/**
 * @brief Gets all resource bonuses from all systems in the faction
 * @return Vector of system resource bonuses
 */
std::vector<SystemResourceBonus> Faction::getResourceBonuses() const
{
    std::vector<SystemResourceBonus> allBonuses;
    for (const auto& system : m_systems) {
        auto systemBonuses = system->resourceBonuses();
        allBonuses.insert(allBonuses.end(), systemBonuses.begin(), systemBonuses.end());
    }
    return allBonuses;
}

/**
 * @brief Gets all notes from all systems in the faction
 * @return Vector of system notes
 */
std::vector<SystemNotes> Faction::getNotes() const
{
    std::vector<SystemNotes> allNotes;
    for (const auto& system : m_systems) {
        auto systemNotes = system->notes();
        allNotes.insert(allNotes.end(), systemNotes.begin(), systemNotes.end());
    }
    return allNotes;
}

/**
 * @brief Gets total resource bonuses by resource type
 * @param resourceTypeId The resource type ID
 * @return Total bonus amount for the resource type
 */
int Faction::getResourceBonusesByType(int resourceTypeId) const
{
    int totalBonus = 0;
    for (const auto& system : m_systems) {
        totalBonus += system->getResourceBonusByType(resourceTypeId);
    }
    return totalBonus;
}

/**
 * @brief Gets resource stock across all faction systems
 * @return Map of resource type to quantity
 */
std::unordered_map<ResourceType, int> Faction::getResourcesStock() const
{
    std::unordered_map<ResourceType, int> stock;
    // Implementation would need to aggregate resources from all systems
    // This is a placeholder implementation
    return stock;
}

/**
 * @brief Converts faction to XML representation
 * @return XML string representation of the faction
 */
std::string Faction::toXml() const
{
    std::ostringstream xml;
    xml << "<" << xmlTag() << " id=\"" << m_id << "\">\n";
    xml << "  <name>" << m_name << "</name>\n";
    xml << "  <description>" << m_description << "</description>\n";
    xml << "  <color>" << m_color << "</color>\n";
    xml << "  <systems>\n";
    for (const auto& system : m_systems) {
        xml << "    <system id=\"" << system->id() << "\" name=\"" << system->name() << "\"/>\n";
    }
    xml << "  </systems>\n";
    xml << "</" << xmlTag() << ">\n";
    return xml.str();
}

} // namespace GalaxyFactions
