#include "ggh/modules/GalaxyFactions/models/Faction.h"

namespace ggh::Galaxy::Factions::models {

SystemNotes::SystemNotes()
    : m_id(0), m_systemId(0), m_note("")
{
}

SystemNotes::SystemNotes(int id, int systemId, const std::string& note)
    : m_id(id), m_systemId(systemId), m_note(note)
{
}

void SystemNotes::setId(int id)
{
    m_id = id;
}

void SystemNotes::setSystemId(int systemId)
{
    m_systemId = systemId;
}

void SystemNotes::setNote(const std::string& note)
{
    m_note = note;
}

} // namespace GalaxyFactions
