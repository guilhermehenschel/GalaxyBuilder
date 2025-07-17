#ifndef GGH_GALAXYFACTORIES_QML_FACTORIES_H
#define GGH_GALAXYFACTORIES_QML_FACTORIES_H

#include <QObject>
#include <QtQml>

/**
 * @brief QML-exposed singleton for Galaxy Factories module
 * 
 * This class provides a simple QML-accessible interface to indicate
 * that the Galaxy.Generator module is available and functional.
 */
class GalaxyFactoriesQml : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_NAMED_ELEMENT(GalaxyFactories)

public:
    explicit GalaxyFactoriesQml(QObject *parent = nullptr) : QObject(parent) {}

    Q_INVOKABLE bool isAvailable() const { return true; }
    Q_INVOKABLE QString version() const { return QStringLiteral("1.0"); }
    
    // QML singleton instance creation
    static GalaxyFactoriesQml* create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine)
        Q_UNUSED(jsEngine)
        return new GalaxyFactoriesQml();
    }
};

#endif // GGH_GALAXYFACTORIES_QML_FACTORIES_H
