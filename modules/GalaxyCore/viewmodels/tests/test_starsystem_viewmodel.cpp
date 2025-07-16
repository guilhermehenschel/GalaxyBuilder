#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include <memory>

#include "ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"

using namespace ggh::GalaxyCore::viewmodels;
using namespace ggh::GalaxyCore::models;
using namespace ggh::GalaxyCore::utilities;

class StarSystemViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!QCoreApplication::instance()) {
            int argc = 0;
            char** argv = nullptr;
            app = std::make_unique<QCoreApplication>(argc, argv);
        }
        
        starSystem = std::make_shared<StarSystemModel>(1, "Sol System", 
                                                     CartesianCoordinates<double>(0.0, 0.0));
        starSystem->setStarType(StarType::YellowStar);
        starSystem->setSystemSize(SystemSize::Medium);
        
        viewModel = std::make_unique<StarSystemViewModel>(starSystem);
    }

    void TearDown() override {
        viewModel.reset();
        starSystem.reset();
    }

    std::unique_ptr<QCoreApplication> app;
    std::shared_ptr<StarSystemModel> starSystem;
    std::unique_ptr<StarSystemViewModel> viewModel;
};

TEST_F(StarSystemViewModelTest, InitialProperties) {
    EXPECT_EQ(viewModel->name(), "Sol System");
    EXPECT_EQ(viewModel->systemId(), 1);
    EXPECT_DOUBLE_EQ(viewModel->positionX(), 0.0);
    EXPECT_DOUBLE_EQ(viewModel->positionY(), 0.0);
    EXPECT_EQ(viewModel->starType(), static_cast<int>(StarType::YellowStar));
    EXPECT_EQ(viewModel->systemSize(), static_cast<int>(SystemSize::Medium));
}

TEST_F(StarSystemViewModelTest, SettersEmitSignals) {
    QSignalSpy nameSpy(viewModel.get(), &StarSystemViewModel::nameChanged);
    QSignalSpy positionSpy(viewModel.get(), &StarSystemViewModel::positionChanged);
    QSignalSpy starTypeSpy(viewModel.get(), &StarSystemViewModel::starTypeChanged);
    QSignalSpy systemSizeSpy(viewModel.get(), &StarSystemViewModel::systemSizeChanged);

    viewModel->setName("Alpha Centauri");
    viewModel->setPositionX(100.0);
    viewModel->setPositionY(-50.0);
    viewModel->setStarType(static_cast<int>(StarType::RedDwarf));
    viewModel->setSystemSize(static_cast<int>(SystemSize::Small));

    EXPECT_EQ(nameSpy.count(), 1);
    EXPECT_EQ(positionSpy.count(), 2); // Two separate position changes
    EXPECT_EQ(starTypeSpy.count(), 1);
    EXPECT_EQ(systemSizeSpy.count(), 1);
}

TEST_F(StarSystemViewModelTest, SetPositionBoth) {
    QSignalSpy positionSpy(viewModel.get(), &StarSystemViewModel::positionChanged);
    
    viewModel->setPosition(123.45, -67.89);
    
    EXPECT_EQ(positionSpy.count(), 1); // Only one signal for both coordinates
    EXPECT_DOUBLE_EQ(viewModel->positionX(), 123.45);
    EXPECT_DOUBLE_EQ(viewModel->positionY(), -67.89);
}

TEST_F(StarSystemViewModelTest, NoSignalOnSameValue) {
    QSignalSpy nameSpy(viewModel.get(), &StarSystemViewModel::nameChanged);
    QSignalSpy positionSpy(viewModel.get(), &StarSystemViewModel::positionChanged);
    
    viewModel->setName("Sol System"); // Same value
    viewModel->setPositionX(0.0); // Same value
    
    EXPECT_EQ(nameSpy.count(), 0);
    EXPECT_EQ(positionSpy.count(), 0);
}

TEST_F(StarSystemViewModelTest, PlanetsListModel) {
    auto* planetsModel = viewModel->planets();
    ASSERT_NE(planetsModel, nullptr);
    
    // Initially no planets
    EXPECT_EQ(planetsModel->rowCount(), 0);
    EXPECT_EQ(viewModel->planetCount(), 0);
}

TEST_F(StarSystemViewModelTest, AddPlanet) {
    viewModel->addPlanet("Earth", static_cast<int>(PlanetType::Rocky),
                        1.0, 1.0, 1, 1.0, 288.0, 184.0);
    
    EXPECT_EQ(viewModel->planetCount(), 1);
    
    auto* planetsModel = viewModel->planets();
    EXPECT_EQ(planetsModel->rowCount(), 1);
    
    auto nameData = planetsModel->data(planetsModel->index(0, 0), Qt::UserRole + 1);
    EXPECT_EQ(nameData.toString(), "Earth");
}

TEST_F(StarSystemViewModelTest, RemovePlanet) {
    // Add a planet first
    viewModel->addPlanet("Mars", static_cast<int>(PlanetType::Desert),
                        0.53, 0.11, 2, 1.52, 293.0, 143.0);
    EXPECT_EQ(viewModel->planetCount(), 1);
    
    // Remove it
    bool removed = viewModel->removePlanetByName("Mars");
    EXPECT_TRUE(removed);
    EXPECT_EQ(viewModel->planetCount(), 0);
    
    // Try to remove non-existent planet
    bool notRemoved = viewModel->removePlanetByName("Venus");
    EXPECT_FALSE(notRemoved);
}

TEST_F(StarSystemViewModelTest, ModelAccess) {
    auto retrievedSystem = viewModel->starSystem();
    EXPECT_EQ(retrievedSystem, starSystem);
    EXPECT_EQ(retrievedSystem->getName(), "Sol System");
}

TEST_F(StarSystemViewModelTest, SetDifferentStarSystem) {
    QSignalSpy nameSpy(viewModel.get(), &StarSystemViewModel::nameChanged);
    
    auto newSystem = std::make_shared<StarSystemModel>(2, "Vega System", 
                                                     CartesianCoordinates<double>(25.0, 8.0));
    viewModel->setStarSystem(newSystem);
    
    EXPECT_EQ(viewModel->name(), "Vega System");
    EXPECT_EQ(viewModel->systemId(), 2);
    EXPECT_DOUBLE_EQ(viewModel->positionX(), 25.0);
    EXPECT_DOUBLE_EQ(viewModel->positionY(), 8.0);
    EXPECT_GT(nameSpy.count(), 0);
}

TEST_F(StarSystemViewModelTest, ToXmlOutput) {
    QString xml = viewModel->toXml();
    EXPECT_TRUE(xml.contains("Sol System"));
    EXPECT_TRUE(xml.contains("<StarSystem"));
}

TEST_F(StarSystemViewModelTest, EnumStrings) {
    viewModel->setStarType(static_cast<int>(StarType::RedDwarf));
    EXPECT_EQ(viewModel->starTypeString(), "Red Dwarf");
    
    viewModel->setStarType(static_cast<int>(StarType::BlackHole));
    EXPECT_EQ(viewModel->starTypeString(), "Black Hole");
    
    viewModel->setSystemSize(static_cast<int>(SystemSize::Huge));
    EXPECT_EQ(viewModel->systemSizeString(), "Huge");
    
    viewModel->setSystemSize(static_cast<int>(SystemSize::Small));
    EXPECT_EQ(viewModel->systemSizeString(), "Small");
}

TEST_F(StarSystemViewModelTest, DefaultConstructor) {
    StarSystemViewModel defaultViewModel;
    EXPECT_FALSE(defaultViewModel.name().isEmpty());
    EXPECT_NE(defaultViewModel.starSystem(), nullptr);
    EXPECT_NE(defaultViewModel.planets(), nullptr);
}

TEST_F(StarSystemViewModelTest, PlanetListModelRoles) {
    viewModel->addPlanet("Test Planet", static_cast<int>(PlanetType::IceGiant),
                        2.0, 3.0, 5, 4.0, 150.0, 50.0);
    
    auto* planetsModel = viewModel->planets();
    auto roleNames = planetsModel->roleNames();
    
    EXPECT_TRUE(roleNames.contains(Qt::UserRole + 1)); // name role
    EXPECT_TRUE(roleNames.contains(Qt::UserRole + 2)); // type role
    EXPECT_EQ(roleNames[Qt::UserRole + 1], "name");
    EXPECT_EQ(roleNames[Qt::UserRole + 2], "planetType");
}
