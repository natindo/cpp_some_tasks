#include <iostream>
#include <cassert>
#include <ostream>
#include "matrix.h"

// constexpr int rows = 5;
// constexpr int cols = 3;

// void print(Matrix& m) {
//     for(int i = 0; i < rows; ++i) {
//         for(int j = 0; j < cols; ++j) {
//             std::cout << m[i][j] << "  ";
//         }
//         std::cout << std::endl;
//     }
//     std::cout << std::endl;
//     std::cout << std::endl;
// }

// int main() {
//     Matrix m (rows, cols);
//     assert(m.getCols() == 3);
//     assert(m.getRows() == 5);

//     for(int i = 0; i < rows; ++i) {
//         for(int j = 0; j < cols; ++j) {
//             m[i][j] = i + j;
//         }
//     }
//     double x = m[4][1];
//     std::cout << x << std::endl;

//     m *= 3;
//     Matrix m1 (rows, cols);

//     for(int i = 0; i < rows; ++i) {
//         for(int j = 0; j < cols; ++j) {
//             m1[i][j] = i + j;
//         }
//     }

//     print(m);
//     print(m1);

//     if (m1 == m){
//         std::cout << "true" << std::endl;
//     }

//     Matrix m2 = m1 + m;

//     print(m2);

//     std::cout << m2 << std::endl;

//     return 0;
// }
