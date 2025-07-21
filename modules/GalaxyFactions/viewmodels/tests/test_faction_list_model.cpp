/**
 * @file test_faction_list_model.cpp
 * @brief Unit tests for FactionListModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "ggh/modules/GalaxyFactions/viewmodels/FactionListModel.h"
#include "ggh/modules/GalaxyFactions/models/Faction.h"

class TestFactionListModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Test default constructor
     */
    void testDefaultConstructor();
    
    /**
     * @brief Test adding factions
     */
    void testAddFaction();
    
    /**
     * @brief Test removing factions
     */
    void testRemoveFaction();
    
    /**
     * @brief Test clearing factions
     */
    void testClearFactions();
    
    /**
     * @brief Test data access
     */
    void testDataAccess();
    
    /**
     * @brief Test role names
     */
    void testRoleNames();
    
    /**
     * @brief Test faction retrieval
     */
    void testGetFaction();
    
    /**
     * @brief Test setting factions list
     */
    void testSetFactions();
};

void TestFactionListModel::testDefaultConstructor()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.getFactions().isEmpty());
}

void TestFactionListModel::testAddFaction()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    // Test adding a faction
    ggh::Galaxy::Factions::models::Faction faction(1, "Test Faction", "Test Description", "#FF0000");
    
    QSignalSpy rowsInsertedSpy(&model, &QAbstractItemModel::rowsInserted);
    model.addFaction(faction);
    
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(rowsInsertedSpy.count(), 1);
    
    // Add another faction
    ggh::Galaxy::Factions::models::Faction faction2(2, "Test Faction 2", "Test Description 2", "#00FF00");
    model.addFaction(faction2);
    
    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(rowsInsertedSpy.count(), 2);
}

void TestFactionListModel::testRemoveFaction()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    // Add some factions first
    ggh::Galaxy::Factions::models::Faction faction1(1, "Faction 1", "Description 1", "#FF0000");
    ggh::Galaxy::Factions::models::Faction faction2(2, "Faction 2", "Description 2", "#00FF00");
    model.addFaction(faction1);
    model.addFaction(faction2);
    
    QCOMPARE(model.rowCount(), 2);
    
    // Remove a faction
    QSignalSpy rowsRemovedSpy(&model, &QAbstractItemModel::rowsRemoved);
    model.removeFaction(1);
    
    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(rowsRemovedSpy.count(), 1);
    
    // Try to remove non-existent faction
    model.removeFaction(999);
    QCOMPARE(model.rowCount(), 1); // Should remain the same
}

void TestFactionListModel::testClearFactions()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    // Add some factions
    ggh::Galaxy::Factions::models::Faction faction1(1, "Faction 1", "Description 1", "#FF0000");
    ggh::Galaxy::Factions::models::Faction faction2(2, "Faction 2", "Description 2", "#00FF00");
    model.addFaction(faction1);
    model.addFaction(faction2);
    
    QCOMPARE(model.rowCount(), 2);
    
    // Clear all factions
    QSignalSpy modelResetSpy(&model, &QAbstractItemModel::modelReset);
    model.clearFactions();
    
    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(modelResetSpy.count(), 1);
    QVERIFY(model.getFactions().isEmpty());
}

void TestFactionListModel::testDataAccess()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    // Add a faction
    ggh::Galaxy::Factions::models::Faction faction(42, "Test Faction", "Test Description", "#FF0000");
    model.addFaction(faction);
    
    QModelIndex index = model.index(0, 0);
    QVERIFY(index.isValid());
    
    // Test different roles
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::IdRole).toInt(), 42);
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::NameRole).toString(), QString("Test Faction"));
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::DescriptionRole).toString(), QString("Test Description"));
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::FactionListModel::ColorRole).toString(), QString("#FF0000"));
    
    // Test invalid index
    QModelIndex invalidIndex = model.index(1, 0);
    QVERIFY(!invalidIndex.isValid());
    QVERIFY(!model.data(invalidIndex, ggh::Galaxy::Factions::viewmodels::FactionListModel::IdRole).isValid());
}

void TestFactionListModel::testRoleNames()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    auto roleNames = model.roleNames();
    
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::FactionListModel::IdRole));
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::FactionListModel::NameRole));
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::FactionListModel::DescriptionRole));
    QVERIFY(roleNames.contains(ggh::Galaxy::Factions::viewmodels::FactionListModel::ColorRole));
    
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::FactionListModel::IdRole], QByteArray("id"));
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::FactionListModel::NameRole], QByteArray("name"));
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::FactionListModel::DescriptionRole], QByteArray("description"));
    QCOMPARE(roleNames[ggh::Galaxy::Factions::viewmodels::FactionListModel::ColorRole], QByteArray("color"));
}

void TestFactionListModel::testGetFaction()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    // Add a faction
    ggh::Galaxy::Factions::models::Faction faction(123, "Get Test Faction", "Get Test Description", "#0000FF");
    model.addFaction(faction);
    
    // Test getting existing faction
    auto retrievedFaction = model.getFaction(123);
    QCOMPARE(retrievedFaction.id(), 123);
    QCOMPARE(retrievedFaction.name(), "Get Test Faction");
    QCOMPARE(retrievedFaction.description(), "Get Test Description");
    QCOMPARE(retrievedFaction.color(), "#0000FF");
    
    // Test getting non-existent faction
    auto nonExistentFaction = model.getFaction(999);
    QCOMPARE(nonExistentFaction.id(), -1);
}

void TestFactionListModel::testSetFactions()
{
    ggh::Galaxy::Factions::viewmodels::FactionListModel model;
    
    // Create a list of factions
    QList<std::shared_ptr<ggh::Galaxy::Factions::models::Faction>> factions;
    factions.append(std::make_shared<ggh::Galaxy::Factions::models::Faction>(1, "Faction 1", "Desc 1", "#FF0000"));
    factions.append(std::make_shared<ggh::Galaxy::Factions::models::Faction>(2, "Faction 2", "Desc 2", "#00FF00"));
    factions.append(std::make_shared<ggh::Galaxy::Factions::models::Faction>(3, "Faction 3", "Desc 3", "#0000FF"));
    
    QSignalSpy modelResetSpy(&model, &QAbstractItemModel::modelReset);
    model.setFactions(factions);
    
    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(modelResetSpy.count(), 1);
    
    // Verify the factions were set correctly
    auto retrievedFactions = model.getFactions();
    QCOMPARE(retrievedFactions.size(), 3);
    QCOMPARE(retrievedFactions[0]->id(), 1);
    QCOMPARE(retrievedFactions[1]->id(), 2);
    QCOMPARE(retrievedFactions[2]->id(), 3);
}

QTEST_MAIN(TestFactionListModel)
#include "test_faction_list_model.moc"
