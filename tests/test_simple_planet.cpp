#include <QtTest>
#include <QObject>
#include <QTemporaryFile>
#include <QTemporaryDir>
#include <QDebug>

#include "SystemDataManager.h"
#include "StarSystemQml.h" 
#include "Planet.h"

class SimplePlanetTest : public QObject
{
    Q_OBJECT

private slots:
    void testBasicPlanetCreation();
    void testPlanetXmlExport();
    void testPlanetXmlImport();

private:
    SystemDataManager* m_dataManager = nullptr;
};

void SimplePlanetTest::testBasicPlanetCreation()
{
    // Test basic planet creation and properties
    m_dataManager = new SystemDataManager(this);
    QVERIFY(m_dataManager != nullptr);
    
    // Create a test system with a planet
    StarSystemQml* system = new StarSystemQml(100, QPointF(50, 50), StarType::YellowStar, this);
    system->setName("Test System");
    
    Planet* planet = system->addPlanet("Test Planet", 80.0);
    QVERIFY(planet != nullptr);
    QCOMPARE(planet->name(), QString("Test Planet"));
    QCOMPARE(planet->orbitalRadius(), 80.0);
    
    // Test planet property modification
    planet->setType(PlanetType::Rocky);
    planet->setSize(1.5);
    planet->setMass(2.0);
    planet->setNumberOfMoons(2);
    
    QCOMPARE(planet->type(), PlanetType::Rocky);
    QCOMPARE(planet->size(), 1.5);
    QCOMPARE(planet->mass(), 2.0);
    QCOMPARE(planet->numberOfMoons(), 2);
    
    system->deleteLater();
}

void SimplePlanetTest::testPlanetXmlExport()
{
    m_dataManager = new SystemDataManager(this);
    
    // Create system with planet
    StarSystemQml* system = new StarSystemQml(200, QPointF(0, 0), StarType::YellowStar, this);
    system->setName("Export Test System");
    
    Planet* planet = system->addPlanet("Export Test Planet", 75.0);
    planet->setType(PlanetType::Rocky);
    planet->setSize(1.2);
    planet->setMass(1.8);
    planet->setNumberOfMoons(1);
    
    // Export to temporary file
    QTemporaryFile tempFile;
    QVERIFY(tempFile.open());
    QString filePath = tempFile.fileName();
    tempFile.close();
    
    bool exportResult = m_dataManager->exportSystemToXml(system, filePath);
    QVERIFY(exportResult);
    QVERIFY(QFile::exists(filePath));
    
    // Verify file has content
    QFile file(filePath);
    QVERIFY(file.open(QIODevice::ReadOnly));
    QByteArray content = file.readAll();
    QVERIFY(content.size() > 0);
    QVERIFY(content.contains("Export Test Planet"));
    file.close();
    
    system->deleteLater();
}

void SimplePlanetTest::testPlanetXmlImport()
{
    m_dataManager = new SystemDataManager(this);
    
    // Create XML content manually
    QString xmlContent = R"(<?xml version="1.0"?>
<StarSystem>
    <System id="300" name="Import Test System" positionX="100.0" positionY="200.0" 
            starType="1" systemSize="2" starMass="1.5" 
            starTemperature="6000.0" starLuminosity="2.0">
        <Planets>
            <Planet name="Import Test Planet" type="0" size="1.4" mass="1.9" 
                    gravity="1.1" moonCount="1" minTemperature="250" 
                    maxTemperature="300" orbitDistance="85.0" />
        </Planets>
    </System>
</StarSystem>)";
    
    // Write to temporary file
    QTemporaryFile tempFile;
    QVERIFY(tempFile.open());
    QTextStream stream(&tempFile);
    stream << xmlContent;
    tempFile.close();
    
    // Import system
    StarSystemQml* system = m_dataManager->importSystemFromXml(tempFile.fileName());
    QVERIFY(system != nullptr);
    
    // Verify planet was imported
    QCOMPARE(system->getPlanetCount(), 1);
    Planet* planet = system->getPlanetAt(0);
    QVERIFY(planet != nullptr);
    
    // Verify planet properties
    QCOMPARE(planet->name(), QString("Import Test Planet"));
    QCOMPARE(planet->type(), PlanetType::Rocky);
    QCOMPARE(planet->size(), 1.4);
    QCOMPARE(planet->mass(), 1.9);
    QCOMPARE(planet->gForce(), 1.1);
    QCOMPARE(planet->numberOfMoons(), 1);
    QCOMPARE(planet->orbitalRadius(), 85.0);
    
    system->deleteLater();
}

QTEST_MAIN(SimplePlanetTest)
#include "test_simple_planet.moc"
