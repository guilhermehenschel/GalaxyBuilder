/**
 * @file test_galaxy_factions_viewmodel.cpp
 * @brief Unit tests for GalaxyFactionsViewModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "ggh/modules/GalaxyFactions/viewmodels/GalaxyFactionsViewModel.h"
#include "ggh/modules/GalaxyFactions/models/GalaxyFactions.h"

class TestGalaxyFactionsViewModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Test default constructor
     */
    void testDefaultConstructor();
    
    /**
     * @brief Test initialization with galaxy factions model
     */
    void testInitialization();
    
    /**
     * @brief Test selected faction ID property
     */
    void testSelectedFactionId();
    
    /**
     * @brief Test adding factions
     */
    void testAddFaction();
    
    /**
     * @brief Test removing factions
     */
    void testRemoveFaction();
    
    /**
     * @brief Test faction list model access
     */
    void testFactionListModel();
};

void TestGalaxyFactionsViewModel::testDefaultConstructor()
{
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel viewModel;
    
    // Should have invalid selected faction ID by default
    QCOMPARE(viewModel.selectedFactionId(), -1);
    
    // Should have a faction list model
    QVERIFY(viewModel.factionListModel() != nullptr);
    QCOMPARE(viewModel.factionListModel()->rowCount(), 0);
}

void TestGalaxyFactionsViewModel::testInitialization()
{
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel viewModel;
    
    // Create a galaxy factions model with some test data
    auto galaxyFactions = std::make_shared<ggh::Galaxy::Factions::models::GalaxyFactions>();
    galaxyFactions->createFaction("Faction 1", "Description 1", "#FF0000");
    galaxyFactions->createFaction("Faction 2", "Description 2", "#00FF00");
    
    // Initialize the view model
    viewModel.initialize(galaxyFactions);
    
    // The faction list model should now contain the factions
    QCOMPARE(viewModel.factionListModel()->rowCount(), 2);
}

void TestGalaxyFactionsViewModel::testSelectedFactionId()
{
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel viewModel;
    
    QSignalSpy selectedIdSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel::selectedFactionIdChanged);
    
    // Test setting selected faction ID
    viewModel.setSelectedFactionId(42);
    QCOMPARE(viewModel.selectedFactionId(), 42);
    QCOMPARE(selectedIdSpy.count(), 1);
    QCOMPARE(selectedIdSpy.first().first().toInt(), 42);
    
    // Test setting the same ID doesn't emit signal
    viewModel.setSelectedFactionId(42);
    QCOMPARE(selectedIdSpy.count(), 1); // Should still be 1
    
    // Test setting different ID
    viewModel.setSelectedFactionId(123);
    QCOMPARE(viewModel.selectedFactionId(), 123);
    QCOMPARE(selectedIdSpy.count(), 2);
}

void TestGalaxyFactionsViewModel::testAddFaction()
{
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel viewModel;
    
    // Initialize with galaxy factions model
    auto galaxyFactions = std::make_shared<ggh::Galaxy::Factions::models::GalaxyFactions>();
    viewModel.initialize(galaxyFactions);
    
    QCOMPARE(viewModel.factionListModel()->rowCount(), 0);
    
    // Add a faction
    viewModel.addFaction("Test Faction", "Test Description", "#FF0000");
    
    // The faction list model should now contain the new faction
    QCOMPARE(viewModel.factionListModel()->rowCount(), 1);
    
    // Verify the faction data
    QModelIndex index = viewModel.factionListModel()->index(0, 0);
    QCOMPARE(viewModel.factionListModel()->data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::NameRole).toString(), 
             QString("Test Faction"));
    QCOMPARE(viewModel.factionListModel()->data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::DescriptionRole).toString(), 
             QString("Test Description"));
    QCOMPARE(viewModel.factionListModel()->data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::ColorRole).toString(), 
             QString("#FF0000"));
}

void TestGalaxyFactionsViewModel::testRemoveFaction()
{
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel viewModel;
    
    // Initialize with galaxy factions model
    auto galaxyFactions = std::make_shared<ggh::Galaxy::Factions::models::GalaxyFactions>();
    viewModel.initialize(galaxyFactions);
    
    // Add a faction first
    viewModel.addFaction("Test Faction", "Test Description", "#FF0000");
    QCOMPARE(viewModel.factionListModel()->rowCount(), 1);
    
    // Get the faction ID (it should be the first one created by the model)
    QModelIndex index = viewModel.factionListModel()->index(0, 0);
    int factionId = viewModel.factionListModel()->data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::IdRole).toInt();
    
    // Set this faction as selected
    viewModel.setSelectedFactionId(factionId);
    QCOMPARE(viewModel.selectedFactionId(), factionId);
    
    QSignalSpy selectedIdSpy(&viewModel, &ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel::selectedFactionIdChanged);
    
    // Remove the faction
    viewModel.removeFaction(factionId);
    
    // The faction list model should now be empty
    QCOMPARE(viewModel.factionListModel()->rowCount(), 0);
    
    // The selected faction ID should be cleared
    QCOMPARE(viewModel.selectedFactionId(), -1);
    QCOMPARE(selectedIdSpy.count(), 1);
}

void TestGalaxyFactionsViewModel::testFactionListModel()
{
    ggh::Galaxy::Factions::viewmodels::GalaxyFactionsViewModel viewModel;
    
    // The faction list model should never be null
    QVERIFY(viewModel.factionListModel() != nullptr);
    
    // It should initially be empty
    QCOMPARE(viewModel.factionListModel()->rowCount(), 0);
    
    // It should be the same instance each time
    auto firstCall = viewModel.factionListModel();
    auto secondCall = viewModel.factionListModel();
    QCOMPARE(firstCall, secondCall);
}

QTEST_MAIN(TestGalaxyFactionsViewModel)
#include "test_galaxy_factions_viewmodel.moc"
