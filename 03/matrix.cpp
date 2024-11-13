#include <iostream>
#include <cassert>
#include <ostream>
#include "main.h"

int main() {

    const int rows = 5;
    const int cols = 3;

    Matrix m (rows, cols);
    assert(m.getCols() == 3);
    assert(m.getRows() == 5);

    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            m[i][j] = i + j;
        }
    }
    double x = m[4][1];
    // std::cout << x << std::endl;

    m *= 3;

    // for(int i = 0; i < rows; ++i) {
    //     for(int j = 0; j < cols; ++j) {
    //         std::cout << m[i][j] << "  ";
    //     }
    //     std::cout << std::endl;
    // }

    Matrix m1 (rows, cols);

    if (m1 == m){

    }

    return 0;
}
