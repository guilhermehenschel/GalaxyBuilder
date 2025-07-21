#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/HandoverType.h"

namespace ggh::Galaxy::Factions {

class HandoverTypeTest : public ::testing::Test {
protected:
    void SetUp() override {
        handoverType = std::make_shared<HandoverType>(1, "Conquest", "Military conquest");
    }

    void TearDown() override {
        handoverType.reset();
    }

    std::shared_ptr<HandoverType> handoverType;
};

TEST_F(HandoverTypeTest, ConstructorInitialization) {
    EXPECT_EQ(handoverType->id(), 1);
    EXPECT_EQ(handoverType->name(), "Conquest");
    EXPECT_EQ(handoverType->description(), "Military conquest");
}

TEST_F(HandoverTypeTest, ParameterizedConstructor) {
    auto testHandoverType = std::make_shared<HandoverType>(2, "Trade", "Economic acquisition");
    
    EXPECT_EQ(testHandoverType->id(), 2);
    EXPECT_EQ(testHandoverType->name(), "Trade");
    EXPECT_EQ(testHandoverType->description(), "Economic acquisition");
}

TEST_F(HandoverTypeTest, SettersAndGetters) {
    handoverType->setId(42);
    handoverType->setName("Diplomacy");
    handoverType->setDescription("Peaceful negotiation");
    
    EXPECT_EQ(handoverType->id(), 42);
    EXPECT_EQ(handoverType->name(), "Diplomacy");
    EXPECT_EQ(handoverType->description(), "Peaceful negotiation");
}

}
