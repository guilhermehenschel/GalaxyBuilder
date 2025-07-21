#ifndef GGH_MODULES_GALAXYFACTIONS_FACTION_HANDOVER_H
#define GGH_MODULES_GALAXYFACTIONS_FACTION_HANDOVER_H

#include "ggh/modules/GalaxyCore/models/GalacticDate.h"
#include <string>

namespace ggh::Galaxy::Factions {

class FactionHandover
{
public:
    FactionHandover(int id, int factionToId, int systemId, int factionFromId, 
                   int handoverTypeId, const GalaxyCore::models::GalacticDate& handoverDate, const std::string& notes);

    // Getters
    int id() const { return m_id; }
    int factionToId() const { return m_factionToId; }
    int systemId() const { return m_systemId; }
    int factionFromId() const { return m_factionFromId; }
    int handoverTypeId() const { return m_handoverTypeId; }
    GalaxyCore::models::GalacticDate handoverDate() const { return m_handoverDate; }
    std::string notes() const { return m_notes; }

    // Setters
    void setId(int id);
    void setFactionToId(int factionToId);
    void setSystemId(int systemId);
    void setFactionFromId(int factionFromId);
    void setHandoverTypeId(int handoverTypeId);
    void setHandoverDate(const GalaxyCore::models::GalacticDate& handoverDate);
    void setNotes(const std::string& notes);

private:
    int m_id;
    int m_factionToId;
    int m_systemId;
    int m_factionFromId;
    int m_handoverTypeId;
    GalaxyCore::models::GalacticDate m_handoverDate;
    std::string m_notes;
};

} // namespace ggh::Galaxy::Factions

#endif // GGH_MODULES_GALAXYFACTIONS_FACTION_HANDOVER_H