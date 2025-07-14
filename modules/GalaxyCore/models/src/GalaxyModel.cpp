#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"

namespace ggh::GalaxyCore::models {
GalaxyModel::GalaxyModel(int width, int height)
    : m_width(width), m_height(height) {
    // Initialize the galaxy with default values
    }

void GalaxyModel::addStarSystem(std::shared_ptr<StarSystemModel>&& system) {
    if (system) {
        m_starSystems[system->getId()] = std::move(system);
    }
}

void GalaxyModel::addStarSystem(SystemId id, const std::string& name, const utilities::CartesianCoordinates<double>& position, StarType type) {
    auto system{std::make_shared<StarSystemModel>(id, name, position, type)};
    addStarSystem(std::move(system));
}

std::shared_ptr<StarSystemModel> GalaxyModel::getStarSystem(SystemId id) const {
    auto it = m_starSystems.find(id);
    if (it != m_starSystems.end()) {
        return it->second;
    }
    return nullptr;
}

bool GalaxyModel::removeStarSystem(SystemId id) {
    auto it = m_starSystems.find(id);
    if (it != m_starSystems.end()) {
        m_starSystems.erase(it);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<StarSystemModel>> GalaxyModel::getAllStarSystems() const {
    std::vector<std::shared_ptr<StarSystemModel>> systems;
    for (const auto& pair : m_starSystems) {
        systems.push_back(pair.second);
    }
    return systems;
}

void GalaxyModel::addTravelLane(std::shared_ptr<TravelLaneModel>&& lane) {
    if (lane) {
        m_travelLanes[lane->getId()] = std::move(lane);
    }
}

void GalaxyModel::addTravelLane(utilities::LaneId id, std::shared_ptr<StarSystemModel> fromSystem, std::shared_ptr<StarSystemModel> toSystem) {
    if (fromSystem && toSystem && fromSystem->getId() != toSystem->getId()) {
        auto lane{std::make_shared<TravelLaneModel>(id, fromSystem, toSystem)};
        addTravelLane(std::move(lane));
    }
}

void GalaxyModel::addTravelLane(utilities::LaneId id, utilities::SystemId fromSystemId, utilities::SystemId toSystemId) {
    auto fromSystem = getStarSystem(fromSystemId);
    auto toSystem = getStarSystem(toSystemId);
    if (fromSystem && toSystem && fromSystemId != toSystemId) {
        addTravelLane(id, fromSystem, toSystem);
    }
}

std::shared_ptr<TravelLaneModel> GalaxyModel::getTravelLane(utilities::LaneId id) const {
    auto it = m_travelLanes.find(id);
    if (it != m_travelLanes.end()) {
        return it->second;
    }
    return nullptr;
}
bool GalaxyModel::removeTravelLane(utilities::LaneId id) {
    auto it = m_travelLanes.find(id);
    if (it != m_travelLanes.end()) {
        m_travelLanes.erase(it);
        return true;
    }
    return false;
}

std::vector<std::shared_ptr<TravelLaneModel>> GalaxyModel::getAllTravelLanes() const {
    std::vector<std::shared_ptr<TravelLaneModel>> lanes;
    for (const auto& pair : m_travelLanes) {
        lanes.push_back(pair.second);
    }
    return lanes;
}

std::string GalaxyModel::toXml() const {
    std::string xml{"<Galaxy>"};
    
    for (const auto& system : getAllStarSystems()) {
        xml += system->toXml();
    }
    
    for (const auto& lane : getAllTravelLanes()) {
        xml += lane->toXml();
    }
    
    xml += "</Galaxy>";
    return xml;
}

int GalaxyModel::getWidth() const noexcept {
    return m_width;
}

int GalaxyModel::getHeight() const noexcept {
    return m_height;
}

void GalaxyModel::setWidth(int width) {
    m_width = width;
}

void GalaxyModel::setHeight(int height) {
    m_height = height;
}
}