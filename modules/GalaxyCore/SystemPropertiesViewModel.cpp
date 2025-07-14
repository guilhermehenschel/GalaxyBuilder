#include "SystemPropertiesViewModel.h"
#include <QTimer>
#include <QDebug>

SystemPropertiesViewModel::SystemPropertiesViewModel(QObject* parent)
    : QObject(parent)
    , m_autoSaveTimer(new QTimer(this))
{
    // Configure auto-save timer
    m_autoSaveTimer->setSingleShot(true);
    m_autoSaveTimer->setInterval(2000); // 2 second delay after last change
    connect(m_autoSaveTimer, &QTimer::timeout, this, &SystemPropertiesViewModel::onAutoSaveTimer);
    
    // Data manager will be set externally or through dependency injection
    // For now, we'll get it from the application when we need it
}

void SystemPropertiesViewModel::setStarSystem(StarSystemQml* system)
{
    if (m_starSystem != system) {
        // Disconnect from old system
        disconnectSystemSignals();
        
        m_starSystem = system;
        
        // Connect to new system
        connectSystemSignals();
        
        // Reset state
        setDirty(false);
        setStatusMessage("");
        
        emit starSystemChanged();
        
        // Auto-load data for new system
        if (m_starSystem) {
            loadSystemData();
        }
    }
}

void SystemPropertiesViewModel::setAutoSaveEnabled(bool enabled)
{
    if (m_autoSaveEnabled != enabled) {
        m_autoSaveEnabled = enabled;
        emit autoSaveEnabledChanged();
    }
}

void SystemPropertiesViewModel::setDataManager(SystemDataManager* dataManager)
{
    qDebug() << "SystemPropertiesViewModel::setDataManager called with:" << dataManager;
    m_dataManager = dataManager;
}

bool SystemPropertiesViewModel::saveSystemData()
{
    qDebug() << "SystemPropertiesViewModel::saveSystemData() called";
    if (!m_starSystem || !m_dataManager) {
        QString error = QString("Cannot save: system=%1 dataManager=%2").arg(m_starSystem ? "available" : "null").arg(m_dataManager ? "available" : "null");
        qDebug() << error;
        setStatusMessage(error);
        return false;
    }
    
    setIsSaving(true);
    setStatusMessage("Saving system data...");
    
    bool success = m_dataManager->saveSystemChanges(m_starSystem);
    qDebug() << "Save result:" << success;
    
    setIsSaving(false);
    
    if (success) {
        setDirty(false);
        setStatusMessage("System data saved successfully");
        emit saveCompleted(true);
        return true;
    } else {
        setStatusMessage("Failed to save system data");
        emit saveCompleted(false);
        return false;
    }
}

bool SystemPropertiesViewModel::loadSystemData()
{
    if (!m_starSystem || !m_dataManager) {
        setStatusMessage("Cannot load: system or data manager not available");
        return false;
    }
    
    setIsLoading(true);
    setStatusMessage("Loading system data...");
    
    bool success = m_dataManager->loadSystemData(m_starSystem);
    
    setIsLoading(false);
    
    if (success) {
        setDirty(false);
        setStatusMessage("System data loaded successfully");
        emit loadCompleted(true);
        return true;
    } else {
        setStatusMessage("No saved data found or failed to load");
        emit loadCompleted(false);
        return false;
    }
}

void SystemPropertiesViewModel::exportToXml(const QString& filePath)
{
    if (!m_starSystem || !m_dataManager) {
        setStatusMessage("Cannot export: system or data manager not available");
        emit exportCompleted(false, filePath);
        return;
    }
    
    setStatusMessage("Exporting system to XML...");
    
    bool success = m_dataManager->exportSystemToXml(m_starSystem, filePath);
    
    if (success) {
        setStatusMessage(QString("System exported to: %1").arg(filePath));
        emit exportCompleted(true, filePath);
    } else {
        setStatusMessage("Failed to export system");
        emit exportCompleted(false, filePath);
    }
}

void SystemPropertiesViewModel::importFromXml(const QString& filePath)
{
    if (!m_dataManager) {
        setStatusMessage("Cannot import: data manager not available");
        emit importCompleted(false, filePath);
        return;
    }
    
    setStatusMessage("Importing system from XML...");
    
    StarSystemQml* importedSystem = m_dataManager->importSystemFromXml(filePath);
    
    if (importedSystem && m_starSystem) {
        // Copy properties from imported system to current system
        m_starSystem->setName(importedSystem->getName());
        m_starSystem->setStarType(importedSystem->getStarType());
        m_starSystem->setSystemSize(importedSystem->getSystemSize());
        m_starSystem->setStarMass(importedSystem->getStarMass());
        m_starSystem->setStarTemperature(importedSystem->getStarTemperature());
        m_starSystem->setStarLuminosity(importedSystem->getStarLuminosity());
        
        // TODO: Copy planets
        
        setDirty(true);
        setStatusMessage(QString("System imported from: %1").arg(filePath));
        emit importCompleted(true, filePath);
        
        // Clean up imported system
        importedSystem->deleteLater();
    } else {
        setStatusMessage("Failed to import system");
        emit importCompleted(false, filePath);
    }
}

void SystemPropertiesViewModel::resetChanges()
{
    if (m_starSystem) {
        loadSystemData();
    }
}

void SystemPropertiesViewModel::markAsClean()
{
    setDirty(false);
}

void SystemPropertiesViewModel::clearStatusMessage()
{
    setStatusMessage("");
}

void SystemPropertiesViewModel::addPlanet()
{
    qDebug() << "SystemPropertiesViewModel::addPlanet() called";
    if (m_starSystem) {
        qDebug() << "Adding planet to star system";
        m_starSystem->addPlanet();
        setDirty(true);
        scheduleAutoSave();
        qDebug() << "Planet added, dirty:" << m_isDirty << "autoSave enabled:" << m_autoSaveEnabled;
    } else {
        qDebug() << "Error: No star system available";
    }
}

void SystemPropertiesViewModel::removePlanet(int index)
{
    if (m_starSystem) {
        bool success = m_starSystem->removePlanetAt(index);
        if (success) {
            setDirty(true);
            scheduleAutoSave();
        }
    }
}

void SystemPropertiesViewModel::movePlanet(int fromIndex, int toIndex)
{
    // TODO: Implement planet reordering if needed
    Q_UNUSED(fromIndex)
    Q_UNUSED(toIndex)
}

void SystemPropertiesViewModel::onSystemPropertyChanged()
{
    setDirty(true);
    scheduleAutoSave();
}

void SystemPropertiesViewModel::onAutoSaveTimer()
{
    if (m_autoSaveEnabled && m_isDirty) {
        saveSystemData();
    }
}

void SystemPropertiesViewModel::setDirty(bool dirty)
{
    if (m_isDirty != dirty) {
        m_isDirty = dirty;
        emit isDirtyChanged();
    }
}

void SystemPropertiesViewModel::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message) {
        m_statusMessage = message;
        emit statusMessageChanged();
    }
}

void SystemPropertiesViewModel::setIsSaving(bool saving)
{
    if (m_isSaving != saving) {
        m_isSaving = saving;
        emit isSavingChanged();
    }
}

void SystemPropertiesViewModel::setIsLoading(bool loading)
{
    if (m_isLoading != loading) {
        m_isLoading = loading;
        emit isLoadingChanged();
    }
}

void SystemPropertiesViewModel::connectSystemSignals()
{
    if (m_starSystem) {
        connect(m_starSystem, &StarSystemQml::nameChanged, this, &SystemPropertiesViewModel::onSystemPropertyChanged);
        connect(m_starSystem, &StarSystemQml::starTypeChanged, this, &SystemPropertiesViewModel::onSystemPropertyChanged);
        connect(m_starSystem, &StarSystemQml::systemSizeChanged, this, &SystemPropertiesViewModel::onSystemPropertyChanged);
        connect(m_starSystem, &StarSystemQml::starMassChanged, this, &SystemPropertiesViewModel::onSystemPropertyChanged);
        connect(m_starSystem, &StarSystemQml::starTemperatureChanged, this, &SystemPropertiesViewModel::onSystemPropertyChanged);
        connect(m_starSystem, &StarSystemQml::starLuminosityChanged, this, &SystemPropertiesViewModel::onSystemPropertyChanged);
        // Note: Planet changes are handled separately through planet model signals
    }
}

void SystemPropertiesViewModel::disconnectSystemSignals()
{
    if (m_starSystem) {
        disconnect(m_starSystem, nullptr, this, nullptr);
    }
}

void SystemPropertiesViewModel::scheduleAutoSave()
{
    if (m_autoSaveEnabled && m_autoSaveTimer) {
        m_autoSaveTimer->start();
    }
}
