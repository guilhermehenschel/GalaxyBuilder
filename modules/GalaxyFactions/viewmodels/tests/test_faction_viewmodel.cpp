/**
 * @file test_faction_viewmodel.cpp
 * @brief Unit tests for FactionViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "ggh/modules/GalaxyFactions/viewmodels/FactionViewModel.h"
#include "ggh/modules/GalaxyFactions/models/Faction.h"

class TestFactionViewModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Test default constructor
     */
    void testDefaultConstructor();
    
    /**
     * @brief Test constructor with faction model
     */
    void testConstructorWithFaction();
    
    /**
     * @brief Test ID getter and setter
     */
    void testIdProperty();
    
    /**
     * @brief Test name getter and setter
     */
    void testNameProperty();
    
    /**
     * @brief Test description getter and setter
     */
    void testDescriptionProperty();
    
    /**
     * @brief Test color getter and setter
     */
    void testColorProperty();
    
    /**
     * @brief Test signal emissions
     */
    void testSignalEmissions();
};

void TestFactionViewModel::testDefaultConstructor()
{
    ggh::Galaxy::Factions::viewmodels::FactionViewModel viewModel;
    
    // Default values should be invalid/empty
    QCOMPARE(viewModel.id(), -1);
    QCOMPARE(viewModel.name(), QString(""));
    QCOMPARE(viewModel.description(), QString(""));
    QCOMPARE(viewModel.color(), QString(""));
}

void TestFactionViewModel::testConstructorWithFaction()
{
    ggh::Galaxy::Factions::models::Faction faction(1, "Test Faction", "Test Description", "#FF0000");
    ggh::Galaxy::Factions::viewmodels::FactionViewModel viewModel(faction);
    
    QCOMPARE(viewModel.id(), 1);
    QCOMPARE(viewModel.name(), QString("Test Faction"));
    QCOMPARE(viewModel.description(), QString("Test Description"));
    QCOMPARE(viewModel.color(), QString("#FF0000"));
}

void TestFactionViewModel::testIdProperty()
{
    ggh::Galaxy::Factions::viewmodels::FactionViewModel viewModel;
    
    // Test setting ID
    viewModel.setId(42);
    QCOMPARE(viewModel.id(), 42);
    
    // Test setting same ID doesn't change
    viewModel.setId(42);
    QCOMPARE(viewModel.id(), 42);
}

void TestFactionViewModel::testNameProperty()
{
    ggh::Galaxy::Factions::viewmodels::FactionViewModel viewModel;
    
    // Test setting name
    viewModel.setName("New Name");
    QCOMPARE(viewModel.name(), QString("New Name"));
    
    // Test setting same name doesn't change
    viewModel.setName("New Name");
    QCOMPARE(viewModel.name(), QString("New Name"));
}

void TestFactionViewModel::testDescriptionProperty()
{
    ggh::Galaxy::Factions::viewmodels::FactionViewModel viewModel;
    
    // Test setting description
    viewModel.setDescription("New Description");
    QCOMPARE(viewModel.description(), QString("New Description"));
    
    // Test setting same description doesn't change
    viewModel.setDescription("New Description");
    QCOMPARE(viewModel.description(), QString("New Description"));
}

void TestFactionViewModel::testColorProperty()
{
    ggh::Galaxy::Factions::viewmodels::FactionViewModel viewModel;
    
    // Test setting color
    viewModel.setColor("#00FF00");
    QCOMPARE(viewModel.color(), QString("#00FF00"));
    
    // Test setting same color doesn't change
    viewModel.setColor("#00FF00");
    QCOMPARE(viewModel.color(), QString("#00FF00"));
}

void TestFactionViewModel::testSignalEmissions()
{
    ggh::Galaxy::Factions::viewmodels::FactionViewModel viewModel;
    
    // Test ID signal
    QSignalSpy idSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::FactionViewModel::idChanged);
    viewModel.setId(123);
    QCOMPARE(idSpy.count(), 1);
    QCOMPARE(idSpy.first().first().toInt(), 123);
    
    // Test name signal
    QSignalSpy nameSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::FactionViewModel::nameChanged);
    viewModel.setName("Signal Test");
    QCOMPARE(nameSpy.count(), 1);
    QCOMPARE(nameSpy.first().first().toString(), QString("Signal Test"));
    
    // Test description signal
    QSignalSpy descSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::FactionViewModel::descriptionChanged);
    viewModel.setDescription("Signal Description");
    QCOMPARE(descSpy.count(), 1);
    QCOMPARE(descSpy.first().first().toString(), QString("Signal Description"));
    
    // Test color signal
    QSignalSpy colorSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::FactionViewModel::colorChanged);
    viewModel.setColor("#0000FF");
    QCOMPARE(colorSpy.count(), 1);
    QCOMPARE(colorSpy.first().first().toString(), QString("#0000FF"));
    
    // Test that setting the same value doesn't emit signal
    viewModel.setId(123);
    QCOMPARE(idSpy.count(), 1); // Should still be 1
}

QTEST_MAIN(TestFactionViewModel)
#include "test_faction_viewmodel.moc"
