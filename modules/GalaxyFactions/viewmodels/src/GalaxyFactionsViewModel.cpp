/**
 * @file GalaxyFactionsViewModel.cpp
 * @brief Implementation of GalaxyFactionsViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/GalaxyFactionsViewModel.h"
#include "ggh/modules/GalaxyFactions/viewmodels/Commons.h"

namespace ggh::Galaxy::Factions::viewmodels {

GalaxyFactionsViewModel::GalaxyFactionsViewModel(QObject* parent)
    : QObject(parent)
    , m_galaxyFactions(nullptr)
    , m_factionListModel(new FactionListModel(this))
    , m_selectedFactionId(Commons::INVALID_ID)
{
}

FactionListModel* GalaxyFactionsViewModel::factionListModel() const
{
    return m_factionListModel;
}

int GalaxyFactionsViewModel::selectedFactionId() const
{
    return m_selectedFactionId;
}

void GalaxyFactionsViewModel::setSelectedFactionId(int factionId)
{
    if (m_selectedFactionId != factionId) {
        m_selectedFactionId = factionId;
        emit selectedFactionIdChanged(factionId);
    }
}

void GalaxyFactionsViewModel::initialize(std::shared_ptr<ggh::Galaxy::Factions::models::GalaxyFactions> galaxyFactions)
{
    m_galaxyFactions = galaxyFactions;
    
    if (m_galaxyFactions) {
        // Clear existing factions
        m_factionListModel->clearFactions();
        
        // Load all factions from the model
        auto factions = m_galaxyFactions->getAllFactions();
        QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>> qFactions;
        
        for (const auto& faction : factions) {
            qFactions.append(faction);
        }
        
        m_factionListModel->setFactions(qFactions);
    }
}

void GalaxyFactionsViewModel::addFaction(const QString& name, const QString& description, const QString& color)
{
    if (!m_galaxyFactions) {
        return;
    }
    
    auto faction = m_galaxyFactions->createFaction(
        name.toStdString(),
        description.toStdString(), 
        color.toStdString()
    );
    
    if (faction) {
        m_factionListModel->addFaction(*faction);
    }
}

void GalaxyFactionsViewModel::removeFaction(int factionId)
{
    if (!m_galaxyFactions) {
        return;
    }
    
    if (m_galaxyFactions->deleteFaction(factionId)) {
        m_factionListModel->removeFaction(factionId);
        
        // If the deleted faction was selected, clear the selection
        if (m_selectedFactionId == factionId) {
            setSelectedFactionId(Commons::INVALID_ID);
        }
    }
}

} // namespace ggh::Galaxy::Factions::viewmodels
