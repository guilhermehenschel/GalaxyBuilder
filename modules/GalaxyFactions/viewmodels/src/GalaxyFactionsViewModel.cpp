/**
 * @file GalaxyFactionsViewModel.cpp
 * @brief Implementation of GalaxyFactionsViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/GalaxyFactionsViewModel.h"
#include "ggh/modules/GalaxyFactions/viewmodels/Commons.h"
#include "ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h"
#include "ggh/modules/GalaxyFactions/models/System.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QUrl>

namespace ggh::Galaxy::Factions::viewmodels {

GalaxyFactionsViewModel::GalaxyFactionsViewModel(QObject* parent)
    : QObject(parent)
    , m_galaxyFactions(nullptr)
    , m_factionListModel(new FactionListModel(this))
    , m_selectedFactionId(Commons::INVALID_ID)
{
}

FactionListModel* GalaxyFactionsViewModel::factionListModel()
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

int GalaxyFactionsViewModel::addFactionWithHomeworld(const QString& name, const QString& description, const QString& color, quint32 homeworldSystemId, QObject* starSystemModel)
{
    qDebug() << "GalaxyFactionsViewModel::addFactionWithHomeworld called with:"
             << "name:" << name
             << "description:" << description  
             << "color:" << color
             << "homeworldSystemId:" << homeworldSystemId
             << "starSystemModel:" << starSystemModel;

    if (!m_galaxyFactions) {
        qDebug() << "ERROR: m_galaxyFactions is null!";
        return -1;
    }
    
    // Create the faction
    qDebug() << "Creating faction with std::string parameters:"
             << "name:" << name.toStdString().c_str()
             << "description:" << description.toStdString().c_str()
             << "color:" << color.toStdString().c_str();
    
    auto faction = m_galaxyFactions->createFaction(
        name.toStdString(),
        description.toStdString(),
        color.toStdString()
    );
    
    if (!faction) {
        qDebug() << "ERROR: Failed to create faction - createFaction returned null!";
        return -1;
    }
    
    qDebug() << "Faction created successfully with ID:" << faction->id();
    
    // Try to cast the QObject to StarSystemViewModel to get the underlying model
    if (starSystemModel) {
        qDebug() << "Processing star system model for homeworld integration...";
        // Check if this is a StarSystemViewModel
        auto starSystemViewModel = qobject_cast<ggh::GalaxyCore::viewmodels::StarSystemViewModel*>(starSystemModel);
        if (starSystemViewModel && starSystemViewModel->starSystem()) {
            qDebug() << "StarSystemViewModel cast successful, creating System object...";
            // Create a System from the StarSystemModel and add it to the faction
            auto system = std::make_shared<ggh::Galaxy::Factions::models::System>(starSystemViewModel->starSystem());
            faction->addSystem(system);
            qDebug() << "System added to faction successfully";
        } else {
            qDebug() << "WARNING: StarSystemViewModel cast failed or starSystem() is null";
            qDebug() << "starSystemViewModel:" << (void*)starSystemViewModel;
            if (starSystemViewModel) {
                qDebug() << "starSystemViewModel->starSystem() is null:" << (starSystemViewModel->starSystem() == nullptr);
            }
        }
    } else {
        qDebug() << "WARNING: starSystemModel parameter is null";
    }
    
    // Add the faction to the list model
    qDebug() << "Adding faction to list model...";
    m_factionListModel->addFaction(*faction);
    
    // Emit the signal with all the faction details
    qDebug() << "Emitting factionCreated signal with ID:" << faction->id();
    emit factionCreated(faction->id(), homeworldSystemId, name, description, color);
    
    qDebug() << "addFactionWithHomeworld completed successfully, returning ID:" << faction->id();
    return faction->id();
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

QString GalaxyFactionsViewModel::exportToXml() const
{
    if (!m_galaxyFactions) {
        return QString();
    }
    
    std::string xmlString = m_galaxyFactions->toXml();
    return QString::fromStdString(xmlString);
}

bool GalaxyFactionsViewModel::exportToXmlFile(const QString& filePath) const
{
    if (!m_galaxyFactions) {
        qWarning() << "GalaxyFactionsViewModel::exportToXmlFile: m_galaxyFactions is null";
        return false;
    }
    
    try {
        // Convert URL to local file path if needed
        QString localPath = filePath;
        if (localPath.startsWith("file:///")) {
            localPath = QUrl(filePath).toLocalFile();
        }
        
        qDebug() << "GalaxyFactionsViewModel::exportToXmlFile: Converting" << filePath << "to" << localPath;
        
        std::string xmlContent = m_galaxyFactions->toXml();
        qDebug() << "GalaxyFactionsViewModel::exportToXmlFile: Generated XML content length:" << xmlContent.length();
        
        QFile file(localPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "GalaxyFactionsViewModel::exportToXmlFile: Failed to open file for writing:" << localPath << "Error:" << file.errorString();
            return false;
        }
        
        QTextStream stream(&file);
        stream << QString::fromStdString(xmlContent);
        
        qDebug() << "GalaxyFactionsViewModel::exportToXmlFile: Successfully exported factions to" << localPath;
        return true;
    } catch (const std::exception& e) {
        qWarning() << "GalaxyFactionsViewModel::exportToXmlFile: Exception occurred:" << e.what();
        return false;
    } catch (...) {
        qWarning() << "GalaxyFactionsViewModel::exportToXmlFile: Unknown exception occurred";
        return false;
    }
}

} // namespace ggh::Galaxy::Factions::viewmodels
