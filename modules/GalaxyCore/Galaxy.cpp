
/**
 * @file Galaxy.cpp
 * @brief Implementation of the Galaxy class for managing star systems and travel lanes.
 * @author Galaxy Builder Team
 * @date 2025
 * @version 1.0.0
 */

#include "Galaxy.h"
#include "StarSystemListModel.h"
#include "TravelLaneListModel.h"
#include <algorithm>


/**
 * @brief Constructs a new Galaxy object.
 *
 * Creates a new galaxy with the specified dimensions. The galaxy is initialized
 * with default spiral shape and empty system/lane collections. Models will be
 * injected from QML to follow dependency injection pattern.
 *
 * @param width The width of the galaxy in coordinate units.
 * @param height The height of the galaxy in coordinate units.
 * @param parent The parent QObject for memory management (default: nullptr).
 *
 * @note Models are not created here but injected via setSystemsModel() and setLanesModel().
 */
Galaxy::Galaxy(int width, int height, QObject* parent)
    : QObject(parent),
      m_width(width), m_height(height), m_shape(GalaxyShape::Spiral),
      m_nextSystemId(1), m_nextLaneId(1) {
    // Models will be injected from QML
}


/**
 * @brief Destroys the Galaxy object.
 *
 * Cleans up the galaxy instance. Models are not owned by this object
 * (they are managed by QML), so no manual deletion is needed.
 */
Galaxy::~Galaxy() {
    // Models are not owned by this object, so no manual deletion needed
}


/**
 * @brief Sets the systems model for dependency injection.
 *
 * Injects the StarSystemListModel that will be used to expose star systems
 * to QML views. This follows the dependency injection pattern where QML
 * creates the model and injects it into the C++ business logic.
 *
 * @param model Pointer to the StarSystemListModel created in QML.
 * @see setLanesModel()
 * @emits systemsModelChanged() when the model is successfully set.
 */
void Galaxy::setSystemsModel(StarSystemListModel* model) {
    if (m_systemsModel != model) {
        m_systemsModel = model;
        if (m_systemsModel) {
            m_systemsModel->setSystems(&m_systems);
        }
        emit systemsModelChanged();
    }
}


/**
 * @brief Sets the travel lanes model for dependency injection.
 *
 * Injects the TravelLaneListModel that will be used to expose travel lanes
 * to QML views. This follows the dependency injection pattern where QML
 * creates the model and injects it into the C++ business logic.
 *
 * @param model Pointer to the TravelLaneListModel created in QML.
 * @see setSystemsModel()
 * @emits lanesModelChanged() when the model is successfully set.
 */
void Galaxy::setLanesModel(TravelLaneListModel* model) {
    if (m_lanesModel != model) {
        m_lanesModel = model;
        if (m_lanesModel) {
            m_lanesModel->setLanes(&m_travelLanes);
        }
        emit lanesModelChanged();
    }
}


/**
 * @brief Adds a new star system to the galaxy.
 *
 * Creates a new star system at the specified position with the given star type.
 * The system is assigned a unique ID and added to both the internal collection
 * and the QML-accessible model.
 *
 * @param position The 2D coordinates where the system should be placed.
 * @param type The type of star for this system.
 * @return SystemId The unique identifier assigned to the new system.
 *
 * @emits systemsChanged() when the system is successfully added.
 */
SystemId Galaxy::addSystem(const QPointF& position, StarType type) {
    SystemId id = m_nextSystemId++;
    auto system = std::make_unique<StarSystem>(id, position, type);
    
    m_systemIndexMap[id] = m_systems.size();
    m_systems.push_back(std::move(system));
    
    // Update the model
    if (m_systemsModel) {
        m_systemsModel->setSystems(&m_systems);
    }
    emit systemsChanged();
    
    return id;
}


/**
 * @brief Retrieves a star system by its ID.
 *
 * Performs a fast lookup of a star system using its unique identifier.
 *
 * @param id The unique identifier of the system to retrieve.
 * @return StarSystem* Pointer to the system if found, nullptr otherwise.
 */
StarSystem* Galaxy::getSystem(SystemId id) const {
    auto it = m_systemIndexMap.find(id);
    if (it != m_systemIndexMap.end() && it->second < m_systems.size()) {
        return m_systems[it->second].get();
    }
    return nullptr;
}


/**
 * @brief Removes a star system and all connected travel lanes.
 *
 * Removes the specified star system from the galaxy and automatically
 * removes all travel lanes that were connected to this system. The
 * internal index maps are updated to maintain consistency.
 *
 * @param id The unique identifier of the system to remove.
 *
 * @emits systemsChanged() and lanesChanged() when removal is successful.
 */
void Galaxy::removeSystem(SystemId id) {
    auto it = m_systemIndexMap.find(id);
    if (it == m_systemIndexMap.end()) {
        return;
    }
    
    std::size_t index = it->second;
    
    // Remove all travel lanes connected to this system
    m_travelLanes.erase(
        std::remove_if(m_travelLanes.begin(), m_travelLanes.end(),
            [id](const auto& lane) {
                return lane->getFromSystem() == id || lane->getToSystem() == id;
            }),
        m_travelLanes.end()
    );
    
    // Remove the system
    m_systems.erase(m_systems.begin() + index);
    m_systemIndexMap.erase(it);
    
    // Update indices in the map
    for (auto& [systemId, systemIndex] : m_systemIndexMap) {
        if (systemIndex > index) {
            --systemIndex;
        }
    }
    
    // Update the models
    if (m_systemsModel) {
        m_systemsModel->setSystems(&m_systems);
    }
    if (m_lanesModel) {
        m_lanesModel->setLanes(&m_travelLanes);
    }
    emit systemsChanged();
    emit lanesChanged();
}


/**
 * @brief Creates a travel lane between two star systems.
 *
 * Establishes a bidirectional travel connection between two existing star systems.
 * The lane is assigned a unique ID and both endpoints must be valid systems.
 * Lane coordinates are automatically calculated from system positions.
 *
 * @param fromSystem The ID of the origin star system.
 * @param toSystem The ID of the destination star system.
 * @return LaneId The unique identifier of the created lane, or 0 if creation failed.
 *
 * @emits lanesChanged() when the lane is successfully created.
 */
LaneId Galaxy::addTravelLane(SystemId fromSystem, SystemId toSystem) {
    StarSystem* from = getSystem(fromSystem);
    StarSystem* to = getSystem(toSystem);
    
    if (!from || !to || fromSystem == toSystem) {
        return 0; // Invalid lane
    }
    
    LaneId id = m_nextLaneId++;
    auto lane = std::make_unique<TravelLane>(id, fromSystem, toSystem,
                                             from->getPosition(), to->getPosition());
    
    m_laneIndexMap[id] = m_travelLanes.size();
    m_travelLanes.push_back(std::move(lane));
    
    // Update the model
    if (m_lanesModel) {
        m_lanesModel->setLanes(&m_travelLanes);
    }
    emit lanesChanged();
    
    return id;
}


/**
 * @brief Retrieves a travel lane by its ID.
 *
 * Performs a fast lookup of a travel lane using its unique identifier.
 *
 * @param id The unique identifier of the lane to retrieve.
 * @return TravelLane* Pointer to the lane if found, nullptr otherwise.
 */
TravelLane* Galaxy::getTravelLane(LaneId id) const {
    auto it = m_laneIndexMap.find(id);
    if (it != m_laneIndexMap.end() && it->second < m_travelLanes.size()) {
        return m_travelLanes[it->second].get();
    }
    return nullptr;
}


/**
 * @brief Removes a travel lane from the galaxy.
 *
 * Removes the specified travel lane from the galaxy and updates the internal
 * index map and the QML-accessible model.
 *
 * @param id The unique identifier of the lane to remove.
 *
 * @emits lanesChanged() when removal is successful.
 */
void Galaxy::removeTravelLane(LaneId id) {
    auto it = m_laneIndexMap.find(id);
    if (it == m_laneIndexMap.end()) {
        return;
    }
    
    std::size_t index = it->second;
    m_travelLanes.erase(m_travelLanes.begin() + index);
    m_laneIndexMap.erase(it);
    
    // Update indices in the map
    for (auto& [laneId, laneIndex] : m_laneIndexMap) {
        if (laneIndex > index) {
            --laneIndex;
        }
    }
    
    // Update the model
    if (m_lanesModel) {
        m_lanesModel->setLanes(&m_travelLanes);
    }
    emit lanesChanged();
}


/**
 * @brief Clears all systems and travel lanes from the galaxy.
 *
 * Resets the galaxy to an empty state, removing all star systems and travel lanes,
 * and resetting all internal indices and models.
 *
 * @emits systemsChanged() and lanesChanged() after clearing.
 */
void Galaxy::clear() {
    m_systems.clear();
    m_travelLanes.clear();
    m_systemIndexMap.clear();
    m_laneIndexMap.clear();
    m_nextSystemId = 1;
    m_nextLaneId = 1;
    
    // Update the models
    if (m_systemsModel) {
        m_systemsModel->setSystems(&m_systems);
    }
    if (m_lanesModel) {
        m_lanesModel->setLanes(&m_travelLanes);
    }
    emit systemsChanged();
    emit lanesChanged();
}
