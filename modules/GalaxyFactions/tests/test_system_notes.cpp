#include <gtest/gtest.h>
#include "ggh/modules/GalaxyFactions/SystemNotes.h"

namespace ggh::Galaxy::Factions {

class SystemNotesTest : public ::testing::Test {
protected:
    void SetUp() override {
        notes = std::make_shared<SystemNotes>();
    }

    void TearDown() override {
        notes.reset();
    }

    std::shared_ptr<SystemNotes> notes;
};

TEST_F(SystemNotesTest, DefaultConstructor) {
    EXPECT_EQ(notes->id(), 0);
    EXPECT_EQ(notes->systemId(), 0);
    EXPECT_EQ(notes->note(), "");
}

TEST_F(SystemNotesTest, ParameterizedConstructor) {
    auto testNotes = std::make_shared<SystemNotes>(1, 10, "System has rich mineral deposits");
    
    EXPECT_EQ(testNotes->id(), 1);
    EXPECT_EQ(testNotes->systemId(), 10);
    EXPECT_EQ(testNotes->note(), "System has rich mineral deposits");
}

TEST_F(SystemNotesTest, SettersAndGetters) {
    notes->setId(42);
    notes->setSystemId(99);
    notes->setNote("Strategic importance: High");
    
    EXPECT_EQ(notes->id(), 42);
    EXPECT_EQ(notes->systemId(), 99);
    EXPECT_EQ(notes->note(), "Strategic importance: High");
}
}
