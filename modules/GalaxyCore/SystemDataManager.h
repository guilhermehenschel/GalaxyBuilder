#pragma once

#include "galaxycore_global.h"
#include "StarSystemQml.h"
#include <QObject>
#include <QString>
#include <QDomDocument>

class GALAXYCORE_EXPORT SystemDataManager : public QObject
{
    Q_OBJECT

public:
    explicit SystemDataManager(QObject* parent = nullptr);
    ~SystemDataManager() = default;

    // XML Export/Import
    Q_INVOKABLE bool exportSystemToXml(StarSystemQml* system, const QString& filePath);
    Q_INVOKABLE StarSystemQml* importSystemFromXml(const QString& filePath);
    
    // Batch operations
    Q_INVOKABLE bool exportGalaxyToXml(const QList<StarSystemQml*>& systems, const QString& filePath);
    Q_INVOKABLE QList<StarSystemQml*> importGalaxyFromXml(const QString& filePath);

    // Persistence
    Q_INVOKABLE bool saveSystemChanges(StarSystemQml* system);
    Q_INVOKABLE bool loadSystemData(StarSystemQml* system);
    
    // Auto-save functionality
    Q_INVOKABLE void enableAutoSave(bool enabled);
    Q_INVOKABLE bool isAutoSaveEnabled() const { return m_autoSaveEnabled; }

signals:
    void systemExported(const QString& filePath);
    void systemImported(const QString& filePath);
    void exportError(const QString& message);
    void importError(const QString& message);
    void autoSaveTriggered(int systemId);

private slots:
    void onSystemDataChanged();

private:
    bool m_autoSaveEnabled;
    QStringList m_changedSystems;
    
    // XML serialization helpers
    QDomElement systemToXml(StarSystemQml* system, QDomDocument& doc);
    QDomElement planetToXml(Planet* planet, QDomDocument& doc);
    StarSystemQml* xmlToSystem(const QDomElement& element);
    Planet* xmlToPlanet(const QDomElement& element);
    
    // Persistence helpers
    QString getSystemDataPath(int systemId) const;
    void setupAutoSave();
};
