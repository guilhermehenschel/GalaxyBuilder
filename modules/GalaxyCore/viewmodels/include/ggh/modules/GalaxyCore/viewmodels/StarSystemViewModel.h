#ifndef GGH_GALAXYCORE_VIEWMODELS_STARSYSTEMVIEWMODEL_H
#define GGH_GALAXYCORE_VIEWMODELS_STARSYSTEMVIEWMODEL_H

#include <QColor>
#include <QObject>
#include <QString>
#include <QAbstractListModel>
#include <QtQml>
#include <memory>
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"
#include "ggh/modules/GalaxyCore/viewmodels/PlanetListModel.h"

namespace ggh::GalaxyCore::viewmodels {

using SystemId = ggh::GalaxyCore::utilities::SystemId;
using StarType = ggh::GalaxyCore::utilities::StarType;
using SystemSize = ggh::GalaxyCore::utilities::SystemSize;

class PlanetListModel; // Forward declaration

/**
 * @class StarSystemViewModel
 * @brief ViewModel for StarSystem data, provides Qt-friendly interface for QML
 */
class StarSystemViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(quint32 systemId READ systemId CONSTANT)
    Q_PROPERTY(double positionX READ positionX WRITE setPositionX NOTIFY positionChanged)
    Q_PROPERTY(double positionY READ positionY WRITE setPositionY NOTIFY positionChanged)
    Q_PROPERTY(int starType READ starType WRITE setStarType NOTIFY starTypeChanged)
    Q_PROPERTY(int systemSize READ systemSize WRITE setSystemSize NOTIFY systemSizeChanged)
    Q_PROPERTY(QColor color READ color NOTIFY colorChanged)
    Q_PROPERTY(QAbstractListModel* planets READ planets CONSTANT)

public:
    explicit StarSystemViewModel(QObject* parent = nullptr);
    explicit StarSystemViewModel(std::shared_ptr<models::StarSystemModel> starSystem, QObject* parent = nullptr);

    // Property getters
    QString name() const;
    quint32 systemId() const;
    double positionX() const;
    double positionY() const;
    int starType() const;
    int systemSize() const;
    QColor color() const;
    QAbstractListModel* planets();

    // Property setters
    void setName(const QString& name);
    void setPositionX(double x);
    void setPositionY(double y);
    void setPosition(double x, double y);
    void setStarType(int type);
    void setSystemSize(int size);

    // Model access
    std::shared_ptr<models::StarSystemModel> starSystem() const;
    void setStarSystem(std::shared_ptr<models::StarSystemModel> starSystem);

    // Utility methods
    Q_INVOKABLE QString toXml() const;
    Q_INVOKABLE QString starTypeString() const;
    Q_INVOKABLE QString systemSizeString() const;
    Q_INVOKABLE void addPlanet(const QString& name, int type, double size, double mass, 
                              int moons, double radius, double maxTemp, double minTemp);
    Q_INVOKABLE bool removePlanetByName(const QString& name);
    Q_INVOKABLE int planetCount() const;

signals:
    void colorChanged();
    void nameChanged();
    void positionChanged();
    void starTypeChanged();
    void systemSizeChanged();

private:
    std::shared_ptr<models::StarSystemModel> m_starSystem;
    std::unique_ptr<PlanetListModel> m_planetsModel;
    
    void initializePlanetsModel();
};
} // namespace ggh::GalaxyCore::viewmodels

#endif // GGH_GALAXYCORE_VIEWMODELS_STARSYSTEMVIEWMODEL_H
