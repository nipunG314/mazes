#include <random>
#include <iostream>

typedef  unsigned int uint;

class Cell {
public:

	Cell() : _row(0), _col(0),
		_north(nullptr), _east(nullptr), _south(nullptr), _west(nullptr),
		_northLinked(false), _eastLinked(false), _southLinked(false), _westLinked(false) {}

	Cell( uint row,  uint col) : _row(row), _col(col),
		_north(nullptr), _east(nullptr), _south(nullptr), _west(nullptr),
		_northLinked(false), _eastLinked(false), _southLinked(false), _westLinked(false) {}

	bool link(Cell *cell, bool biDir = true) {
		if (cell == _north) {
			_northLinked = true;
			if (biDir)
				cell->_southLinked = true;
			return true;
		}
		if (cell == _south) {
			_southLinked = true;
			if (biDir)
				cell->_northLinked = true;
			return true;
		}
		if (cell == _east) {
			_eastLinked = true;
			if (biDir)
				cell->_westLinked = true;
			return true;
		}
		if (cell == _west) {
			_westLinked = true;
			if (biDir)
				cell->_eastLinked = true;
			return true;
		}
		return false;
	}

	bool unlink(Cell *cell) {
		if (cell == _north) {
			_northLinked = false;
			cell->_southLinked = false;
			return true;
		}
		if (cell == _south) {
			_southLinked = false;
			cell->_northLinked = false;
			return true;
		}
		if (cell == _east) {
			_eastLinked = false;
			cell->_westLinked = false;
			return true;
		}
		if (cell == _west) {
			_westLinked = false;
			cell->_eastLinked = false;
			return true;
		}
		return false;
	}

	bool linked(Cell *cell) {
		return (_northLinked || _southLinked || _eastLinked || _westLinked);
	}

	 uint _row, _col;
	Cell *_north, *_east, *_south, *_west;
	bool _northLinked, _eastLinked, _southLinked, _westLinked;
};

class AbstractGrid {
public:

	AbstractGrid() : _rows(0), _cols(0), _grid(nullptr) {}

	AbstractGrid( uint rows,  uint cols) : _rows(rows), _cols(cols) {
		_grid = new Cell[_rows * _cols];
		_rnd = std::mt19937(size());
	}

	~AbstractGrid() {
		if (_grid != nullptr) {
			delete _grid;
			_grid = nullptr;
		}
	}

	virtual void initGrid() = 0;
	virtual void configureCells() = 0;
	friend std::ostream& operator<<(std::ostream& out, const AbstractGrid& grid);

	Cell *getCell( uint row,  uint col) const {
		if (row < 0 || row >= _rows || col < 0 || col >= _cols)
			return nullptr;
		return &_grid[row * _rows + col];
	}

	Cell *pickRandom() {
		if (_grid == nullptr)
			return nullptr;
		 uint row = _rnd() % _rows;
		 uint col = _rnd() % _cols;
		return getCell(row, col);
	}

	 uint size() { return _rows * _cols; }

	 uint _rows, _cols;
	
private:
	Cell *_grid;
	std::mt19937 _rnd;
};

std::ostream& operator<<(std::ostream& out, const AbstractGrid& grid) {
	out << "+";
	for (uint k = 0; k < grid._cols; k++)
		out << "---+";
	out << "\n";

	for (uint i = 0; i < grid._rows; i++) {
		out << "|";
		for (uint j = 0; j < grid._cols; j++) {
			out << "   ";
			if (grid.getCell(i, j)->_eastLinked)
				out << " ";
			else
				out << "|";
		}
		out << "\n";
		out << "+";
		for (uint j = 0; j < grid._cols; j++) {
			if (grid.getCell(i, j)->_southLinked)
				out << "   ";
			else
				out << "---";
			out << "+";
		}
		out << "\n";
	}

	return out;
}

class SimpleGrid : public AbstractGrid {
public:
	SimpleGrid() : AbstractGrid() {}

	SimpleGrid(uint _rows, uint _cols) : AbstractGrid(_rows, _cols) {
		initGrid();
		configureCells();
	}

private:
	void initGrid() {
		Cell *tmp;
		for (uint i = 0; i < _rows; i++) {
			for (uint j = 0; j < _cols; j++) {
				tmp = getCell(i, j);
				tmp->_row = i;
				tmp->_col = j;
			}
		}
	}

	void configureCells() {
		Cell *tmp;
		
		// Top Left
		tmp = getCell(0, 0);
		tmp->_east = getCell(0, 1);
		tmp->_south = getCell(1, 0);

		// Bottom Left
		tmp = getCell(_rows - 1, 0);
		tmp->_east = getCell(_rows - 1, 1);
		tmp->_north = getCell(_rows - 2, 0);

		// Bottom Right
		tmp = getCell(_rows - 1, _cols - 1);
		tmp->_west = getCell(_rows - 1, _cols - 2);
		tmp->_north = getCell(_rows - 2, _cols - 1);

		// Top Left
		tmp = getCell(0, _cols - 1);
		tmp->_west = getCell(0, _cols - 2);
		tmp->_south = getCell(1, _cols - 1);

		// Top Middle
		for (uint j = 1; j < _cols - 1; j++) {
			tmp = getCell(0, j);
			tmp->_east = getCell(0, j - 1);
			tmp->_west = getCell(0, j + 1);
			tmp->_south = getCell(1, j);
		}
		
		// Bottom Middle
		for (uint j = 1; j < _cols - 1; j++) {
			tmp = getCell(_rows - 1, j);
			tmp->_east = getCell(_rows - 1, j - 1);
			tmp->_west = getCell(_rows - 1, j + 1);
			tmp->_north = getCell(_rows - 2, j);
		}

		// Left Middle
		for (uint i = 1; i < _rows - 1; i++) {
			tmp = getCell(i, 0);
			tmp->_north = getCell(i - 1, 0);
			tmp->_south = getCell(i + 1, 0);
			tmp->_east = getCell(i, 1);
		}

		// Left Middle
		for (uint i = 1; i < _rows - 1; i++) {
			tmp = getCell(i, _cols - 1);
			tmp->_north = getCell(i - 1, _cols - 1);
			tmp->_south = getCell(i + 1, _cols - 1);
			tmp->_west = getCell(i, _cols - 2);
		}

		// Everything else...
		for (uint i = 1; i < _rows - 1; i++) {
			for (uint j = 1; j < _cols - 1; j++) {
				tmp = getCell(i, j);
				tmp->_north = getCell(i - 1, j);
				tmp->_south = getCell(i + 1, j);
				tmp->_east = getCell(i, j - 1);
				tmp->_west = getCell(i, j + 1);
			}
		}
	}
};

int main() {
	SimpleGrid x(4, 4);

	Cell *ptr = x.getCell(1, 1);

	ptr->link(ptr->_north);

	std::cout << x;

	return 0;
}
