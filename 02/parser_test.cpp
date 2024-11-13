#include "parser.h"
#include <cstddef>
#include <vector>
#include <gtest/gtest.h>

TEST(parser_test, simpleNumberTest)
{
    TokenParser parser;
    std::vector<std::string> digits;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.Parse("12345");

    ASSERT_EQ(digits.size(), 1);
    EXPECT_EQ(digits[0], "12345");
}

TEST(parser_test, simpleStringTest)
{
   TokenParser parser;
   std::vector<std::string> strings;

    parser.SetStringTokenCallback([&](const std::string& token) {
         strings.push_back(token);
    });

    parser.Parse("hello");

    ASSERT_EQ(strings.size(), 1);
    EXPECT_EQ(strings[0], "hello");
}

TEST(parser_test, simpleMixedTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("12345 hello");

    ASSERT_EQ(digits.size(), 1);
    EXPECT_EQ(digits[0], "12345");

    ASSERT_EQ(strings.size(), 1);
    EXPECT_EQ(strings[0], "hello");
}

TEST(parser_test, emptyStringTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("");

    ASSERT_EQ(digits.size(), 0);
    ASSERT_EQ(strings.size(), 0);
}

TEST(parser_test, emptyDigitTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("hello");

    ASSERT_EQ(digits.size(), 0);
    ASSERT_EQ(strings.size(), 1);
    EXPECT_EQ(strings[0], "hello");
}

TEST(parser_test, overflowDigitsTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("123456789012345678901234567890");

    ASSERT_EQ(strings.size(), 1);
    ASSERT_EQ(digits.size(), 0);
    EXPECT_EQ(strings[0], "123456789012345678901234567890");
}

TEST(parser_test, negativeNumbersTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("-12345");

    ASSERT_EQ(digits.size(), 0);
    ASSERT_EQ(strings.size(), 1);
    EXPECT_EQ(strings[0], "-12345");
}

TEST(parser_test, spaceSeparatorTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("12345 hello");

    ASSERT_EQ(digits.size(), 1);
    EXPECT_EQ(digits[0], "12345");

    ASSERT_EQ(strings.size(), 1);
    EXPECT_EQ(strings[0], "hello");
}

TEST(parser_test, tabSeparatorTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("12345\thello");

    ASSERT_EQ(digits.size(), 1);
    EXPECT_EQ(digits[0], "12345");

    ASSERT_EQ(strings.size(), 1);
    EXPECT_EQ(strings[0], "hello");
}

TEST(parser_test, lineFeedSeparatorTest)
{
    TokenParser parser;
    std::vector<std::string> digits;
    std::vector<std::string> strings;

    parser.SetDigitTokenCallback([&](const std::string& token) {
        digits.push_back(token);
    });

    parser.SetStringTokenCallback([&](const std::string& token) {
        strings.push_back(token);
    });

    parser.Parse("12345\nhello");

    ASSERT_EQ(digits.size(), 1);
    EXPECT_EQ(digits[0], "12345");

    ASSERT_EQ(strings.size(), 1);
    EXPECT_EQ(strings[0], "hello");
}

//Токены разделены пробелами, символами табуляции и первода строки.
int main(int argc, char** argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}