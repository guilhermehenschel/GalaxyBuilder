#pragma once

#include "Common.h"
#include <QPointF>
#include <QColor>

/**
 * @class TravelLane
 * @brief Represents a travel lane connecting two star systems.
 */
class TravelLane {
public:
    /**
     * @brief Construct a new Travel Lane object
     * 
     * @param id Lane identifier
     * @param fromSystem System identifier of the starting system
     * @param toSystem System identifier of the destination system
     * @param startPos Starting position of the lane
     * @param endPos Ending position of the lane
     */
    TravelLane(LaneId id, SystemId fromSystem, SystemId toSystem, 
               const QPointF& startPos, const QPointF& endPos);
    ~TravelLane() = default;

    // Copy and move constructors/operators
    TravelLane(const TravelLane&) = default;
    TravelLane& operator=(const TravelLane&) = default;
    TravelLane(TravelLane&&) = default;
    TravelLane& operator=(TravelLane&&) = default;

    /**
     * @brief Get the ID of the travel lane.
     * @return The ID of the travel lane.
     */
    LaneId getId() const noexcept { return m_id; }

    /**
     * @brief Get the system ID of the starting point.
     * @return The system ID of the starting point.
     */
    SystemId getFromSystem() const noexcept { return m_fromSystem; }

    /**
     * @brief Get the system ID of the endpoint.
     * @return The system ID of the endpoint.
     */
    SystemId getToSystem() const noexcept { return m_toSystem; }

    /**
     * @brief Get the starting position of the travel lane.
     * @return The starting position as a QPointF.
     */
    const QPointF& getStartPosition() const noexcept { return m_startPos; }

    /**
     * @brief Get the ending position of the travel lane.
     * @return The ending position as a QPointF.
     */
    const QPointF& getEndPosition() const noexcept { return m_endPos; }

    /**
     * @brief Get the length of the travel lane.
     * @return The length of the travel lane as a double.
     */
    double getLength() const noexcept;

    /**
     * @brief Get the color of the travel lane.
     * @return The color of the travel lane as a QColor.
     */
    QColor getLaneColor() const noexcept { return m_color; }

    /**
     * @brief Set the color of the travel lane.
     * @param color The new color for the travel lane.
     */
    void setColor(const QColor& color) { m_color = color; }

private:
    LaneId m_id; ///< Lane identifier
    SystemId m_fromSystem; ///< System identifier of the starting system
    SystemId m_toSystem; ///< System identifier of the destination system
    QPointF m_startPos; ///< Starting position of the lane
    QPointF m_endPos; ///< Ending position of the lane
    QColor m_color; ///< Color of the travel lane
};
