#include "ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"

namespace ggh::GalaxyCore::viewmodels {

// StarSystemViewModel implementation
StarSystemViewModel::StarSystemViewModel(QObject* parent)
    : QObject(parent)
    , m_starSystem(std::make_shared<models::StarSystemModel>(0, "New System", 
                                                           utilities::CartesianCoordinates<double>(0.0, 0.0)))
{
    initializePlanetsModel();
}

StarSystemViewModel::StarSystemViewModel(std::shared_ptr<models::StarSystemModel> starSystem, QObject* parent)
    : QObject(parent), m_starSystem(starSystem)
{
    if (!m_starSystem) {
        m_starSystem = std::make_shared<models::StarSystemModel>(0, "New System", 
                                                               utilities::CartesianCoordinates<double>(0.0, 0.0));
    }
    initializePlanetsModel();
}

QString StarSystemViewModel::name() const
{
    return QString::fromStdString(m_starSystem->getName());
}

quint32 StarSystemViewModel::systemId() const
{
    return static_cast<quint32>(m_starSystem->getId());
}

double StarSystemViewModel::positionX() const
{
    return m_starSystem->getPosition().x;
}

double StarSystemViewModel::positionY() const
{
    return m_starSystem->getPosition().y;
}

int StarSystemViewModel::starType() const
{
    return static_cast<int>(m_starSystem->getStarType());
}

int StarSystemViewModel::systemSize() const
{
    return static_cast<int>(m_starSystem->getSystemSize());
}

QAbstractListModel* StarSystemViewModel::planets()
{
    return m_planetsModel.get();
}

void StarSystemViewModel::setName(const QString& name)
{
    if (QString::fromStdString(m_starSystem->getName()) != name) {
        m_starSystem->setName(name.toStdString());
        emit nameChanged();
    }
}

void StarSystemViewModel::setPositionX(double x)
{
    auto currentPos = m_starSystem->getPosition();
    if (currentPos.x != x) {
        m_starSystem->setPosition(utilities::CartesianCoordinates<double>(x, currentPos.y));
        emit positionChanged();
    }
}

void StarSystemViewModel::setPositionY(double y)
{
    auto currentPos = m_starSystem->getPosition();
    if (currentPos.y != y) {
        m_starSystem->setPosition(utilities::CartesianCoordinates<double>(currentPos.x, y));
        emit positionChanged();
    }
}

void StarSystemViewModel::setPosition(double x, double y)
{
    auto currentPos = m_starSystem->getPosition();
    if (currentPos.x != x || currentPos.y != y) {
        m_starSystem->setPosition(utilities::CartesianCoordinates<double>(x, y));
        emit positionChanged();
    }
}

void StarSystemViewModel::setStarType(int type)
{
    auto starType = static_cast<StarType>(type);
    if (m_starSystem->getStarType() != starType) {
        m_starSystem->setStarType(starType);
        emit starTypeChanged();
    }
}

void StarSystemViewModel::setSystemSize(int size)
{
    auto systemSize = static_cast<SystemSize>(size);
    if (m_starSystem->getSystemSize() != systemSize) {
        m_starSystem->setSystemSize(systemSize);
        emit systemSizeChanged();
    }
}

std::shared_ptr<models::StarSystemModel> StarSystemViewModel::starSystem() const
{
    return m_starSystem;
}

void StarSystemViewModel::setStarSystem(std::shared_ptr<models::StarSystemModel> starSystem)
{
    if (m_starSystem != starSystem) {
        m_starSystem = starSystem;
        if (!m_starSystem) {
            m_starSystem = std::make_shared<models::StarSystemModel>(0, "New System", 
                                                                   utilities::CartesianCoordinates<double>(0.0, 0.0));
        }
        initializePlanetsModel();
        
        // Emit all change signals
        emit nameChanged();
        emit positionChanged();
        emit starTypeChanged();
        emit systemSizeChanged();
    }
}

QString StarSystemViewModel::toXml() const
{
    return QString::fromStdString(m_starSystem->toXml());
}

QString StarSystemViewModel::starTypeString() const
{
    switch (m_starSystem->getStarType()) {
        case StarType::RedDwarf: return "Red Dwarf";
        case StarType::YellowStar: return "Yellow Star";
        case StarType::BlueStar: return "Blue Star";
        case StarType::WhiteDwarf: return "White Dwarf";
        case StarType::RedGiant: return "Red Giant";
        case StarType::Neutron: return "Neutron Star";
        case StarType::BlackHole: return "Black Hole";
        default: return "Unknown";
    }
}

QString StarSystemViewModel::systemSizeString() const
{
    switch (m_starSystem->getSystemSize()) {
        case SystemSize::Small: return "Small";
        case SystemSize::Medium: return "Medium";
        case SystemSize::Large: return "Large";
        case SystemSize::Huge: return "Huge";
        default: return "Unknown";
    }
}

void StarSystemViewModel::addPlanet(const QString& name, int type, double size, double mass, 
                                   int moons, double radius, double maxTemp, double minTemp)
{
    auto planetType = static_cast<utilities::PlanetType>(type);
    models::Planet planet(name.toStdString(), planetType, size, mass, moons, radius, maxTemp, minTemp);
    m_starSystem->addPlanet(planet);
    m_planetsModel->refresh();
}

bool StarSystemViewModel::removePlanetByName(const QString& name)
{
    // Find planet by name and remove it
    const auto& planets = m_starSystem->getPlanets();
    auto it = std::find_if(planets.begin(), planets.end(), 
                          [&name](const models::Planet& planet) {
                              return QString::fromStdString(planet.name()) == name;
                          });
    
    if (it != planets.end()) {
        bool removed = m_starSystem->removePlanet(*it);
        if (removed) {
            m_planetsModel->refresh();
        }
        return removed;
    }
    return false;
}

int StarSystemViewModel::planetCount() const
{
    return static_cast<int>(m_starSystem->getPlanets().size());
}

void StarSystemViewModel::initializePlanetsModel()
{
    m_planetsModel = std::make_unique<PlanetListModel>(m_starSystem, this);
}

// PlanetListModel implementation
PlanetListModel::PlanetListModel(std::shared_ptr<models::StarSystemModel> starSystem, QObject* parent)
    : QAbstractListModel(parent), m_starSystem(starSystem)
{
}

int PlanetListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_starSystem ? static_cast<int>(m_starSystem->getPlanets().size()) : 0;
}

QVariant PlanetListModel::data(const QModelIndex& index, int role) const
{
    if (!m_starSystem || !index.isValid() || index.row() >= static_cast<int>(m_starSystem->getPlanets().size())) {
        return QVariant();
    }

    const auto& planet = m_starSystem->getPlanets()[index.row()];

    switch (role) {
        case NameRole:
            return QString::fromStdString(planet.name());
        case TypeRole:
            return static_cast<int>(planet.type());
        case SizeRole:
            return planet.size();
        case MassRole:
            return planet.mass();
        case MoonsRole:
            return planet.numberOfMoons();
        case OrbitRole:
            return planet.orbitalRadius();
        case MaxTempRole:
            return planet.maxTemperature();
        case MinTempRole:
            return planet.minTemperature();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> PlanetListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[TypeRole] = "planetType";
    roles[SizeRole] = "size";
    roles[MassRole] = "mass";
    roles[MoonsRole] = "numberOfMoons";
    roles[OrbitRole] = "orbitalRadius";
    roles[MaxTempRole] = "maxTemperature";
    roles[MinTempRole] = "minTemperature";
    return roles;
}

void PlanetListModel::refresh()
{
    beginResetModel();
    endResetModel();
}

} // namespace ggh::GalaxyCore::viewmodels
