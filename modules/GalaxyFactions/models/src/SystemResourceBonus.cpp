#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"

namespace ggh::Galaxy::Factions::models {

SystemResourceBonus::SystemResourceBonus()
    : m_resourceType{}, m_bonusAmount{0}
{
}

SystemResourceBonus::SystemResourceBonus(const ResourceType& resourceType, int bonusAmount)
    : m_resourceType{resourceType}, m_bonusAmount{bonusAmount}
{
}

int SystemResourceBonus::id() const
{
    return m_resourceType.id();
}

void SystemResourceBonus::setId(int id)
{
    // ID is managed by the ResourceType
    auto updatedResourceType = m_resourceType;
    updatedResourceType.setId(id);
    m_resourceType = updatedResourceType;
}

void SystemResourceBonus::setSystemId(int systemId)
{
    // System ID is not stored directly in this class
    // This method is kept for interface compatibility
}

void SystemResourceBonus::setResourceType(const ResourceType& resourceType)
{
    m_resourceType = resourceType;
}

void SystemResourceBonus::setBonusAmount(int bonusAmount)
{
    m_bonusAmount = bonusAmount;
}

} // namespace GalaxyFactions
