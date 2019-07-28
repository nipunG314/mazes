#ifndef GRID_H
#define GRID_H

#include <iostream>
#include <random>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

typedef  unsigned int uint;

enum {
	kWallThickness = 10,
	kPassageThickness = 100,
	kShift = (kWallThickness + kPassageThickness)
};

class Cell {
public:

	Cell() : _row(0), _col(0),
		_north(nullptr), _east(nullptr), _south(nullptr), _west(nullptr),
		_northLinked(false), _eastLinked(false), _southLinked(false), _westLinked(false) {}

	Cell(uint row, uint col) : _row(row), _col(col),
		_north(nullptr), _east(nullptr), _south(nullptr), _west(nullptr),
		_northLinked(false), _eastLinked(false), _southLinked(false), _westLinked(false) {}

	bool link(Cell *cell, bool biDir = true);

	bool unlink(Cell *cell);

	bool linked(Cell *cell) const {
		return (_northLinked || _southLinked || _eastLinked || _westLinked);
	}

	uint _row, _col;
	Cell *_north, *_east, *_south, *_west;
	bool _northLinked, _eastLinked, _southLinked, _westLinked;
};

class AbstractGrid {
public:

	AbstractGrid() : _rows(0), _cols(0), _grid(nullptr) {}

	AbstractGrid(uint rows, uint cols) : _rows(rows), _cols(cols) {
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

	Cell *getCell(uint row, uint col) const;

	Cell *pickRandom();


	cv::Mat saveImage(const cv::String& filename);

	uint size() const { return _rows * _cols; }

	uint imageWidth() const { return _rows * kShift + kWallThickness; }
	uint imageHeight() const { return _cols * kShift + kWallThickness; }

	uint _rows, _cols;

private:
	Cell *_grid;
	std::mt19937 _rnd;
};

#endif // !GRID_H
