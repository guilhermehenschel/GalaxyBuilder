#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include <memory>

#include "ggh/modules/GalaxyCore/viewmodels/PlanetViewModel.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"

using namespace ggh::GalaxyCore::viewmodels;
using namespace ggh::GalaxyCore::models;
using namespace ggh::GalaxyCore::utilities;

class PlanetViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!QCoreApplication::instance()) {
            int argc = 0;
            char** argv = nullptr;
            app = std::make_unique<QCoreApplication>(argc, argv);
        }
        
        planet = std::make_shared<Planet>("Earth", PlanetType::Rocky, 
                                        1.0, 1.0, 1, 1.0, 288.0, 184.0);
        viewModel = std::make_unique<PlanetViewModel>(planet);
    }

    void TearDown() override {
        viewModel.reset();
        planet.reset();
    }

    std::unique_ptr<QCoreApplication> app;
    std::shared_ptr<Planet> planet;
    std::unique_ptr<PlanetViewModel> viewModel;
};

TEST_F(PlanetViewModelTest, InitialProperties) {
    EXPECT_EQ(viewModel->name(), "Earth");
    EXPECT_EQ(viewModel->planetType(), static_cast<int>(PlanetType::Rocky));
    EXPECT_DOUBLE_EQ(viewModel->size(), 1.0);
    EXPECT_DOUBLE_EQ(viewModel->mass(), 1.0);
    EXPECT_EQ(viewModel->numberOfMoons(), 1);
    EXPECT_DOUBLE_EQ(viewModel->orbitalRadius(), 1.0);
    EXPECT_DOUBLE_EQ(viewModel->maxTemperature(), 288.0);
    EXPECT_DOUBLE_EQ(viewModel->minTemperature(), 184.0);
}

TEST_F(PlanetViewModelTest, SettersEmitSignals) {
    QSignalSpy nameSpy(viewModel.get(), &PlanetViewModel::nameChanged);
    QSignalSpy typeSpy(viewModel.get(), &PlanetViewModel::planetTypeChanged);
    QSignalSpy sizeSpy(viewModel.get(), &PlanetViewModel::sizeChanged);
    QSignalSpy massSpy(viewModel.get(), &PlanetViewModel::massChanged);
    QSignalSpy moonsSpy(viewModel.get(), &PlanetViewModel::numberOfMoonsChanged);
    QSignalSpy orbitSpy(viewModel.get(), &PlanetViewModel::orbitalRadiusChanged);
    QSignalSpy maxTempSpy(viewModel.get(), &PlanetViewModel::maxTemperatureChanged);
    QSignalSpy minTempSpy(viewModel.get(), &PlanetViewModel::minTemperatureChanged);

    viewModel->setName("Mars");
    viewModel->setPlanetType(static_cast<int>(PlanetType::Desert));
    viewModel->setSize(0.53);
    viewModel->setMass(0.11);
    viewModel->setNumberOfMoons(2);
    viewModel->setOrbitalRadius(1.52);
    viewModel->setMaxTemperature(293.0);
    viewModel->setMinTemperature(143.0);

    EXPECT_EQ(nameSpy.count(), 1);
    EXPECT_EQ(typeSpy.count(), 1);
    EXPECT_EQ(sizeSpy.count(), 1);
    EXPECT_EQ(massSpy.count(), 1);
    EXPECT_EQ(moonsSpy.count(), 1);
    EXPECT_EQ(orbitSpy.count(), 1);
    EXPECT_EQ(maxTempSpy.count(), 1);
    EXPECT_EQ(minTempSpy.count(), 1);
}

TEST_F(PlanetViewModelTest, NoSignalOnSameValue) {
    QSignalSpy nameSpy(viewModel.get(), &PlanetViewModel::nameChanged);
    
    viewModel->setName("Earth"); // Same value
    EXPECT_EQ(nameSpy.count(), 0);
}

TEST_F(PlanetViewModelTest, ModelAccess) {
    auto retrievedPlanet = viewModel->planet();
    EXPECT_EQ(retrievedPlanet, planet);
    EXPECT_EQ(retrievedPlanet->name(), "Earth");
}

TEST_F(PlanetViewModelTest, SetDifferentPlanet) {
    QSignalSpy nameSpy(viewModel.get(), &PlanetViewModel::nameChanged);
    
    auto newPlanet = std::make_shared<Planet>("Venus", PlanetType::Volcanic, 
                                            0.95, 0.82, 0, 0.72, 737.0, 737.0);
    viewModel->setPlanet(newPlanet);
    
    EXPECT_EQ(viewModel->name(), "Venus");
    EXPECT_EQ(viewModel->planetType(), static_cast<int>(PlanetType::Volcanic));
    EXPECT_GT(nameSpy.count(), 0);
}

TEST_F(PlanetViewModelTest, ToXmlOutput) {
    QString xml = viewModel->toXml();
    EXPECT_TRUE(xml.contains("Earth"));
    EXPECT_TRUE(xml.contains("<Planet"));
}

TEST_F(PlanetViewModelTest, PlanetTypeString) {
    viewModel->setPlanetType(static_cast<int>(PlanetType::Rocky));
    EXPECT_EQ(viewModel->planetTypeString(), "Rocky");
    
    viewModel->setPlanetType(static_cast<int>(PlanetType::GasGiant));
    EXPECT_EQ(viewModel->planetTypeString(), "Gas Giant");
}

TEST_F(PlanetViewModelTest, DefaultConstructor) {
    PlanetViewModel defaultViewModel;
    EXPECT_TRUE(defaultViewModel.name().isEmpty());
    EXPECT_NE(defaultViewModel.planet(), nullptr);
}
