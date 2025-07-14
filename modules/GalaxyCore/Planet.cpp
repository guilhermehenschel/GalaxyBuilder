#include "Planet.h"
#include <QColor>

using PlanetType = ggh::GalaxyCore::utilities::PlanetType;

Planet::Planet(QObject *parent)
    : QObject(parent)
    , m_name("Unnamed Planet")
    , m_type(PlanetType::Rocky)
    , m_numberOfMoons(0)
    , m_size(1.0)
    , m_mass(1.0)
    , m_gForce(1.0)
    , m_maxTemperature(15.0)
    , m_minTemperature(-15.0)
    , m_orbitalRadius(1.0)
    , m_orbitalPeriod(365.0)
{
}

Planet::Planet(const QString &name, PlanetType type, QObject *parent)
    : QObject(parent)
    , m_name(name)
    , m_type(type)
    , m_numberOfMoons(0)
    , m_size(1.0)
    , m_mass(1.0)
    , m_gForce(1.0)
    , m_maxTemperature(15.0)
    , m_minTemperature(-15.0)
    , m_orbitalRadius(1.0)
    , m_orbitalPeriod(365.0)
{
}

void Planet::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

void Planet::setType(PlanetType type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
    }
}

void Planet::setNumberOfMoons(int numberOfMoons)
{
    if (m_numberOfMoons != numberOfMoons) {
        m_numberOfMoons = numberOfMoons;
        emit numberOfMoonsChanged();
    }
}

void Planet::setSize(double size)
{
    if (m_size != size) {
        m_size = size;
        emit sizeChanged();
    }
}

void Planet::setMass(double mass)
{
    if (m_mass != mass) {
        m_mass = mass;
        emit massChanged();
    }
}

void Planet::setGForce(double gForce)
{
    if (m_gForce != gForce) {
        m_gForce = gForce;
        emit gForceChanged();
    }
}

void Planet::setMaxTemperature(double maxTemperature)
{
    if (m_maxTemperature != maxTemperature) {
        m_maxTemperature = maxTemperature;
        emit maxTemperatureChanged();
    }
}

void Planet::setMinTemperature(double minTemperature)
{
    if (m_minTemperature != minTemperature) {
        m_minTemperature = minTemperature;
        emit minTemperatureChanged();
    }
}

void Planet::setOrbitalRadius(double orbitalRadius)
{
    if (m_orbitalRadius != orbitalRadius) {
        m_orbitalRadius = orbitalRadius;
        emit orbitalRadiusChanged();
    }
}

void Planet::setOrbitalPeriod(double orbitalPeriod)
{
    if (m_orbitalPeriod != orbitalPeriod) {
        m_orbitalPeriod = orbitalPeriod;
        emit orbitalPeriodChanged();
    }
}

QString Planet::typeString() const
{
    switch (m_type) {
    case PlanetType::Rocky: return "Rocky";
    case PlanetType::GasGiant: return "Gas Giant";
    case PlanetType::IceGiant: return "Ice Giant";
    case PlanetType::Desert: return "Desert";
    case PlanetType::Ocean: return "Ocean";
    case PlanetType::Frozen: return "Frozen";
    case PlanetType::Volcanic: return "Volcanic";
    case PlanetType::Toxic: return "Toxic";
    default: return "Unknown";
    }
}

QColor Planet::planetColor() const
{
    switch (m_type) {
    case PlanetType::Rocky: return QColor("#8B4513");     // Brown
    case PlanetType::GasGiant: return QColor("#FFA500");  // Orange
    case PlanetType::IceGiant: return QColor("#4169E1");  // Royal Blue
    case PlanetType::Desert: return QColor("#F4A460");    // Sandy Brown
    case PlanetType::Ocean: return QColor("#006994");     // Deep Blue
    case PlanetType::Frozen: return QColor("#B0E0E6");    // Powder Blue
    case PlanetType::Volcanic: return QColor("#DC143C");  // Crimson
    case PlanetType::Toxic: return QColor("#9ACD32");     // Yellow Green
    default: return QColor("#808080");                    // Gray
    }
}
