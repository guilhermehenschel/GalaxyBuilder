#include "ggh/modules/GalaxyFactions/FactionHandover.h"

namespace ggh::Galaxy::Factions {

FactionHandover::FactionHandover(int id, int factionToId, int systemId, int factionFromId, 
                               int handoverTypeId, const GalaxyCore::models::GalacticDate& handoverDate, const std::string& notes)
    : m_id(id), m_factionToId(factionToId), m_systemId(systemId), m_factionFromId(factionFromId),
      m_handoverTypeId(handoverTypeId), m_handoverDate(handoverDate), m_notes(notes)
{
}

void FactionHandover::setId(int id)
{
    m_id = id;
}

void FactionHandover::setFactionToId(int factionToId)
{
    m_factionToId = factionToId;
}

void FactionHandover::setSystemId(int systemId)
{
    m_systemId = systemId;
}

void FactionHandover::setFactionFromId(int factionFromId)
{
    m_factionFromId = factionFromId;
}

void FactionHandover::setHandoverTypeId(int handoverTypeId)
{
    m_handoverTypeId = handoverTypeId;
}

void FactionHandover::setHandoverDate(const GalaxyCore::models::GalacticDate& handoverDate)
{
    m_handoverDate = handoverDate;
}

void FactionHandover::setNotes(const std::string& notes)
{
    m_notes = notes;
}

} // namespace GalaxyFactions
