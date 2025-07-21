#ifndef GGH_MODULES_GALAXYFACTIONS_SYSTEM_H
#define GGH_MODULES_GALAXYFACTIONS_SYSTEM_H

#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyFactions/models/ResourceType.h"
#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"
#include "ggh/modules/GalaxyFactions/models/SystemNotes.h"
#include "ggh/modules/GalaxyFactions/models/FactionHandover.h"
#include <string>
#include <vector>
#include <memory>

namespace ggh::Galaxy::Factions::models {

class System
{
public:
    System(std::shared_ptr<GalaxyCore::models::StarSystemModel> starSystemModel);

    // Getters
    int id() const;
    std::string name() const;
    std::string description() const;
    std::vector<SystemResourceBonus> resourceBonuses() const;
    std::vector<SystemNotes> notes() const;

    // Setters
    void setId(int id);
    void setName(const std::string& name);
    void setDescription(const std::string& description);

    // Resource management
    void addResourceBonus(const SystemResourceBonus& bonus);
    void addResourceBonus(const ResourceType& resourceType, int bonusAmount);
    void removeResourceBonus(const ResourceType& resourceType);
    void addNote(const SystemNotes& note);
    void removeNote(int noteId);

    // Business methods
    std::vector<SystemResourceBonus> getResourceBonuses() const;
    std::vector<SystemNotes> getNotes() const;
    int getResourceBonusByType(int resourceTypeId) const;
    std::vector<FactionHandover> getHandoverHistory() const;

private:
    std::shared_ptr<GalaxyCore::models::StarSystemModel> m_starSystemModel;
    std::vector<SystemResourceBonus> m_resourceBonuses;
    std::vector<SystemNotes> m_notes;
    std::vector<FactionHandover> m_handoverHistory;
};

} // namespace ggh::Galaxy::Factions


#endif // !GGH_MODULES_GALAXYFACTIONS_SYSTEM_H
