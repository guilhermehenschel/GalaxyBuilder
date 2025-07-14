#include "StarSystemQml.h"
#include <QRandomGenerator>
#include <QStringList>
#include <cmath>

StarSystemQml::StarSystemQml(QObject* parent)
    : QObject(parent)
    , m_id(0)
    , m_position(0, 0)
    , m_starType(StarType::YellowStar)
    , m_systemSize(SystemSize::Medium)
    , m_starMass(1.0)
    , m_starTemperature(5778.0)
    , m_starLuminosity(1.0)
    , m_planetsModel(new PlanetListModel(this))
{
    generateName();
    updateStarProperties();
}

StarSystemQml::StarSystemQml(SystemId id, const QPointF& position, StarType type, QObject* parent)
    : QObject(parent)
    , m_id(id)
    , m_position(position)
    , m_starType(type)
    , m_systemSize(SystemSize::Medium)
    , m_starMass(1.0)
    , m_starTemperature(5778.0)
    , m_starLuminosity(1.0)
    , m_planetsModel(new PlanetListModel(this))
{
    generateName();
    updateStarProperties();
}

QColor StarSystemQml::getStarColor() const noexcept {
    return getColorForStarType(m_starType);
}

double StarSystemQml::getInfluenceRadius() const noexcept {
    switch (m_systemSize) {
        case SystemSize::Small:  return 50.0;
        case SystemSize::Medium: return 75.0;
        case SystemSize::Large:  return 100.0;
        default: return 75.0;
    }
}

void StarSystemQml::setPosition(const QPointF& position) {
    if (m_position != position) {
        m_position = position;
        emit positionChanged();
    }
}

void StarSystemQml::setStarType(StarType type) {
    if (m_starType != type) {
        m_starType = type;
        updateStarProperties();
        emit starTypeChanged();
    }
}

void StarSystemQml::setSystemSize(SystemSize size) {
    if (m_systemSize != size) {
        m_systemSize = size;
        emit systemSizeChanged();
    }
}

void StarSystemQml::setName(const QString& name) {
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

void StarSystemQml::setStarMass(double mass) {
    if (qAbs(m_starMass - mass) > 0.001) {
        m_starMass = mass;
        emit starMassChanged();
    }
}

void StarSystemQml::setStarTemperature(double temperature) {
    if (qAbs(m_starTemperature - temperature) > 0.1) {
        m_starTemperature = temperature;
        emit starTemperatureChanged();
    }
}

void StarSystemQml::setStarLuminosity(double luminosity) {
    if (qAbs(m_starLuminosity - luminosity) > 0.001) {
        m_starLuminosity = luminosity;
        emit starLuminosityChanged();
    }
}

Planet* StarSystemQml::addPlanet() {
    return m_planetsModel->addPlanet();
}

Planet* StarSystemQml::addPlanet(const QString& name, double orbitDistance) {
    return m_planetsModel->addPlanet(name, orbitDistance);
}

bool StarSystemQml::removePlanet(Planet* planet) {
    return m_planetsModel->removePlanet(planet);
}

bool StarSystemQml::removePlanetAt(int index) {
    return m_planetsModel->removePlanetAt(index);
}

Planet* StarSystemQml::getPlanetAt(int index) const {
    return m_planetsModel->getPlanetAt(index);
}

int StarSystemQml::getPlanetCount() const {
    return m_planetsModel->getPlanetCount();
}

void StarSystemQml::clearPlanets() {
    m_planetsModel->clearPlanets();
}

double StarSystemQml::distanceTo(const StarSystemQml& other) const {
    QPointF diff = m_position - other.m_position;
    return std::sqrt(diff.x() * diff.x() + diff.y() * diff.y());
}

bool StarSystemQml::isWithinRange(const StarSystemQml& other, double maxDistance) const {
    return distanceTo(other) <= maxDistance;
}

void StarSystemQml::generateName() {
    static const QStringList prefixes = {
        "Alpha", "Beta", "Gamma", "Delta", "Epsilon", "Zeta", "Eta", "Theta",
        "Iota", "Kappa", "Lambda", "Mu", "Nu", "Xi", "Omicron", "Pi",
        "Rho", "Sigma", "Tau", "Upsilon", "Phi", "Chi", "Psi", "Omega"
    };
    
    static const QStringList suffixes = {
        "Centauri", "Draconis", "Orionis", "Cygni", "Lyrae", "Aquilae",
        "Cassiopeiae", "Andromedae", "Persei", "Aurigae", "Geminorum",
        "Leonis", "Virginis", "Librae", "Scorpii", "Sagittarii"
    };
    
    auto* generator = QRandomGenerator::global();
    QString prefix = prefixes.at(generator->bounded(prefixes.size()));
    QString suffix = suffixes.at(generator->bounded(suffixes.size()));
    
    m_name = QString("%1 %2").arg(prefix, suffix);
}

void StarSystemQml::updateStarProperties() {
    // Update star properties based on star type
    switch (m_starType) {
        case StarType::RedDwarf:
            m_starMass = 0.3;
            m_starTemperature = 3500.0;
            m_starLuminosity = 0.1;
            break;
        case StarType::YellowStar:
            m_starMass = 1.0;
            m_starTemperature = 5778.0;
            m_starLuminosity = 1.0;
            break;
        case StarType::BlueStar:
            m_starMass = 15.0;
            m_starTemperature = 30000.0;
            m_starLuminosity = 10000.0;
            break;
        case StarType::RedGiant:
            m_starMass = 2.0;
            m_starTemperature = 4000.0;
            m_starLuminosity = 100.0;
            break;
        case StarType::WhiteDwarf:
            m_starMass = 0.6;
            m_starTemperature = 10000.0;
            m_starLuminosity = 0.01;
            break;
        case StarType::Neutron:
            m_starMass = 1.4;
            m_starTemperature = 1000000.0;
            m_starLuminosity = 0.001;
            break;
        case StarType::BlackHole:
            m_starMass = 10.0;
            m_starTemperature = 0.0;
            m_starLuminosity = 0.0;
            break;
    }
}

QColor StarSystemQml::getColorForStarType(StarType type) {
    switch (type) {
        case StarType::RedDwarf:   return QColor(255, 100, 100);
        case StarType::YellowStar: return QColor(255, 255, 150);
        case StarType::BlueStar:   return QColor(150, 200, 255);
        case StarType::RedGiant:   return QColor(255, 150, 100);
        case StarType::WhiteDwarf: return QColor(255, 255, 255);
        case StarType::Neutron:    return QColor(200, 200, 255);
        case StarType::BlackHole:  return QColor(50, 50, 50);
        default: return QColor(255, 255, 150);
    }
}
