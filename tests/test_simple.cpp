#include <gtest/gtest.h>

// Simple test to verify GTest is working
TEST(SimpleTest, BasicAssertions) {
    EXPECT_EQ(2 + 2, 4);
    EXPECT_TRUE(true);
    EXPECT_FALSE(false);
}

TEST(SimpleTest, StringComparison) {
    std::string hello = "Hello";
    std::string world = "World";
    EXPECT_NE(hello, world);
    EXPECT_EQ(hello + " " + world, "Hello World");
}
