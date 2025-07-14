#pragma once

#include "galaxycore_global.h"
#include "StarSystemQml.h"
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QJsonObject>
#include <QHash>

class GALAXYCORE_EXPORT SystemDataManager : public QObject
{
    Q_OBJECT

public:
    explicit SystemDataManager(QObject* parent = nullptr);
    ~SystemDataManager() = default;

    // XML Export/Import - Only creates XML when actually needed
    Q_INVOKABLE bool exportSystemToXml(StarSystemQml* system, const QString& filePath);
    Q_INVOKABLE StarSystemQml* importSystemFromXml(const QString& filePath);
    
    // Batch operations - Only creates XML when export is requested
    Q_INVOKABLE bool exportGalaxyToXml(const QList<StarSystemQml*>& systems, const QString& filePath);
    Q_INVOKABLE QList<StarSystemQml*> importGalaxyFromXml(const QString& filePath);
    
    // JSON Export/Import for lightweight operations
    Q_INVOKABLE bool exportSystemToJson(StarSystemQml* system, const QString& filePath);
    Q_INVOKABLE StarSystemQml* importSystemFromJson(const QString& filePath);
    Q_INVOKABLE bool exportGalaxyToJson(const QList<StarSystemQml*>& systems, const QString& filePath);
    Q_INVOKABLE QList<StarSystemQml*> importGalaxyFromJson(const QString& filePath);

    // Memory-based persistence - No XML creation, uses internal data structures
    Q_INVOKABLE bool saveSystemChanges(StarSystemQml* system);
    Q_INVOKABLE bool loadSystemData(StarSystemQml* system);
    
    // Auto-save functionality - Uses memory-based persistence by default
    Q_INVOKABLE void enableAutoSave(bool enabled);
    Q_INVOKABLE bool isAutoSaveEnabled() const { return m_autoSaveEnabled; }
    
    // MVVM-friendly validation methods
    Q_INVOKABLE bool validateSystemData(StarSystemQml* system) const;
    Q_INVOKABLE QString getValidationErrors(StarSystemQml* system) const;
    
    // Lazy XML preview - Creates XML string without file I/O for preview
    Q_INVOKABLE QString generateXmlPreview(StarSystemQml* system) const;
    Q_INVOKABLE QString generateGalaxyXmlPreview(const QList<StarSystemQml*>& systems) const;

signals:
    void systemExported(const QString& filePath);
    void systemImported(const QString& filePath);
    void exportError(const QString& message);
    void importError(const QString& message);
    void autoSaveTriggered(int systemId);
    void validationFailed(const QString& error);

private slots:
    void onSystemDataChanged();

private:
    bool m_autoSaveEnabled;
    
    // Memory-based persistence cache - avoids XML creation for frequent operations
    QHash<int, QJsonObject> m_systemCache;
    
    // XML serialization helpers - Only used when XML is actually needed
    QDomElement systemToXml(StarSystemQml* system, QDomDocument& doc) const;
    QDomElement planetToXml(Planet* planet, QDomDocument& doc) const;
    StarSystemQml* xmlToSystem(const QDomElement& element);
    Planet* xmlToPlanet(const QDomElement& element);
    
    // Memory-based persistence helpers
    QJsonObject systemToJson(StarSystemQml* system) const;
    bool systemFromJson(StarSystemQml* system, const QJsonObject& json) const;
    QString getSystemDataPath(int systemId) const;
    
    void setupAutoSave();
    bool createXmlDocumentForSystem(StarSystemQml* system, QDomDocument& doc) const;
    bool createXmlDocumentForGalaxy(const QList<StarSystemQml*>& systems, QDomDocument& doc) const;
};
