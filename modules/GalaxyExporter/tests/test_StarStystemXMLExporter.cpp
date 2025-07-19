#include "ggh/modules/GalaxyExporter/StarSystemXMLExporter.h"

#include <gtest/gtest.h>

namespace ggh::Galaxy::Exporter {
class StarSystemXMLExporterTest : public ::testing::Test {
protected:
    void SetUp() override {
        ggh::GalaxyCore::utilities::CartesianCoordinates<double> pos{500.0, 600.0};
        starSystemModel = std::make_shared<ggh::GalaxyCore::models::StarSystemModel>(1, "Test System", pos);
        
        // Add some test planets
        ggh::GalaxyCore::models::Planet planet1("Planet A", ggh::GalaxyCore::models::PlanetType::Rocky, 1.0, 1.0, 0, 1.0, 100.0, -50.0);
        starSystemModel->addPlanet(std::move(planet1));

        ggh::GalaxyCore::models::Planet planet2("Planet B", ggh::GalaxyCore::models::PlanetType::GasGiant, 2.0, 5.0, 4, 2.0, 200.0, -100.0);
        starSystemModel->addPlanet(std::move(planet2));

        exporter = std::make_unique<StarSystemXMLExporter>(starSystemModel);
    }

    void TearDown() override {
        exporter.reset();
        starSystemModel.reset();
    }

    std::shared_ptr<ggh::GalaxyCore::models::StarSystemModel> starSystemModel;
    std::unique_ptr<StarSystemXMLExporter> exporter;
};

TEST_F(StarSystemXMLExporterTest, Construction) {
    EXPECT_NE(exporter, nullptr);
}

TEST_F(StarSystemXMLExporterTest, ExportToFile) {
    // Test exporting to a test file
    std::string testFilePath = "test_system_export.xml";
    
    bool result = exporter->exportToFile(testFilePath);
    EXPECT_TRUE(result);
    
    // Check that the format is correct
    EXPECT_EQ(exporter->getFormat(), "XML");
}
}
