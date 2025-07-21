/**
 * @file SystemResourceBonusViewModel.cpp
 * @brief Implementation of SystemResourceBonusViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/SystemResourceBonusViewModel.h"
#include "ggh/modules/GalaxyFactions/viewmodels/Commons.h"

namespace ggh::Galaxy::Factions::viewmodels {

SystemResourceBonusViewModel::SystemResourceBonusViewModel(QObject* parent)
    : QObject(parent)
    , m_resourceBonus(nullptr)
{
}

SystemResourceBonusViewModel::SystemResourceBonusViewModel(
    std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> resourceBonus, 
    QObject* parent)
    : QObject(parent)
    , m_resourceBonus(resourceBonus)
{
}

int SystemResourceBonusViewModel::id() const
{
    if (m_resourceBonus) {
        return m_resourceBonus->id();
    }
    return Commons::INVALID_ID;
}

QString SystemResourceBonusViewModel::name() const
{
    if (m_resourceBonus) {
        return QString::fromStdString(m_resourceBonus->resourceType().name());
    }
    return QString::fromLatin1(Commons::EMPTY_STRING);
}

double SystemResourceBonusViewModel::value() const
{
    if (m_resourceBonus) {
        return static_cast<double>(m_resourceBonus->bonusAmount());
    }
    return 0.0;
}

void SystemResourceBonusViewModel::setId(int id)
{
    if (m_resourceBonus && m_resourceBonus->id() != id) {
        m_resourceBonus->setId(id);
        emit idChanged(id);
    }
}

void SystemResourceBonusViewModel::setName(const QString& name)
{
    if (m_resourceBonus) {
        const std::string stdName = name.toStdString();
        auto resourceType = m_resourceBonus->resourceType();
        if (resourceType.name() != stdName) {
            resourceType.setName(stdName);
            m_resourceBonus->setResourceType(resourceType);
            emit nameChanged(name);
        }
    }
}

void SystemResourceBonusViewModel::setValue(double value)
{
    if (m_resourceBonus) {
        const int intValue = static_cast<int>(value);
        if (m_resourceBonus->bonusAmount() != intValue) {
            m_resourceBonus->setBonusAmount(intValue);
            emit valueChanged(value);
        }
    }
}

} // namespace ggh::Galaxy::Factions::viewmodels
