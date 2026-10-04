#include "matrix.h"

#include <iostream>
#include <tuple>

int main() {
	Matrix<int, 0> matrix;
	for (int i = 0; i < 10; ++i) {
		matrix[i][i] = i;
		matrix[i][9 - i] = 9 - i;
	}

	for (int x = 1; x <= 8; ++x) {
		for (int y = 1; y <= 8; ++y) {
			if (y > 1) {
				std::cout << ' ';
			}
			std::cout << matrix[x][y];
		}
		std::cout << '\n';
	}

	std::cout << matrix.size() << '\n';
	for (auto cell : matrix) {
		int x;
		int y;
		int value;
		std::tie(x, y, value) = cell;
		std::cout << x << ' ' << y << ' ' << value << '\n';
	}

	return 0;
}
