#include <gtest/gtest.h>
#include <string>
#include "SeparateChaining.hpp"
#include "LinearProbing.hpp"
#include "CuckooHash.hpp"
#include "StringPair.hpp"
#include "TextGenerator.hpp"
#include "FreqList.hpp"

// Hash Table Tests
TEST(SeparateChaining, BasicOperations) {
    SeparateChaining<std::string, int> table(5);
    table.insert("a", 1);
    table.insert("b", 2);
    EXPECT_EQ(table.size(), 2u);
    ASSERT_NE(table.find("a"), nullptr);
    EXPECT_EQ(*table.find("a"), 1);
    ASSERT_NE(table.find("b"), nullptr);
    EXPECT_EQ(*table.find("b"), 2);
    EXPECT_EQ(table.find("c"), nullptr);

    table.insert("a", 10);
    ASSERT_NE(table.find("a"), nullptr);
    EXPECT_EQ(*table.find("a"), 10);

    EXPECT_TRUE(table.remove("a"));
    EXPECT_EQ(table.find("a"), nullptr);
    EXPECT_FALSE(table.remove("a"));

    table["c"] = 3;
    ASSERT_NE(table.find("c"), nullptr);
    EXPECT_EQ(*table.find("c"), 3);
    EXPECT_EQ(table.size(), 2u);
}


// Main for Google Test
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}