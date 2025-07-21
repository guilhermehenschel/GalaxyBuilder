#include "ggh/modules/GalaxyFactions/models/ResourceType.h"

namespace ggh::Galaxy::Factions::models {

ResourceType::ResourceType()
    : m_id(0), m_name(""), m_description("")
{
}

ResourceType::ResourceType(int id, const std::string& name, const std::string& description)
    : m_id(id), m_name(name), m_description(description)
{
}

void ResourceType::setId(int id)
{
    m_id = id;
}

void ResourceType::setName(const std::string& name)
{
    m_name = name;
}

void ResourceType::setDescription(const std::string& description)
{
    m_description = description;
}

} // namespace GalaxyFactions
