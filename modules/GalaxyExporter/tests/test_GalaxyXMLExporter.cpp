#include "ggh/modules/GalaxyExporter/GalaxyXMLExporter.h"

#include <gtest/gtest.h>

#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

namespace ggh::Galaxy::Exporter {

class GalaxyXMLExporterTest : public ::testing::Test {
protected:
    void SetUp() override {
        galaxyModel = std::make_shared<GalaxyCore::models::GalaxyModel>(1000, 1000);
        
        // Add some test star systems
        ggh::GalaxyCore::utilities::CartesianCoordinates<double> pos1{100.0, 200.0};
        auto system1 = std::make_shared<GalaxyCore::models::StarSystemModel>(1, "Alpha", pos1);
        GalaxyCore::models::Planet planet1("Alpha I", GalaxyCore::models::PlanetType::Rocky, 1.0, 1.0, 0, 1.0, 100.0, -50.0);
        system1->addPlanet(std::move(planet1));
        galaxyModel->addStarSystem(std::move(system1));
        
        ggh::GalaxyCore::utilities::CartesianCoordinates<double> pos2{300.0, 400.0};
        auto system2 = std::make_shared<GalaxyCore::models::StarSystemModel>(2, "Beta", pos2);
        GalaxyCore::models::Planet planet2("Beta I", GalaxyCore::models::PlanetType::GasGiant, 2.0, 5.0, 4, 2.0, 200.0, -100.0);
        system2->addPlanet(std::move(planet2));
        galaxyModel->addStarSystem(std::move(system2));
        
        exporter = std::make_unique<GalaxyXMLExporter>(galaxyModel);
    }

    void TearDown() override {
        exporter.reset();
        galaxyModel.reset();
    }

    std::shared_ptr<GalaxyCore::models::GalaxyModel> galaxyModel;
    std::unique_ptr<GalaxyXMLExporter> exporter;
};

TEST_F(GalaxyXMLExporterTest, Construction) {
    EXPECT_NE(exporter, nullptr);
}

TEST_F(GalaxyXMLExporterTest, ExportToFile) {
    // Test exporting to a test file
    std::string testFilePath = "test_galaxy_export.xml";
    
    bool result = exporter->exportToFile(testFilePath);
    EXPECT_TRUE(result);
    
    // Check that the format is correct
    EXPECT_EQ(exporter->getFormat(), "XML");
}
}
