#ifndef GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_LIST_MODEL_H
#define GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_LIST_MODEL_H

#include <QAbstractListModel>
#include <QList>
#include <memory>

#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"

namespace ggh::Galaxy::Factions::viewmodels
{
class SystemResourceBonusListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit SystemResourceBonusListModel(QObject* parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Custom methods
    void addResourceBonus(std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> resourceBonus);
    void removeResourceBonus(std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> resourceBonus);

private:
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus>> m_resourceBonuses;
};
}

#endif // !GGH_MODULES_GALAXYFACTIONS_VIEWMODELS_SYSTEM_RESOURCE_BONUS_LIST_MODEL_H