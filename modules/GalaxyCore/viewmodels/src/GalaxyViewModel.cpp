#include "ggh/modules/GalaxyCore/viewmodels/GalaxyViewModel.h"

namespace ggh::GalaxyCore::viewmodels {

// GalaxyViewModel implementation
GalaxyViewModel::GalaxyViewModel(QObject* parent)
    : QObject(parent)
    , m_galaxy(std::make_shared<models::GalaxyModel>(1000, 800))
{
    initializeStarSystemsModel();
}

GalaxyViewModel::GalaxyViewModel(std::shared_ptr<models::GalaxyModel> galaxy, QObject* parent)
    : QObject(parent), m_galaxy(galaxy)
{
    if (!m_galaxy) {
        m_galaxy = std::make_shared<models::GalaxyModel>(1000, 800);
    }
    initializeStarSystemsModel();
}

qint32 GalaxyViewModel::width() const
{
    return static_cast<qint32>(m_galaxy->getWidth());
}

qint32 GalaxyViewModel::height() const
{
    return static_cast<qint32>(m_galaxy->getHeight());
}

quint32 GalaxyViewModel::systemCount() const
{
    return static_cast<quint32>(m_galaxy->getAllStarSystems().size());
}

QAbstractListModel* GalaxyViewModel::starSystems()
{
    return m_starSystemsModel.get();
}

void GalaxyViewModel::setWidth(qint32 width)
{
    if (m_galaxy->getWidth() != width) {
        m_galaxy->setWidth(width);
        emit dimensionsChanged();
    }
}

void GalaxyViewModel::setHeight(qint32 height)
{
    if (m_galaxy->getHeight() != height) {
        m_galaxy->setHeight(height);
        emit dimensionsChanged();
    }
}

void GalaxyViewModel::setDimensions(qint32 width, qint32 height)
{
    bool changed = false;
    if (m_galaxy->getWidth() != width) {
        m_galaxy->setWidth(width);
        changed = true;
    }
    if (m_galaxy->getHeight() != height) {
        m_galaxy->setHeight(height);
        changed = true;
    }
    if (changed) {
        emit dimensionsChanged();
    }
}

QString GalaxyViewModel::toXml() const
{
    return QString::fromStdString(m_galaxy->toXml());
}

void GalaxyViewModel::addStarSystem(quint32 systemId, const QString& name, double x, double y)
{
    utilities::CartesianCoordinates<double> position(x, y);
    m_galaxy->addStarSystem(static_cast<utilities::SystemId>(systemId), name.toStdString(), position);
    
    m_starSystemsModel->refresh();
    emit systemCountChanged();
}

bool GalaxyViewModel::removeStarSystem(quint32 systemId)
{
    bool removed = m_galaxy->removeStarSystem(static_cast<utilities::SystemId>(systemId));
    if (removed) {
        m_starSystemsModel->refresh();
        emit systemCountChanged();
    }
    return removed;
}

void GalaxyViewModel::clearSystems()
{
    // Clear all systems by removing them one by one
    auto systems = m_galaxy->getAllStarSystems();
    for (const auto& system : systems) {
        m_galaxy->removeStarSystem(system->getId());
    }
    
    m_starSystemsModel->refresh();
    emit systemCountChanged();
}

std::shared_ptr<models::GalaxyModel> GalaxyViewModel::galaxy() const
{
    return m_galaxy;
}

void GalaxyViewModel::setGalaxy(std::shared_ptr<models::GalaxyModel> galaxy)
{
    if (m_galaxy != galaxy) {
        m_galaxy = galaxy;
        if (!m_galaxy) {
            m_galaxy = std::make_shared<models::GalaxyModel>(1000, 800);
        }
        initializeStarSystemsModel();
        
        // Emit all change signals
        emit dimensionsChanged();
        emit systemCountChanged();
    }
}

void GalaxyViewModel::initializeStarSystemsModel()
{
    m_starSystemsModel = std::make_unique<StarSystemListModel>(m_galaxy, this);
}

// StarSystemListModel implementation
StarSystemListModel::StarSystemListModel(std::shared_ptr<models::GalaxyModel> galaxy, QObject* parent)
    : QAbstractListModel(parent), m_galaxy(galaxy)
{
}

int StarSystemListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_galaxy ? static_cast<int>(m_galaxy->getAllStarSystems().size()) : 0;
}

QVariant StarSystemListModel::data(const QModelIndex& index, int role) const
{
    if (!m_galaxy || !index.isValid()) {
        return QVariant();
    }
    
    auto systems = m_galaxy->getAllStarSystems();
    if (index.row() >= static_cast<int>(systems.size())) {
        return QVariant();
    }

    const auto& system = systems[index.row()];

    switch (role) {
        case SystemIdRole:
            return static_cast<quint32>(system->getId());
        case NameRole:
            return QString::fromStdString(system->getName());
        case PositionXRole:
            return system->getPosition().x;
        case PositionYRole:
            return system->getPosition().y;
        case StarTypeRole:
            return static_cast<int>(system->getStarType());
        case SystemSizeRole:
            return static_cast<int>(system->getSystemSize());
        case PlanetCountRole:
            return static_cast<int>(system->getPlanets().size());
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> StarSystemListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SystemIdRole] = "systemId";
    roles[NameRole] = "name";
    roles[PositionXRole] = "positionX";
    roles[PositionYRole] = "positionY";
    roles[StarTypeRole] = "starType";
    roles[SystemSizeRole] = "systemSize";
    roles[PlanetCountRole] = "planetCount";
    return roles;
}

void StarSystemListModel::refresh()
{
    beginResetModel();
    endResetModel();
}

} // namespace ggh::GalaxyCore::viewmodels
