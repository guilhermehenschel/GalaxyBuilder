/**
 * @file FactionListModel.cpp
 * @brief Implementation of FactionListModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include "ggh/modules/GalaxyFactions/viewmodels/FactionListModel.h"

namespace ggh::Galaxy::Factions::viewmodels {

FactionListModel::FactionListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int FactionListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_factions.size();
}

QVariant FactionListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_factions.size()) {
        return QVariant();
    }

    const auto& faction = m_factions.at(index.row());
    if (!faction) {
        return QVariant();
    }

    switch (role) {
    case IdRole:
        return faction->id();
    case NameRole:
        return QString::fromStdString(faction->name());
    case DescriptionRole:
        return QString::fromStdString(faction->description());
    case ColorRole:
        return QString::fromStdString(faction->color());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> FactionListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[DescriptionRole] = "description";
    roles[ColorRole] = "color";
    return roles;
}

void FactionListModel::addFaction(const ggh::Galaxy::Factions::models::Faction& faction)
{
    beginInsertRows(QModelIndex(), m_factions.size(), m_factions.size());
    m_factions.append(std::make_shared<ggh::Galaxy::Factions::models::Faction>(faction));
    endInsertRows();
}

void FactionListModel::clearFactions()
{
    beginResetModel();
    m_factions.clear();
    endResetModel();
}

void FactionListModel::removeFaction(int id)
{
    for (int i = 0; i < m_factions.size(); ++i) {
        if (m_factions[i] && m_factions[i]->id() == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_factions.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}

ggh::Galaxy::Factions::models::Faction FactionListModel::getFaction(int id) const
{
    for (const auto& faction : m_factions) {
        if (faction && faction->id() == id) {
            return *faction;
        }
    }
    // Return default faction if not found
    return ggh::Galaxy::Factions::models::Faction(-1, "", "", "");
}

QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>> FactionListModel::getFactions() const
{
    return m_factions;
}

void FactionListModel::setFactions(const QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>>& factions)
{
    beginResetModel();
    m_factions = factions;
    endResetModel();
}

} // namespace ggh::Galaxy::Factions::viewmodels
