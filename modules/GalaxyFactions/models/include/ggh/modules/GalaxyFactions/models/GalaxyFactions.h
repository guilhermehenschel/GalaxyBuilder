#ifndef GGH_MODULES_GALAXYFACTIONS_GALAXY_FACTIONS_H
#define GGH_MODULES_GALAXYFACTIONS_GALAXY_FACTIONS_H

#include "ggh/modules/GalaxyFactions/models/Faction.h"
#include <string>
#include <vector>
#include <memory>

namespace ggh::Galaxy::Factions::models {

/**
 * @class GalaxyFactions
 * @brief Main manager class for creating, modifying, and managing galaxy factions
 */
class GalaxyFactions
{
public:
    GalaxyFactions();
    ~GalaxyFactions();

    // Faction management
    std::shared_ptr<Faction> createFaction(const std::string& name, const std::string& description, const std::string& color);
    std::shared_ptr<Faction> modifyFaction(int id, const std::string& name, const std::string& description, const std::string& color);
    bool deleteFaction(int id);
    std::shared_ptr<Faction> getFaction(int id) const;
    std::vector<std::shared_ptr<Faction>> getAllFactions() const;

    // Export functionality
    std::string toXml() const;
    static std::string xmlTag() noexcept {
        return "Factions";
    }

private:
    std::vector<std::shared_ptr<Faction>> m_factions;
    int m_nextFactionId;

    // Helper methods
    std::vector<std::shared_ptr<Faction>>::iterator findFaction(int id);
    std::vector<std::shared_ptr<Faction>>::const_iterator findFaction(int id) const;
};

} // namespace Factions
#endif // !GGH_MODULES_GALAXYFACTIONS_GALAXY_FACTIONS_H