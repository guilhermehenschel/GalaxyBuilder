#pragma once

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include "galaxycore_global.h"

enum class PlanetType {
    Rocky,
    GasGiant,
    IceGiant,
    Desert,
    Ocean,
    Frozen,
    Volcanic,
    Toxic
};

/**
 * @class Planet
 * @brief Represents a planet with various properties such as size, mass, and temperature.
 */
class GALAXYCORE_EXPORT Planet : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(PlanetType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int numberOfMoons READ numberOfMoons WRITE setNumberOfMoons NOTIFY numberOfMoonsChanged)
    Q_PROPERTY(double size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(double mass READ mass WRITE setMass NOTIFY massChanged)
    Q_PROPERTY(double gForce READ gForce WRITE setGForce NOTIFY gForceChanged)
    Q_PROPERTY(double maxTemperature READ maxTemperature WRITE setMaxTemperature NOTIFY maxTemperatureChanged)
    Q_PROPERTY(double minTemperature READ minTemperature WRITE setMinTemperature NOTIFY minTemperatureChanged)
    Q_PROPERTY(double orbitalRadius READ orbitalRadius WRITE setOrbitalRadius NOTIFY orbitalRadiusChanged)
    Q_PROPERTY(double orbitDistance READ orbitalRadius WRITE setOrbitDistance NOTIFY orbitalRadiusChanged)
    Q_PROPERTY(double orbitalPeriod READ orbitalPeriod WRITE setOrbitalPeriod NOTIFY orbitalPeriodChanged)

public:
    Q_ENUM(PlanetType)
    explicit Planet(QObject *parent = nullptr);
    Planet(const QString &name, PlanetType type, QObject *parent = nullptr);

    // Getters
    QString name() const { return m_name; }
    PlanetType type() const { return m_type; }
    int numberOfMoons() const { return m_numberOfMoons; }
    double size() const { return m_size; }
    double mass() const { return m_mass; }
    double gForce() const { return m_gForce; }
    double maxTemperature() const { return m_maxTemperature; }
    double minTemperature() const { return m_minTemperature; }
    double orbitalRadius() const { return m_orbitalRadius; }
    double orbitalPeriod() const { return m_orbitalPeriod; }

    // Setters
    /**
     * @brief Set the name of the planet.
     * @param name The new name of the planet.
     */
    void setName(const QString &name);

    /**
     * @brief Set the type of the planet.
     * @param type The new type of the planet.
     */
    void setType(PlanetType type);

    /**
     * @brief Set the number of moons orbiting the planet.
     * @param numberOfMoons The number of moons.
     */
    void setNumberOfMoons(int numberOfMoons);

    void setSize(double size);
    void setMass(double mass);
    void setGForce(double gForce);
    void setMaxTemperature(double maxTemperature);
    void setMinTemperature(double minTemperature);
    void setOrbitalRadius(double orbitalRadius);
    void setOrbitDistance(double distance) { setOrbitalRadius(distance); }
    void setOrbitalPeriod(double orbitalPeriod);

    // Helper methods
    Q_INVOKABLE QString typeString() const;
    Q_INVOKABLE QColor planetColor() const;

signals:
    void nameChanged();
    void typeChanged();
    void numberOfMoonsChanged();
    void sizeChanged();
    void massChanged();
    void gForceChanged();
    void maxTemperatureChanged();
    void minTemperatureChanged();
    void orbitalRadiusChanged();
    void orbitalPeriodChanged();

private:
    QString m_name;
    PlanetType m_type;
    int m_numberOfMoons;
    double m_size;              // Radius in Earth radii
    double m_mass;              // Mass in Earth masses
    double m_gForce;            // Surface gravity in g
    double m_maxTemperature;    // Maximum temperature in Celsius
    double m_minTemperature;    // Minimum temperature in Celsius
    double m_orbitalRadius;     // Distance from star in AU
    double m_orbitalPeriod;     // Orbital period in Earth days
};

Q_DECLARE_METATYPE(PlanetType)
