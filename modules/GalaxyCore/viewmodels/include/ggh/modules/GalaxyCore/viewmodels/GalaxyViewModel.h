#ifndef GGH_GALAXYCORE_VIEWMODELS_GALAXYVIEWMODEL_H
#define GGH_GALAXYCORE_VIEWMODELS_GALAXYVIEWMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QString>
#include <memory>
#include <QtQml/qqml.h>

#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/StarSystemListModel.h"
namespace ggh::GalaxyCore::viewmodels {

class GalaxyViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 width READ width WRITE setWidth NOTIFY dimensionsChanged)
    Q_PROPERTY(qint32 height READ height WRITE setHeight NOTIFY dimensionsChanged)
    Q_PROPERTY(quint32 systemCount READ systemCount NOTIFY systemCountChanged)
    Q_PROPERTY(ggh::GalaxyCore::viewmodels::StarSystemListModel* starSystems READ starSystems CONSTANT)

    QML_ELEMENT

public:
    explicit GalaxyViewModel(QObject* parent = nullptr);
    explicit GalaxyViewModel(std::shared_ptr<models::GalaxyModel> galaxy, QObject* parent = nullptr);

    // Property getters
    qint32 width() const;
    qint32 height() const;
    quint32 systemCount() const;
    StarSystemListModel* starSystems();

    // Property setters
    void setWidth(qint32 width);
    void setHeight(qint32 height);
    void setDimensions(qint32 width, qint32 height);

    // QML invokable methods
    Q_INVOKABLE QString toXml() const;
    Q_INVOKABLE void addStarSystem(quint32 systemId, const QString& name, double x, double y);
    Q_INVOKABLE bool removeStarSystem(quint32 systemId);
    Q_INVOKABLE void clearSystems();

    // Model access
    std::shared_ptr<models::GalaxyModel> galaxy() const;
    void setGalaxy(std::shared_ptr<models::GalaxyModel> galaxy);

signals:
    void dimensionsChanged();
    void systemCountChanged();

private:
    void initializeStarSystemsModel();

    std::shared_ptr<models::GalaxyModel> m_galaxy;
    std::unique_ptr<StarSystemListModel> m_starSystemsModel;
};
} // namespace ggh::GalaxyCore::viewmodels

Q_DECLARE_METATYPE(ggh::GalaxyCore::viewmodels::GalaxyViewModel*)

#endif // GGH_GALAXYCORE_VIEWMODELS_GALAXYVIEWMODEL_H