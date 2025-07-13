#include "StarSystemListModel.h"

StarSystemListModel::StarSystemListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int StarSystemListModel::rowCount(const QModelIndex& parent) const {
    if (parent.isValid() || !m_systems) return 0;
    return static_cast<int>(m_systems->size());
}

QVariant StarSystemListModel::data(const QModelIndex& index, int role) const {
    if (!m_systems || !index.isValid() || index.row() < 0 || index.row() >= static_cast<int>(m_systems->size()))
        return QVariant();
    const StarSystem* system = m_systems->at(index.row()).get();
    switch (role) {
    case IdRole: return QVariant::fromValue(system->getId());
    case XRole: return system->getPosition().x();
    case YRole: return system->getPosition().y();
    case NameRole: return system->getName();
    case TypeRole: return static_cast<int>(system->getStarType());
    case SizeRole: return static_cast<int>(system->getSystemSize());
    default: return QVariant();
    }
}

QHash<int, QByteArray> StarSystemListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[XRole] = "x";
    roles[YRole] = "y";
    roles[NameRole] = "name";
    roles[TypeRole] = "type";
    roles[SizeRole] = "size";
    return roles;
}

void StarSystemListModel::setSystems(const std::vector<std::unique_ptr<StarSystem>>* systems) {
    beginResetModel();
    m_systems = systems;
    endResetModel();
}

void StarSystemListModel::notifyDataChanged() {
    if (m_systems && !m_systems->empty()) {
        emit dataChanged(createIndex(0, 0), createIndex(static_cast<int>(m_systems->size()) - 1, 0));
    }
}
