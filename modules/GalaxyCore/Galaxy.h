#pragma once

#include <QObject>
#include <QQmlEngine>
#include "Common.h"
#include "StarSystem.h"
#include "TravelLane.h"
#include "StarSystemListModel.h"
#include "TravelLaneListModel.h"
#include "galaxycore_global.h"
#include <vector>
#include <unordered_map>
#include <memory>

/**
 * @class Galaxy
 * @brief Represents the galaxy containing star systems and travel lanes.
 */
class GALAXYCORE_EXPORT Galaxy : public QObject {
    Q_OBJECT
    Q_PROPERTY(StarSystemListModel* systemsModel READ systemsModel WRITE setSystemsModel NOTIFY systemsModelChanged)
    Q_PROPERTY(TravelLaneListModel* lanesModel READ lanesModel WRITE setLanesModel NOTIFY lanesModelChanged)
public:
    Galaxy(int width = DEFAULT_GALAXY_WIDTH, int height = DEFAULT_GALAXY_HEIGHT, QObject* parent = nullptr);
    ~Galaxy() override;

    // Delete copy constructor and assignment operator to prevent expensive copies
    Galaxy(const Galaxy&) = delete;
    Galaxy& operator=(const Galaxy&) = delete;

    // Move constructor and assignment operator
    Galaxy(Galaxy&&) = default;
    Galaxy& operator=(Galaxy&&) = default;

    // Getters
    int getWidth() const noexcept { return m_width; }
    int getHeight() const noexcept { return m_height; }
    const std::vector<std::unique_ptr<StarSystem>>& getSystems() const noexcept { return m_systems; }
    const std::vector<std::unique_ptr<TravelLane>>& getTravelLanes() const noexcept { return m_travelLanes; }
    GalaxyShape getShape() const noexcept { return m_shape; }
    StarSystemListModel* systemsModel() const { return m_systemsModel; }
    TravelLaneListModel* lanesModel() const { return m_lanesModel; }
    
    // Setters for dependency injection
    void setSystemsModel(StarSystemListModel* model);
    void setLanesModel(TravelLaneListModel* model);

    /**
     * @brief Add a new star system to the galaxy.
     * @param position The position of the new star system.
     * @param type The type of the star (default is YellowStar).
     * @return The ID of the newly added star system.
     */
    SystemId addSystem(const QPointF& position, StarType type = StarType::YellowStar);

    /**
     * @brief Get a star system by its ID.
     * @param id The ID of the star system.
     * @return A pointer to the star system, or nullptr if not found.
     */
    StarSystem* getSystem(SystemId id) const;

    /**
     * @brief Remove a star system from the galaxy.
     * @param id The ID of the star system to remove.
     */
    void removeSystem(SystemId id);

    // Travel lane management
    LaneId addTravelLane(SystemId fromSystem, SystemId toSystem);
    TravelLane* getTravelLane(LaneId id) const;
    void removeTravelLane(LaneId id);

    // Utility methods
    void clear();
    std::size_t getSystemCount() const noexcept { return m_systems.size(); }
    std::size_t getTravelLaneCount() const noexcept { return m_travelLanes.size(); }
    
    void setShape(GalaxyShape shape) { m_shape = shape; }

signals:
    void systemsChanged();
    void lanesChanged();
    void systemsModelChanged();
    void lanesModelChanged();

private:
    int m_width;
    int m_height;
    GalaxyShape m_shape;
    std::vector<std::unique_ptr<StarSystem>> m_systems;
    std::vector<std::unique_ptr<TravelLane>> m_travelLanes;
    std::unordered_map<SystemId, std::size_t> m_systemIndexMap;
    std::unordered_map<LaneId, std::size_t> m_laneIndexMap;
    SystemId m_nextSystemId;
    LaneId m_nextLaneId;
    StarSystemListModel* m_systemsModel = nullptr;
    TravelLaneListModel* m_lanesModel = nullptr;
};
