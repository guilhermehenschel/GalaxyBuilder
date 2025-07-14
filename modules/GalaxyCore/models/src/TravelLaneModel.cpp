#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"

namespace ggh::GalaxyCore::models {
TravelLaneModel::TravelLaneModel(utilities::LaneId id, std::shared_ptr<StarSystemModel> fromSystem, std::shared_ptr<StarSystemModel> toSystem)
    : m_id(id), m_fromSystem(fromSystem), m_toSystem(toSystem) {}

utilities::LaneId TravelLaneModel::getId() const noexcept {
    return m_id;}

std::shared_ptr<StarSystemModel> TravelLaneModel::getFromSystem() const noexcept {
    return m_fromSystem;}

std::shared_ptr<StarSystemModel> TravelLaneModel::getToSystem() const noexcept {
    return m_toSystem;}

const utilities::CartesianCoordinates<double>& TravelLaneModel::getStartPosition() const noexcept {
    return m_fromSystem->getPosition();}

const utilities::CartesianCoordinates<double>& TravelLaneModel::getEndPosition() const noexcept {
    return m_toSystem->getPosition();}

double TravelLaneModel::getLength() const noexcept {
    auto distance{utilities::calculateDistance(m_fromSystem->getPosition(), m_toSystem->getPosition())};
    return distance;
}

std::string TravelLaneModel::toXml() const {
    return std::format("<TravelLane id=\"{}\" fromSystem=\"{}\" toSystem=\"{}\" length=\"{}\"/>",
                       m_id, m_fromSystem->getId(), m_toSystem->getId(),
                       getLength());
    }
}