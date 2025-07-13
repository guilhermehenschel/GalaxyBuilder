#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include "TravelLane.h"
#include "galaxycore_global.h"
#include <vector>
#include <memory>

class GALAXYCORE_EXPORT TravelLaneListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        FromSystemRole,
        ToSystemRole,
        StartXRole,
        StartYRole,
        EndXRole,
        EndYRole
    };

    TravelLaneListModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setLanes(const std::vector<std::unique_ptr<TravelLane>>* lanes);

private:
    const std::vector<std::unique_ptr<TravelLane>>* m_lanes = nullptr;
};
