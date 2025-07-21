/**
 * @file test_system_viewmodel.cpp
 * @brief Unit tests for SystemViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "ggh/modules/GalaxyFactions/viewmodels/SystemViewModel.h"
#include "ggh/modules/GalaxyFactions/models/System.h"
#include "ggh/modules/GalaxyCore/models/StarSystemModel.h"
#include "ggh/modules/GalaxyCore/utilities/Coordinates.h"

class TestSystemViewModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Test default constructor
     */
    void testDefaultConstructor();
    
    /**
     * @brief Test constructor with system model
     */
    void testConstructorWithSystem();
    
    /**
     * @brief Test ID property
     */
    void testIdProperty();
    
    /**
     * @brief Test name property
     */
    void testNameProperty();
    
    /**
     * @brief Test description property
     */
    void testDescriptionProperty();
    
    /**
     * @brief Test type property
     */
    void testTypeProperty();
    
    /**
     * @brief Test signal emissions
     */
    void testSignalEmissions();

private:
    /**
     * @brief Helper to create a test system
     */
    std::shared_ptr<ggh::Galaxy::Factions::models::System> createTestSystem();
};

std::shared_ptr<ggh::Galaxy::Factions::models::System> TestSystemViewModel::createTestSystem()
{
    // Create a mock star system model with 2D coordinates
    ggh::GalaxyCore::utilities::CartesianCoordinates<double> coords(100.0, 200.0);
    auto starSystem = std::make_shared<ggh::GalaxyCore::models::StarSystemModel>(1, "Test System", coords);
    
    // Create and return the system
    return std::make_shared<ggh::Galaxy::Factions::models::System>(starSystem);
}

void TestSystemViewModel::testDefaultConstructor()
{
    ggh::Galaxy::Factions::viewmodels::SystemViewModel viewModel;
    
    // Default values should be invalid/empty when no system is set
    QCOMPARE(viewModel.id(), -1);
    QCOMPARE(viewModel.name(), QString(""));
    QCOMPARE(viewModel.description(), QString(""));
    QCOMPARE(viewModel.type(), QString("System")); // Default type
}

void TestSystemViewModel::testConstructorWithSystem()
{
    auto system = createTestSystem();
    
    // Set some test data on the system (note: setId and setDescription are no-ops in System model)
    system->setName("Test System");
    system->setDescription("Test System Description"); // This is a no-op
    
    ggh::Galaxy::Factions::viewmodels::SystemViewModel viewModel(system);
    
    QCOMPARE(viewModel.id(), 1); // ID comes from StarSystemModel constructor
    QCOMPARE(viewModel.name(), QString("Test System"));
    QCOMPARE(viewModel.description(), QString("Test System")); // Description same as name
    QCOMPARE(viewModel.type(), QString("System"));
}

void TestSystemViewModel::testIdProperty()
{
    auto system = createTestSystem();
    ggh::Galaxy::Factions::viewmodels::SystemViewModel viewModel(system);
    
    // Test that setId is a no-op (System model doesn't support changing ID)
    int originalId = viewModel.id();
    viewModel.setId(100);
    QCOMPARE(viewModel.id(), originalId); // Should remain unchanged
    
    // Test setting same ID
    viewModel.setId(originalId);
    QCOMPARE(viewModel.id(), originalId);
}

void TestSystemViewModel::testNameProperty()
{
    auto system = createTestSystem();
    ggh::Galaxy::Factions::viewmodels::SystemViewModel viewModel(system);
    
    // Test setting name
    viewModel.setName("New System Name");
    QCOMPARE(viewModel.name(), QString("New System Name"));
    
    // Test setting same name
    viewModel.setName("New System Name");
    QCOMPARE(viewModel.name(), QString("New System Name"));
}

void TestSystemViewModel::testDescriptionProperty()
{
    auto system = createTestSystem();
    ggh::Galaxy::Factions::viewmodels::SystemViewModel viewModel(system);
    
    // Test that setDescription is a no-op (System model doesn't support separate description)
    // Description returns the same as name
    QString originalDescription = viewModel.description();
    viewModel.setDescription("New System Description");
    QCOMPARE(viewModel.description(), originalDescription); // Should remain unchanged (same as name)
    
    // Test setting same description
    viewModel.setDescription(originalDescription);
    QCOMPARE(viewModel.description(), originalDescription);
}

void TestSystemViewModel::testTypeProperty()
{
    auto system = createTestSystem();
    ggh::Galaxy::Factions::viewmodels::SystemViewModel viewModel(system);
    
    // Test setting type (note: this might just emit signal without changing underlying model)
    viewModel.setType("Special System");
    QCOMPARE(viewModel.type(), QString("System")); // Might still return default
}

void TestSystemViewModel::testSignalEmissions()
{
    auto system = createTestSystem();
    ggh::Galaxy::Factions::viewmodels::SystemViewModel viewModel(system);
    
    // Test ID signal (setId is no-op, so no signal should be emitted)
    QSignalSpy idSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::SystemViewModel::idChanged);
    viewModel.setId(200);
    QCOMPARE(idSpy.count(), 0); // No signal because setId is no-op
    
    // Test name signal (this should work)
    QSignalSpy nameSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::SystemViewModel::nameChanged);
    viewModel.setName("Signal Test System");
    QCOMPARE(nameSpy.count(), 1);
    QCOMPARE(nameSpy.first().first().toString(), QString("Signal Test System"));
    
    // Test description signal (setDescription is no-op, so no signal should be emitted)
    QSignalSpy descSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::SystemViewModel::descriptionChanged);
    viewModel.setDescription("Signal Description");
    QCOMPARE(descSpy.count(), 0); // No signal because setDescription is no-op
    
    // Test type signal (setType just emits signal)
    QSignalSpy typeSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::SystemViewModel::typeChanged);
    viewModel.setType("Special Type");
    QCOMPARE(typeSpy.count(), 1);
    QCOMPARE(typeSpy.first().first().toString(), QString("Special Type"));
    
    // Test that setting the same name doesn't emit signal
    viewModel.setName("Signal Test System");
    QCOMPARE(nameSpy.count(), 1); // Should still be 1
}

QTEST_MAIN(TestSystemViewModel)
#include "test_system_viewmodel.moc"
