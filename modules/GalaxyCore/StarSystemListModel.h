/**
 * @file StarSystemListModel.h
 * @brief QAbstractListModel for exposing star systems to QML.
 */

#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include "StarSystem.h"
#include "galaxycore_global.h"
#include <vector>
#include <memory>

/**
 * @class StarSystemListModel
 * @brief List model for star systems, used in QML views.
 */
class GALAXYCORE_EXPORT StarSystemListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        XRole,
        YRole,
        NameRole,
        TypeRole,
        SizeRole
    };

    StarSystemListModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setSystems(const std::vector<std::unique_ptr<StarSystem>>* systems);
    void notifyDataChanged();

private:
    const std::vector<std::unique_ptr<StarSystem>>* m_systems = nullptr;
};
