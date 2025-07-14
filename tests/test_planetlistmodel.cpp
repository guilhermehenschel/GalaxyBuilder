/**
 * @file test_planetlistmodel.cpp
 * @brief Unit tests for the PlanetListModel class.
 */

#include <QtTest>
#include <QObject>
#include <QSignalSpy>
#include <QAbstractItemModelTester>
#include <QtWidgets/QApplication>
#include <memory>

#include "PlanetListModel.h"
#include "Planet.h"

/**
 * @class TestPlanetListModel
 * @brief Test class for PlanetListModel functionality.
 * 
 * This class contains comprehensive unit tests for the PlanetListModel class,
 * covering all public methods and ensuring proper model behavior.
 */
class TestPlanetListModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Called before the first test function is executed.
     */
    void initTestCase();
    
    /**
     * @brief Called after the last test function is executed.
     */
    void cleanupTestCase();
    
    /**
     * @brief Called before each test function is executed.
     */
    void init();
    
    /**
     * @brief Called after each test function is executed.
     */
    void cleanup();

    // Basic functionality tests
    void testInitialState();
    void testAddPlanet();
    void testAddPlanetWithParameters();
    void testRemovePlanet();
    void testRemovePlanetAt();
    void testGetPlanetAt();
    void testGetPlanetCount();
    void testClearPlanets();
    
    // Model interface tests
    void testRowCount();
    void testData();
    void testRoleNames();
    void testModelSignals();
    void testInvalidIndices();
    
    // Signal/slot tests
    void testPlanetPropertyChanges();
    void testDataChangedSignals();
    
    // Edge cases
    void testRemoveNullPlanet();
    void testRemoveInvalidIndex();
    void testGetInvalidIndex();
    void testMultiplePlanets();
    
    // Integration tests
    void testModelTester();

private:
    std::unique_ptr<PlanetListModel> m_model;
    
    /**
     * @brief Helper function to create a test planet.
     * @param name The planet name.
     * @param orbitDistance The orbital distance.
     * @return Pointer to the created planet.
     */
    Planet* createTestPlanet(const QString& name = "Test Planet", double orbitDistance = 1.0);
    
    /**
     * @brief Helper function to verify planet data matches expected values.
     * @param planet The planet to verify.
     * @param expectedName Expected name.
     * @param expectedOrbitDistance Expected orbit distance.
     */
    void verifyPlanetData(Planet* planet, const QString& expectedName, double expectedOrbitDistance);
};

void TestPlanetListModel::initTestCase()
{
    // Initialize any global test resources if needed
}

void TestPlanetListModel::cleanupTestCase()
{
    // Clean up any global test resources if needed
}

void TestPlanetListModel::init()
{
    m_model = std::make_unique<PlanetListModel>();
}

void TestPlanetListModel::cleanup()
{
    m_model.reset();
}

void TestPlanetListModel::testInitialState()
{
    QCOMPARE(m_model->rowCount(), 0);
    QCOMPARE(m_model->getPlanetCount(), 0);
    QVERIFY(m_model->getPlanets().empty());
}

void TestPlanetListModel::testAddPlanet()
{
    QSignalSpy rowsInsertedSpy(m_model.get(), &QAbstractItemModel::rowsInserted);
    
    Planet* planet = m_model->addPlanet();
    
    QVERIFY(planet != nullptr);
    QCOMPARE(m_model->rowCount(), 1);
    QCOMPARE(m_model->getPlanetCount(), 1);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    
    // Verify default values
    QCOMPARE(planet->name(), "Planet 1");
    QCOMPARE(planet->orbitalRadius(), 1.0);
}

void TestPlanetListModel::testAddPlanetWithParameters()
{
    QSignalSpy rowsInsertedSpy(m_model.get(), &QAbstractItemModel::rowsInserted);
    
    const QString testName = "Mars";
    const double testDistance = 1.52;
    
    Planet* planet = m_model->addPlanet(testName, testDistance);
    
    QVERIFY(planet != nullptr);
    QCOMPARE(planet->name(), testName);
    QCOMPARE(planet->orbitalRadius(), testDistance);
    QCOMPARE(m_model->rowCount(), 1);
    QCOMPARE(rowsInsertedSpy.count(), 1);
}

void TestPlanetListModel::testRemovePlanet()
{
    // Add a planet first
    Planet* planet = m_model->addPlanet("Test Planet", 1.0);
    QCOMPARE(m_model->rowCount(), 1);
    
    QSignalSpy rowsRemovedSpy(m_model.get(), &QAbstractItemModel::rowsRemoved);
    
    bool result = m_model->removePlanet(planet);
    
    QVERIFY(result);
    QCOMPARE(m_model->rowCount(), 0);
    QCOMPARE(m_model->getPlanetCount(), 0);
    QCOMPARE(rowsRemovedSpy.count(), 1);
}

void TestPlanetListModel::testRemovePlanetAt()
{
    // Add two planets
    m_model->addPlanet("Planet 1", 1.0);
    m_model->addPlanet("Planet 2", 2.0);
    QCOMPARE(m_model->rowCount(), 2);
    
    QSignalSpy rowsRemovedSpy(m_model.get(), &QAbstractItemModel::rowsRemoved);
    
    bool result = m_model->removePlanetAt(0);
    
    QVERIFY(result);
    QCOMPARE(m_model->rowCount(), 1);
    QCOMPARE(rowsRemovedSpy.count(), 1);
    
    // Verify the remaining planet
    Planet* remainingPlanet = m_model->getPlanetAt(0);
    QVERIFY(remainingPlanet != nullptr);
    QCOMPARE(remainingPlanet->name(), "Planet 2");
}

void TestPlanetListModel::testGetPlanetAt()
{
    // Add test planets
    Planet* planet1 = m_model->addPlanet("Planet 1", 1.0);
    Planet* planet2 = m_model->addPlanet("Planet 2", 2.0);
    
    QCOMPARE(m_model->getPlanetAt(0), planet1);
    QCOMPARE(m_model->getPlanetAt(1), planet2);
}

void TestPlanetListModel::testGetPlanetCount()
{
    QCOMPARE(m_model->getPlanetCount(), 0);
    
    m_model->addPlanet();
    QCOMPARE(m_model->getPlanetCount(), 1);
    
    m_model->addPlanet();
    QCOMPARE(m_model->getPlanetCount(), 2);
    
    m_model->removePlanetAt(0);
    QCOMPARE(m_model->getPlanetCount(), 1);
}

void TestPlanetListModel::testClearPlanets()
{
    // Add multiple planets
    m_model->addPlanet("Planet 1", 1.0);
    m_model->addPlanet("Planet 2", 2.0);
    m_model->addPlanet("Planet 3", 3.0);
    QCOMPARE(m_model->rowCount(), 3);
    
    QSignalSpy modelResetSpy(m_model.get(), &QAbstractItemModel::modelReset);
    
    m_model->clearPlanets();
    
    QCOMPARE(m_model->rowCount(), 0);
    QCOMPARE(m_model->getPlanetCount(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
}

void TestPlanetListModel::testRowCount()
{
    QCOMPARE(m_model->rowCount(), 0);
    QCOMPARE(m_model->rowCount(QModelIndex()), 0);
    
    m_model->addPlanet();
    QCOMPARE(m_model->rowCount(), 1);
    
    m_model->addPlanet();
    QCOMPARE(m_model->rowCount(), 2);
}

void TestPlanetListModel::testData()
{
    Planet* planet = m_model->addPlanet("Mars", 1.52);
    planet->setType(PlanetType::Rocky);
    planet->setSize(0.53);
    planet->setMass(0.11);
    planet->setNumberOfMoons(2);
    planet->setMaxTemperature(20.0);
    planet->setMinTemperature(-87.0);
    
    QModelIndex index = m_model->index(0, 0);
    QVERIFY(index.isValid());
    
    // Test different roles
    QCOMPARE(m_model->data(index, PlanetListModel::NameRole).toString(), "Mars");
    QCOMPARE(m_model->data(index, PlanetListModel::TypeRole).toInt(), static_cast<int>(PlanetType::Rocky));
    QCOMPARE(m_model->data(index, PlanetListModel::SizeRole).toDouble(), 0.53);
    QCOMPARE(m_model->data(index, PlanetListModel::MassRole).toDouble(), 0.11);
    QCOMPARE(m_model->data(index, PlanetListModel::MoonsRole).toInt(), 2);
    QCOMPARE(m_model->data(index, PlanetListModel::OrbitDistanceRole).toDouble(), 1.52);
    
    // Test PlanetRole
    QVariant planetVariant = m_model->data(index, PlanetListModel::PlanetRole);
    Planet* retrievedPlanet = planetVariant.value<Planet*>();
    QCOMPARE(retrievedPlanet, planet);
}

void TestPlanetListModel::testRoleNames()
{
    QHash<int, QByteArray> roles = m_model->roleNames();
    
    QVERIFY(roles.contains(PlanetListModel::PlanetRole));
    QVERIFY(roles.contains(PlanetListModel::NameRole));
    QVERIFY(roles.contains(PlanetListModel::TypeRole));
    QVERIFY(roles.contains(PlanetListModel::SizeRole));
    QVERIFY(roles.contains(PlanetListModel::MassRole));
    QVERIFY(roles.contains(PlanetListModel::MoonsRole));
    QVERIFY(roles.contains(PlanetListModel::OrbitDistanceRole));
    QVERIFY(roles.contains(PlanetListModel::TemperatureRole));
    
    QCOMPARE(roles[PlanetListModel::PlanetRole], "planet");
    QCOMPARE(roles[PlanetListModel::NameRole], "name");
    QCOMPARE(roles[PlanetListModel::TypeRole], "type");
}

void TestPlanetListModel::testModelSignals()
{
    QSignalSpy rowsInsertedSpy(m_model.get(), &QAbstractItemModel::rowsInserted);
    QSignalSpy rowsRemovedSpy(m_model.get(), &QAbstractItemModel::rowsRemoved);
    QSignalSpy dataChangedSpy(m_model.get(), &QAbstractItemModel::dataChanged);
    
    // Test insert signals
    m_model->addPlanet();
    QCOMPARE(rowsInsertedSpy.count(), 1);
    
    // Test data changed signals
    Planet* planet = m_model->getPlanetAt(0);
    planet->setName("Modified Name");
    QCOMPARE(dataChangedSpy.count(), 1);
    
    // Test remove signals
    m_model->removePlanetAt(0);
    QCOMPARE(rowsRemovedSpy.count(), 1);
}

void TestPlanetListModel::testInvalidIndices()
{
    m_model->addPlanet();
    
    // Test invalid indices
    QModelIndex invalidIndex = m_model->index(-1, 0);
    QVERIFY(!invalidIndex.isValid());
    
    invalidIndex = m_model->index(1, 0); // Only one item, so index 1 is invalid
    QVERIFY(!invalidIndex.isValid());
    
    // Test data with invalid index
    QVariant result = m_model->data(QModelIndex(), PlanetListModel::NameRole);
    QVERIFY(!result.isValid());
}

void TestPlanetListModel::testPlanetPropertyChanges()
{
    Planet* planet = m_model->addPlanet("Test Planet", 1.0);
    QSignalSpy dataChangedSpy(m_model.get(), &QAbstractItemModel::dataChanged);
    
    // Change various properties and verify signals
    planet->setName("New Name");
    QCOMPARE(dataChangedSpy.count(), 1);
    
    planet->setType(PlanetType::GasGiant);
    QCOMPARE(dataChangedSpy.count(), 2);
    
    planet->setSize(2.0);
    QCOMPARE(dataChangedSpy.count(), 3);
}

void TestPlanetListModel::testDataChangedSignals()
{
    m_model->addPlanet("Planet 1", 1.0);
    m_model->addPlanet("Planet 2", 2.0);
    
    QSignalSpy dataChangedSpy(m_model.get(), &QAbstractItemModel::dataChanged);
    
    m_model->notifyDataChanged();
    QCOMPARE(dataChangedSpy.count(), 1);
    
    // Verify the signal covers all items
    QList<QVariant> arguments = dataChangedSpy.takeFirst();
    QModelIndex topLeft = arguments.at(0).value<QModelIndex>();
    QModelIndex bottomRight = arguments.at(1).value<QModelIndex>();
    
    QCOMPARE(topLeft.row(), 0);
    QCOMPARE(bottomRight.row(), 1);
}

void TestPlanetListModel::testRemoveNullPlanet()
{
    bool result = m_model->removePlanet(nullptr);
    QVERIFY(!result);
}

void TestPlanetListModel::testRemoveInvalidIndex()
{
    bool result = m_model->removePlanetAt(-1);
    QVERIFY(!result);
    
    result = m_model->removePlanetAt(0); // No planets added
    QVERIFY(!result);
    
    m_model->addPlanet();
    result = m_model->removePlanetAt(1); // Only one planet, index 1 is invalid
    QVERIFY(!result);
}

void TestPlanetListModel::testGetInvalidIndex()
{
    Planet* result = m_model->getPlanetAt(-1);
    QVERIFY(result == nullptr);
    
    result = m_model->getPlanetAt(0); // No planets added
    QVERIFY(result == nullptr);
    
    m_model->addPlanet();
    result = m_model->getPlanetAt(1); // Only one planet, index 1 is invalid
    QVERIFY(result == nullptr);
}

void TestPlanetListModel::testMultiplePlanets()
{
    // Add multiple planets and verify order
    Planet* planet1 = m_model->addPlanet("Mercury", 0.39);
    Planet* planet2 = m_model->addPlanet("Venus", 0.72);
    Planet* planet3 = m_model->addPlanet("Earth", 1.0);
    Planet* planet4 = m_model->addPlanet("Mars", 1.52);
    
    QCOMPARE(m_model->getPlanetCount(), 4);
    
    // Verify order is maintained
    QCOMPARE(m_model->getPlanetAt(0), planet1);
    QCOMPARE(m_model->getPlanetAt(1), planet2);
    QCOMPARE(m_model->getPlanetAt(2), planet3);
    QCOMPARE(m_model->getPlanetAt(3), planet4);
    
    // Remove middle planet and verify adjustment
    m_model->removePlanetAt(1);
    QCOMPARE(m_model->getPlanetCount(), 3);
    QCOMPARE(m_model->getPlanetAt(0), planet1);
    QCOMPARE(m_model->getPlanetAt(1), planet3); // Venus removed, Earth moved up
    QCOMPARE(m_model->getPlanetAt(2), planet4);
}

void TestPlanetListModel::testModelTester()
{
    // Use Qt's built-in model tester to verify model integrity
    QAbstractItemModelTester tester(m_model.get());
    
    // Perform various operations while the tester monitors
    m_model->addPlanet("Planet 1", 1.0);
    m_model->addPlanet("Planet 2", 2.0);
    m_model->addPlanet("Planet 3", 3.0);
    
    Planet* planet = m_model->getPlanetAt(1);
    if (planet) {
        planet->setName("Modified Planet");
        planet->setSize(1.5);
    }
    
    m_model->removePlanetAt(0);
    m_model->clearPlanets();
    
    // If we reach here without assertions, the model is properly implemented
    QVERIFY(true);
}

Planet* TestPlanetListModel::createTestPlanet(const QString& name, double orbitDistance)
{
    return m_model->addPlanet(name, orbitDistance);
}

void TestPlanetListModel::verifyPlanetData(Planet* planet, const QString& expectedName, double expectedOrbitDistance)
{
    QVERIFY(planet != nullptr);
    QCOMPARE(planet->name(), expectedName);
    QCOMPARE(planet->orbitalRadius(), expectedOrbitDistance);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TestPlanetListModel test;
    return QTest::qExec(&test, argc, argv);
}

#include "test_planetlistmodel.moc"
