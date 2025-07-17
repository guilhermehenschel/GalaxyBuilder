#ifndef GGH_GALAXYCORE_VIEWMODELS_TRAVELLANEVIEWMODEL_H
#define GGH_GALAXYCORE_VIEWMODELS_TRAVELLANEVIEWMODEL_H

#include <QObject>
#include <QString>
#include <QtQml>
#include <memory>

#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"

namespace ggh::GalaxyCore::viewmodels {

class TravelLaneViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(quint32 fromSystemId READ fromSystemId WRITE setFromSystemId NOTIFY fromSystemIdChanged)
    Q_PROPERTY(quint32 toSystemId READ toSystemId WRITE setToSystemId NOTIFY toSystemIdChanged)
    Q_PROPERTY(QString fromSystemName READ fromSystemName NOTIFY fromSystemNameChanged)
    Q_PROPERTY(QString toSystemName READ toSystemName NOTIFY toSystemNameChanged)
    Q_PROPERTY(double fromX READ fromX NOTIFY fromPositionChanged)
    Q_PROPERTY(double fromY READ fromY NOTIFY fromPositionChanged)
    Q_PROPERTY(double toX READ toX NOTIFY toPositionChanged)
    Q_PROPERTY(double toY READ toY NOTIFY toPositionChanged)
    Q_PROPERTY(double distance READ distance NOTIFY distanceChanged)
    Q_PROPERTY(double travelTime READ travelTime WRITE setTravelTime NOTIFY travelTimeChanged)
    Q_PROPERTY(int laneType READ laneType WRITE setLaneType NOTIFY laneTypeChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)

public:
    explicit TravelLaneViewModel(QObject* parent = nullptr);
    explicit TravelLaneViewModel(std::shared_ptr<models::TravelLaneModel> travelLane, QObject* parent = nullptr);

    // Property getters
    quint32 fromSystemId() const;
    quint32 toSystemId() const;
    QString fromSystemName() const;
    QString toSystemName() const;
    double fromX() const;
    double fromY() const;
    double toX() const;
    double toY() const;
    double distance() const;
    double travelTime() const;
    int laneType() const;
    bool isActive() const;

    // Property setters
    void setFromSystemId(quint32 systemId);
    void setToSystemId(quint32 systemId);
    void setTravelTime(double time);
    void setLaneType(int type);
    void setIsActive(bool active);

    // QML invokable methods
    Q_INVOKABLE QString toXml() const;
    Q_INVOKABLE QString laneTypeString() const;
    Q_INVOKABLE void updatePositions(double fromX, double fromY, double toX, double toY);

    // Model access
    std::shared_ptr<models::TravelLaneModel> travelLane() const;
    void setTravelLane(std::shared_ptr<models::TravelLaneModel> travelLane);

signals:
    void fromSystemIdChanged();
    void toSystemIdChanged();
    void fromSystemNameChanged();
    void toSystemNameChanged();
    void fromPositionChanged();
    void toPositionChanged();
    void distanceChanged();
    void travelTimeChanged();
    void laneTypeChanged();
    void isActiveChanged();

private:
    std::shared_ptr<models::TravelLaneModel> m_travelLane;
};

} // namespace ggh::GalaxyCore::viewmodels

Q_DECLARE_METATYPE(ggh::GalaxyCore::viewmodels::TravelLaneViewModel*)

#endif // GGH_GALAXYCORE_VIEWMODELS_TRAVELLANEVIEWMODEL_H