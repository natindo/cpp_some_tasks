#include "bigint.h"
#include <gtest/gtest.h>

///////////////// init /////////////////

TEST(bigint_test_init, integerInput)
{
    BigInt a(1);
    EXPECT_EQ(BigInt("1"), a);
}

TEST(bigint_test_init, negativeIntegerInput)
{
    BigInt a(-1);
    EXPECT_EQ(BigInt("-1"), a);
}

TEST(bigint_test_init, exceptionInput)
{
    EXPECT_THROW(
        BigInt f("123a"), std::invalid_argument);
}

///////////////// comparisons /////////////////

TEST(bigint_test_comparison, equality)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(a == b, 1);
}

TEST(bigint_test_comparison, noEquality)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(a != c, 1);
}

TEST(bigint_test_comparison, larger)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(a > c, 1);
}

TEST(bigint_test_comparison, smaller)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(c < a, 1);
}

TEST(bigint_test_comparison, smallerWithNegative)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(d < c, 1);
}

TEST(bigint_test_comparison, smallerWithNegativeAndAbs)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(d < a, 1);
}

TEST(bigint_test_comparison, smallerOrEqually)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(d <= b, 1);
}

TEST(bigint_test_comparison, largerOrEqually)
{
    BigInt a("100");
    BigInt b("100");
    BigInt c("99");
    BigInt d("-100");
    
    EXPECT_EQ(a >= b, 1);
}

///////////////// math_operations /////////////////

TEST(bigint_test_math_operations, plusWithBig)
{
    BigInt a("100");
    BigInt b("100");
    
    EXPECT_EQ(a + b, 200);
}

TEST(bigint_test_math_operations, plusWithInt)
{
    BigInt a("100");
    
    EXPECT_EQ(a + 100, 200);
}

TEST(bigint_test_math_operations, unaryMinus)
{
    BigInt a("100");
    
    EXPECT_EQ(-a, -100);
}

TEST(bigint_test_math_operations, minusWithBig)
{
    BigInt a("100");
    BigInt b("99");
    
    EXPECT_EQ(a - b, 1);
}

TEST(bigint_test_math_operations, minusWithInt)
{
    BigInt a("100");
    
    EXPECT_EQ(a - 98, 2);
}

TEST(bigint_test_math_operations, multiplyWithBig)
{
    BigInt a("100");
    BigInt b("100");
    
    EXPECT_EQ(a * b, 10000);
}

TEST(bigint_test_math_operations, multiplyWithInt)
{
    BigInt a("100");
    
    EXPECT_EQ(a * 2, 200);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}