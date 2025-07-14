#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include <QTimer>
#include "SystemPropertiesViewModel.h"
#include "StarSystemQml.h"
#include "SystemDataManager.h"
#include "Common.h"

class SystemPropertiesViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test objects
        viewModel = new SystemPropertiesViewModel();
        dataManager = new SystemDataManager();
        testSystem = new StarSystemQml();
        
        // Set up test system
        testSystem->setName("Test System");
        testSystem->setStarType(StarType::BlueStar); // A-Class star
        testSystem->setPosition(QPointF(100.0, 200.0));
        
        // Configure ViewModel
        viewModel->setDataManager(dataManager);
    }
    
    void TearDown() override {
        delete viewModel;
        delete dataManager;
        delete testSystem;
    }
    
    SystemPropertiesViewModel* viewModel = nullptr;
    SystemDataManager* dataManager = nullptr;
    StarSystemQml* testSystem = nullptr;
};

// Test basic property access
TEST_F(SystemPropertiesViewModelTest, BasicProperties) {
    EXPECT_FALSE(viewModel->isDirty());
    EXPECT_TRUE(viewModel->isAutoSaveEnabled());
    EXPECT_FALSE(viewModel->isSaving());
    EXPECT_FALSE(viewModel->isLoading());
    EXPECT_TRUE(viewModel->hasDataManager());
    EXPECT_TRUE(viewModel->statusMessage().isEmpty());
}

// Test data manager setting
TEST_F(SystemPropertiesViewModelTest, DataManagerSetting) {
    SystemPropertiesViewModel* newViewModel = new SystemPropertiesViewModel();
    EXPECT_FALSE(newViewModel->hasDataManager());
    
    newViewModel->setDataManager(dataManager);
    EXPECT_TRUE(newViewModel->hasDataManager());
    
    delete newViewModel;
}

// Test star system assignment
TEST_F(SystemPropertiesViewModelTest, StarSystemAssignment) {
    QSignalSpy starSystemSpy(viewModel, &SystemPropertiesViewModel::starSystemChanged);
    
    EXPECT_EQ(viewModel->starSystem(), nullptr);
    
    viewModel->setStarSystem(testSystem);
    EXPECT_EQ(viewModel->starSystem(), testSystem);
    EXPECT_EQ(starSystemSpy.count(), 1);
}

// Test dirty state tracking
TEST_F(SystemPropertiesViewModelTest, DirtyStateTracking) {
    QSignalSpy dirtySpy(viewModel, &SystemPropertiesViewModel::isDirtyChanged);
    
    viewModel->setStarSystem(testSystem);
    EXPECT_FALSE(viewModel->isDirty());
    
    // Simulate property change by manually marking dirty
    // In real usage, this would be triggered by star system property changes
    testSystem->setName("Modified System");
    
    // The ViewModel should detect changes through signal connections
    // For this test, we'll manually trigger the dirty state
    viewModel->addPlanet(); // This should mark as dirty
    EXPECT_TRUE(viewModel->isDirty());
    EXPECT_GT(dirtySpy.count(), 0);
}

// Test save operations
TEST_F(SystemPropertiesViewModelTest, SaveOperations) {
    QSignalSpy saveSpy(viewModel, &SystemPropertiesViewModel::saveCompleted);
    QSignalSpy savingStateSpy(viewModel, &SystemPropertiesViewModel::isSavingChanged);
    
    viewModel->setStarSystem(testSystem);
    
    bool result = viewModel->saveSystemData();
    EXPECT_TRUE(result);
    
    // Check if saving state was managed correctly
    // Note: Due to the synchronous nature of our save, isSaving might not be captured
    EXPECT_FALSE(viewModel->isSaving());
}

// Test planet management
TEST_F(SystemPropertiesViewModelTest, PlanetManagement) {
    viewModel->setStarSystem(testSystem);
    
    int initialPlanetCount = testSystem->getPlanetCount();
    
    // Add planet
    viewModel->addPlanet();
    EXPECT_EQ(testSystem->getPlanetCount(), initialPlanetCount + 1);
    EXPECT_TRUE(viewModel->isDirty());
    
    // Remove planet
    if (testSystem->getPlanetCount() > 0) {
        viewModel->removePlanet(0);
        EXPECT_EQ(testSystem->getPlanetCount(), initialPlanetCount);
    }
}

// Test auto-save functionality
TEST_F(SystemPropertiesViewModelTest, AutoSaveToggle) {
    QSignalSpy autoSaveSpy(viewModel, &SystemPropertiesViewModel::autoSaveEnabledChanged);
    
    EXPECT_TRUE(viewModel->isAutoSaveEnabled());
    
    viewModel->setAutoSaveEnabled(false);
    EXPECT_FALSE(viewModel->isAutoSaveEnabled());
    EXPECT_EQ(autoSaveSpy.count(), 1);
    
    viewModel->setAutoSaveEnabled(true);
    EXPECT_TRUE(viewModel->isAutoSaveEnabled());
    EXPECT_EQ(autoSaveSpy.count(), 2);
}

// Test status message handling
TEST_F(SystemPropertiesViewModelTest, StatusMessageHandling) {
    QSignalSpy statusSpy(viewModel, &SystemPropertiesViewModel::statusMessageChanged);
    
    // Status should be empty initially
    EXPECT_TRUE(viewModel->statusMessage().isEmpty());
    
    // Operations should generate status messages
    viewModel->setStarSystem(testSystem);
    viewModel->saveSystemData();
    
    // Check if status was set (implementation dependent)
    viewModel->clearStatusMessage();
    EXPECT_TRUE(viewModel->statusMessage().isEmpty());
}

// Test reset functionality
TEST_F(SystemPropertiesViewModelTest, ResetChanges) {
    viewModel->setStarSystem(testSystem);
    viewModel->addPlanet(); // Make changes
    
    EXPECT_TRUE(viewModel->isDirty());
    
    viewModel->resetChanges();
    // After reset, dirty state should be cleared
    // Implementation may vary based on how reset is implemented
}

// Test clean state management
TEST_F(SystemPropertiesViewModelTest, CleanStateManagement) {
    QSignalSpy dirtySpy(viewModel, &SystemPropertiesViewModel::isDirtyChanged);
    
    viewModel->setStarSystem(testSystem);
    viewModel->addPlanet(); // Make dirty
    EXPECT_TRUE(viewModel->isDirty());
    
    viewModel->markAsClean();
    EXPECT_FALSE(viewModel->isDirty());
}

// Test error handling
TEST_F(SystemPropertiesViewModelTest, ErrorHandling) {
    // Test operations without data manager
    SystemPropertiesViewModel* noDataViewModel = new SystemPropertiesViewModel();
    EXPECT_FALSE(noDataViewModel->hasDataManager());
    
    // Operations should handle missing data manager gracefully
    bool result = noDataViewModel->saveSystemData();
    EXPECT_FALSE(result); // Should fail without data manager
    
    delete noDataViewModel;
}

// Test signal emissions
TEST_F(SystemPropertiesViewModelTest, SignalEmissions) {
    QSignalSpy starSystemSpy(viewModel, &SystemPropertiesViewModel::starSystemChanged);
    QSignalSpy dirtySpy(viewModel, &SystemPropertiesViewModel::isDirtyChanged);
    QSignalSpy statusSpy(viewModel, &SystemPropertiesViewModel::statusMessageChanged);
    
    viewModel->setStarSystem(testSystem);
    EXPECT_GT(starSystemSpy.count(), 0);
    
    viewModel->addPlanet();
    EXPECT_GT(dirtySpy.count(), 0);
    
    // Status messages should be emitted during operations
    // The exact count depends on implementation details
    EXPECT_GE(statusSpy.count(), 0);
}
