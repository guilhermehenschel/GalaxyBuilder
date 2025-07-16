#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include <memory>

#include "ggh/modules/GalaxyCore/viewmodels/TravelLaneViewModel.h"
#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

using namespace ggh::GalaxyCore::viewmodels;
using namespace ggh::GalaxyCore::models;
using namespace ggh::GalaxyCore::utilities;

class TravelLaneViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!QCoreApplication::instance()) {
            int argc = 0;
            char** argv = nullptr;
            app = std::make_unique<QCoreApplication>(argc, argv);
        }
        
        // Create two star systems to connect with a travel lane
        fromSystem = std::make_shared<StarSystemModel>(1, "Sol System", CartesianCoordinates<double>(0.0, 0.0));
        toSystem = std::make_shared<StarSystemModel>(2, "Alpha Centauri", CartesianCoordinates<double>(25.0, 8.0));
        
        travelLane = std::make_shared<TravelLaneModel>(1, fromSystem, toSystem);
        viewModel = std::make_unique<TravelLaneViewModel>(travelLane);
    }

    void TearDown() override {
        viewModel.reset();
        travelLane.reset();
        fromSystem.reset();
        toSystem.reset();
    }

    std::unique_ptr<QCoreApplication> app;
    std::shared_ptr<StarSystemModel> fromSystem;
    std::shared_ptr<StarSystemModel> toSystem;
    std::shared_ptr<TravelLaneModel> travelLane;
    std::unique_ptr<TravelLaneViewModel> viewModel;
};

TEST_F(TravelLaneViewModelTest, InitialProperties) {
    EXPECT_EQ(viewModel->fromSystemId(), 1);
    EXPECT_EQ(viewModel->toSystemId(), 2);
    EXPECT_DOUBLE_EQ(viewModel->distance(), travelLane->getLength());
}

TEST_F(TravelLaneViewModelTest, ModelAccess) {
    auto retrievedLane = viewModel->travelLane();
    EXPECT_EQ(retrievedLane, travelLane);
}

TEST_F(TravelLaneViewModelTest, SetDifferentTravelLane) {
    auto newFromSystem = std::make_shared<StarSystemModel>(3, "Vega System", CartesianCoordinates<double>(147.0, 38.8));
    auto newToSystem = std::make_shared<StarSystemModel>(4, "Sirius System", CartesianCoordinates<double>(50.0, 20.0));
    auto newLane = std::make_shared<TravelLaneModel>(2, newFromSystem, newToSystem);
    
    viewModel->setTravelLane(newLane);
    
    EXPECT_EQ(viewModel->fromSystemId(), 3);
    EXPECT_EQ(viewModel->toSystemId(), 4);
}

TEST_F(TravelLaneViewModelTest, ToXmlOutput) {
    QString xml = viewModel->toXml();
    EXPECT_TRUE(xml.contains("Lane"));
}

TEST_F(TravelLaneViewModelTest, DefaultConstructor) {
    TravelLaneViewModel defaultViewModel;
    // Default constructor should create a viewmodel with no lane
    EXPECT_EQ(defaultViewModel.travelLane(), nullptr);
}
