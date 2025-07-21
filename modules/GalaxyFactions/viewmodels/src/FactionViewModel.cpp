/**
 * @file FactionViewModel.cpp
 * @brief Implementation of FactionViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/FactionViewModel.h"
#include "ggh/modules/GalaxyFactions/viewmodels/Commons.h"

namespace ggh::Galaxy::Factions::viewmodels {

FactionViewModel::FactionViewModel(QObject* parent)
    : QObject(parent)
    , m_faction(Commons::INVALID_ID, Commons::EMPTY_STRING, Commons::EMPTY_STRING, Commons::EMPTY_STRING)
{
}

FactionViewModel::FactionViewModel(const ggh::Galaxy::Factions::models::Faction& faction, QObject* parent)
    : QObject(parent)
    , m_faction(faction)
{
}

int FactionViewModel::id() const
{
    return m_faction.id();
}

QString FactionViewModel::name() const
{
    return QString::fromStdString(m_faction.name());
}

QString FactionViewModel::description() const
{
    return QString::fromStdString(m_faction.description());
}

QString FactionViewModel::color() const
{
    return QString::fromStdString(m_faction.color());
}

void FactionViewModel::setId(int id)
{
    if (m_faction.id() != id) {
        m_faction.setId(id);
        emit idChanged(id);
    }
}

void FactionViewModel::setName(const QString& name)
{
    const std::string stdName = name.toStdString();
    if (m_faction.name() != stdName) {
        m_faction.setName(stdName);
        emit nameChanged(name);
    }
}

void FactionViewModel::setDescription(const QString& description)
{
    const std::string stdDescription = description.toStdString();
    if (m_faction.description() != stdDescription) {
        m_faction.setDescription(stdDescription);
        emit descriptionChanged(description);
    }
}

void FactionViewModel::setColor(const QString& color)
{
    const std::string stdColor = color.toStdString();
    if (m_faction.color() != stdColor) {
        m_faction.setColor(stdColor);
        emit colorChanged(color);
    }
}

} // namespace ggh::Galaxy::Factions::viewmodels
