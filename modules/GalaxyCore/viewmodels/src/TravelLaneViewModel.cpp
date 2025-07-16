#include "ggh/modules/GalaxyCore/viewmodels/TravelLaneViewModel.h"

namespace ggh::GalaxyCore::viewmodels {

TravelLaneViewModel::TravelLaneViewModel(QObject* parent)
    : QObject(parent)
    , m_travelLane(nullptr) // Will need a valid constructor later
{
}

TravelLaneViewModel::TravelLaneViewModel(std::shared_ptr<models::TravelLaneModel> travelLane, QObject* parent)
    : QObject(parent), m_travelLane(travelLane)
{
}

quint32 TravelLaneViewModel::fromSystemId() const
{
    if (!m_travelLane || !m_travelLane->getFromSystem()) return 0;
    return static_cast<quint32>(m_travelLane->getFromSystem()->getId());
}

quint32 TravelLaneViewModel::toSystemId() const
{
    if (!m_travelLane || !m_travelLane->getToSystem()) return 0;
    return static_cast<quint32>(m_travelLane->getToSystem()->getId());
}

QString TravelLaneViewModel::fromSystemName() const
{
    if (!m_travelLane || !m_travelLane->getFromSystem()) return QString();
    return QString::fromStdString(m_travelLane->getFromSystem()->getName());
}

QString TravelLaneViewModel::toSystemName() const
{
    if (!m_travelLane || !m_travelLane->getToSystem()) return QString();
    return QString::fromStdString(m_travelLane->getToSystem()->getName());
}

double TravelLaneViewModel::fromX() const
{
    if (!m_travelLane) return 0.0;
    return m_travelLane->getStartPosition().x;
}

double TravelLaneViewModel::fromY() const
{
    if (!m_travelLane) return 0.0;
    return m_travelLane->getStartPosition().y;
}

double TravelLaneViewModel::toX() const
{
    if (!m_travelLane) return 0.0;
    return m_travelLane->getEndPosition().x;
}

double TravelLaneViewModel::toY() const
{
    if (!m_travelLane) return 0.0;
    return m_travelLane->getEndPosition().y;
}

double TravelLaneViewModel::distance() const
{
    if (!m_travelLane) return 0.0;
    return m_travelLane->getLength();
}

double TravelLaneViewModel::travelTime() const
{
    // TravelLaneModel doesn't have travel time, return distance as approximation
    return distance();
}

int TravelLaneViewModel::laneType() const
{
    // TravelLaneModel doesn't have lane type, return 0 (Standard)
    return 0;
}

bool TravelLaneViewModel::isActive() const
{
    // TravelLaneModel doesn't have active status, return true
    return m_travelLane != nullptr;
}

void TravelLaneViewModel::setFromSystemId(quint32 systemId)
{
    // TravelLaneModel doesn't support changing system IDs after construction
    // This would require recreating the lane
    Q_UNUSED(systemId)
}

void TravelLaneViewModel::setToSystemId(quint32 systemId)
{
    // TravelLaneModel doesn't support changing system IDs after construction
    // This would require recreating the lane
    Q_UNUSED(systemId)
}

void TravelLaneViewModel::setTravelTime(double time)
{
    // TravelLaneModel doesn't have travel time property
    Q_UNUSED(time)
}

void TravelLaneViewModel::setLaneType(int type)
{
    // TravelLaneModel doesn't have lane type property
    Q_UNUSED(type)
}

void TravelLaneViewModel::setIsActive(bool active)
{
    // TravelLaneModel doesn't have active property
    Q_UNUSED(active)
}

QString TravelLaneViewModel::toXml() const
{
    if (!m_travelLane) return QString();
    return QString::fromStdString(m_travelLane->toXml());
}

QString TravelLaneViewModel::laneTypeString() const
{
    // Since TravelLaneModel doesn't have lane types, return "Standard"
    return "Standard";
}

void TravelLaneViewModel::updatePositions(double fromX, double fromY, double toX, double toY)
{
    // TravelLaneModel doesn't support updating positions after construction
    Q_UNUSED(fromX)
    Q_UNUSED(fromY)
    Q_UNUSED(toX)
    Q_UNUSED(toY)
}

std::shared_ptr<models::TravelLaneModel> TravelLaneViewModel::travelLane() const
{
    return m_travelLane;
}

void TravelLaneViewModel::setTravelLane(std::shared_ptr<models::TravelLaneModel> travelLane)
{
    if (m_travelLane != travelLane) {
        m_travelLane = travelLane;
        
        // Emit all change signals
        emit fromSystemIdChanged();
        emit toSystemIdChanged();
        emit fromSystemNameChanged();
        emit toSystemNameChanged();
        emit fromPositionChanged();
        emit toPositionChanged();
        emit distanceChanged();
        emit travelTimeChanged();
        emit laneTypeChanged();
        emit isActiveChanged();
    }
}

} // namespace ggh::GalaxyCore::viewmodels
