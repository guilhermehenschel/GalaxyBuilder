/**
 * @file PlanetListModel.cpp
 * @brief Implementation of the PlanetListModel class.
 */

#include "PlanetListModel.h"
#include <QDebug>

PlanetListModel::PlanetListModel(QObject* parent)
    : QAbstractListModel(parent) {
}

int PlanetListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return static_cast<int>(m_planets.size());
}

QVariant PlanetListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_planets.size()))
        return QVariant();

    const Planet* planet = m_planets[index.row()].get();
    
    switch (role) {
    case PlanetRole:
        return QVariant::fromValue(const_cast<Planet*>(planet));
    case NameRole:
        return planet->name();
    case TypeRole:
        return static_cast<int>(planet->type());
    case SizeRole:
        return planet->size();
    case MassRole:
        return planet->mass();
    case MoonsRole:
        return planet->numberOfMoons();
    case OrbitDistanceRole:
        return planet->orbitalRadius();
    case TemperatureRole:
        return (planet->maxTemperature() + planet->minTemperature()) / 2.0;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> PlanetListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[PlanetRole] = "planet";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    roles[MassRole] = "mass";
    roles[MoonsRole] = "moons";
    roles[OrbitDistanceRole] = "orbitDistance";
    roles[TemperatureRole] = "temperature";
    return roles;
}

Planet* PlanetListModel::addPlanet() {
    QString name = QString("Planet %1").arg(m_planets.size() + 1);
    double orbitDistance = 50.0 + m_planets.size() * 30.0; // Increment orbit distance for visibility
    return addPlanet(name, orbitDistance);
}

Planet* PlanetListModel::addPlanet(const QString& name, double orbitDistance) {
    beginInsertRows(QModelIndex(), m_planets.size(), m_planets.size());
    
    auto planet = std::make_unique<Planet>(name, PlanetType::Rocky, this);
    planet->setOrbitalRadius(orbitDistance);
    
    Planet* planetPtr = planet.get();
    connectPlanetSignals(planetPtr);
    
    m_planets.push_back(std::move(planet));
    
    endInsertRows();
    return planetPtr;
}

bool PlanetListModel::removePlanet(Planet* planet) {
    if (!planet) return false;
    
    for (size_t i = 0; i < m_planets.size(); ++i) {
        if (m_planets[i].get() == planet) {
            return removePlanetAt(static_cast<int>(i));
        }
    }
    return false;
}

bool PlanetListModel::removePlanetAt(int index) {
    if (index < 0 || index >= static_cast<int>(m_planets.size())) {
        return false;
    }
    
    beginRemoveRows(QModelIndex(), index, index);
    
    disconnectPlanetSignals(m_planets[index].get());
    m_planets.erase(m_planets.begin() + index);
    
    endRemoveRows();
    return true;
}

Planet* PlanetListModel::getPlanetAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_planets.size())) {
        return nullptr;
    }
    return m_planets[index].get();
}

int PlanetListModel::getPlanetCount() const {
    return static_cast<int>(m_planets.size());
}

void PlanetListModel::clearPlanets() {
    if (m_planets.empty()) return;
    
    beginResetModel();
    
    for (auto& planet : m_planets) {
        disconnectPlanetSignals(planet.get());
    }
    m_planets.clear();
    
    endResetModel();
}

void PlanetListModel::notifyDataChanged() {
    if (!m_planets.empty()) {
        emit dataChanged(createIndex(0, 0), createIndex(static_cast<int>(m_planets.size()) - 1, 0));
    }
}

void PlanetListModel::onPlanetChanged() {
    Planet* planet = qobject_cast<Planet*>(sender());
    if (!planet) return;
    
    // Find the planet in our list and emit dataChanged for its row
    for (size_t i = 0; i < m_planets.size(); ++i) {
        if (m_planets[i].get() == planet) {
            QModelIndex index = createIndex(static_cast<int>(i), 0);
            emit dataChanged(index, index);
            break;
        }
    }
}

void PlanetListModel::connectPlanetSignals(Planet* planet) {
    if (!planet) return;
    
    connect(planet, &Planet::nameChanged, this, &PlanetListModel::onPlanetChanged);
    connect(planet, &Planet::typeChanged, this, &PlanetListModel::onPlanetChanged);
    connect(planet, &Planet::sizeChanged, this, &PlanetListModel::onPlanetChanged);
    connect(planet, &Planet::massChanged, this, &PlanetListModel::onPlanetChanged);
    connect(planet, &Planet::numberOfMoonsChanged, this, &PlanetListModel::onPlanetChanged);
    connect(planet, &Planet::orbitalRadiusChanged, this, &PlanetListModel::onPlanetChanged);
    connect(planet, &Planet::maxTemperatureChanged, this, &PlanetListModel::onPlanetChanged);
    connect(planet, &Planet::minTemperatureChanged, this, &PlanetListModel::onPlanetChanged);
}

void PlanetListModel::disconnectPlanetSignals(Planet* planet) {
    if (!planet) return;
    
    disconnect(planet, nullptr, this, nullptr);
}
