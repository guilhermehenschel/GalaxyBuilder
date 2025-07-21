#include "ggh/modules/GalaxyFactions/HandoverType.h"

namespace ggh::Galaxy::Factions {

HandoverType::HandoverType(int id, const std::string& name, const std::string& description)
    : m_id(id), m_name(name), m_description(description)
{
}

void HandoverType::setId(int id)
{
    m_id = id;
}

void HandoverType::setName(const std::string& name)
{
    m_name = name;
}

void HandoverType::setDescription(const std::string& description)
{
    m_description = description;
}

} // namespace GalaxyFactions
