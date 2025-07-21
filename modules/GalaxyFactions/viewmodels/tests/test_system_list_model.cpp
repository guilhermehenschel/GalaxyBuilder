/**
 * @file test_system_list_model.cpp
 * @brief Unit tests for SystemListModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "ggh/modules/GalaxyFactions/viewmodels/SystemListModel.h"
#include "ggh/modules/GalaxyFactions/models/System.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

class TestSystemListModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Test default constructor
     */
    void testDefaultConstructor();
    
    /**
     * @brief Test adding systems
     */
    void testAddSystem();
    
    /**
     * @brief Test removing systems
     */
    void testRemoveSystem();
    
    /**
     * @brief Test clearing systems
     */
    void testClearSystems();
    
    /**
     * @brief Test data access
     */
    void testDataAccess();
    
    /**
     * @brief Test role names
     */
    void testRoleNames();
    
    /**
     * @brief Test setting systems list
     */
    void testSetSystems();

private:
    /**
     * @brief Helper to create a test system
     */
    std::shared_ptr<ggh::Galaxy::Factions::models::System> createTestSystem(int id, const std::string& name, const std::string& description);
};

std::shared_ptr<ggh::Galaxy::Factions::models::System> TestSystemListModel::createTestSystem(int id, const std::string& name, const std::string& description)
{
    // Create a mock star system model with 2D coordinates
    ggh::GalaxyCore::utilities::CartesianCoordinates<double> coords(static_cast<double>(id * 100), static_cast<double>(id * 200));
    auto starSystem = std::make_shared<ggh::GalaxyCore::models::StarSystemModel>(id, name, coords);
    
    // Create the system (note: setId and setDescription are no-ops in System model)
    auto system = std::make_shared<ggh::Galaxy::Factions::models::System>(starSystem);
    system->setName(name); // This works
    
    return system;
}

void TestSystemListModel::testDefaultConstructor()
{
    ggh::Galaxy::Factions::viewmodels::SystemListModel model;
    
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.getSystems().isEmpty());
}

void TestSystemListModel::testAddSystem()
{
    ggh::Galaxy::Factions::viewmodels::SystemListModel model;
    
    // Test adding a system
    auto system = createTestSystem(1, "Test System", "Test Description");
    
    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    model.addSystem(system);
    
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    
    // Add another system
    auto system2 = createTestSystem(2, "Test System 2", "Test Description 2");
    model.addSystem(system2);
    
    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(rowsInsertedSpy.count(), 2);
    
    // Test adding null system (should be ignored)
    model.addSystem(nullptr);
    QCOMPARE(model.rowCount(), 2); // Should remain the same
}

void TestSystemListModel::testRemoveSystem()
{
    ggh::Galaxy::Factions::viewmodels::SystemListModel model;
    
    // Add some systems first
    auto system1 = createTestSystem(1, "System 1", "Description 1");
    auto system2 = createTestSystem(2, "System 2", "Description 2");
    model.addSystem(system1);
    model.addSystem(system2);
    
    QCOMPARE(model.rowCount(), 2);
    
    // Remove a system
    QSignalSpy rowsRemovedSpy(&model, &QAbstractItemModel::rowsRemoved);
    model.removeSystem(1);
    
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(rowsRemovedSpy.count(), 1);
    
    // Try to remove non-existent system
    model.removeSystem(999);
    QCOMPARE(model.rowCount(), 1); // Should remain the same
}

void TestSystemListModel::testClearSystems()
{
    ggh::Galaxy::Factions::viewmodels::SystemListModel model;
    
    // Add some systems
    auto system1 = createTestSystem(1, "System 1", "Description 1");
    auto system2 = createTestSystem(2, "System 2", "Description 2");
    model.addSystem(system1);
    model.addSystem(system2);
    
    QCOMPARE(model.rowCount(), 2);
    
    // Clear all systems
    QSignalSpy modelResetSpy(&model, &QAbstractItemModel::modelReset);
    model.clearSystems();
    
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
    QVERIFY(model.getSystems().isEmpty());
}

void TestSystemListModel::testDataAccess()
{
    ggh::Galaxy::Factions::viewmodels::SystemListModel model;
    
    // Add a system (note: description will be same as name since setDescription is no-op)
    auto system = createTestSystem(42, "Test System", "Test Description");
    model.addSystem(system);
    
    QModelIndex index = model.index(0, 0);
    QVERIFY(index.isValid());
    
    // Test different roles
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::SystemListModel::IdRole).toInt(), 42);
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::SystemListModel::NameRole).toString(), QString("Test System"));
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::SystemListModel::DescriptionRole).toString(), QString("Test System")); // Same as name
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::SystemListModel::TypeRole).toString(), QString("System"));
    
    // Test invalid index
    QModelIndex invalidIndex = model.index(1, 0);
    QVERIFY(!invalidIndex.isValid());
    QVERIFY(!model.data(invalidIndex, ggh::Galaxy::Factions::viewmodels::SystemListModel::IdRole).isValid());
}

void TestSystemListModel::testRoleNames()
{
    ggh::Galaxy::Factions::viewmodels::SystemListModel model;
    
    auto roleNames = model.roleNames();
    
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::SystemListModel::IdRole));
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::SystemListModel::NameRole));
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::SystemListModel::DescriptionRole));
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::SystemListModel::TypeRole));
    
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::SystemListModel::IdRole], QByteArray("id"));
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::SystemListModel::NameRole], QByteArray("name"));
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::SystemListModel::DescriptionRole], QByteArray("description"));
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::SystemListModel::TypeRole], QByteArray("type"));
}

void TestSystemListModel::testSetSystems()
{
    ggh::Galaxy::Factions::viewmodels::SystemListModel model;
    
    // Create a list of systems
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::System>> systems;
    systems.append(createTestSystem(1, "System 1", "Desc 1"));
    systems.append(createTestSystem(2, "System 2", "Desc 2"));
    systems.append(createTestSystem(3, "System 3", "Desc 3"));
    
    QSignalSpy modelResetSpy(&model, &QAbstractItemModel::modelReset);
    model.setSystems(systems);
    
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(modelResetSpy.count(), 1);
    
    // Verify the systems were set correctly
    auto retrievedSystems = model.getSystems();
    QCOMPARE(retrievedSystems.size(), 3);
    QCOMPARE(retrievedSystems[0]->id(), 1);
    QCOMPARE(retrievedSystems[1]->id(), 2);
    QCOMPARE(retrievedSystems[2]->id(), 3);
}

QTEST_MAIN(TestSystemListModel)
#include "test_system_list_model.moc"
