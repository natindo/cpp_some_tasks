#include <gtest/gtest.h>
#include "format_string.h"

TEST(format_string_test, simleTest) {
    EXPECT_EQ(format("{1}+{1} = {0}", 2, "one"), "one+one = 2");
}

TEST(format_string_test, noPlaceholders) {
    EXPECT_EQ(format("Just a string"), "Just a string");
}

TEST(format_string_test, oneArg) {
    EXPECT_EQ(format("Hello {0}!", "World"), "Hello World!");
}

TEST(format_string_test, allArg) {
    EXPECT_EQ(format("{0} {1}", "Hello", "World!"), "Hello World!");
}

TEST(format_string_test, repeatArg) {
    EXPECT_EQ(format("{0} {0}", "123"), "123 123");
}

TEST(format_string_test, diffTypes) {
    EXPECT_EQ(format("{0} {1} {2}", 1, 1.1, std::string("1")), "1 1.1 1");
}

TEST(format_string_test, emptyString) {
    EXPECT_EQ(format("", 1, 1), "");
}

TEST(format_string_test, exceptionIndex) {
    EXPECT_THROW({
        format("{0} {1}", 1);
    }, FormatException);
}

TEST(format_string_test, exceptionArg) {
    EXPECT_THROW({
        format("{hello}");
    }, FormatException);
}

TEST(format_string_test, exceptionWithoutOpenBrace) {
    EXPECT_THROW({
        format("1}", 1);
    }, FormatException);
}

TEST(format_string_test, exceptionWithoutCloseBrace) {
    EXPECT_THROW({
        format("{1", 1);
    }, FormatException);
}

int main(int argc, char** argv) {
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}