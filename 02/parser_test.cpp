#include "parser.h"
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

// TEST(parser_test, simpleStringTest)
// {
//    TokenParser parser;
// }

int main(int argc, char** argv)
{
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}