/**
 * @file GalaxyFactions.cpp
 * @brief Implementation of the GalaxyFactions main manager class
 */

#include "ggh/modules/GalaxyFactions/GalaxyFactions.h"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace ggh::Galaxy::Factions {

/**
 * @brief Default constructor for GalaxyFactions
 */
GalaxyFactions::GalaxyFactions()
    : m_nextFactionId(1)
{
}

/**
 * @brief Destructor for GalaxyFactions
 */
GalaxyFactions::~GalaxyFactions()
{
}

/**
 * @brief Creates a new faction
 * @param name The faction name
 * @param description The faction description
 * @param color The faction color
 * @return Shared pointer to the created faction
 */
std::shared_ptr<Faction> GalaxyFactions::createFaction(const std::string& name, const std::string& description, const std::string& color)
{
    auto faction = std::make_shared<Faction>(m_nextFactionId++, name, description, color);
    m_factions.push_back(faction);
    return faction;
}

/**
 * @brief Modifies an existing faction
 * @param id The faction ID to modify
 * @param name The new faction name
 * @param description The new faction description
 * @param color The new faction color
 * @return Shared pointer to the modified faction, or nullptr if not found
 */
std::shared_ptr<Faction> GalaxyFactions::modifyFaction(int id, const std::string& name, const std::string& description, const std::string& color)
{
    auto it = findFaction(id);
    if (it != m_factions.end()) {
        (*it)->setName(name);
        (*it)->setDescription(description);
        (*it)->setColor(color);
        return *it;
    }
    return nullptr;
}

/**
 * @brief Deletes a faction by ID
 * @param id The faction ID to delete
 * @return True if faction was deleted, false if not found
 */
bool GalaxyFactions::deleteFaction(int id)
{
    auto it = findFaction(id);
    if (it != m_factions.end()) {
        m_factions.erase(it);
        return true;
    }
    return false;
}

/**
 * @brief Gets a faction by ID
 * @param id The faction ID to retrieve
 * @return Shared pointer to the faction, or nullptr if not found
 */
std::shared_ptr<Faction> GalaxyFactions::getFaction(int id) const
{
    auto it = findFaction(id);
    return (it != m_factions.end()) ? *it : nullptr;
}

/**
 * @brief Gets all factions
 * @return Vector of all faction shared pointers
 */
std::vector<std::shared_ptr<Faction>> GalaxyFactions::getAllFactions() const
{
    return m_factions;
}

/**
 * @brief Helper method to find faction by ID (non-const version)
 * @param id The faction ID to find
 * @return Iterator to the faction, or end() if not found
 */
std::vector<std::shared_ptr<Faction>>::iterator GalaxyFactions::findFaction(int id)
{
    return std::find_if(m_factions.begin(), m_factions.end(),
        [id](const std::shared_ptr<Faction>& f) { return f->id() == id; });
}

/**
 * @brief Helper method to find faction by ID (const version)
 * @param id The faction ID to find
 * @return Const iterator to the faction, or end() if not found
 */
std::vector<std::shared_ptr<Faction>>::const_iterator GalaxyFactions::findFaction(int id) const
{
    return std::find_if(m_factions.begin(), m_factions.end(),
        [id](const std::shared_ptr<Faction>& f) { return f->id() == id; });
}

/**
 * @brief Generate XML representation of all factions
 * @return XML string containing all faction data
 */
std::string GalaxyFactions::toXml() const
{
    std::ostringstream xml;
    xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    xml << "<" << xmlTag() << ">\n";
    
    for (const auto& faction : m_factions) {
        xml << "  " << faction->toXml() << "\n";
    }
    
    xml << "</" << xmlTag() << ">\n";
    return xml.str();
}

} // namespace ggh::Galaxy::Factions

