#include "ggh/modules/GalaxyFactories/AbstractGalaxyFactory.h"
#include "ggh/modules/GalaxyFactories/Types.h"

#include <gtest/gtest.h>
#include <memory>

using namespace ggh::GalaxyFactories;

// Mock implementation of AbstractGalaxyFactory for testing
class MockGalaxyFactory : public AbstractGalaxyFactory {
private:
    GenerationParameters m_params;
    
public:
    std::unique_ptr<GalaxyModel> generateGalaxy() override {
        // Create a mock galaxy with the stored parameters
        auto galaxy = std::make_unique<GalaxyModel>(m_params.width, m_params.height);
        // Note: GalaxyModel doesn't store shape, but we can validate parameters
        return galaxy;
    }
    
    void setParameters(const GenerationParameters& params) override {
        m_params = params;
    }
    
    GenerationParameters getParameters() const override {
        return m_params;
    }
};

class AbstractGalaxyFactoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        factory = std::make_unique<MockGalaxyFactory>();
    }

    void TearDown() override {
        factory.reset();
    }

    std::unique_ptr<MockGalaxyFactory> factory;
};

TEST_F(AbstractGalaxyFactoryTest, SetAndGetParameters) {
    GenerationParameters params;
    params.systemCount = 100;
    params.width = 1500;
    params.height = 1200;
    params.shape = GalaxyShape::Ring;
    params.spiralArms = 4.0;
    params.spiralTightness = 0.8;
    params.coreRadius = 0.3;
    params.edgeRadius = 0.9;
    params.seed = 54321;

    factory->setParameters(params);
    auto retrievedParams = factory->getParameters();

    EXPECT_EQ(retrievedParams.systemCount, params.systemCount);
    EXPECT_EQ(retrievedParams.width, params.width);
    EXPECT_EQ(retrievedParams.height, params.height);
    EXPECT_EQ(retrievedParams.shape, params.shape);
    EXPECT_DOUBLE_EQ(retrievedParams.spiralArms, params.spiralArms);
    EXPECT_DOUBLE_EQ(retrievedParams.spiralTightness, params.spiralTightness);
    EXPECT_DOUBLE_EQ(retrievedParams.coreRadius, params.coreRadius);
    EXPECT_DOUBLE_EQ(retrievedParams.edgeRadius, params.edgeRadius);
    EXPECT_EQ(retrievedParams.seed, params.seed);
}

TEST_F(AbstractGalaxyFactoryTest, GenerateGalaxy) {
    GenerationParameters params;
    params.systemCount = 50;
    params.width = 800;
    params.height = 600;
    params.shape = GalaxyShape::Elliptical;
    params.seed = 42;

    factory->setParameters(params);
    auto galaxy = factory->generateGalaxy();

    ASSERT_NE(galaxy, nullptr);
    EXPECT_EQ(galaxy->getWidth(), params.width);
    EXPECT_EQ(galaxy->getHeight(), params.height);
    // Galaxy shape is handled at generation level, not stored in model
    EXPECT_EQ(factory->getParameters().shape, params.shape);
}

TEST_F(AbstractGalaxyFactoryTest, ParameterPersistence) {
    GenerationParameters params1;
    params1.systemCount = 25;
    params1.width = 400;
    params1.height = 300;
    params1.shape = GalaxyShape::Cluster;
    params1.seed = 123;

    factory->setParameters(params1);
    
    // Generate galaxy
    auto galaxy = factory->generateGalaxy();
    
    // Parameters should persist
    auto retrievedParams = factory->getParameters();
    EXPECT_EQ(retrievedParams.systemCount, params1.systemCount);
    EXPECT_EQ(retrievedParams.width, params1.width);
    EXPECT_EQ(retrievedParams.height, params1.height);
    EXPECT_EQ(retrievedParams.shape, params1.shape);
    EXPECT_EQ(retrievedParams.seed, params1.seed);
}
