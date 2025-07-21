/**
 * @file test_system_resource_bonus_viewmodel.cpp
 * @brief Unit tests for SystemResourceBonusViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "ggh/modules/GalaxyFactions/viewmodels/SystemResourceBonusViewModel.h"
#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"
#include "ggh/modules/GalaxyFactions/models/ResourceType.h"

class TestSystemResourceBonusViewModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Test default constructor
     */
    void testDefaultConstructor();
    
    /**
     * @brief Test constructor with resource bonus model
     */
    void testConstructorWithResourceBonus();
    
    /**
     * @brief Test ID property
     */
    void testIdProperty();
    
    /**
     * @brief Test name property
     */
    void testNameProperty();
    
    /**
     * @brief Test value property
     */
    void testValueProperty();
    
    /**
     * @brief Test signal emissions
     */
    void testSignalEmissions();

private:
    /**
     * @brief Helper to create a test resource bonus
     */
    std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> createTestResourceBonus();
};

std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> TestSystemResourceBonusViewModel::createTestResourceBonus()
{
    ggh::Galaxy::Factions::models::ResourceType resourceType(1, "Test Resource", "Test Resource Description");
    auto resourceBonus = std::make_shared<ggh::Galaxy::Factions::models::SystemResourceBonus>(resourceType, 10);
    resourceBonus->setId(42);
    return resourceBonus;
}

void TestSystemResourceBonusViewModel::testDefaultConstructor()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel viewModel;
    
    // Default values should be invalid/empty when no resource bonus is set
    QCOMPARE(viewModel.id(), -1);
    QCOMPARE(viewModel.name(), QString(""));
    QCOMPARE(viewModel.value(), 0.0);
}

void TestSystemResourceBonusViewModel::testConstructorWithResourceBonus()
{
    auto resourceBonus = createTestResourceBonus();
    
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel viewModel(resourceBonus);
    
    QCOMPARE(viewModel.id(), 42);
    QCOMPARE(viewModel.name(), QString("Test Resource"));
    QCOMPARE(viewModel.value(), 10.0);
}

void TestSystemResourceBonusViewModel::testIdProperty()
{
    auto resourceBonus = createTestResourceBonus();
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel viewModel(resourceBonus);
    
    // Test setting ID
    viewModel.setId(100);
    QCOMPARE(viewModel.id(), 100);
    
    // Test setting same ID
    viewModel.setId(100);
    QCOMPARE(viewModel.id(), 100);
}

void TestSystemResourceBonusViewModel::testNameProperty()
{
    auto resourceBonus = createTestResourceBonus();
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel viewModel(resourceBonus);
    
    // Test setting name
    viewModel.setName("New Resource Name");
    QCOMPARE(viewModel.name(), QString("New Resource Name"));
    
    // Test setting same name
    viewModel.setName("New Resource Name");
    QCOMPARE(viewModel.name(), QString("New Resource Name"));
}

void TestSystemResourceBonusViewModel::testValueProperty()
{
    auto resourceBonus = createTestResourceBonus();
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel viewModel(resourceBonus);
    
    // Test setting value
    viewModel.setValue(25.5);
    QCOMPARE(viewModel.value(), 25.0); // Should be converted to int and back to double
    
    // Test setting same value
    viewModel.setValue(25.0);
    QCOMPARE(viewModel.value(), 25.0);
}

void TestSystemResourceBonusViewModel::testSignalEmissions()
{
    auto resourceBonus = createTestResourceBonus();
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel viewModel(resourceBonus);
    
    // Test ID signal
    QSignalSpy idSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel::idChanged);
    viewModel.setId(200);
    QCOMPARE(idSpy.count(), 1);
    QCOMPARE(idSpy.first().first().toInt(), 200);
    
    // Test name signal
    QSignalSpy nameSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel::nameChanged);
    viewModel.setName("Signal Test Resource");
    QCOMPARE(nameSpy.count(), 1);
    QCOMPARE(nameSpy.first().first().toString(), QString("Signal Test Resource"));
    
    // Test value signal
    QSignalSpy valueSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::SystemResourceBonusViewModel::valueChanged);
    viewModel.setValue(50.0);
    QCOMPARE(valueSpy.count(), 1);
    QCOMPARE(valueSpy.first().first().toDouble(), 50.0);
    
    // Test that setting the same value doesn't emit signal
    viewModel.setId(200);
    QCOMPARE(idSpy.count(), 1); // Should still be 1
    
    viewModel.setValue(50.0);
    QCOMPARE(valueSpy.count(), 1); // Should still be 1
}

QTEST_MAIN(TestSystemResourceBonusViewModel)
#include "test_system_resource_bonus_viewmodel.moc"
