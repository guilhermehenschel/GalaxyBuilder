#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_LIST_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_LIST_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/System.h"

namespace ggh::Galaxy::Factions::viewmodels
{
class SystemListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SystemListModel(QObject* parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Custom methods
    void addSystem(FactionViewModel* faction);
    void removeSystem(FactionViewModel* faction);

private:
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::System>> m_systems;
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_LIST_MODEL_H