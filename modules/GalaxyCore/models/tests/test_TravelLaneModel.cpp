#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"

#include <gtest/gtest.h>

namespace ggh::GalaxyCore::models {
TEST(TravelLaneModelTest, Construction) {
    auto fromSystem = std::make_shared<StarSystemModel>(1, "Alpha Centauri", utilities::CartesianCoordinates<double>(0.0, 0.0));
    auto toSystem = std::make_shared<StarSystemModel>(2, "Proxima Centauri", utilities::CartesianCoordinates<double>(100.0, 100.0));

    TravelLaneModel lane(1, fromSystem, toSystem);

    EXPECT_EQ(lane.getId(), 1);
    EXPECT_EQ(lane.getFromSystem()->getId(), 1);
    EXPECT_EQ(lane.getToSystem()->getId(), 2);
    EXPECT_EQ(lane.getStartPosition(), fromSystem->getPosition());
    EXPECT_EQ(lane.getEndPosition(), toSystem->getPosition());
    EXPECT_DOUBLE_EQ(lane.getLength(), utilities::calculateDistance<double>(fromSystem->getPosition(), toSystem->getPosition()));
}

TEST(TravelLaneModelTest, ToXml) {
    auto fromSystem = std::make_shared<StarSystemModel>(1, "Alpha Centauri", utilities::CartesianCoordinates<double>(0.0, 0.0));
    auto toSystem = std::make_shared<StarSystemModel>(2, "Proxima Centauri", utilities::CartesianCoordinates<double>(100.0, 100.0));

    TravelLaneModel lane(1, fromSystem, toSystem);
    std::string xml = lane.toXml();

    EXPECT_EQ(xml, "<TravelLane id=\"1\" fromSystem=\"1\" toSystem=\"2\" length=\"141.4213562373095\"/>");
}
}