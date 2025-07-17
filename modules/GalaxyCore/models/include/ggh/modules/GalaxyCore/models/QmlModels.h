#ifndef GGH_GALAXYCORE_MODELS_QML_MODELS_H
#define GGH_GALAXYCORE_MODELS_QML_MODELS_H

#include <QObject>
#include <QtQml>

/**
 * @brief QML-exposed singleton for Galaxy Core Models module
 * 
 * This class provides a simple QML-accessible interface to indicate
 * that the GalaxyCore.Model module is available and functional.
 */
class GalaxyCoreModelsQml : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_NAMED_ELEMENT(GalaxyCoreModels)

public:
    explicit GalaxyCoreModelsQml(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE bool isAvailable() const { return true; }
    Q_INVOKABLE QString version() const { return QStringLiteral("1.0"); }
    
    // QML singleton instance creation
    static GalaxyCoreModelsQml* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return new GalaxyCoreModelsQml();
    }
};

#endif // GGH_GALAXYCORE_MODELS_QML_MODELS_H
