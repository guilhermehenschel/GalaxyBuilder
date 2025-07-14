#include "ImportExportViewModel.h"
#include "GalaxyController.h"
#include "StarSystemListModel.h"
#include <QQmlEngine>
#include <QQmlContext>
#include <QCoreApplication>
#include <QDebug>

ImportExportViewModel::ImportExportViewModel(QObject* parent)
    : QObject(parent)
{
    // Get data manager from application context
    if (auto* app = qobject_cast<QCoreApplication*>(QCoreApplication::instance())) {
        auto* engine = qmlEngine(this);
        if (engine) {
            auto* context = engine->rootContext();
            if (context) {
                QObject* controller = context->contextProperty("galaxyController").value<QObject*>();
                if (controller) {
                    m_dataManager = controller->property("systemDataManager").value<SystemDataManager*>();
                    
                    // Connect to galaxy changes to update available systems
                    connect(controller, SIGNAL(galaxyChanged()), this, SLOT(onGalaxyChanged()));
                }
            }
        }
    }
    
    // Initialize available systems
    refreshAvailableSystems();
}

void ImportExportViewModel::setSelectedSystems(const QList<StarSystemQml*>& systems)
{
    if (m_selectedSystems != systems) {
        m_selectedSystems = systems;
        emit selectedSystemsChanged();
    }
}

void ImportExportViewModel::setDataManager(SystemDataManager* dataManager)
{
    qDebug() << "ImportExportViewModel::setDataManager called with:" << dataManager;
    m_dataManager = dataManager;
}

void ImportExportViewModel::setSystemsModel(QObject* model)
{
    qDebug() << "ImportExportViewModel::setSystemsModel called with:" << model;
    
    // Prevent infinite recursion if the same model is set again
    if (m_systemsModel == model) {
        qDebug() << "Same model already set, skipping update";
        return;
    }
    
    m_systemsModel = model;
    // Update available systems from the model
    updateAvailableSystems();
}

void ImportExportViewModel::refreshAvailableSystems()
{
    updateAvailableSystems();
}

void ImportExportViewModel::selectAllSystems()
{
    setSelectedSystems(m_availableSystems);
    setStatusMessage(QString("Selected all %1 systems").arg(m_availableSystems.size()));
}

void ImportExportViewModel::clearSelection()
{
    setSelectedSystems(QList<StarSystemQml*>());
    setStatusMessage("Selection cleared");
}

void ImportExportViewModel::toggleSystemSelection(StarSystemQml* system)
{
    if (!system) return;
    
    QList<StarSystemQml*> newSelection = m_selectedSystems;
    
    if (newSelection.contains(system)) {
        newSelection.removeAll(system);
        setStatusMessage(QString("Deselected system: %1").arg(system->getName()));
    } else {
        newSelection.append(system);
        setStatusMessage(QString("Selected system: %1").arg(system->getName()));
    }
    
    setSelectedSystems(newSelection);
}

void ImportExportViewModel::selectSystem(StarSystemQml* system)
{
    if (!system) return;
    
    QList<StarSystemQml*> newSelection = m_selectedSystems;
    
    if (!newSelection.contains(system)) {
        newSelection.append(system);
        setSelectedSystems(newSelection);
        setStatusMessage(QString("Selected system: %1").arg(system->getName()));
    }
}

void ImportExportViewModel::deselectSystem(StarSystemQml* system)
{
    if (!system) return;
    
    QList<StarSystemQml*> newSelection = m_selectedSystems;
    
    if (newSelection.contains(system)) {
        newSelection.removeAll(system);
        setSelectedSystems(newSelection);
        setStatusMessage(QString("Deselected system: %1").arg(system->getName()));
    }
}

void ImportExportViewModel::exportSelectedSystems(const QString& filePath)
{
    if (m_selectedSystems.isEmpty()) {
        setStatusMessage("No systems selected for export");
        emit exportCompleted(false, filePath, 0);
        return;
    }
    
    if (!m_dataManager) {
        setStatusMessage("Data manager not available");
        emit exportCompleted(false, filePath, 0);
        return;
    }
    
    setIsExporting(true);
    setStatusMessage(QString("Exporting %1 selected systems...").arg(m_selectedSystems.size()));
    
    // XML is created on-demand only when export is requested
    bool success = m_dataManager->exportGalaxyToXml(m_selectedSystems, filePath);
    
    setIsExporting(false);
    
    if (success) {
        setStatusMessage(QString("Successfully exported %1 systems to %2")
                        .arg(m_selectedSystems.size())
                        .arg(filePath));
        emit exportCompleted(true, filePath, m_selectedSystems.size());
    } else {
        setStatusMessage("Failed to export selected systems");
        emit exportCompleted(false, filePath, 0);
    }
}

void ImportExportViewModel::exportAllSystems(const QString& filePath)
{
    if (m_availableSystems.isEmpty()) {
        setStatusMessage("No systems available for export");
        emit exportCompleted(false, filePath, 0);
        return;
    }
    
    if (!m_dataManager) {
        setStatusMessage("Data manager not available");
        emit exportCompleted(false, filePath, 0);
        return;
    }
    
    setIsExporting(true);
    setStatusMessage(QString("Exporting all %1 systems...").arg(m_availableSystems.size()));
    
    // XML is created on-demand only when export is requested
    bool success = m_dataManager->exportGalaxyToXml(m_availableSystems, filePath);
    
    setIsExporting(false);
    
    if (success) {
        setStatusMessage(QString("Successfully exported all %1 systems to %2")
                        .arg(m_availableSystems.size())
                        .arg(filePath));
        emit exportCompleted(true, filePath, m_availableSystems.size());
    } else {
        setStatusMessage("Failed to export all systems");
        emit exportCompleted(false, filePath, 0);
    }
}

void ImportExportViewModel::exportSingleSystem(StarSystemQml* system, const QString& filePath)
{
    if (!system) {
        setStatusMessage("No system specified for export");
        emit exportCompleted(false, filePath, 0);
        return;
    }
    
    if (!m_dataManager) {
        setStatusMessage("Data manager not available");
        emit exportCompleted(false, filePath, 0);
        return;
    }
    
    setIsExporting(true);
    setStatusMessage(QString("Exporting system %1...").arg(system->getName()));
    
    // XML is created on-demand only when export is requested
    bool success = m_dataManager->exportSystemToXml(system, filePath);
    
    setIsExporting(false);
    
    if (success) {
        setStatusMessage(QString("Successfully exported system %1 to %2")
                        .arg(system->getName())
                        .arg(filePath));
        emit exportCompleted(true, filePath, 1);
    } else {
        setStatusMessage(QString("Failed to export system %1").arg(system->getName()));
        emit exportCompleted(false, filePath, 0);
    }
}

void ImportExportViewModel::importGalaxy(const QString& filePath)
{
    if (!m_dataManager) {
        setStatusMessage("Data manager not available");
        emit importCompleted(false, filePath, 0);
        return;
    }
    
    setIsImporting(true);
    setStatusMessage("Importing galaxy from XML...");
    
    QList<StarSystemQml*> importedSystems = m_dataManager->importGalaxyFromXml(filePath);
    
    setIsImporting(false);
    
    if (!importedSystems.isEmpty()) {
        setStatusMessage(QString("Successfully imported %1 systems from %2")
                        .arg(importedSystems.size())
                        .arg(filePath));
        
        // Emit signal with imported systems for integration into current galaxy
        emit systemsImported(importedSystems);
        emit importCompleted(true, filePath, importedSystems.size());
        
        // Update available systems list
        refreshAvailableSystems();
    } else {
        setStatusMessage("Failed to import galaxy or no systems found");
        emit importCompleted(false, filePath, 0);
    }
}

void ImportExportViewModel::importSingleSystem(const QString& filePath)
{
    if (!m_dataManager) {
        setStatusMessage("Data manager not available");
        emit importCompleted(false, filePath, 0);
        return;
    }
    
    setIsImporting(true);
    setStatusMessage("Importing system from XML...");
    
    StarSystemQml* importedSystem = m_dataManager->importSystemFromXml(filePath);
    
    setIsImporting(false);
    
    if (importedSystem) {
        setStatusMessage(QString("Successfully imported system %1 from %2")
                        .arg(importedSystem->getName())
                        .arg(filePath));
        
        // Emit signal with imported system for integration into current galaxy
        QList<StarSystemQml*> systems;
        systems.append(importedSystem);
        emit systemsImported(systems);
        emit importCompleted(true, filePath, 1);
        
        // Update available systems list
        refreshAvailableSystems();
    } else {
        setStatusMessage("Failed to import system");
        emit importCompleted(false, filePath, 0);
    }
}

void ImportExportViewModel::importSystem(const QString& filePath)
{
    // Alias for importSingleSystem for QML compatibility
    importSingleSystem(filePath);
}

void ImportExportViewModel::onGalaxyChanged()
{
    refreshAvailableSystems();
    clearSelection(); // Clear selection when galaxy changes
}

void ImportExportViewModel::setStatusMessage(const QString& message)
{
    if (m_statusMessage != message) {
        m_statusMessage = message;
        emit statusMessageChanged();
    }
}

void ImportExportViewModel::setIsExporting(bool exporting)
{
    if (m_isExporting != exporting) {
        m_isExporting = exporting;
        emit isExportingChanged();
    }
}

void ImportExportViewModel::setIsImporting(bool importing)
{
    if (m_isImporting != importing) {
        m_isImporting = importing;
        emit isImportingChanged();
    }
}

void ImportExportViewModel::updateAvailableSystems()
{
    // Prevent infinite recursion
    if (m_updatingAvailableSystems) {
        qDebug() << "ImportExportViewModel::updateAvailableSystems - already updating, skipping to prevent recursion";
        return;
    }
    
    m_updatingAvailableSystems = true;
    QList<StarSystemQml*> newSystems;
    
    // Use the stored systems model if available
    if (m_systemsModel) {
        StarSystemListModel* systemsModel = qobject_cast<StarSystemListModel*>(m_systemsModel);
        if (systemsModel) {
            int rowCount = systemsModel->rowCount();
            qDebug() << "ImportExportViewModel::updateAvailableSystems - found" << rowCount << "systems in model";
            
            // Create StarSystemQml objects from the model data
            for (int i = 0; i < rowCount; ++i) {
                QModelIndex index = systemsModel->index(i, 0);
                if (index.isValid()) {
                    // Get data from the model using the defined roles
                    QVariant idVar = systemsModel->data(index, Qt::UserRole + 1); // IdRole
                    QVariant xVar = systemsModel->data(index, Qt::UserRole + 2);  // XRole  
                    QVariant yVar = systemsModel->data(index, Qt::UserRole + 3);  // YRole
                    QVariant nameVar = systemsModel->data(index, Qt::UserRole + 4); // NameRole
                    QVariant typeVar = systemsModel->data(index, Qt::UserRole + 5); // TypeRole
                    QVariant sizeVar = systemsModel->data(index, Qt::UserRole + 6); // SizeRole
                    
                    if (idVar.isValid() && xVar.isValid() && yVar.isValid()) {
                        // Create StarSystemQml object
                        int systemId = idVar.toInt();
                        QPointF position(xVar.toDouble(), yVar.toDouble());
                        StarType starType = static_cast<StarType>(typeVar.toInt());
                        
                        auto* starSystem = new StarSystemQml(static_cast<SystemId>(systemId), 
                                                            position, starType, this);
                        
                        if (nameVar.isValid()) {
                            starSystem->setName(nameVar.toString());
                        }
                        if (sizeVar.isValid()) {
                            starSystem->setSystemSize(static_cast<SystemSize>(sizeVar.toInt()));
                        }
                        
                        newSystems.append(starSystem);
                    }
                }
            }
        } else {
            qDebug() << "ImportExportViewModel::updateAvailableSystems - stored model is not a StarSystemListModel";
        }
    }
    
    if (m_availableSystems != newSystems) {
        // Clean up old systems that we own
        for (auto* system : m_availableSystems) {
            if (system && system->parent() == this) {
                system->deleteLater();
            }
        }
        
        m_availableSystems = newSystems;
        emit availableSystemsChanged();
        
        setStatusMessage(QString("Found %1 available systems").arg(m_availableSystems.size()));
    }
    
    m_updatingAvailableSystems = false;
}
