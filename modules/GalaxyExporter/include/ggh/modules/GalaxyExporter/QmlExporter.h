#ifndef GGH_GALAXYEXPORTER_QML_EXPORTER_H
#define GGH_GALAXYEXPORTER_QML_EXPORTER_H

#include <QObject>
#include <QtQml>

/**
 * @brief QML-exposed singleton for Galaxy Exporter module
 * 
 * This class provides a simple QML-accessible interface to indicate
 * that the Galaxy.Exporter module is available and functional.
 */
class GalaxyExporterQml : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_NAMED_ELEMENT(GalaxyExporter)

public:
    explicit GalaxyExporterQml(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE bool isAvailable() const { return true; }
    Q_INVOKABLE QString version() const { return QStringLiteral("1.0"); }
    
    // QML singleton instance creation
    static GalaxyExporterQml* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return new GalaxyExporterQml();
    }
};

#endif // GGH_GALAXYEXPORTER_QML_EXPORTER_H
