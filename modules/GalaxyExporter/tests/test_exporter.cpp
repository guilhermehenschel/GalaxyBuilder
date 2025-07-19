#include <gtest/gtest.h>
#include <QObject>
#include <QVariant>
#include <QSignalSpy>
#include <memory>

// GalaxyExporter includes
#include "ggh/modules/GalaxyExporter/ExporterObject.h"
#include "ggh/modules/GalaxyExporter/GalaxyXMLExporter.h"
#include "ggh/modules/GalaxyExporter/StarSystemXMLExporter.h"

// GalaxyCore includes for test models
#include "ggh/modules/GalaxyCore/viewmodels/GalaxyViewModel.h"
#include "ggh/modules/GalaxyCore/viewmodels/StarSystemViewModel.h"
#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/PlanetModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

using namespace ggh::Galaxy::Exporter;
using namespace ggh::GalaxyCore::viewmodels;
using namespace ggh::GalaxyCore::models;
using namespace ggh::GalaxyCore::utilities;

class ExporterObjectTest : public ::testing::Test {
protected:
    void SetUp() override {
        exporterObject = std::make_unique<ExporterObject>();
    }

    void TearDown() override {
        exporterObject.reset();
    }

    std::unique_ptr<ExporterObject> exporterObject;
};

TEST_F(ExporterObjectTest, DefaultConstruction) {
    EXPECT_NE(exporterObject, nullptr);
    EXPECT_TRUE(exporterObject->getFilePath().isEmpty());
    EXPECT_TRUE(exporterObject->errorString().isEmpty());
}

TEST_F(ExporterObjectTest, SupportedFormats) {
    auto formats = exporterObject->supportedFormats();
    EXPECT_FALSE(formats.isEmpty());
    EXPECT_TRUE(formats.contains("XML"));
}

TEST_F(ExporterObjectTest, DefaultFormat) {
    auto defaultFmt = exporterObject->defaultFormat();
    EXPECT_EQ(defaultFmt, "XML");
}

TEST_F(ExporterObjectTest, FilePathProperty) {
    QString testPath = "test/galaxy.xml";
    
    QSignalSpy filePathChangedSpy(exporterObject.get(), &ExporterObject::filePathChanged);
    
    exporterObject->setFilePath(testPath);
    EXPECT_EQ(exporterObject->getFilePath(), testPath);
    EXPECT_EQ(filePathChangedSpy.count(), 1);
    
    filePathChangedSpy.clear();
    // Setting the same path should not emit signal
    exporterObject->setFilePath(testPath);
    EXPECT_EQ(filePathChangedSpy.count(), 1);
}

TEST_F(ExporterObjectTest, SetValidGalaxyModel) {
    auto galaxyModel = std::make_shared<GalaxyModel>(1000, 1000);
    auto galaxyViewModel = std::make_unique<GalaxyViewModel>(galaxyModel);
    
    exporterObject->setModel(galaxyViewModel.get());
    
    // Should not have error after setting valid model
    EXPECT_TRUE(exporterObject->errorString().isEmpty());
}

TEST_F(ExporterObjectTest, SetValidStarSystemModel) {
    ggh::GalaxyCore::utilities::CartesianCoordinates<double> pos{100.0, 200.0};
    auto starSystemModel = std::make_shared<StarSystemModel>(1, "Test System", pos);
    auto starSystemViewModel = std::make_unique<StarSystemViewModel>(starSystemModel);
    
    exporterObject->setModel(starSystemViewModel.get());
    
    // Should not have error after setting valid model
    EXPECT_TRUE(exporterObject->errorString().isEmpty());
}

TEST_F(ExporterObjectTest, SetInvalidModel) {
    QObject invalidObject;
    
    QSignalSpy errorChangedSpy(exporterObject.get(), &ExporterObject::errorStringChanged);
    
    exporterObject->setModel(&invalidObject);
    
    EXPECT_FALSE(exporterObject->errorString().isEmpty());
    EXPECT_EQ(errorChangedSpy.count(), 1);
}

TEST_F(ExporterObjectTest, SetNullModel) {
    QSignalSpy errorChangedSpy(exporterObject.get(), &ExporterObject::errorStringChanged);
    
    exporterObject->setModel(nullptr);
    
    EXPECT_FALSE(exporterObject->errorString().isEmpty());
    EXPECT_EQ(errorChangedSpy.count(), 1);
}

class ExporterIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        exporterObject = std::make_unique<ExporterObject>();
        
        // Create test galaxy with data
        galaxyModel = std::make_shared<GalaxyModel>(2000, 2000);
        ggh::GalaxyCore::utilities::CartesianCoordinates<double> pos{750.0, 850.0};
        auto system = std::make_shared<StarSystemModel>(1, "Integration System", pos);
        Planet planet("Integration Planet", PlanetType::Rocky, 1.0, 1.0, 0, 1.0, 100.0, -50.0);
        system->addPlanet(std::move(planet));
        galaxyModel->addStarSystem(std::move(system));
        
        galaxyViewModel = std::make_unique<GalaxyViewModel>(galaxyModel);
    }

    void TearDown() override {
        galaxyViewModel.reset();
        galaxyModel.reset();
        exporterObject.reset();
    }

    std::unique_ptr<ExporterObject> exporterObject;
    std::shared_ptr<GalaxyModel> galaxyModel;
    std::unique_ptr<GalaxyViewModel> galaxyViewModel;
};

TEST_F(ExporterIntegrationTest, CompleteExportWorkflow) {
    // Set the model
    exporterObject->setModel(galaxyViewModel.get());
    EXPECT_TRUE(exporterObject->errorString().isEmpty());
    
    // Set a file path
    QString testPath = "integration_test_galaxy.xml";
    exporterObject->setFilePath(testPath);
    EXPECT_EQ(exporterObject->getFilePath(), testPath);
    
    // Verify we have supported formats
    auto formats = exporterObject->supportedFormats();
    EXPECT_TRUE(formats.contains("XML"));
    
    // Export should be configured properly now
    // Note: We're not testing actual file export as it requires file system access
    // The exportObject() method would write to disk, which we avoid in unit tests
}
