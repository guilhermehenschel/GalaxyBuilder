#include "TravelLaneListModel.h"

TravelLaneListModel::TravelLaneListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int TravelLaneListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid() || !m_lanes) return 0;
    return static_cast<int>(m_lanes->size());
}

QVariant TravelLaneListModel::data(const QModelIndex& index, int role) const {
    if (!m_lanes || !index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_lanes->size()))
        return QVariant();
    const TravelLane* lane = m_lanes->at(index.row()).get();
    switch (role) {
    case IdRole: return QVariant::fromValue(lane->getId());
    case FromSystemRole: return QVariant::fromValue(lane->getFromSystem());
    case ToSystemRole: return QVariant::fromValue(lane->getToSystem());
    case StartXRole: return lane->getStartPosition().x();
    case StartYRole: return lane->getStartPosition().y();
    case EndXRole: return lane->getEndPosition().x();
    case EndYRole: return lane->getEndPosition().y();
    default: return QVariant();
    }
}

QHash<int, QByteArray> TravelLaneListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[FromSystemRole] = "fromSystem";
    roles[ToSystemRole] = "toSystem";
    roles[StartXRole] = "startX";
    roles[StartYRole] = "startY";
    roles[EndXRole] = "endX";
    roles[EndYRole] = "endY";
    return roles;
}

void TravelLaneListModel::setLanes(const std::vector<std::unique_ptr<TravelLane>>* lanes) {
    beginResetModel();
    m_lanes = lanes;
    endResetModel();
}
