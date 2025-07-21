/**
 * @file test_system_resource_bonus_list_model.cpp
 * @brief Unit tests for SystemResourceBonusListModel
 * @author Galaxy Builder Project
 * @date 2025
 */

#include <QtTest/QtTest>
#include <QSignalSpy>

#include "ggh/modules/GalaxyFactions/viewmodels/SystemResourceBonusListModel.h"
#include "ggh/modules/GalaxyFactions/models/SystemResourceBonus.h"
#include "ggh/modules/GalaxyFactions/models/ResourceType.h"

class TestSystemResourceBonusListModel : public QObject
{
    Q_OBJECT

private slots:
    /**
     * @brief Test default constructor
     */
    void testDefaultConstructor();
    
    /**
     * @brief Test rowCount
     */
    void testRowCount();
    
    /**
     * @brief Test role names
     */
    void testRoleNames();
    
    /**
     * @brief Test data retrieval
     */
    void testData();
    
    /**
     * @brief Test adding resource bonuses
     */
    void testAddResourceBonus();
    
    /**
     * @brief Test removing resource bonuses
     */
    void testRemoveResourceBonus();
    
    /**
     * @brief Test clearing resource bonuses
     */
    void testClearResourceBonuses();
    
    /**
     * @brief Test getting resource bonus by index
     */
    void testGetResourceBonus();
    
    /**
     * @brief Test model signals
     */
    void testModelSignals();

private:
    /**
     * @brief Helper to create a test resource bonus
     */
    std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> createTestResourceBonus(int id, const QString& name, double value);
};

std::shared_ptr<ggh::Galaxy::Factions::models::SystemResourceBonus> TestSystemResourceBonusListModel::createTestResourceBonus(int id, const QString& name, double value)
{
    ggh::Galaxy::Factions::models::ResourceType resourceType(id, name.toStdString(), name.toStdString() + " Description");
    auto resourceBonus = std::make_shared<ggh::Galaxy::Factions::models::SystemResourceBonus>(resourceType, static_cast<int>(value));
    resourceBonus->setId(id);
    return resourceBonus;
}

void TestSystemResourceBonusListModel::testDefaultConstructor()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    
    QCOMPARE(model.rowCount(), 0);
    QVERIFY(model.roleNames().contains(ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::IdRole));
    QVERIFY(model.roleNames().contains(ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::NameRole));
    QVERIFY(model.roleNames().contains(ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::ValueRole));
}

void TestSystemResourceBonusListModel::testRowCount()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    
    QCOMPARE(model.rowCount(), 0);
    
    auto resourceBonus1 = createTestResourceBonus(1, "Resource 1", 10.0);
    model.addResourceBonus(resourceBonus1);
    QCOMPARE(model.rowCount(), 1);
    
    auto resourceBonus2 = createTestResourceBonus(2, "Resource 2", 20.0);
    model.addResourceBonus(resourceBonus2);
    QCOMPARE(model.rowCount(), 2);
}

void TestSystemResourceBonusListModel::testRoleNames()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    auto roles = model.roleNames();
    
    QCOMPARE(roles[ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::IdRole], QByteArray("id"));
    QCOMPARE(roles[ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::NameRole], QByteArray("name"));
    QCOMPARE(roles[ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::ValueRole], QByteArray("value"));
}

void TestSystemResourceBonusListModel::testData()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    auto resourceBonus = createTestResourceBonus(42, "Test Resource", 15.0);
    model.addResourceBonus(resourceBonus);
    
    QModelIndex index = model.index(0, 0);
    
    // Test ID role
    QVariant idData = model.data(index, ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::IdRole);
    QCOMPARE(idData.toInt(), 42);
    
    // Test Name role
    QVariant nameData = model.data(index, ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::NameRole);
    QCOMPARE(nameData.toString(), QString("Test Resource"));
    
    // Test Value role
    QVariant valueData = model.data(index, ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::ValueRole);
    QCOMPARE(valueData.toDouble(), 15.0);
    
    // Test invalid role
    QVariant invalidData = model.data(index, Qt::DisplayRole);
    QVERIFY(!invalidData.isValid());
    
    // Test invalid index
    QModelIndex invalidIndex = model.index(1, 0);
    QVariant invalidIndexData = model.data(invalidIndex, ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::IdRole);
    QVERIFY(!invalidIndexData.isValid());
}

void TestSystemResourceBonusListModel::testAddResourceBonus()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    
    // Test adding valid resource bonus
    auto resourceBonus = createTestResourceBonus(1, "Resource", 10.0);
    model.addResourceBonus(resourceBonus);
    
    QCOMPARE(model.rowCount(), 1);
    QModelIndex index = model.index(0, 0);
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::IdRole).toInt(), 1);
    
    // Test adding nullptr (should not crash or add)
    model.addResourceBonus(nullptr);
    QCOMPARE(model.rowCount(), 1);
}

void TestSystemResourceBonusListModel::testRemoveResourceBonus()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    auto resourceBonus1 = createTestResourceBonus(1, "Resource 1", 10.0);
    auto resourceBonus2 = createTestResourceBonus(2, "Resource 2", 20.0);
    
    model.addResourceBonus(resourceBonus1);
    model.addResourceBonus(resourceBonus2);
    QCOMPARE(model.rowCount(), 2);
    
    // Test removing valid ID
    model.removeResourceBonus(1);
    QCOMPARE(model.rowCount(), 1);
    
    QModelIndex index = model.index(0, 0);
    QCOMPARE(model.data(index, ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel::IdRole).toInt(), 2);
    
    // Test removing invalid ID
    model.removeResourceBonus(999);
    QCOMPARE(model.rowCount(), 1);
    
    model.removeResourceBonus(-1);
    QCOMPARE(model.rowCount(), 1);
}

void TestSystemResourceBonusListModel::testClearResourceBonuses()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    
    // Add some resource bonuses
    auto resourceBonus1 = createTestResourceBonus(1, "Resource 1", 10.0);
    auto resourceBonus2 = createTestResourceBonus(2, "Resource 2", 20.0);
    model.addResourceBonus(resourceBonus1);
    model.addResourceBonus(resourceBonus2);
    QCOMPARE(model.rowCount(), 2);
    
    // Clear all
    model.clearResourceBonuses();
    QCOMPARE(model.rowCount(), 0);
    
    // Test clearing empty model
    model.clearResourceBonuses();
    QCOMPARE(model.rowCount(), 0);
}

void TestSystemResourceBonusListModel::testGetResourceBonus()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    auto resourceBonus = createTestResourceBonus(42, "Test Resource", 15.0);
    model.addResourceBonus(resourceBonus);
    
    // Test getting all resource bonuses
    auto allResourceBonuses = model.getResourceBonuses();
    QCOMPARE(allResourceBonuses.size(), 1);
    QVERIFY(allResourceBonuses[0] != nullptr);
    QCOMPARE(allResourceBonuses[0]->id(), 42);
    
    // Test empty model
    model.clearResourceBonuses();
    auto emptyList = model.getResourceBonuses();
    QCOMPARE(emptyList.size(), 0);
}

void TestSystemResourceBonusListModel::testModelSignals()
{
    ggh::Galaxy::Factions::viewmodels::SystemResourceBonusListModel model;
    
    QSignalSpy beginInsertSpy(&model, &QAbstractItemModel::rowsAboutToBeInserted);
    QSignalSpy endInsertSpy(&model, &QAbstractItemModel::rowsInserted);
    QSignalSpy beginRemoveSpy(&model, &QAbstractItemModel::rowsAboutToBeRemoved);
    QSignalSpy endRemoveSpy(&model, &QAbstractItemModel::rowsRemoved);
    
    // Test insert signals
    auto resourceBonus = createTestResourceBonus(1, "Resource", 10.0);
    model.addResourceBonus(resourceBonus);
    
    QCOMPARE(beginInsertSpy.count(), 1);
    QCOMPARE(endInsertSpy.count(), 1);
    
    // Test remove signals - remove by the correct ID
    model.removeResourceBonus(1); // Use the actual ID from the resource bonus
    
    QCOMPARE(beginRemoveSpy.count(), 1);
    QCOMPARE(endRemoveSpy.count(), 1);
}

QTEST_MAIN(TestSystemResourceBonusListModel)
#include "test_system_resource_bonus_list_model.moc"
