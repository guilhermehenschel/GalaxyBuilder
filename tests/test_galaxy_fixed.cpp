#include <gtest/gtest.h>
#include "Galaxy.h"
#include "StarSystem.h"
#include "TravelLane.h"
#include "Common.h"
#include <QPointF>

class GalaxyTest : public ::testing::Test {
protected:
    void SetUp() override {
        galaxy = std::make_unique<Galaxy>();
    }

    void TearDown() override {
        galaxy.reset();
    }

    std::unique_ptr<Galaxy> galaxy;
};

TEST_F(GalaxyTest, InitialState) {
    EXPECT_EQ(galaxy->getSystemCount(), 0);
    EXPECT_EQ(galaxy->getTravelLaneCount(), 0);
}

TEST_F(GalaxyTest, GetDimensions) {
    // Test default dimensions
    EXPECT_EQ(galaxy->getWidth(), DEFAULT_GALAXY_WIDTH);
    EXPECT_EQ(galaxy->getHeight(), DEFAULT_GALAXY_HEIGHT);
}

TEST_F(GalaxyTest, SetShape) {
    galaxy->setShape(GalaxyShape::Spiral);
    EXPECT_EQ(galaxy->getShape(), GalaxyShape::Spiral);
    
    galaxy->setShape(GalaxyShape::Ring);
    EXPECT_EQ(galaxy->getShape(), GalaxyShape::Ring);
}

TEST_F(GalaxyTest, AddSystem) {
    SystemId systemId = galaxy->addSystem(QPointF(100, 100), StarType::YellowStar);
    
    EXPECT_EQ(galaxy->getSystemCount(), 1);
    
    StarSystem* system = galaxy->getSystem(systemId);
    EXPECT_NE(system, nullptr);
}

TEST_F(GalaxyTest, RemoveSystem) {
    SystemId systemId = galaxy->addSystem(QPointF(100, 100), StarType::YellowStar);
    EXPECT_EQ(galaxy->getSystemCount(), 1);
    
    galaxy->removeSystem(systemId);
    EXPECT_EQ(galaxy->getSystemCount(), 0);
}

TEST_F(GalaxyTest, AddTravelLane) {
    SystemId systemId1 = galaxy->addSystem(QPointF(100, 100), StarType::YellowStar);
    SystemId systemId2 = galaxy->addSystem(QPointF(200, 200), StarType::YellowStar);
    
    LaneId laneId = galaxy->addTravelLane(systemId1, systemId2);
    
    EXPECT_EQ(galaxy->getTravelLaneCount(), 1);
    
    TravelLane* lane = galaxy->getTravelLane(laneId);
    EXPECT_NE(lane, nullptr);
    EXPECT_EQ(lane->getFromSystem(), systemId1);
    EXPECT_EQ(lane->getToSystem(), systemId2);
}

TEST_F(GalaxyTest, RemoveTravelLane) {
    SystemId systemId1 = galaxy->addSystem(QPointF(100, 100), StarType::YellowStar);
    SystemId systemId2 = galaxy->addSystem(QPointF(200, 200), StarType::YellowStar);
    
    LaneId laneId = galaxy->addTravelLane(systemId1, systemId2);
    EXPECT_EQ(galaxy->getTravelLaneCount(), 1);
    
    galaxy->removeTravelLane(laneId);
    EXPECT_EQ(galaxy->getTravelLaneCount(), 0);
}

TEST_F(GalaxyTest, AddMultipleSystems) {
    SystemId systemId1 = galaxy->addSystem(QPointF(100, 100), StarType::YellowStar);
    SystemId systemId2 = galaxy->addSystem(QPointF(200, 200), StarType::RedDwarf);
    SystemId systemId3 = galaxy->addSystem(QPointF(300, 300), StarType::BlueStar);
    
    EXPECT_EQ(galaxy->getSystemCount(), 3);
    
    EXPECT_NE(galaxy->getSystem(systemId1), nullptr);
    EXPECT_NE(galaxy->getSystem(systemId2), nullptr);
    EXPECT_NE(galaxy->getSystem(systemId3), nullptr);
}

TEST_F(GalaxyTest, AddMultipleTravelLanes) {
    SystemId systemId1 = galaxy->addSystem(QPointF(100, 100), StarType::YellowStar);
    SystemId systemId2 = galaxy->addSystem(QPointF(200, 200), StarType::YellowStar);
    SystemId systemId3 = galaxy->addSystem(QPointF(300, 300), StarType::YellowStar);
    
    LaneId laneId1 = galaxy->addTravelLane(systemId1, systemId2);
    LaneId laneId2 = galaxy->addTravelLane(systemId2, systemId3);
    
    EXPECT_EQ(galaxy->getTravelLaneCount(), 2);
    
    TravelLane* lane1 = galaxy->getTravelLane(laneId1);
    TravelLane* lane2 = galaxy->getTravelLane(laneId2);
    
    EXPECT_NE(lane1, nullptr);
    EXPECT_NE(lane2, nullptr);
    
    EXPECT_EQ(lane1->getFromSystem(), systemId1);
    EXPECT_EQ(lane1->getToSystem(), systemId2);
    EXPECT_EQ(lane2->getFromSystem(), systemId2);
    EXPECT_EQ(lane2->getToSystem(), systemId3);
}

TEST_F(GalaxyTest, ClearGalaxy) {
    SystemId systemId1 = galaxy->addSystem(QPointF(100, 100), StarType::YellowStar);
    SystemId systemId2 = galaxy->addSystem(QPointF(200, 200), StarType::YellowStar);
    LaneId laneId = galaxy->addTravelLane(systemId1, systemId2);
    
    EXPECT_EQ(galaxy->getSystemCount(), 2);
    EXPECT_EQ(galaxy->getTravelLaneCount(), 1);
    
    galaxy->clear();
    
    EXPECT_EQ(galaxy->getSystemCount(), 0);
    EXPECT_EQ(galaxy->getTravelLaneCount(), 0);
}

TEST_F(GalaxyTest, GetInvalidSystem) {
    SystemId invalidId = 999;
    StarSystem* system = galaxy->getSystem(invalidId);
    EXPECT_EQ(system, nullptr);
}

TEST_F(GalaxyTest, GetInvalidTravelLane) {
    LaneId invalidId = 999;
    TravelLane* lane = galaxy->getTravelLane(invalidId);
    EXPECT_EQ(lane, nullptr);
}
