#ifndef SIMPLE_GRID_H
#define SIMPLE_GRID_H

#include "grid.h"

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

#endif // !SIMPLE_GRID_H
