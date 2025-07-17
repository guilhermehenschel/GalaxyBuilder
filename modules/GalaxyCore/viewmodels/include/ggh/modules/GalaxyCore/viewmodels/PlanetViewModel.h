#ifndef GGH_GALAXYCORE_VIEWMODELS_PLANETVIEWMODEL_H
#define GGH_GALAXYCORE_VIEWMODELS_PLANETVIEWMODEL_H

#include <QObject>
#include <QString>
#include <QtQml>
#include <memory>
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"

namespace ggh::GalaxyCore::viewmodels {

using PlanetType = ggh::GalaxyCore::utilities::PlanetType;

/**
 * @class PlanetViewModel
 * @brief ViewModel for Planet data, provides Qt-friendly interface for QML
 */
class PlanetViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int planetType READ planetType WRITE setPlanetType NOTIFY planetTypeChanged)
    Q_PROPERTY(double size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(double mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(int numberOfMoons READ numberOfMoons WRITE setNumberOfMoons NOTIFY numberOfMoonsChanged)
    Q_PROPERTY(double orbitalRadius READ orbitalRadius WRITE setOrbitalRadius NOTIFY orbitalRadiusChanged)
    Q_PROPERTY(double maxTemperature READ maxTemperature WRITE setMaxTemperature NOTIFY maxTemperatureChanged)
    Q_PROPERTY(double minTemperature READ minTemperature WRITE setMinTemperature NOTIFY minTemperatureChanged)

public:
    explicit PlanetViewModel(QObject* parent = nullptr);
    explicit PlanetViewModel(std::shared_ptr<models::Planet> planet, QObject* parent = nullptr);

    // Property getters
    QString name() const;
    int planetType() const;
    double size() const;
    double mass() const;
    int numberOfMoons() const;
    double orbitalRadius() const;
    double maxTemperature() const;
    double minTemperature() const;

    // Property setters
    void setName(const QString& name);
    void setPlanetType(int type);
    void setSize(double size);
    void setMass(double mass);
    void setNumberOfMoons(int moons);
    void setOrbitalRadius(double radius);
    void setMaxTemperature(double temp);
    void setMinTemperature(double temp);

    // Model access
    std::shared_ptr<models::Planet> planet() const;
    void setPlanet(std::shared_ptr<models::Planet> planet);

    // Utility methods
    Q_INVOKABLE QString toXml() const;
    Q_INVOKABLE QString planetTypeString() const;

signals:
    void nameChanged();
    void planetTypeChanged();
    void sizeChanged();
    void massChanged();
    void numberOfMoonsChanged();
    void orbitalRadiusChanged();
    void maxTemperatureChanged();
    void minTemperatureChanged();

private:
    std::shared_ptr<models::Planet> m_planet;
    
    void connectToModel();
};

} // namespace ggh::GalaxyCore::viewmodels

#endif // GGH_GALAXYCORE_VIEWMODELS_PLANETVIEWMODEL_H
