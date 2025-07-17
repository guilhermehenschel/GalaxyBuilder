#ifndef GGH_GALAXYCORE_MODELS_GALAXY_MODEL_H
#define GGH_GALAXYCORE_MODELS_GALAXY_MODEL_H

#include <memory>

#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"
/**
 * @file GalaxyModel.h
 * @brief Contains the GalaxyModel class representing a galaxy with star systems and travel lanes.
 *
 * This file defines the GalaxyModel class, which encapsulates the properties and behaviors of a galaxy,
 * including its star systems and travel lanes.
 */ 
namespace ggh::GalaxyCore::models
{
class GalaxyModel {
public:
    /**
     * @brief Constructs a GalaxyModel with the specified width and height.
     * @param width The width of the galaxy.
     * @param height The height of the galaxy.
     */
    GalaxyModel(int width, int height);

    // Getters
    int getWidth() const noexcept;
    int getHeight() const noexcept;

    // Setters
    void setWidth(int width);
    void setHeight(int height);

    /**
     * @brief Adds a star system to the galaxy.
     * @param system The star system to add.
     */
    void addStarSystem(std::shared_ptr<StarSystemModel>&& system);

    /**
     * @brief Adds a star system with the given ID, name, position, and type.
     * @param id The unique identifier for the star system.
     * @param name The name of the star system.
     * @param position The position of the star system in Cartesian coordinates.
     * @param type The type of the star (default is YellowStar).
     */
    void addStarSystem(SystemId id, const std::string& name, const utilities::CartesianCoordinates<double>& position, StarType type = StarType::YellowStar);

    /**
     * @brief Gets a star system by its ID.
     * @param id The unique identifier of the star system.
     * @return A shared pointer to the StarSystemModel if found, nullptr otherwise.
     */
    std::shared_ptr<StarSystemModel> getStarSystem(SystemId id) const;

    /**
     * @brief Removes a star system by its ID.
     * @param id The unique identifier of the star system to remove.
     * @return True if the system was removed, false if it was not found.
     */
    bool removeStarSystem(SystemId id);

    /**
     * @brief Gets all star systems in the galaxy.
     * @return A vector of shared pointers to all StarSystemModel objects.
     */
    std::vector<std::shared_ptr<StarSystemModel>> getAllStarSystems() const;

    /**
     * @brief Adds a travel lane to the galaxy.
     * @param lane The travel lane to add.
     */
    void addTravelLane(std::shared_ptr<TravelLaneModel>&& lane);

    /**
     * @brief Adds a travel lane with the given ID, from system, and to system.
     * @param id The unique identifier for the travel lane.
     * @param fromSystem The starting star system of the travel lane.
     * @param toSystem The destination star system of the travel lane.
     */
    void addTravelLane(utilities::LaneId id, std::shared_ptr<StarSystemModel> fromSystem, std::shared_ptr<StarSystemModel> toSystem);
    
    void addTravelLane(utilities::LaneId id, utilities::SystemId fromSystemId, utilities::SystemId toSystemId);

    /**
     * @brief Gets a travel lane by its ID.
     * @param id The unique identifier of the travel lane.
     * @return A shared pointer to the TravelLaneModel if found, nullptr otherwise.
     */
    std::shared_ptr<TravelLaneModel> getTravelLane(utilities::LaneId id) const;

    /**
     * @brief Removes a travel lane by its ID.
     * @param id The unique identifier of the travel lane to remove.
     * @return True if the lane was removed, false if it was not found.
     */
    bool removeTravelLane(utilities::LaneId id);

    /**
     * @brief Gets all travel lanes in the galaxy.
     * @return A vector of shared pointers to all TravelLaneModel objects.
     */
    std::vector<std::shared_ptr<TravelLaneModel>> getAllTravelLanes() const;

    /**
     * @brief Converts the galaxy model to an XML representation.
     * @return A string containing the XML representation of the galaxy.
     */
    std::string toXml() const;

    /**
     * @brief Gets the XML tag name for the galaxy model.
     * @return A string containing the XML tag name.
     */
    static std::string xmlTag() noexcept {
        return "Galaxy";
    }

 private:
    int m_width;  ///< Width of the galaxy
    int m_height; ///< Height of the galaxy
    std::unordered_map<utilities::SystemId, std::shared_ptr<StarSystemModel>> m_starSystems{}; ///< Map of star systems by their unique IDs
    std::unordered_map<utilities::LaneId, std::shared_ptr<TravelLaneModel>> m_travelLanes{}; ///< Map of travel lanes by their unique IDs

    // Additional properties and methods can be added as needed
    // For example, methods to manage travel lanes, serialize to XML, etc.
};
} // namespace ggh::GalaxyCore::models


#endif // !GGH_GALAXYCORE_MODELS_GALAXY_MODEL_H