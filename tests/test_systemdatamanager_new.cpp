#include <gtest/gtest.h>
#include <QCoreApplication>
#include "../modules/GalaxyCore/SystemDataManager.h"

class SystemDataManagerTest : public ::testing::Test
{
protected:
    void SetUp() override;
    void TearDown() override;

    std::unique_ptr<SystemDataManager> m_manager;
    static QCoreApplication* s_app;
};

QCoreApplication* SystemDataManagerTest::s_app = nullptr;

void SystemDataManagerTest::SetUp()
{
    if (!QCoreApplication::instance() && !s_app) {
        int argc = 1;
        char* argv[] = {const_cast<char*>("test_systemdatamanager"), nullptr};
        s_app = new QCoreApplication(argc, argv);
    }
    
    m_manager = std::make_unique<SystemDataManager>();
}

void SystemDataManagerTest::TearDown()
{
    m_manager.reset();
}

TEST_F(SystemDataManagerTest, BasicCreation)
{
    EXPECT_NE(m_manager.get(), nullptr);
}

TEST_F(SystemDataManagerTest, SystemTypes)
{
    // Test basic system type functionality
    EXPECT_TRUE(true); // Placeholder test
}
