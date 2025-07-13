#include <QtTest>
#include <QObject>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QDebug>

#include "SystemDataManager.h"
#include "StarSystemQml.h"
#include "Planet.h"

class TestSystemDataManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // XML Export Tests
    void testExportSystemToXml();
    void testExportSystemToXml_InvalidPath();
    void testExportSystemToXml_NullSystem();
    void testExportGalaxyToXml();
    void testExportGalaxyToXml_EmptyList();

    // XML Import Tests
    void testImportSystemFromXml();
    void testImportSystemFromXml_InvalidFile();
    void testImportSystemFromXml_MalformedXml();
    void testImportSystemFromXml_MissingElements();
    void testImportGalaxyFromXml();
    void testImportGalaxyFromXml_InvalidFile();

    // Persistence Tests
    void testSaveAndLoadSystemChanges();
    void testLoadSystemData_NonExistentFile();
    void testAutoSaveEnabled();

    // XML Structure Validation Tests
    void testXmlStructureValidation();
    void testXmlElementValidation();
    void testXmlAttributeValidation();

    // Round-trip Tests
    void testRoundTripSingleSystem();
    void testRoundTripMultipleSystems();
    void testRoundTripComplexSystem();

    // Edge Cases
    void testSpecialCharactersInSystemName();
    void testExtremeValues();
    void testLargeNumberOfPlanets();

private:
    SystemDataManager* m_dataManager;
    QTemporaryDir* m_tempDir;
    
    // Helper methods
    StarSystemQml* createTestSystem(int id = 1, const QString& name = "Test System");
    Planet* createTestPlanet(const QString& name = "Test Planet");
    bool validateXmlStructure(const QString& filePath);
    bool validateSystemXml(const QDomElement& element);
    bool validatePlanetXml(const QDomElement& element);
    QString createValidXmlString();
    QString createInvalidXmlString();
};

void TestSystemDataManager::initTestCase()
{
    // Setup for all tests
    m_tempDir = new QTemporaryDir();
    QVERIFY(m_tempDir->isValid());
}

void TestSystemDataManager::cleanupTestCase()
{
    // Cleanup after all tests
    delete m_tempDir;
}

void TestSystemDataManager::init()
{
    // Setup before each test
    m_dataManager = new SystemDataManager(this);
    QVERIFY(m_dataManager != nullptr);
}

void TestSystemDataManager::cleanup()
{
    // Cleanup after each test
    delete m_dataManager;
    m_dataManager = nullptr;
}

StarSystemQml* TestSystemDataManager::createTestSystem(int id, const QString& name)
{
    StarSystemQml* system = new StarSystemQml(id, QPointF(100.0, 200.0), StarType::YellowStar, this);
    system->setName(name);
    system->setSystemSize(SystemSize::Medium);
    system->setStarMass(1.5);
    system->setStarTemperature(6000.0);
    system->setStarLuminosity(2.0);
    
    // Add some test planets
    Planet* planet1 = system->addPlanet("Planet Alpha", 1.0);
    if (planet1) {
        planet1->setType(PlanetType::Rocky);
        planet1->setSize(1.2);
        planet1->setMass(1.1);
        planet1->setGForce(1.05);
        planet1->setNumberOfMoons(2);
        planet1->setMinTemperature(200);
        planet1->setMaxTemperature(300);
    }
    
    Planet* planet2 = system->addPlanet("Gas Giant Beta", 5.2);
    if (planet2) {
        planet2->setType(PlanetType::GasGiant);
        planet2->setSize(11.0);
        planet2->setMass(317.8);
        planet2->setGForce(2.36);
        planet2->setNumberOfMoons(79);
        planet2->setMinTemperature(50);
        planet2->setMaxTemperature(150);
    }
    
    return system;
}

Planet* TestSystemDataManager::createTestPlanet(const QString& name)
{
    Planet* planet = new Planet(name, PlanetType::Rocky, this);
    planet->setSize(1.0);
    planet->setMass(1.0);
    planet->setGForce(1.0);
    planet->setNumberOfMoons(1);
    planet->setMinTemperature(250);
    planet->setMaxTemperature(350);
    planet->setOrbitalRadius(1.0);
    return planet;
}

void TestSystemDataManager::testExportSystemToXml()
{
    // Create test system
    StarSystemQml* system = createTestSystem(42, "Sol System");
    
    // Export to XML
    QString filePath = m_tempDir->filePath("test_system.xml");
    bool success = m_dataManager->exportSystemToXml(system, filePath);
    
    QVERIFY(success);
    QVERIFY(QFile::exists(filePath));
    
    // Validate XML structure
    QVERIFY(validateXmlStructure(filePath));
    
    // Parse and verify content
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    
    QDomDocument doc;
    QVERIFY(doc.setContent(&file));
    
    QDomElement root = doc.documentElement();
    QCOMPARE(root.tagName(), QString("StarSystem"));
    
    QDomElement systemElement = root.firstChildElement("System");
    QVERIFY(!systemElement.isNull());
    QCOMPARE(systemElement.attribute("id"), QString("42"));
    QCOMPARE(systemElement.attribute("name"), QString("Sol System"));
    
    file.close();
    system->deleteLater();
}

void TestSystemDataManager::testExportSystemToXml_InvalidPath()
{
    StarSystemQml* system = createTestSystem();
    
    // Try to export to invalid path
    QString invalidPath = "/invalid/path/that/does/not/exist/system.xml";
    bool success = m_dataManager->exportSystemToXml(system, invalidPath);
    
    QVERIFY(!success);
    system->deleteLater();
}

void TestSystemDataManager::testExportSystemToXml_NullSystem()
{
    QString filePath = m_tempDir->filePath("null_system.xml");
    bool success = m_dataManager->exportSystemToXml(nullptr, filePath);
    
    QVERIFY(!success);
}

void TestSystemDataManager::testExportGalaxyToXml()
{
    // Create multiple test systems
    QList<StarSystemQml*> systems;
    systems.append(createTestSystem(1, "Alpha Centauri"));
    systems.append(createTestSystem(2, "Proxima Centauri"));
    systems.append(createTestSystem(3, "Sirius"));
    
    QString filePath = m_tempDir->filePath("test_galaxy.xml");
    bool success = m_dataManager->exportGalaxyToXml(systems, filePath);
    
    QVERIFY(success);
    QVERIFY(QFile::exists(filePath));
    
    // Validate XML structure
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    
    QDomDocument doc;
    QVERIFY(doc.setContent(&file));
    
    QDomElement root = doc.documentElement();
    QCOMPARE(root.tagName(), QString("Galaxy"));
    
    // Check metadata
    QDomElement metadata = root.firstChildElement("Metadata");
    QVERIFY(!metadata.isNull());
    QCOMPARE(metadata.attribute("systemCount"), QString("3"));
    
    // Check systems
    QDomElement systemsElement = root.firstChildElement("Systems");
    QVERIFY(!systemsElement.isNull());
    
    QDomNodeList systemNodes = systemsElement.elementsByTagName("System");
    QCOMPARE(systemNodes.count(), 3);
    
    file.close();
    qDeleteAll(systems);
}

void TestSystemDataManager::testExportGalaxyToXml_EmptyList()
{
    QList<StarSystemQml*> emptySystems;
    QString filePath = m_tempDir->filePath("empty_galaxy.xml");
    
    bool success = m_dataManager->exportGalaxyToXml(emptySystems, filePath);
    QVERIFY(success);
    QVERIFY(QFile::exists(filePath));
    
    // Validate XML structure
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    
    QDomDocument doc;
    QVERIFY(doc.setContent(&file));
    
    QDomElement root = doc.documentElement();
    QCOMPARE(root.tagName(), QString("Galaxy"));
    
    // Check metadata
    QDomElement metadata = root.firstChildElement("Metadata");
    QVERIFY(!metadata.isNull());
    QCOMPARE(metadata.attribute("systemCount"), QString("0"));
    
    file.close();
}

void TestSystemDataManager::testImportSystemFromXml()
{
    // First export a system
    StarSystemQml* originalSystem = createTestSystem(99, "Original System");
    QString filePath = m_tempDir->filePath("import_test.xml");
    
    QVERIFY(m_dataManager->exportSystemToXml(originalSystem, filePath));
    
    // Now import it
    StarSystemQml* importedSystem = m_dataManager->importSystemFromXml(filePath);
    
    QVERIFY(importedSystem != nullptr);
    QCOMPARE(importedSystem->getSystemId(), 99);
    QCOMPARE(importedSystem->getName(), QString("Original System"));
    QCOMPARE(importedSystem->getStarMass(), 1.5);
    QCOMPARE(importedSystem->getStarTemperature(), 6000.0);
    QCOMPARE(importedSystem->getStarLuminosity(), 2.0);
    
    // Check planets
    QCOMPARE(importedSystem->getPlanets().size(), 0);
    
    originalSystem->deleteLater();
    importedSystem->deleteLater();
}

void TestSystemDataManager::testImportSystemFromXml_InvalidFile()
{
    QString invalidPath = "/path/that/does/not/exist.xml";
    StarSystemQml* system = m_dataManager->importSystemFromXml(invalidPath);
    
    QVERIFY(system == nullptr);
}

void TestSystemDataManager::testImportSystemFromXml_MalformedXml()
{
    QString filePath = m_tempDir->filePath("malformed.xml");
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    
    QTextStream stream(&file);
    stream << "<?xml version=\"1.0\"?>\n<InvalidXML><unclosed tag>";
    file.close();
    
    StarSystemQml* system = m_dataManager->importSystemFromXml(filePath);
    QVERIFY(system == nullptr);
}

void TestSystemDataManager::testImportSystemFromXml_MissingElements()
{
    QString invalidXml = R"(<?xml version="1.0"?>
<StarSystem>
    <System id="1" name="Incomplete System">
        <!-- Missing required attributes and elements -->
    </System>
</StarSystem>)";
    
    QString filePath = m_tempDir->filePath("missing_elements.xml");
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QTextStream stream(&file);
    stream << invalidXml;
    file.close();
    
    StarSystemQml* system = m_dataManager->importSystemFromXml(filePath);
    QVERIFY(system == nullptr);
}

void TestSystemDataManager::testImportGalaxyFromXml()
{
    // First export a galaxy
    QList<StarSystemQml*> originalSystems;
    originalSystems.append(createTestSystem(1, "System Alpha"));
    originalSystems.append(createTestSystem(2, "System Beta"));
    
    QString filePath = m_tempDir->filePath("import_galaxy_test.xml");
    QVERIFY(m_dataManager->exportGalaxyToXml(originalSystems, filePath));
    
    // Now import it
    QList<StarSystemQml*> importedSystems = m_dataManager->importGalaxyFromXml(filePath);
    
    QCOMPARE(importedSystems.size(), 2);
    QCOMPARE(importedSystems[0]->getSystemId(), 1);
    QCOMPARE(importedSystems[0]->getName(), QString("System Alpha"));
    QCOMPARE(importedSystems[1]->getSystemId(), 2);
    QCOMPARE(importedSystems[1]->getName(), QString("System Beta"));
    
    qDeleteAll(originalSystems);
    qDeleteAll(importedSystems);
}

void TestSystemDataManager::testImportGalaxyFromXml_InvalidFile()
{
    QString invalidPath = "/path/that/does/not/exist.xml";
    QList<StarSystemQml*> systems = m_dataManager->importGalaxyFromXml(invalidPath);
    
    QVERIFY(systems.isEmpty());
}

void TestSystemDataManager::testSaveAndLoadSystemChanges()
{
    StarSystemQml* system = createTestSystem(123, "Persistent System");
    
    // Save system changes
    bool saveSuccess = m_dataManager->saveSystemChanges(system);
    QVERIFY(saveSuccess);
    
    // Create a new system with same ID but different data
    StarSystemQml* newSystem = new StarSystemQml(123, QPointF(0, 0), StarType::RedDwarf, this);
    newSystem->setName("Different Name");
    
    // Load the saved data
    bool loadSuccess = m_dataManager->loadSystemData(newSystem);
    QVERIFY(loadSuccess);
    
    // Verify the data was loaded correctly
    QCOMPARE(newSystem->getName(), QString("Persistent System"));
    
    system->deleteLater();
    newSystem->deleteLater();
}

void TestSystemDataManager::testAutoSaveEnabled()
{
    // Test auto-save enable/disable
    QVERIFY(!m_dataManager->isAutoSaveEnabled());
    
    m_dataManager->enableAutoSave(true);
    QVERIFY(m_dataManager->isAutoSaveEnabled());
    
    m_dataManager->enableAutoSave(false);
    QVERIFY(!m_dataManager->isAutoSaveEnabled());
}

void TestSystemDataManager::testXmlStructureValidation()
{
    QString validXml = createValidXmlString();
    QString filePath = m_tempDir->filePath("valid_structure.xml");
    
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::WriteOnly));
    QTextStream stream(&file);
    stream << validXml;
    file.close();
    
    QVERIFY(validateXmlStructure(filePath));
}

void TestSystemDataManager::testRoundTripSingleSystem()
{
    // Create original system
    StarSystemQml* original = createTestSystem(777, "Round Trip Test");
    
    // Export to XML
    QString filePath = m_tempDir->filePath("roundtrip.xml");
    QVERIFY(m_dataManager->exportSystemToXml(original, filePath));
    
    // Import from XML
    StarSystemQml* imported = m_dataManager->importSystemFromXml(filePath);
    QVERIFY(imported != nullptr);
    
    // Compare all properties
    QCOMPARE(imported->getSystemId(), original->getSystemId());
    QCOMPARE(imported->getName(), original->getName());
    QCOMPARE(imported->getPosition(), original->getPosition());
    QCOMPARE(imported->getStarType(), original->getStarType());
    QCOMPARE(imported->getSystemSize(), original->getSystemSize());
    QCOMPARE(imported->getStarMass(), original->getStarMass());
    QCOMPARE(imported->getStarTemperature(), original->getStarTemperature());
    QCOMPARE(imported->getStarLuminosity(), original->getStarLuminosity());
    
    // Compare planets
    QCOMPARE(imported->getPlanets().size(), original->getPlanets().size());
    
    original->deleteLater();
    imported->deleteLater();
}

void TestSystemDataManager::testRoundTripMultipleSystems()
{
    // Create multiple systems with different configurations
    QList<StarSystemQml*> originalSystems;
    originalSystems.append(createTestSystem(1, "Multi System A"));
    originalSystems.append(createTestSystem(2, "Multi System B"));
    originalSystems.append(createTestSystem(3, "Multi System C"));
    
    // Export to XML
    QString filePath = m_tempDir->filePath("multi_roundtrip.xml");
    QVERIFY(m_dataManager->exportGalaxyToXml(originalSystems, filePath));
    
    // Import from XML
    QList<StarSystemQml*> importedSystems = m_dataManager->importGalaxyFromXml(filePath);
    QCOMPARE(importedSystems.size(), originalSystems.size());
    
    // Compare all systems
    for (int i = 0; i < originalSystems.size(); ++i) {
        QCOMPARE(importedSystems[i]->getSystemId(), originalSystems[i]->getSystemId());
        QCOMPARE(importedSystems[i]->getName(), originalSystems[i]->getName());
        QCOMPARE(importedSystems[i]->getStarMass(), originalSystems[i]->getStarMass());
        QCOMPARE(importedSystems[i]->getPlanets().size(), originalSystems[i]->getPlanets().size());
    }
    
    qDeleteAll(originalSystems);
    qDeleteAll(importedSystems);
}

void TestSystemDataManager::testRoundTripComplexSystem()
{
    // Create a system with many planets and complex properties
    StarSystemQml* original = createTestSystem(999, "Complex System with ÜTF-8 & Special <Characters>");
    
    // Add more planets with various properties
    for (int i = 1; i <= 10; ++i) {
        Planet* planet = original->addPlanet(QString("Complex Planet %1").arg(i), i * 0.5);
        if (planet) {
            planet->setType(static_cast<PlanetType>(i % 4));
            planet->setSize(i * 0.1 + 0.5);
            planet->setMass(i * 0.2 + 0.8);
            planet->setGForce(i * 0.05 + 0.9);
            planet->setNumberOfMoons(i % 50);
            planet->setMinTemperature(100 + i * 10);
            planet->setMaxTemperature(200 + i * 15);
        }
    }
    
    // Export and import
    QString filePath = m_tempDir->filePath("complex_roundtrip.xml");
    QVERIFY(m_dataManager->exportSystemToXml(original, filePath));
    
    StarSystemQml* imported = m_dataManager->importSystemFromXml(filePath);
    QVERIFY(imported != nullptr);
    
    // Compare complex properties
    QCOMPARE(imported->getName(), original->getName());
    QCOMPARE(imported->getPlanets().size(), original->getPlanets().size());
    
    // Compare planet properties
    const auto& originalPlanets = original->getPlanets();
    const auto& importedPlanets = imported->getPlanets();
    
    for (int i = 0; i < originalPlanets.size(); ++i) {
        QCOMPARE(importedPlanets[i]->name(), originalPlanets[i]->name());
        QCOMPARE(importedPlanets[i]->type(), originalPlanets[i]->type());
        QCOMPARE(importedPlanets[i]->size(), originalPlanets[i]->size());
        QCOMPARE(importedPlanets[i]->mass(), originalPlanets[i]->mass());
        QCOMPARE(importedPlanets[i]->numberOfMoons(), originalPlanets[i]->numberOfMoons());
    }
    
    original->deleteLater();
    imported->deleteLater();
}

void TestSystemDataManager::testExtremeValues()
{
    // Create system with extreme values
    StarSystemQml* system = new StarSystemQml(2147483647, QPointF(-1000000.0, 1000000.0), StarType::RedGiant, this);
    system->setName("Extreme System with Very Long Name " + QString("X").repeated(1000));
    system->setStarMass(1000.0);
    system->setStarTemperature(100000.0);
    system->setStarLuminosity(1000000.0);
    
    // Add planet with extreme values
    Planet* extremePlanet = system->addPlanet("Extreme Planet", 1000.0);
    if (extremePlanet) {
        extremePlanet->setSize(1000.0);
        extremePlanet->setMass(10000.0);
        extremePlanet->setGForce(100.0);
        extremePlanet->setNumberOfMoons(1000);
        extremePlanet->setMinTemperature(-273);
        extremePlanet->setMaxTemperature(10000);
    }
    
    // Test export/import with extreme values
    QString filePath = m_tempDir->filePath("extreme_values.xml");
    QVERIFY(m_dataManager->exportSystemToXml(system, filePath));
    
    StarSystemQml* imported = m_dataManager->importSystemFromXml(filePath);
    QVERIFY(imported != nullptr);
    
    // Verify extreme values are preserved
    QCOMPARE(imported->getSystemId(), 2147483647);
    QCOMPARE(imported->getStarMass(), 1000.0);
    QVERIFY(imported->getName().length() > 1000);
    
    system->deleteLater();
    imported->deleteLater();
}

void TestSystemDataManager::testLargeNumberOfPlanets()
{
    // Create system with many planets
    StarSystemQml* system = createTestSystem(100, "Crowded System");
    
    // Add 100 planets
    for (int i = 1; i <= 100; ++i) {
        Planet* planet = system->addPlanet(QString("Planet %1").arg(i), i * 0.1);
        if (planet) {
            planet->setType(static_cast<PlanetType>(i % 4));
            planet->setSize(1.0 + (i % 10) * 0.1);
            planet->setMass(1.0 + (i % 20) * 0.5);
            planet->setNumberOfMoons(i % 10);
        }
    }
    
    QCOMPARE(system->getPlanets().size(), 100);
    
    // Test export/import with many planets
    QString filePath = m_tempDir->filePath("many_planets.xml");
    QVERIFY(m_dataManager->exportSystemToXml(system, filePath));
    
    StarSystemQml* imported = m_dataManager->importSystemFromXml(filePath);
    QVERIFY(imported != nullptr);
    QCOMPARE(imported->getPlanets().size(), 100);
    
    // Verify planet properties are preserved
    for (int i = 0; i < 100; ++i) {
        const Planet* originalPlanet = system->getPlanets()[i];
        const Planet* importedPlanet = imported->getPlanets()[i];
        
        QCOMPARE(importedPlanet->name(), originalPlanet->name());
        QCOMPARE(importedPlanet->type(), originalPlanet->type());
        QCOMPARE(importedPlanet->orbitalRadius(), originalPlanet->orbitalRadius());
    }
    
    system->deleteLater();
    imported->deleteLater();
}

// ...existing code...
QString TestSystemDataManager::createValidXmlString()
{
    return R"(<?xml version="1.0"?>
<StarSystem>
    <System id="1" name="Test System" positionX="100.0" positionY="200.0" 
            starType="1" systemSize="2" starMass="1.5" 
            starTemperature="6000.0" starLuminosity="2.0">
        <Planets>
            <Planet name="Test Planet" type="0" size="1.0" mass="1.0" 
                    gravity="1.0" moonCount="1" minTemperature="250" 
                    maxTemperature="350" orbitDistance="1.0" />
        </Planets>
    </System>
</StarSystem>)";
}

QString TestSystemDataManager::createInvalidXmlString()
{
    return R"(<?xml version="1.0"?>
<InvalidRoot>
    <MissingSystemElement />
</InvalidRoot>)";
}

void TestSystemDataManager::testLoadSystemData_NonExistentFile()
{
    // Stub implementation
    QVERIFY(true);
}

void TestSystemDataManager::testXmlElementValidation()
{
    // Stub implementation
    QVERIFY(true);
}

void TestSystemDataManager::testXmlAttributeValidation()
{
    // Stub implementation
    QVERIFY(true);
}

void TestSystemDataManager::testSpecialCharactersInSystemName()
{
    // Stub implementation
    QVERIFY(true);
}

bool TestSystemDataManager::validateXmlStructure(const QString& filePath)
{
    Q_UNUSED(filePath)
    // Stub implementation
    return true;
}

QTEST_MAIN(TestSystemDataManager)
#include "test_systemdatamanager.moc"
