#include <gtest/gtest.h>
#include <QCoreApplication>
#include <QSignalSpy>
#include <memory>

#include "ggh/modules/GalaxyCore/viewmodels/GalaxyViewModel.h"
#include "ggh/modules/GalaxyCore/models/GalaxyModel.h"

using namespace ggh::GalaxyCore::viewmodels;
using namespace ggh::GalaxyCore::models;

class GalaxyViewModelTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (!QCoreApplication::instance()) {
            int argc = 0;
            char** argv = nullptr;
            app = std::make_unique<QCoreApplication>(argc, argv);
        }
        
        galaxy = std::make_shared<GalaxyModel>(1000, 800);
        viewModel = std::make_unique<GalaxyViewModel>(galaxy);
    }

    void TearDown() override {
        viewModel.reset();
        galaxy.reset();
    }

    std::unique_ptr<QCoreApplication> app;
    std::shared_ptr<GalaxyModel> galaxy;
    std::unique_ptr<GalaxyViewModel> viewModel;
};

TEST_F(GalaxyViewModelTest, InitialProperties) {
    EXPECT_EQ(viewModel->width(), 1000);
    EXPECT_EQ(viewModel->height(), 800);
    EXPECT_EQ(viewModel->systemCount(), 0);
}

TEST_F(GalaxyViewModelTest, SettersEmitSignals) {
    QSignalSpy dimensionsSpy(viewModel.get(), &GalaxyViewModel::dimensionsChanged);
    
    viewModel->setWidth(1200);
    viewModel->setHeight(900);
    
    EXPECT_EQ(dimensionsSpy.count(), 2); // Width and height changed separately
}

TEST_F(GalaxyViewModelTest, SetDimensionsBoth) {
    QSignalSpy dimensionsSpy(viewModel.get(), &GalaxyViewModel::dimensionsChanged);
    
    viewModel->setDimensions(1500, 1200);
    
    EXPECT_EQ(dimensionsSpy.count(), 1); // Only one signal for both dimensions
    EXPECT_EQ(viewModel->width(), 1500);
    EXPECT_EQ(viewModel->height(), 1200);
}

TEST_F(GalaxyViewModelTest, NoSignalOnSameValue) {
    QSignalSpy dimensionsSpy(viewModel.get(), &GalaxyViewModel::dimensionsChanged);
    
    viewModel->setWidth(1000); // Same value
    
    EXPECT_EQ(dimensionsSpy.count(), 0);
}

TEST_F(GalaxyViewModelTest, StarSystemsListModel) {
    auto* systemsModel = viewModel->starSystems();
    ASSERT_NE(systemsModel, nullptr);
    
    // Initially no systems
    EXPECT_EQ(systemsModel->rowCount(), 0);
    EXPECT_EQ(viewModel->systemCount(), 0);
}

TEST_F(GalaxyViewModelTest, AddStarSystem) {
    QSignalSpy systemCountSpy(viewModel.get(), &GalaxyViewModel::systemCountChanged);
    
    viewModel->addStarSystem(1, "Sol", 0.0, 0.0);
    
    EXPECT_EQ(viewModel->systemCount(), 1);
    EXPECT_EQ(systemCountSpy.count(), 1);
    
    auto* systemsModel = viewModel->starSystems();
    EXPECT_EQ(systemsModel->rowCount(), 1);
    
    auto nameData = systemsModel->data(systemsModel->index(0, 0), Qt::UserRole + 2); // NameRole
    EXPECT_EQ(nameData.toString(), "Sol");
}

TEST_F(GalaxyViewModelTest, RemoveStarSystem) {
    // Add a system first
    viewModel->addStarSystem(1, "Alpha Centauri", 25.0, 8.0);
    EXPECT_EQ(viewModel->systemCount(), 1);
    
    QSignalSpy systemCountSpy(viewModel.get(), &GalaxyViewModel::systemCountChanged);
    
    // Remove it
    bool removed = viewModel->removeStarSystem(1);
    EXPECT_TRUE(removed);
    EXPECT_EQ(viewModel->systemCount(), 0);
    EXPECT_EQ(systemCountSpy.count(), 1);
    
    // Try to remove non-existent system
    bool notRemoved = viewModel->removeStarSystem(999);
    EXPECT_FALSE(notRemoved);
}

TEST_F(GalaxyViewModelTest, ClearSystems) {
    // Add multiple systems
    viewModel->addStarSystem(1, "Sol", 0.0, 0.0);
    viewModel->addStarSystem(2, "Alpha Centauri", 25.0, 8.0);
    viewModel->addStarSystem(3, "Vega", 147.0, 38.8);
    EXPECT_EQ(viewModel->systemCount(), 3);
    
    QSignalSpy systemCountSpy(viewModel.get(), &GalaxyViewModel::systemCountChanged);
    
    viewModel->clearSystems();
    
    EXPECT_EQ(viewModel->systemCount(), 0);
    EXPECT_EQ(systemCountSpy.count(), 1);
}

TEST_F(GalaxyViewModelTest, ModelAccess) {
    auto retrievedGalaxy = viewModel->galaxy();
    EXPECT_EQ(retrievedGalaxy, galaxy);
}

TEST_F(GalaxyViewModelTest, SetDifferentGalaxy) {
    QSignalSpy dimensionsSpy(viewModel.get(), &GalaxyViewModel::dimensionsChanged);
    QSignalSpy systemCountSpy(viewModel.get(), &GalaxyViewModel::systemCountChanged);
    
    auto newGalaxy = std::make_shared<GalaxyModel>(2000, 1500);
    viewModel->setGalaxy(newGalaxy);
    
    EXPECT_EQ(viewModel->width(), 2000);
    EXPECT_EQ(viewModel->height(), 1500);
    EXPECT_GT(dimensionsSpy.count(), 0);
    EXPECT_GT(systemCountSpy.count(), 0);
}

TEST_F(GalaxyViewModelTest, ToXmlOutput) {
    QString xml = viewModel->toXml();
    EXPECT_TRUE(xml.contains("<Galaxy"));
}

TEST_F(GalaxyViewModelTest, DefaultConstructor) {
    GalaxyViewModel defaultViewModel;
    EXPECT_GT(defaultViewModel.width(), 0);
    EXPECT_GT(defaultViewModel.height(), 0);
    EXPECT_NE(defaultViewModel.galaxy(), nullptr);
    EXPECT_NE(defaultViewModel.starSystems(), nullptr);
}

TEST_F(GalaxyViewModelTest, StarSystemListModelRoles) {
    viewModel->addStarSystem(1, "Test System", 100.0, -50.0);
    
    auto* systemsModel = viewModel->starSystems();
    auto roleNames = systemsModel->roleNames();
    
    EXPECT_TRUE(roleNames.contains(Qt::UserRole + 1)); // systemId role
    EXPECT_TRUE(roleNames.contains(Qt::UserRole + 2)); // name role
    EXPECT_TRUE(roleNames.contains(Qt::UserRole + 3)); // positionX role
    EXPECT_TRUE(roleNames.contains(Qt::UserRole + 4)); // positionY role
    
    EXPECT_EQ(roleNames[Qt::UserRole + 1], "systemId");
    EXPECT_EQ(roleNames[Qt::UserRole + 2], "name");
    EXPECT_EQ(roleNames[Qt::UserRole + 3], "positionX");
    EXPECT_EQ(roleNames[Qt::UserRole + 4], "positionY");
}

TEST_F(GalaxyViewModelTest, StarSystemListModelData) {
    viewModel->addStarSystem(1, "Test System", 123.45, -67.89);
    
    auto* systemsModel = viewModel->starSystems();
    auto index = systemsModel->index(0, 0);
    
    auto nameData = systemsModel->data(index, Qt::UserRole + 2); // NameRole
    auto posXData = systemsModel->data(index, Qt::UserRole + 3); // PositionXRole
    auto posYData = systemsModel->data(index, Qt::UserRole + 4); // PositionYRole
    
    EXPECT_EQ(nameData.toString(), "Test System");
    EXPECT_DOUBLE_EQ(posXData.toDouble(), 123.45);
    EXPECT_DOUBLE_EQ(posYData.toDouble(), -67.89);
}
