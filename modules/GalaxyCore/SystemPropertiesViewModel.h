#pragma once

#include "galaxycore_global.h"
#include "StarSystemQml.h"
#include "SystemDataManager.h"
#include <QObject>
#include <QQmlEngine>
#include <QTimer>

/**
 * @class SystemPropertiesViewModel
 * @brief ViewModel for system properties dialog following MVVM pattern
 * 
 * This ViewModel manages the business logic for system property editing,
 * acts as intermediary between View (QML) and Model (StarSystemQml),
 * and handles data persistence operations.
 */
class GALAXYCORE_EXPORT SystemPropertiesViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    
    // Properties exposed to QML View
    Q_PROPERTY(StarSystemQml* starSystem READ starSystem WRITE setStarSystem NOTIFY starSystemChanged)
    Q_PROPERTY(bool isDirty READ isDirty NOTIFY isDirtyChanged)
    Q_PROPERTY(bool isAutoSaveEnabled READ isAutoSaveEnabled WRITE setAutoSaveEnabled NOTIFY autoSaveEnabledChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)
    Q_PROPERTY(bool isSaving READ isSaving NOTIFY isSavingChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

public:
    explicit SystemPropertiesViewModel(QObject* parent = nullptr);
    ~SystemPropertiesViewModel() = default;

    // Property getters
    StarSystemQml* starSystem() const { return m_starSystem; }
    bool isDirty() const { return m_isDirty; }
    bool isAutoSaveEnabled() const { return m_autoSaveEnabled; }
    QString statusMessage() const { return m_statusMessage; }
    bool isSaving() const { return m_isSaving; }
    bool isLoading() const { return m_isLoading; }
    Q_INVOKABLE bool hasDataManager() const { return m_dataManager != nullptr; }

    // Property setters
    void setStarSystem(StarSystemQml* system);
    void setAutoSaveEnabled(bool enabled);
    Q_INVOKABLE void setDataManager(SystemDataManager* dataManager);

    // Business logic methods
    Q_INVOKABLE bool saveSystemData();
    Q_INVOKABLE bool loadSystemData();
    Q_INVOKABLE void exportToXml(const QString& filePath);
    Q_INVOKABLE void importFromXml(const QString& filePath);
    Q_INVOKABLE void resetChanges();
    Q_INVOKABLE void markAsClean();
    Q_INVOKABLE void clearStatusMessage();

    // Planet management operations
    Q_INVOKABLE void addPlanet();
    Q_INVOKABLE void removePlanet(int index);
    Q_INVOKABLE void movePlanet(int fromIndex, int toIndex);

signals:
    void starSystemChanged();
    void isDirtyChanged();
    void autoSaveEnabledChanged();
    void statusMessageChanged();
    void isSavingChanged();
    void isLoadingChanged();
    
    // Business logic signals
    void saveCompleted(bool success);
    void loadCompleted(bool success);
    void exportCompleted(bool success, const QString& filePath);
    void importCompleted(bool success, const QString& filePath);

private slots:
    void onSystemPropertyChanged();
    void onAutoSaveTimer();

private:
    StarSystemQml* m_starSystem = nullptr;
    SystemDataManager* m_dataManager = nullptr;
    bool m_isDirty = false;
    bool m_autoSaveEnabled = true;
    QString m_statusMessage;
    bool m_isSaving = false;
    bool m_isLoading = false;
    QTimer* m_autoSaveTimer = nullptr;
    
    void setDirty(bool dirty);
    void setStatusMessage(const QString& message);
    void setIsSaving(bool saving);
    void setIsLoading(bool loading);
    void connectSystemSignals();
    void disconnectSystemSignals();
    void scheduleAutoSave();
};
