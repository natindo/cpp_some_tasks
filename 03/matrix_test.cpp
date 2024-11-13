#include "matrix.h"
#include <gtest/gtest.h>

TEST(matrix_test, simpleTest) {
    Matrix m(5, 3);
    ASSERT_EQ(m.getCols(), 3);
    ASSERT_EQ(m.getRows(), 5);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = i + j;
        }
    }
    double x = m[4][1];
    ASSERT_EQ(x, 5);
    Matrix m1(5, 3);
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            m1[i][j] = i + j;
        }
    }
    std::stringstream ss;
    ss << m;
    std::string expected = "0  1  2  \n1  2  3  \n2  3  4  \n3  4  5  \n4  5  6  \n";
    ASSERT_EQ(ss.str(), expected);
    ASSERT_TRUE(m1 == m);
    Matrix m2 = m1 + m;
    std::stringstream ss1;
    ss1 << m2;
    std::string expected1 = "0  2  4  \n2  4  6  \n4  6  8  \n6  8  10  \n8  10  12  \n";
    ASSERT_EQ(ss1.str(), expected1);
}

int main(int argc, char** argv) {
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}