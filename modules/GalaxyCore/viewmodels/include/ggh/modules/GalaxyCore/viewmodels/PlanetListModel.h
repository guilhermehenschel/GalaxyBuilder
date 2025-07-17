#ifndef GGH_MODELS_GALAXYCORE_PLANETLISTMODEL_H
#define GGH_MODELS_GALAXYCORE_PLANETLISTMODEL_H

#include <QAbstractListModel>
#include <memory>
#include <QtQml/qqml.h>

#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"

namespace ggh::GalaxyCore::viewmodels {
/**
 * @class PlanetListModel
 * @brief Abstract list model for planets in a star system
 */
class PlanetListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("This class not creatable in QML, use StarSystemViewModel instead")

public:
    enum PlanetRoles {
        NameRole = Qt::UserRole + 1,
        TypeRole,
        SizeRole,
        MassRole,
        MoonsRole,
        OrbitRole,
        MaxTempRole,
        MinTempRole
    };
    Q_ENUM(PlanetRoles)

    explicit PlanetListModel(std::shared_ptr<models::StarSystemModel> starSystem, QObject* parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Model mutation methods
    void refresh();

private:
    std::shared_ptr<models::StarSystemModel> m_starSystem;
};
}

#endif // !GGH_MODELS_GALAXYCORE_PLANETLISTMODEL_H