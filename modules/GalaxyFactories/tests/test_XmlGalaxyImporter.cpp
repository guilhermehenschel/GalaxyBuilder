#include <gtest/gtest.h>
#include <QFile>
#include <QTextStream>
#include <QTemporaryDir>
#include <QTemporaryFile>

#include "ggh/modules/GalaxyFactories/XmlGalaxyImporter.h"
#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/models/TravelLaneModel.h"
#include "ggh/modules/GalaxyCore/utilities/Common.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

using namespace ggh::GalaxyFactories;
using namespace ggh::GalaxyCore::models;
using namespace ggh::GalaxyCore::utilities;

class XmlGalaxyImporterTest : public ::testing::Test {
protected:
    void SetUp() override {
        importer = std::make_unique<XmlGalaxyImporter>();
        tempDir = std::make_unique<QTemporaryDir>();
        ASSERT_TRUE(tempDir->isValid());
    }

    void TearDown() override {
        importer.reset();
        tempDir.reset();
    }

    // Helper method to create a test XML file
    QString createTestXmlFile(const QString& xmlContent) {
        QTemporaryFile* tempFile = new QTemporaryFile(tempDir->path() + "/XXXXXX.xml");
        tempFile->setAutoRemove(false);
        if (tempFile->open()) {
            QTextStream stream(tempFile);
            stream << xmlContent;
            tempFile->close();
            QString filePath = tempFile->fileName();
            delete tempFile;
            return filePath;
        }
        delete tempFile;
        return QString();
    }

    // Helper method to create valid galaxy XML
    QString createValidGalaxyXml() {
        return R"(<?xml version="1.0" encoding="UTF-8"?>
<Galaxy>
    <StarSystem id="1" name="Alpha" positionX="100.0" positionY="200.0" starType="1" systemSize="2">
        <Planet name="Alpha I" type="0" size="1.0" mass="1.0" numberOfMoons="0" orbitalRadius="1.0" maxTemperature="100.0" minTemperature="-50.0"/>
    </StarSystem>
    <StarSystem id="2" name="Beta" positionX="300.0" positionY="400.0" starType="2" systemSize="1">
        <Planet name="Beta I" type="1" size="2.0" mass="5.0" numberOfMoons="4" orbitalRadius="2.0" maxTemperature="200.0" minTemperature="-100.0"/>
        <Planet name="Beta II" type="0" size="1.5" mass="3.0" numberOfMoons="1" orbitalRadius="3.0" maxTemperature="150.0" minTemperature="-75.0"/>
    </StarSystem>
    <TravelLane id="1" fromSystem="1" toSystem="2" length="360.555"/>
</Galaxy>)";
    }

    std::unique_ptr<XmlGalaxyImporter> importer;
    std::unique_ptr<QTemporaryDir> tempDir;
};

TEST_F(XmlGalaxyImporterTest, Construction) {
    EXPECT_NE(importer, nullptr);
}

TEST_F(XmlGalaxyImporterTest, ImportValidGalaxy) {
    QString xmlFilePath = createTestXmlFile(createValidGalaxyXml());
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    
    ASSERT_NE(galaxy, nullptr);
    
    // Check star systems
    auto systems = galaxy->getAllStarSystems();
    EXPECT_EQ(systems.size(), 2);
    
    // Check first system
    auto alpha = galaxy->getStarSystem(1);
    ASSERT_NE(alpha, nullptr);
    EXPECT_EQ(alpha->getName(), "Alpha");
    EXPECT_DOUBLE_EQ(alpha->getPosition().x, 100.0);
    EXPECT_DOUBLE_EQ(alpha->getPosition().y, 200.0);
    EXPECT_EQ(alpha->getStarType(), StarType::YellowStar);
    EXPECT_EQ(alpha->getPlanets().size(), 1);
    
    // Check second system
    auto beta = galaxy->getStarSystem(2);
    ASSERT_NE(beta, nullptr);
    EXPECT_EQ(beta->getName(), "Beta");
    EXPECT_DOUBLE_EQ(beta->getPosition().x, 300.0);
    EXPECT_DOUBLE_EQ(beta->getPosition().y, 400.0);
    EXPECT_EQ(beta->getStarType(), StarType::BlueStar);
    EXPECT_EQ(beta->getPlanets().size(), 2);
    
    // Check travel lanes
    auto lanes = galaxy->getAllTravelLanes();
    EXPECT_EQ(lanes.size(), 1);
    
    auto lane = galaxy->getTravelLane(1);
    ASSERT_NE(lane, nullptr);
    EXPECT_EQ(lane->getFromSystem()->getId(), 1);
    EXPECT_EQ(lane->getToSystem()->getId(), 2);
}

TEST_F(XmlGalaxyImporterTest, ImportEmptyGalaxy) {
    QString xmlContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<Galaxy>
</Galaxy>)";
    
    QString xmlFilePath = createTestXmlFile(xmlContent);
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    
    ASSERT_NE(galaxy, nullptr);
    EXPECT_EQ(galaxy->getAllStarSystems().size(), 0);
    EXPECT_EQ(galaxy->getAllTravelLanes().size(), 0);
}

TEST_F(XmlGalaxyImporterTest, ImportNonExistentFile) {
    auto galaxy = importer->importGalaxy("non_existent_file.xml");
    EXPECT_EQ(galaxy, nullptr);
}

TEST_F(XmlGalaxyImporterTest, ImportInvalidXml) {
    QString xmlContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<InvalidRoot>
    <StarSystem id="1" name="Test"/>
</InvalidRoot>)";
    
    QString xmlFilePath = createTestXmlFile(xmlContent);
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    EXPECT_EQ(galaxy, nullptr);
}

TEST_F(XmlGalaxyImporterTest, ImportMalformedXml) {
    QString xmlContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<Galaxy>
    <StarSystem id="1" name="Test"
</Galaxy>)"; // Missing closing > for StarSystem
    
    QString xmlFilePath = createTestXmlFile(xmlContent);
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    EXPECT_EQ(galaxy, nullptr);
}

TEST_F(XmlGalaxyImporterTest, ImportInvalidSystemData) {
    QString xmlContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<Galaxy>
    <StarSystem id="0" name="" positionX="100.0" positionY="200.0" starType="1" systemSize="2"/>
</Galaxy>)"; // Invalid id=0 and empty name
    
    QString xmlFilePath = createTestXmlFile(xmlContent);
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    EXPECT_EQ(galaxy, nullptr);
}

TEST_F(XmlGalaxyImporterTest, ImportSystemsWithPlanets) {
    QString xmlContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<Galaxy>
    <StarSystem id="1" name="TestSystem" positionX="100.0" positionY="200.0" starType="1" systemSize="2">
        <Planet name="TestPlanet1" type="0" size="1.0" mass="1.0" numberOfMoons="0" orbitalRadius="1.0" maxTemperature="100.0" minTemperature="-50.0"/>
        <Planet name="TestPlanet2" type="1" size="2.0" mass="5.0" numberOfMoons="2" orbitalRadius="2.0" maxTemperature="200.0" minTemperature="-100.0"/>
    </StarSystem>
</Galaxy>)";
    
    QString xmlFilePath = createTestXmlFile(xmlContent);
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    
    ASSERT_NE(galaxy, nullptr);
    
    auto system = galaxy->getStarSystem(1);
    ASSERT_NE(system, nullptr);
    EXPECT_EQ(system->getPlanets().size(), 2);
    
    auto planets = system->getPlanets();
    EXPECT_EQ(planets[0]->name(), "TestPlanet1");
    EXPECT_EQ(planets[1]->name(), "TestPlanet2");
}

TEST_F(XmlGalaxyImporterTest, ImportTravelLanesWithInvalidSystems) {
    QString xmlContent = R"(<?xml version="1.0" encoding="UTF-8"?>
<Galaxy>
    <StarSystem id="1" name="Alpha" positionX="100.0" positionY="200.0" starType="1" systemSize="2"/>
    <TravelLane id="1" fromSystem="1" toSystem="999" length="100.0"/>
</Galaxy>)"; // TravelLane references non-existent system 999
    
    QString xmlFilePath = createTestXmlFile(xmlContent);
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    EXPECT_EQ(galaxy, nullptr);
}

TEST_F(XmlGalaxyImporterTest, FactoryPattern) {
    // Test using the factory interface
    importer->setXmlPath("test.xml");
    
    // Since file doesn't exist, should return nullptr
    auto galaxy = importer->generateGalaxy();
    EXPECT_EQ(galaxy, nullptr);
}

TEST_F(XmlGalaxyImporterTest, RoundTripCompatibility) {
    // Test that imported galaxy matches original structure
    QString xmlContent = createValidGalaxyXml();
    QString xmlFilePath = createTestXmlFile(xmlContent);
    ASSERT_FALSE(xmlFilePath.isEmpty());

    auto galaxy = importer->importGalaxy(xmlFilePath);
    ASSERT_NE(galaxy, nullptr);
    
    // Export and re-import to test round-trip compatibility
    QString exportedXml = galaxy->toXml().c_str();
    QString exportFilePath = createTestXmlFile(exportedXml);
    ASSERT_FALSE(exportFilePath.isEmpty());
    
    auto reimportedGalaxy = importer->importGalaxy(exportFilePath);
    ASSERT_NE(reimportedGalaxy, nullptr);
    
    // Compare basic structure
    EXPECT_EQ(galaxy->getAllStarSystems().size(), reimportedGalaxy->getAllStarSystems().size());
    EXPECT_EQ(galaxy->getAllTravelLanes().size(), reimportedGalaxy->getAllTravelLanes().size());
    
    // Compare first system details
    auto originalSystem = galaxy->getStarSystem(1);
    auto reimportedSystem = reimportedGalaxy->getStarSystem(1);
    ASSERT_NE(originalSystem, nullptr);
    ASSERT_NE(reimportedSystem, nullptr);
    
    EXPECT_EQ(originalSystem->getName(), reimportedSystem->getName());
    EXPECT_DOUBLE_EQ(originalSystem->getPosition().x, reimportedSystem->getPosition().x);
    EXPECT_DOUBLE_EQ(originalSystem->getPosition().y, reimportedSystem->getPosition().y);
}
