#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"

#include <gtest/gtest.h>

namespace ggh::GalaxyCore::models
{
TEST(GalaxyModelTest, AddAndGetStarSystem) {
    GalaxyModel galaxy(1000, 1000);
    SystemId systemId = 1;
    std::string systemName = "Test System";
    utilities::CartesianCoordinates<double> position(100.0, 200.0);
    StarType starType = StarType::YellowStar;

    galaxy.addStarSystem(systemId, systemName, position, starType);

    auto system = galaxy.getStarSystem(systemId);
    ASSERT_NE(system, nullptr);
    EXPECT_EQ(system->getId(), systemId);
    EXPECT_EQ(system->getName(), systemName);
    EXPECT_EQ(system->getPosition(), position);
    EXPECT_EQ(system->getStarType(), starType);
}

TEST(GalaxyModelTest, RemoveStarSystem) {
    GalaxyModel galaxy(1000, 1000);
    SystemId systemId = 1;
    std::string systemName = "Test System";
    utilities::CartesianCoordinates<double> position(100.0, 200.0);
    StarType starType = StarType::YellowStar;

    galaxy.addStarSystem(systemId, systemName, position, starType);
    EXPECT_TRUE(galaxy.removeStarSystem(systemId));
    EXPECT_EQ(galaxy.getStarSystem(systemId), nullptr);
}

TEST(GalaxyModelTest, GetAllStarSystems) {
    GalaxyModel galaxy(1000, 1000);
    galaxy.addStarSystem(1, "System A", utilities::CartesianCoordinates<double>(100.0, 200.0), StarType::YellowStar);
    galaxy.addStarSystem(2, "System B", utilities::CartesianCoordinates<double>(300.0, 400.0), StarType::RedGiant);

    auto systems = galaxy.getAllStarSystems();
    ASSERT_EQ(systems.size(), 2);
    EXPECT_EQ(systems[0]->getName(), "System A");
    EXPECT_EQ(systems[1]->getName(), "System B");
}

TEST(GalaxyModelTest, AddAndGetTravelLane) {
    GalaxyModel galaxy(1000, 1000);
    SystemId fromSystemId = 1;
    SystemId toSystemId = 2;
    utilities::CartesianCoordinates<double> position1(100.0, 200.0);
    utilities::CartesianCoordinates<double> position2(300.0, 400.0);
    galaxy.addStarSystem(fromSystemId, "From System", position1, StarType::YellowStar);
    galaxy.addStarSystem(toSystemId, "To System", position2, StarType::RedGiant);

    utilities::LaneId laneId = 1;
    galaxy.addTravelLane(laneId, fromSystemId, toSystemId);

    auto lane = galaxy.getTravelLane(laneId);
    ASSERT_NE(lane, nullptr);
    EXPECT_EQ(lane->getFromSystem()->getId(), fromSystemId);
    EXPECT_EQ(lane->getToSystem()->getId(), toSystemId);
}

TEST(GalaxyModelTest, RemoveTravelLane) {
    GalaxyModel galaxy(1000, 1000);
    SystemId fromSystemId = 1;
    SystemId toSystemId = 2;
    utilities::CartesianCoordinates<double> position1(100.0, 200.0);
    utilities::CartesianCoordinates<double> position2(300.0, 400.0);
    galaxy.addStarSystem(fromSystemId, "From System", position1, StarType::YellowStar);
    galaxy.addStarSystem(toSystemId, "To System", position2, StarType::RedGiant);

    utilities::LaneId laneId = 1;
    galaxy.addTravelLane(laneId, fromSystemId, toSystemId);
    EXPECT_TRUE(galaxy.removeTravelLane(laneId));
    EXPECT_EQ(galaxy.getTravelLane(laneId), nullptr);
}
} // namespace ggh::GalaxyCore::models
