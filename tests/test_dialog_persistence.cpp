#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QFile>
#include <QDir>
#include <QDebug>
#include "SystemDataManager.h"
#include "StarSystem.h"
#include "Planet.h"

class DialogPersistenceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create temporary directory for test files
        tempDir = std::make_unique<QTemporaryDir>();
        ASSERT_TRUE(tempDir->isValid());
        
        // Create SystemDataManager
        dataManager = new SystemDataManager();
        
        // Create a test star system
        testSystem = new StarSystem();
        testSystem->setName("Test System");
        testSystem->setPosition(QPointF(100, 200));
        testSystem->setStarType(2); // Binary
        testSystem->setStarMass(1.5);
        testSystem->setStarTemperature(6000);
        testSystem->setStarLuminosity(2.0);
    }
    
    void TearDown() override {
        delete testSystem;
        delete dataManager;
        tempDir.reset();
    }
    
    std::unique_ptr<QTemporaryDir> tempDir;
    SystemDataManager* dataManager;
    StarSystem* testSystem;
};

TEST_F(DialogPersistenceTest, PlanetDataPersistsThroughDialogCycles) {
    qDebug() << "=== Testing Planet Data Persistence Through Dialog Cycles ===";
    
    // Simulate opening SystemPropertiesDialog and adding planets
    qDebug() << "1. Initial state - adding planets to system";
    
    // Add planets similar to what would happen in the dialog
    testSystem->addPlanet(); // Planet 0
    testSystem->addPlanet(); // Planet 1
    testSystem->addPlanet(); // Planet 2
    
    // Get the planets and set their properties
    auto planetsModel = testSystem->getPlanetsModel();
    ASSERT_EQ(planetsModel->rowCount(), 3) << "Should have 3 planets";
    
    // Set properties for planet 0
    auto planet0 = planetsModel->getPlanet(0);
    ASSERT_NE(planet0, nullptr) << "Planet 0 should exist";
    planet0->setName("Kepler-442b");
    planet0->setType(Planet::PlanetType::Ocean);
    planet0->setSize(1.2);
    planet0->setMass(2.3);
    planet0->setGravity(1.1);
    planet0->setMoons(2);
    planet0->setMinTemperature(-20);
    planet0->setMaxTemperature(45);
    planet0->setOrbitDistance(120.0);
    
    // Set properties for planet 1
    auto planet1 = planetsModel->getPlanet(1);
    ASSERT_NE(planet1, nullptr) << "Planet 1 should exist";
    planet1->setName("Titan-2");
    planet1->setType(Planet::PlanetType::GasGiant);
    planet1->setSize(3.4);
    planet1->setMass(15.8);
    planet1->setGravity(2.8);
    planet1->setMoons(12);
    planet1->setMinTemperature(-180);
    planet1->setMaxTemperature(-120);
    planet1->setOrbitDistance(450.0);
    
    // Set properties for planet 2
    auto planet2 = planetsModel->getPlanet(2);
    ASSERT_NE(planet2, nullptr) << "Planet 2 should exist";
    planet2->setName("Mars-like");
    planet2->setType(Planet::PlanetType::Desert);
    planet2->setSize(0.8);
    planet2->setMass(0.6);
    planet2->setGravity(0.7);
    planet2->setMoons(0);
    planet2->setMinTemperature(-80);
    planet2->setMaxTemperature(20);
    planet2->setOrbitDistance(200.0);
    
    qDebug() << "2. Simulating dialog close - saving system data";
    
    // Simulate dialog closing and saving data (like onClosing handler)
    bool saveSuccess = dataManager->saveSystemChanges(testSystem);
    ASSERT_TRUE(saveSuccess) << "Save should succeed";
    
    qDebug() << "3. Simulating clearing/modifying system (like dialog reopening)";
    
    // Remove all planets to simulate system being "cleared" or reset
    while (planetsModel->rowCount() > 0) {
        testSystem->removePlanetAt(0);
    }
    ASSERT_EQ(planetsModel->rowCount(), 0) << "All planets should be removed";
    
    qDebug() << "4. Simulating dialog reopen - loading system data";
    
    // Simulate dialog reopening and loading data (like loadSystemData)
    bool loadSuccess = dataManager->loadSystemData(testSystem);
    ASSERT_TRUE(loadSuccess) << "Load should succeed";
    
    qDebug() << "5. Verifying planet data was restored correctly";
    
    // Verify planets were restored
    ASSERT_EQ(planetsModel->rowCount(), 3) << "Should have 3 planets after reload";
    
    // Verify planet 0 data
    auto restoredPlanet0 = planetsModel->getPlanet(0);
    ASSERT_NE(restoredPlanet0, nullptr) << "Restored planet 0 should exist";
    EXPECT_EQ(restoredPlanet0->getName(), "Kepler-442b") << "Planet 0 name should be restored";
    EXPECT_EQ(restoredPlanet0->getType(), Planet::PlanetType::Ocean) << "Planet 0 type should be restored";
    EXPECT_NEAR(restoredPlanet0->getSize(), 1.2, 0.01) << "Planet 0 size should be restored";
    EXPECT_NEAR(restoredPlanet0->getMass(), 2.3, 0.01) << "Planet 0 mass should be restored";
    EXPECT_NEAR(restoredPlanet0->getGravity(), 1.1, 0.01) << "Planet 0 gravity should be restored";
    EXPECT_EQ(restoredPlanet0->getMoons(), 2) << "Planet 0 moons should be restored";
    EXPECT_EQ(restoredPlanet0->getMinTemperature(), -20) << "Planet 0 min temp should be restored";
    EXPECT_EQ(restoredPlanet0->getMaxTemperature(), 45) << "Planet 0 max temp should be restored";
    EXPECT_NEAR(restoredPlanet0->getOrbitDistance(), 120.0, 0.01) << "Planet 0 orbit distance should be restored";
    
    // Verify planet 1 data
    auto restoredPlanet1 = planetsModel->getPlanet(1);
    ASSERT_NE(restoredPlanet1, nullptr) << "Restored planet 1 should exist";
    EXPECT_EQ(restoredPlanet1->getName(), "Titan-2") << "Planet 1 name should be restored";
    EXPECT_EQ(restoredPlanet1->getType(), Planet::PlanetType::GasGiant) << "Planet 1 type should be restored";
    EXPECT_NEAR(restoredPlanet1->getSize(), 3.4, 0.01) << "Planet 1 size should be restored";
    EXPECT_NEAR(restoredPlanet1->getMass(), 15.8, 0.01) << "Planet 1 mass should be restored";
    EXPECT_NEAR(restoredPlanet1->getGravity(), 2.8, 0.01) << "Planet 1 gravity should be restored";
    EXPECT_EQ(restoredPlanet1->getMoons(), 12) << "Planet 1 moons should be restored";
    EXPECT_EQ(restoredPlanet1->getMinTemperature(), -180) << "Planet 1 min temp should be restored";
    EXPECT_EQ(restoredPlanet1->getMaxTemperature(), -120) << "Planet 1 max temp should be restored";
    EXPECT_NEAR(restoredPlanet1->getOrbitDistance(), 450.0, 0.01) << "Planet 1 orbit distance should be restored";
    
    // Verify planet 2 data
    auto restoredPlanet2 = planetsModel->getPlanet(2);
    ASSERT_NE(restoredPlanet2, nullptr) << "Restored planet 2 should exist";
    EXPECT_EQ(restoredPlanet2->getName(), "Mars-like") << "Planet 2 name should be restored";
    EXPECT_EQ(restoredPlanet2->getType(), Planet::PlanetType::Desert) << "Planet 2 type should be restored";
    EXPECT_NEAR(restoredPlanet2->getSize(), 0.8, 0.01) << "Planet 2 size should be restored";
    EXPECT_NEAR(restoredPlanet2->getMass(), 0.6, 0.01) << "Planet 2 mass should be restored";
    EXPECT_NEAR(restoredPlanet2->getGravity(), 0.7, 0.01) << "Planet 2 gravity should be restored";
    EXPECT_EQ(restoredPlanet2->getMoons(), 0) << "Planet 2 moons should be restored";
    EXPECT_EQ(restoredPlanet2->getMinTemperature(), -80) << "Planet 2 min temp should be restored";
    EXPECT_EQ(restoredPlanet2->getMaxTemperature(), 20) << "Planet 2 max temp should be restored";
    EXPECT_NEAR(restoredPlanet2->getOrbitDistance(), 200.0, 0.01) << "Planet 2 orbit distance should be restored";
    
    qDebug() << "6. Testing multiple dialog cycles";
    
    // Test another cycle - modify planet and save/reload again
    restoredPlanet0->setName("Modified Kepler");
    restoredPlanet0->setSize(1.5);
    
    // Save again
    saveSuccess = dataManager->saveSystemChanges(testSystem);
    ASSERT_TRUE(saveSuccess) << "Second save should succeed";
    
    // Clear and reload again
    while (planetsModel->rowCount() > 0) {
        testSystem->removePlanetAt(0);
    }
    ASSERT_EQ(planetsModel->rowCount(), 0) << "All planets should be removed again";
    
    loadSuccess = dataManager->loadSystemData(testSystem);
    ASSERT_TRUE(loadSuccess) << "Second load should succeed";
    
    // Verify modified data persisted
    ASSERT_EQ(planetsModel->rowCount(), 3) << "Should still have 3 planets";
    auto finalPlanet0 = planetsModel->getPlanet(0);
    ASSERT_NE(finalPlanet0, nullptr) << "Final planet 0 should exist";
    EXPECT_EQ(finalPlanet0->getName(), "Modified Kepler") << "Modified name should persist";
    EXPECT_NEAR(finalPlanet0->getSize(), 1.5, 0.01) << "Modified size should persist";
    
    qDebug() << "=== Planet Dialog Persistence Test PASSED ===";
}

TEST_F(DialogPersistenceTest, AutoSaveAndDialogPersistence) {
    qDebug() << "=== Testing Auto-Save and Dialog Persistence ===";
    
    // Enable auto-save (like the checkbox in dialog)
    dataManager->enableAutoSave(true);
    
    // Add a planet
    testSystem->addPlanet();
    auto planetsModel = testSystem->getPlanetsModel();
    ASSERT_EQ(planetsModel->rowCount(), 1) << "Should have 1 planet";
    
    auto planet = planetsModel->getPlanet(0);
    planet->setName("AutoSave Test Planet");
    planet->setType(Planet::PlanetType::Volcanic);
    planet->setSize(0.9);
    
    // Trigger auto-save (like what happens when properties change)
    bool autoSaveSuccess = dataManager->saveSystemChanges(testSystem);
    ASSERT_TRUE(autoSaveSuccess) << "Auto-save should succeed";
    
    // Clear system
    while (planetsModel->rowCount() > 0) {
        testSystem->removePlanetAt(0);
    }
    ASSERT_EQ(planetsModel->rowCount(), 0) << "Planet should be removed";
    
    // Load data back
    bool loadSuccess = dataManager->loadSystemData(testSystem);
    ASSERT_TRUE(loadSuccess) << "Load should succeed";
    
    // Verify auto-saved data was restored
    ASSERT_EQ(planetsModel->rowCount(), 1) << "Should have 1 planet after auto-save load";
    auto restoredPlanet = planetsModel->getPlanet(0);
    ASSERT_NE(restoredPlanet, nullptr) << "Restored planet should exist";
    EXPECT_EQ(restoredPlanet->getName(), "AutoSave Test Planet") << "Auto-saved name should persist";
    EXPECT_EQ(restoredPlanet->getType(), Planet::PlanetType::Volcanic) << "Auto-saved type should persist";
    EXPECT_NEAR(restoredPlanet->getSize(), 0.9, 0.01) << "Auto-saved size should persist";
    
    qDebug() << "=== Auto-Save Dialog Persistence Test PASSED ===";
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
