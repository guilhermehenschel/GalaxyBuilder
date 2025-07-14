#pragma once

#include "galaxycore_global.h"
#include "StarSystemQml.h"
#include "SystemDataManager.h"
#include <QObject>
#include <QQmlEngine>

/**
 * @class ImportExportViewModel
 * @brief ViewModel for import/export operations following MVVM pattern
 * 
 * This ViewModel manages galaxy-level import/export operations,
 * handles file operations, and provides progress feedback.
 * XML is only created when actually needed for export operations.
 */
class GALAXYCORE_EXPORT ImportExportViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    // Properties exposed to QML View
    Q_PROPERTY(QList<StarSystemQml*> selectedSystems READ selectedSystems WRITE setSelectedSystems NOTIFY selectedSystemsChanged)
    Q_PROPERTY(QList<StarSystemQml*> availableSystems READ availableSystems NOTIFY availableSystemsChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(bool isExporting READ isExporting NOTIFY isExportingChanged)
    Q_PROPERTY(bool isImporting READ isImporting NOTIFY isImportingChanged)
    Q_PROPERTY(int selectedCount READ selectedCount NOTIFY selectedSystemsChanged)

public:
    explicit ImportExportViewModel(QObject* parent = nullptr);
    ~ImportExportViewModel() = default;

    // Property getters
    QList<StarSystemQml*> selectedSystems() const { return m_selectedSystems; }
    QList<StarSystemQml*> availableSystems() const { return m_availableSystems; }
    QString statusMessage() const { return m_statusMessage; }
    bool isExporting() const { return m_isExporting; }
    bool isImporting() const { return m_isImporting; }
    int selectedCount() const { return m_selectedSystems.size(); }

    // Property setters
    void setSelectedSystems(const QList<StarSystemQml*>& systems);
    Q_INVOKABLE void setDataManager(SystemDataManager* dataManager);
    Q_INVOKABLE void setSystemsModel(QObject* model);
    Q_INVOKABLE bool hasDataManager() const { return m_dataManager != nullptr; }

    // Business logic methods
    Q_INVOKABLE void refreshAvailableSystems();
    Q_INVOKABLE void selectAllSystems();
    Q_INVOKABLE void clearSelection();
    Q_INVOKABLE void toggleSystemSelection(StarSystemQml* system);
    Q_INVOKABLE void selectSystem(StarSystemQml* system);
    Q_INVOKABLE void deselectSystem(StarSystemQml* system);
    
    // Export operations (XML created on-demand)
    Q_INVOKABLE void exportSelectedSystems(const QString& filePath);
    Q_INVOKABLE void exportAllSystems(const QString& filePath);
    Q_INVOKABLE void exportSingleSystem(StarSystemQml* system, const QString& filePath);
    
    // Import operations
    Q_INVOKABLE void importGalaxy(const QString& filePath);
    Q_INVOKABLE void importSystem(const QString& filePath);
    Q_INVOKABLE void importSingleSystem(const QString& filePath);

signals:
    void selectedSystemsChanged();
    void availableSystemsChanged();
    void statusMessageChanged();
    void isExportingChanged();
    void isImportingChanged();
    
    // Business logic signals
    void exportCompleted(bool success, const QString& filePath, int systemCount);
    void importCompleted(bool success, const QString& filePath, int systemCount);
    void systemsImported(const QList<StarSystemQml*>& systems);

private slots:
    void onGalaxyChanged();

private:
    QList<StarSystemQml*> m_selectedSystems;
    QList<StarSystemQml*> m_availableSystems;
    SystemDataManager* m_dataManager = nullptr;
    QObject* m_systemsModel = nullptr;  // Store the systems model
    QString m_statusMessage;
    bool m_isExporting = false;
    bool m_isImporting = false;
    bool m_updatingAvailableSystems = false;  // Guard flag to prevent infinite recursion
    
    void setStatusMessage(const QString& message);
    void setIsExporting(bool exporting);
    void setIsImporting(bool importing);
    void updateAvailableSystems();
};
