#include "ggh/modules/GalaxyExporter/include/ggh/modules/GalaxyExporter/ExporterObject.h"

#include <gtest/gtest.h>

namespace ggh::Galaxy::Exporter {

class GalaxyXMLExporterTest : public ::testing::Test {
protected:
    void SetUp() override {
        galaxyModel = std::make_shared<GalaxyModel>(1000, 1000);
        
        // Add some test star systems
        ggh::GalaxyCore::utilities::CartesianCoordinates<double> pos1{100.0, 200.0};
        auto system1 = std::make_shared<StarSystemModel>(1, "Alpha", pos1);
        Planet planet1("Alpha I", PlanetType::Rocky, 1.0, 1.0, 0, 1.0, 100.0, -50.0);
        system1->addPlanet(planet1);
        galaxyModel->addStarSystem(std::move(system1));
        
        ggh::GalaxyCore::utilities::CartesianCoordinates<double> pos2{300.0, 400.0};
        auto system2 = std::make_shared<StarSystemModel>(2, "Beta", pos2);
        Planet planet2("Beta I", PlanetType::GasGiant, 2.0, 5.0, 4, 2.0, 200.0, -100.0);
        system2->addPlanet(planet2);
        galaxyModel->addStarSystem(std::move(system2));
        
        exporter = std::make_unique<GalaxyXMLExporter>(galaxyModel);
    }

    void TearDown() override {
        exporter.reset();
        galaxyModel.reset();
    }

    std::shared_ptr<GalaxyModel> galaxyModel;
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
