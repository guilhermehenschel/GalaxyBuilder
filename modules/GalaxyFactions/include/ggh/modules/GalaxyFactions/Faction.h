#ifndef GGH_MODULES_GALAXYFACTIONS_FACTION_H
#define GGH_MODULES_GALAXYFACTIONS_FACTION_H

#include "ggh/modules/GalaxyFactions/System.h"
#include "ggh/modules/GalaxyFactions/SystemResourceBonus.h"
#include "ggh/modules/GalaxyFactions/SystemNotes.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace ggh::Galaxy::Factions {
class Faction
{
public:
    Faction(int id, const std::string& name, const std::string& description, const std::string& color);

    // Getters
    int id() const { return m_id; }
    std::string name() const { return m_name; }
    std::string description() const { return m_description; }
    std::string color() const { return m_color; }
    std::vector<std::shared_ptr<System>> systems() const { return m_systems; }

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setColor(const std::string& color);

    // System management
    void addSystem(std::shared_ptr<System> system);
    void removeSystem(int systemId);

    // Business methods
    std::vector<std::shared_ptr<System>> getSystems() const;
    std::string getFactionDetails() const;
    std::vector<SystemResourceBonus> getResourceBonuses() const;
    std::vector<SystemNotes> getNotes() const;
    int getResourceBonusesByType(int resourceTypeId) const;
    std::unordered_map<ResourceType, int> getResourcesStock() const;

    std::string toXml() const;
    static std::string xmlTag() noexcept {
        return "Faction";
    }

private:
    int m_id;
    std::string m_name;
    std::string m_description;
    std::string m_color;
    std::vector<std::shared_ptr<System>> m_systems;
    std::unordered_map<ResourceType, int> m_resourcesStock; // Maps resource type to its stock
};

} // namespace ggh::Galaxy::Factions

#endif // !GGH_MODULES_GALAXYFACTIONS_FACTION_H
