#pragma once

#include "Common.h"
#include "Planet.h"
#include <QObject>
#include <QQmlEngine>
#include <QPointF>
#include <QColor>
#include <QString>
#include <QList>

// Core StarSystem data class (non-QObject)
class CoreStarSystem {
public:
    CoreStarSystem(SystemId id, const QPointF& position, StarType type = StarType::YellowStar);
    ~CoreStarSystem() = default;

    // Copy and move constructors/operators
    CoreStarSystem(const CoreStarSystem&) = default;
    CoreStarSystem& operator=(const CoreStarSystem&) = default;
    CoreStarSystem(CoreStarSystem&&) = default;
    CoreStarSystem& operator=(CoreStarSystem&&) = default;

    // Getters
    SystemId getId() const noexcept { return m_id; }
    const QPointF& getPosition() const noexcept { return m_position; }
    StarType getStarType() const noexcept { return m_starType; }
    SystemSize getSystemSize() const noexcept { return m_systemSize; }
    const QString& getName() const noexcept { return m_name; }
    QColor getStarColor() const noexcept;
    double getInfluenceRadius() const noexcept;

    // Setters
    void setPosition(const QPointF& position) { m_position = position; }
    void setStarType(StarType type) { m_starType = type; }
    void setSystemSize(SystemSize size) { m_systemSize = size; }
    void setName(const QString& name) { m_name = name; }

    // Utility methods
    double distanceTo(const CoreStarSystem& other) const;
    bool isWithinRange(const CoreStarSystem& other, double maxDistance) const;

private:
    SystemId m_id;
    QPointF m_position;
    StarType m_starType;
    SystemSize m_systemSize;
    QString m_name;

    void generateName();
    static QColor getColorForStarType(StarType type);
};

// QML-friendly StarSystem wrapper
class StarSystem : public QObject {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(int systemId READ getSystemId CONSTANT)
    Q_PROPERTY(QPointF position READ getPosition WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(StarType starType READ getStarType WRITE setStarType NOTIFY starTypeChanged)
    Q_PROPERTY(SystemSize systemSize READ getSystemSize WRITE setSystemSize NOTIFY systemSizeChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QColor starColor READ getStarColor NOTIFY starTypeChanged)
    Q_PROPERTY(double influenceRadius READ getInfluenceRadius NOTIFY systemSizeChanged)
    Q_PROPERTY(double starMass READ getStarMass WRITE setStarMass NOTIFY starMassChanged)
    Q_PROPERTY(double starTemperature READ getStarTemperature WRITE setStarTemperature NOTIFY starTemperatureChanged)
    Q_PROPERTY(double starLuminosity READ getStarLuminosity WRITE setStarLuminosity NOTIFY starLuminosityChanged)
    Q_PROPERTY(QList<Planet*> planets READ getPlanets NOTIFY planetsChanged)

public:
    explicit StarSystem(QObject* parent = nullptr);
    StarSystem(SystemId id, const QPointF& position, StarType type = StarType::YellowStar, QObject* parent = nullptr);
    ~StarSystem() = default;

    // Getters
    int getSystemId() const noexcept { return static_cast<int>(m_id); }
    const QPointF& getPosition() const noexcept { return m_position; }
    StarType getStarType() const noexcept { return m_starType; }
    SystemSize getSystemSize() const noexcept { return m_systemSize; }
    const QString& getName() const noexcept { return m_name; }
    QColor getStarColor() const noexcept;
    double getInfluenceRadius() const noexcept;
    double getStarMass() const noexcept { return m_starMass; }
    double getStarTemperature() const noexcept { return m_starTemperature; }
    double getStarLuminosity() const noexcept { return m_starLuminosity; }
    const QList<Planet*>& getPlanets() const noexcept { return m_planets; }

    // Setters
    void setPosition(const QPointF& position);
    void setStarType(StarType type);
    void setSystemSize(SystemSize size);
    void setName(const QString& name);
    void setStarMass(double mass);
    void setStarTemperature(double temperature);
    void setStarLuminosity(double luminosity);

    // Planet management
    Q_INVOKABLE Planet* addPlanet();
    Q_INVOKABLE Planet* addPlanet(const QString& name, double orbitDistance);
    Q_INVOKABLE bool removePlanet(Planet* planet);
    Q_INVOKABLE bool removePlanetAt(int index);
    Q_INVOKABLE Planet* getPlanetAt(int index) const;
    Q_INVOKABLE int getPlanetCount() const;
    Q_INVOKABLE void clearPlanets();

    // Utility methods
    double distanceTo(const StarSystem& other) const;
    bool isWithinRange(const StarSystem& other, double maxDistance) const;

signals:
    void positionChanged();
    void starTypeChanged();
    void systemSizeChanged();
    void nameChanged();
    void starMassChanged();
    void starTemperatureChanged();
    void starLuminosityChanged();
    void planetsChanged();

private:
    SystemId m_id;
    QPointF m_position;
    StarType m_starType;
    SystemSize m_systemSize;
    QString m_name;
    double m_starMass;
    double m_starTemperature;
    double m_starLuminosity;
    QList<Planet*> m_planets;

    void generateName();
    void updateStarProperties();
    static QColor getColorForStarType(StarType type);
};
