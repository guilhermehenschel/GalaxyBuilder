#include "ggh/modules/GalaxyCore/viewmodels/StarSystemListModel.h"

namespace ggh::GalaxyCore::viewmodels {
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

bool StarSystemListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!m_galaxy || !index.isValid() || index.row() >= rowCount()) {
        return false;
    }

    auto systems{m_galaxy->getAllStarSystems()};
    if (index.row() >= static_cast<int>(systems.size())) {
        return false;
    }

    auto& system{systems[index.row()]};

    switch (role) {
        case NameRole:
            system->setName(value.toString().toStdString());
            break;
        case PositionXRole: {
            auto pos = system->getPosition();
            pos.x = value.toDouble();
            system->setPosition(pos);
            break;
        }
        case PositionYRole: {
            auto pos = system->getPosition();
            pos.y = value.toDouble();
            system->setPosition(pos);
            break;
        }
        case StarTypeRole:
            system->setStarType(static_cast<utilities::StarType>(value.toInt()));
            break;
        default:
            return false; // Unsupported role
    }

    emit dataChanged(index, index, {role});
    return true;
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
}