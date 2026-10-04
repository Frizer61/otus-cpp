#pragma once

#include <cstddef>
#include <map>
#include <tuple>
#include <utility>

template<typename T, T defaultValue>
class Matrix {
	using Storage = std::map<std::pair<int, int>, T>;
	Storage cells;

public:
	class Cell {
		Matrix& matrix;
		std::pair<int, int> position;

	public:
		Cell(Matrix& matrix, int x, int y) : matrix(matrix), position(x, y) {
		}

		Cell(const Cell&) = default;

		operator T() const {
			auto it = matrix.cells.find(position);
			if (it == matrix.cells.end()) {
				return defaultValue;
			}
			return it->second;
		}

		Cell& operator=(const T& value) {
			if (value == defaultValue) {
				matrix.cells.erase(position);
			} else {
				matrix.cells[position] = value;
			}
			return *this;
		}

		Cell& operator=(const Cell& other) {
			return *this = static_cast<T>(other);
		}
	};

	class Row {
		Matrix& matrix;
		int x;

	public:
		Row(Matrix& matrix, int x) : matrix(matrix), x(x) {
		}

		Cell operator[](int y) {
			return Cell(matrix, x, y);
		}
	};

	class Iterator {
		typename Storage::const_iterator it;

	public:
		Iterator(typename Storage::const_iterator it) : it(it) {
		}

		std::tuple<int, int, T> operator*() const {
			return std::make_tuple(it->first.first, it->first.second, it->second);
		}

		Iterator& operator++() {
			++it;
			return *this;
		}

		bool operator!=(const Iterator& other) const {
			return it != other.it;
		}
	};

	Row operator[](int x) {
		return Row(*this, x);
	}

	std::size_t size() const {
		return cells.size();
	}

	Iterator begin() const {
		return Iterator(cells.begin());
	}

	Iterator end() const {
		return Iterator(cells.end());
	}
};
