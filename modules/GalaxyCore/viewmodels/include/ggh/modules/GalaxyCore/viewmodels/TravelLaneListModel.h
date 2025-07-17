#ifndef GGH_MODULES_GALAXYCORE_VIEWMODELS_TRAVEL_LANE_LIST_MODEL_H
#define GGH_MODULES_GALAXYCORE_VIEWMODELS_TRAVEL_LANE_LIST_MODEL_H

#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <memory>
#include <QtQml/qqml.h>

#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"

namespace ggh::GalaxyCore::viewmodels {
class TravelLaneListModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Cannot create instances of TravelLaneListModel directly, must be created through GalaxyModel")

public:
    enum Roles {
        LaneIdRole = Qt::UserRole + 1,
        FromSystemIdRole,
        ToSystemIdRole,
        FromSystemNameRole,
        ToSystemNameRole,
        FromXRole,
        FromYRole,
        ToXRole,
        ToYRole,
        DistanceRole,
        TravelTimeRole,
        LaneTypeRole,
        IsActiveRole
    };
    Q_ENUM(Roles)

    explicit TravelLaneListModel(std::shared_ptr<models::GalaxyModel> galaxy, QObject* parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    // Model update methods
    void refresh();

private:
    std::shared_ptr<models::GalaxyModel> m_galaxy;
};
}

Q_DECLARE_METATYPE(ggh::GalaxyCore::viewmodels::TravelLaneListModel*)

#endif // !GGH_MODULES_GALAXYCORE_VIEWMODELS_TRAVEL_LANE_LIST_MODEL_H
