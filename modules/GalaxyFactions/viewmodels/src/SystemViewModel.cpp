/**
 * @file SystemViewModel.cpp
 * @brief Implementation of SystemViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/SystemViewModel.h"
#include "ggh/modules/GalaxyFactions/viewmodels/Commons.h"

namespace ggh::Galaxy::Factions::viewmodels {

SystemViewModel::SystemViewModel(QObject* parent)
    : QObject(parent)
    , m_system(nullptr)
{
}

SystemViewModel::SystemViewModel(std::shared_ptr<ggh::Galaxy::Factions::models::System> system, QObject* parent)
    : QObject(parent)
    , m_system(system)
{
}

int SystemViewModel::id() const
{
    if (m_system) {
        return m_system->id();
    }
    return Commons::INVALID_ID;
}

QString SystemViewModel::name() const
{
    if (m_system) {
        return QString::fromStdString(m_system->name());
    }
    return QString::fromLatin1(Commons::EMPTY_STRING);
}

QString SystemViewModel::description() const
{
    if (m_system) {
        return QString::fromStdString(m_system->description());
    }
    return QString::fromLatin1(Commons::EMPTY_STRING);
}

QString SystemViewModel::type() const
{
    // Note: The System model doesn't seem to have a type property
    // This might need to be implemented in the System model first
    // For now, return a placeholder
    return QString("System");
}

void SystemViewModel::setId(int id)
{
    if (m_system) {
        int oldId = m_system->id();
        m_system->setId(id);
        int newId = m_system->id();
        if (oldId != newId) {
            emit idChanged(newId);
        }
    }
}

void SystemViewModel::setName(const QString& name)
{
    if (m_system) {
        const std::string stdName = name.toStdString();
        if (m_system->name() != stdName) {
            m_system->setName(stdName);
            emit nameChanged(name);
        }
    }
}

void SystemViewModel::setDescription(const QString& description)
{
    if (m_system) {
        const std::string oldDescription = m_system->description();
        const std::string stdDescription = description.toStdString();
        m_system->setDescription(stdDescription);
        const std::string newDescription = m_system->description();
        if (oldDescription != newDescription) {
            emit descriptionChanged(QString::fromStdString(newDescription));
        }
    }
}

void SystemViewModel::setType(const QString& type)
{
    // Note: The System model doesn't seem to have a setType method
    // This might need to be implemented in the System model first
    // For now, just emit the signal
    emit typeChanged(type);
}

} // namespace ggh::Galaxy::Factions::viewmodels
