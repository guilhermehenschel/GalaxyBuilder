#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/models/ResourceType.h"

namespace ggh::Galaxy::Factions::models {

class ResourceTypeTest : public ::testing::Test {
protected:
    void SetUp() override {
        resourceType = std::make_shared<ResourceType>();
    }

    void TearDown() override {
        resourceType.reset();
    }

    std::shared_ptr<ResourceType> resourceType;
};

TEST_F(ResourceTypeTest, DefaultConstructor) {
    EXPECT_EQ(resourceType->id(), 0);
    EXPECT_EQ(resourceType->name(), "");
    EXPECT_EQ(resourceType->description(), "");
}

TEST_F(ResourceTypeTest, ParameterizedConstructor) {
    auto resource = std::make_shared<ResourceType>(1, "Energy", "Primary energy resource");
    
    EXPECT_EQ(resource->id(), 1);
    EXPECT_EQ(resource->name(), "Energy");
    EXPECT_EQ(resource->description(), "Primary energy resource");
}

TEST_F(ResourceTypeTest, SettersAndGetters) {
    resourceType->setId(42);
    resourceType->setName("Minerals");
    resourceType->setDescription("Basic construction materials");
    
    EXPECT_EQ(resourceType->id(), 42);
    EXPECT_EQ(resourceType->name(), "Minerals");
    EXPECT_EQ(resourceType->description(), "Basic construction materials");
}
}