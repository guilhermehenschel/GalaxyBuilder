#include "ggh/modules/GalaxyCore/viewmodels/TravelLaneListModel.h"

namespace ggh::GalaxyCore::viewmodels {
TravelLaneListModel::TravelLaneListModel(std::shared_ptr<models::GalaxyModel> galaxy, QObject* parent)
    : QAbstractListModel(parent), m_galaxy(galaxy)
{
}

int TravelLaneListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_galaxy ? static_cast<int>(m_galaxy->getAllTravelLanes().size()) : 0;
}

QVariant TravelLaneListModel::data(const QModelIndex& index, int role) const
{
    if (!m_galaxy || !index.isValid()) {
        return QVariant();
    }
    
    auto lanes = m_galaxy->getAllTravelLanes();
    if (index.row() >= static_cast<int>(lanes.size())) {
        return QVariant();
    }

    const auto& lane = lanes[index.row()];

    switch (role) {
        case LaneIdRole:
            return static_cast<quint32>(lane->getId());
        case FromSystemIdRole:
            return lane->getFromSystem() ? static_cast<quint32>(lane->getFromSystem()->getId()) : 0;
        case ToSystemIdRole:
            return lane->getToSystem() ? static_cast<quint32>(lane->getToSystem()->getId()) : 0;
        case FromSystemNameRole:
            return lane->getFromSystem() ? QString::fromStdString(lane->getFromSystem()->getName()) : QString();
        case ToSystemNameRole:
            return lane->getToSystem() ? QString::fromStdString(lane->getToSystem()->getName()) : QString();
        case FromXRole:
            return lane->getFromSystem() ? lane->getFromSystem()->getPosition().x : 0.0;
        case FromYRole:
            return lane->getFromSystem() ? lane->getFromSystem()->getPosition().y : 0.0;
        case ToXRole:
            return lane->getToSystem() ? lane->getToSystem()->getPosition().x : 0.0;
        case ToYRole:
            return lane->getToSystem() ? lane->getToSystem()->getPosition().y : 0.0;
        case DistanceRole:
            return lane->getLength();
        case TravelTimeRole:
            // For now, use distance as travel time
            return lane->getLength();
        case LaneTypeRole:
            // TravelLaneModel doesn't have lane type, return 0 (Standard)
            return 0;
        case IsActiveRole:
            // TravelLaneModel doesn't have active status, return true
            return true;
        default:
            return QVariant();
    }
}

bool TravelLaneListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    // TravelLanes are typically read-only after creation, but we can support some modifications if needed
    Q_UNUSED(index)
    Q_UNUSED(value)
    Q_UNUSED(role)
    
    // For now, travel lanes are immutable after creation
    return false;
}

QHash<int, QByteArray> TravelLaneListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[LaneIdRole] = "laneId";
    roles[FromSystemIdRole] = "fromSystemId";
    roles[ToSystemIdRole] = "toSystemId";
    roles[FromSystemNameRole] = "fromSystemName";
    roles[ToSystemNameRole] = "toSystemName";
    roles[FromXRole] = "fromX";
    roles[FromYRole] = "fromY";
    roles[ToXRole] = "toX";
    roles[ToYRole] = "toY";
    roles[DistanceRole] = "distance";
    roles[TravelTimeRole] = "travelTime";
    roles[LaneTypeRole] = "laneType";
    roles[IsActiveRole] = "isActive";
    return roles;
}

void TravelLaneListModel::refresh()
{
    beginResetModel();
    endResetModel();
}
}
