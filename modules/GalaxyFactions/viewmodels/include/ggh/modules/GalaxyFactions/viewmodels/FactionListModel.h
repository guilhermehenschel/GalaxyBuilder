#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_LIST_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_LIST_MODEL_H

#include <QAbstractListModel>

#include <memory>

#include "ggh/modules/GalaxyFactions/models/Faction.h"

namespace namespace ggh::Galaxy::Factions::viewmodels
{
class FactionListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        DescriptionRole,
        ColorRole
    };
    Q_ENUM(Roles)

    explicit FactionListModel(QObject* parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Model manipulation methods
    void addFaction(const ggh::Galaxy::Factions::models::Faction& faction);
    void clearFactions();
    void removeFaction(int id);
    ggh::Galaxy::Factions::models::Faction getFaction(int id) const;
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>> getFactions() const;
    void setFactions(const QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>>& factions);

 private:
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>> m_factions;

};

} // namespace ggh::Galaxy::Factions::viewmodels


#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_FACTION_LIST_MODEL_H