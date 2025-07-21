#ifndef GGH_MODULES_GALAXYFACTIONS_SYSTEM_RESOURCE_BONUS_H
#define GGH_MODULES_GALAXYFACTIONS_SYSTEM_RESOURCE_BONUS_H

#include "ggh/modules/GalaxyFactions/models/ResourceType.h"
#include <string>
#include <stdexcept>

namespace ggh::Galaxy::Factions::models {

class SystemResourceBonus
{
public:
    SystemResourceBonus();
    SystemResourceBonus(const ResourceType& resourceType, int bonusAmount);

    // Getters
    int id() const;
    ResourceType resourceType() const { return m_resourceType; }
    int bonusAmount() const { return m_bonusAmount; }

    // Setters
    void setId(int id);
    void setSystemId(int systemId);
    void setResourceType(const ResourceType& resourceType);
    void setBonusAmount(int bonusAmount);

    // Business methods
    std::string toXml() const;
    static std::string xmlTag() noexcept {
        return "SystemResourceBonus";
    }
    friend bool operator==(const SystemResourceBonus& lhs, const SystemResourceBonus& rhs) {
        return lhs.m_resourceType == rhs.m_resourceType && lhs.m_bonusAmount == rhs.m_bonusAmount;
    }

    friend bool operator!=(const SystemResourceBonus& lhs, const SystemResourceBonus& rhs) {
        return !(lhs == rhs);
    }

    // Overloaded operators
    friend bool operator<(const SystemResourceBonus& lhs, const SystemResourceBonus& rhs) {
        return lhs.m_bonusAmount < rhs.m_bonusAmount;
    }
    friend bool operator>(const SystemResourceBonus& lhs, const SystemResourceBonus& rhs) {
        return lhs.m_bonusAmount > rhs.m_bonusAmount;
    }
    friend bool operator<=(const SystemResourceBonus& lhs, const SystemResourceBonus& rhs) {
        return lhs.m_bonusAmount <= rhs.m_bonusAmount;
    }
    friend bool operator>=(const SystemResourceBonus& lhs, const SystemResourceBonus& rhs) {
        return lhs.m_bonusAmount >= rhs.m_bonusAmount;
    }

    SystemResourceBonus& operator+=(const SystemResourceBonus& other) {
        if (m_resourceType.id() != other.m_resourceType.id()) {
            throw std::invalid_argument("Cannot add bonuses of different resource types");
        }
        if (m_resourceType == other.m_resourceType) {
            m_bonusAmount += other.m_bonusAmount;
        }
        return *this;
    }

    SystemResourceBonus& operator-=(const SystemResourceBonus& other) {
        if (m_resourceType.id() != other.m_resourceType.id()) {
            throw std::invalid_argument("Cannot subtract bonuses of different resource types");
        }
        if (m_resourceType == other.m_resourceType) {
            m_bonusAmount -= other.m_bonusAmount;
            if (m_bonusAmount < 0) {
                m_bonusAmount = 0; // Ensure bonus amount does not go negative
            }
        }
        return *this;
    }

    SystemResourceBonus operator+(const SystemResourceBonus& other) const {
        if (m_resourceType.id() != other.m_resourceType.id()) {
            throw std::invalid_argument("Cannot add bonuses of different resource types");
        }
        return SystemResourceBonus(m_resourceType, m_bonusAmount + other.m_bonusAmount);
    }

    SystemResourceBonus operator-(const SystemResourceBonus& other) const {
        if (m_resourceType.id() != other.m_resourceType.id()) {
            throw std::invalid_argument("Cannot subtract bonuses of different resource types");
        }
        int newBonusAmount = m_bonusAmount - other.m_bonusAmount;
        if (newBonusAmount < 0) {
            newBonusAmount = 0; // Ensure bonus amount does not go negative
        }
        return SystemResourceBonus(m_resourceType, newBonusAmount);
    }


private:
    ResourceType m_resourceType;
    int m_bonusAmount;
};

} // namespace ggh::Galaxy::Factions

#endif // !GGH_MODULES_GALAXYFACTIONS_SYSTEM_RESOURCE_BONUS_H
