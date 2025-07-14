#ifndef GGH_MODULES_GALAXYCORE_MODELS_TRAVELLANDEMODEL_H
#define GGH_MODULES_GALAXYCORE_MODELS_TRAVELLANDEMODEL_H

#include <memory>

#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"

namespace ggh::GalaxyCore::models {
/**
 * @class TravelLane
 * @brief Represents a travel lane connecting two star systems.
 */
class TravelLaneModel {
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
    TravelLaneModel(utilities::LaneId id, std::shared_ptr<StarSystemModel> fromSystem, std::shared_ptr<StarSystemModel> toSystem);

    /**
     * @brief Get the ID of the travel lane.
     * @return The ID of the travel lane.
     */
    utilities::LaneId getId() const noexcept;
    /**
     * @brief Get the system ID of the starting point.
     * @return The system ID of the starting point.
     */
    std::shared_ptr<StarSystemModel> getFromSystem() const noexcept;

    /**
     * @brief Get the system ID of the endpoint.
     * @return The system ID of the endpoint.
     */
    std::shared_ptr<StarSystemModel> getToSystem() const noexcept;

    /**
     * @brief Get the starting position of the travel lane.
     * @return The starting position as a QPointF.
     */
    const utilities::CartesianCoordinates<double>& getStartPosition() const noexcept;

    /**
     * @brief Get the ending position of the travel lane.
     * @return The ending position as a QPointF.
     */
    const utilities::CartesianCoordinates<double>& getEndPosition() const noexcept;

    /**
     * @brief Get the length of the travel lane.
     * @return The length of the travel lane as a double.
     */
    double getLength() const noexcept;

    /**
     * @brief Converts the travel lane to a string representation.
     * @return A xml representation of the travel lane.
     */
    std::string toXml() const;

private:
    utilities::LaneId m_id; ///< Lane identifier
    std::shared_ptr<StarSystemModel> m_fromSystem; ///< System identifier of the starting system
    std::shared_ptr<StarSystemModel> m_toSystem; ///< System identifier of the destination system

};
}

#endif // !GGH_MODULES_GALAXYCORE_MODELS_TRAVELLANDEMODEL_H