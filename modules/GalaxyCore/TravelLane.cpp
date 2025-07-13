#include "TravelLane.h"
#include <cmath>

TravelLane::TravelLane(LaneId id, SystemId fromSystem, SystemId toSystem,
                       const QPointF& startPos, const QPointF& endPos)
    : m_id(id), m_fromSystem(fromSystem), m_toSystem(toSystem),
      m_startPos(startPos), m_endPos(endPos), m_color(QColor(100, 200, 255, 128)) {
}

double TravelLane::getLength() const noexcept {
    const double dx = m_endPos.x() - m_startPos.x();
    const double dy = m_endPos.y() - m_startPos.y();
    return std::sqrt(dx * dx + dy * dy);
}
