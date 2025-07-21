/**
 * @file SystemResourceBonusListModel.cpp
 * @brief Implementation of SystemResourceBonusListModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/SystemResourceBonusListModel.h"

namespace ggh::Galaxy::Factions::viewmodels {

SystemResourceBonusListModel::SystemResourceBonusListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int SystemResourceBonusListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_resourceBonuses.size();
}

QVariant SystemResourceBonusListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_resourceBonuses.size()) {
        return QVariant();
    }

    const auto& resourceBonus = m_resourceBonuses.at(index.row());
    if (!resourceBonus) {
        return QVariant();
    }

    switch (role) {
    case IdRole:
        return resourceBonus->id();
    case NameRole:
        return QString::fromStdString(resourceBonus->resourceType().name());
    case ValueRole:
        return resourceBonus->bonusAmount();
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> SystemResourceBonusListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[ValueRole] = "value";
    return roles;
}

void SystemResourceBonusListModel::addResourceBonus(std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> resourceBonus)
{
    if (!resourceBonus) {
        return;
    }

    beginInsertRows(QModelIndex(), m_resourceBonuses.size(), m_resourceBonuses.size());
    m_resourceBonuses.append(resourceBonus);
    endInsertRows();
}

void SystemResourceBonusListModel::removeResourceBonus(int resourceBonusId)
{
    for (int i = 0; i < m_resourceBonuses.size(); ++i) {
        if (m_resourceBonuses[i] && m_resourceBonuses[i]->id() == resourceBonusId) {
            beginRemoveRows(QModelIndex(), i, i);
            m_resourceBonuses.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

void SystemResourceBonusListModel::clearResourceBonuses()
{
    beginResetModel();
    m_resourceBonuses.clear();
    endResetModel();
}

QList<std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus>> SystemResourceBonusListModel::getResourceBonuses() const
{
    return m_resourceBonuses;
}

void SystemResourceBonusListModel::setResourceBonuses(const QList<std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus>>& resourceBonuses)
{
    beginResetModel();
    m_resourceBonuses = resourceBonuses;
    endResetModel();
}

} // namespace ggh::Galaxy::Factions::viewmodels
