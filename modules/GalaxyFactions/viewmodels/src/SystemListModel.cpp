/**
 * @file SystemListModel.cpp
 * @brief Implementation of SystemListModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/SystemListModel.h"

namespace ggh::Galaxy::Factions::viewmodels {

SystemListModel::SystemListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int SystemListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_systems.size();
}

QVariant SystemListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_systems.size()) {
        return QVariant();
    }

    const auto& system = m_systems.at(index.row());
    if (!system) {
        return QVariant();
    }

    switch (role) {
    case IdRole:
        return system->id();
    case NameRole:
        return QString::fromStdString(system->name());
    case DescriptionRole:
        return QString::fromStdString(system->description());
    case TypeRole:
        // Note: System model might not have a type property
        // This is a placeholder implementation
        return QString("System");
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SystemListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[TypeRole] = "type";
    return roles;
}

void SystemListModel::addSystem(std::shared_ptr<ggh::Galaxy::Factions::models::System> system)
{
    if (!system) {
        return;
    }

    beginInsertRows(QModelIndex(), m_systems.size(), m_systems.size());
    m_systems.append(system);
    endInsertRows();
}

void SystemListModel::removeSystem(int systemId)
{
    for (int i = 0; i < m_systems.size(); ++i) {
        if (m_systems[i] && m_systems[i]->id() == systemId) {
            beginRemoveRows(QModelIndex(), i, i);
            m_systems.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void SystemListModel::clearSystems()
{
    beginResetModel();
    m_systems.clear();
    endResetModel();
}

QList<std::shared_ptr<ggh::Galaxy::Factions::models::System>> SystemListModel::getSystems() const
{
    return m_systems;
}

void SystemListModel::setSystems(const QList<std::shared_ptr<ggh::Galaxy::Factions::models::System>>& systems)
{
    beginResetModel();
    m_systems = systems;
    endResetModel();
}

} // namespace ggh::Galaxy::Factions::viewmodels
