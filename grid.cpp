#include "grid.h"
#include "simple-grid.h"

bool Cell::link(Cell *cell, bool biDir) {
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

bool Cell::unlink(Cell *cell) {
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

Cell *AbstractGrid::getCell(uint row, uint col) const {
	if (row < 0 || row >= _rows || col < 0 || col >= _cols)
		return nullptr;
	return &_grid[row * _rows + col];
}

Cell *AbstractGrid::pickRandom() {
	if (_grid == nullptr)
		return nullptr;
	uint row = _rnd() % _rows;
	uint col = _rnd() % _cols;
	return getCell(row, col);
}

cv::Mat AbstractGrid::saveImage(const cv::String& filename) {
	cv::Mat image((int)imageWidth(), (int)imageHeight(), CV_8UC3);

	cv::Vec3b white(255, 255, 255);
	cv::Vec3b black(0, 0, 0);

	for (int i = 0; i < kWallThickness; i++) {
		for (int j = 0; j < image.cols; j++) {
			image.at<cv::Vec3b>(cv::Point(j, i)) = white;
		}
	}

	int row, col;
	int drawRow, drawCol;

	for (int i = 0; i < (int)(imageHeight() - kWallThickness); i++) {
		row = i / kShift;
		drawRow = i + kWallThickness;
		
		for (int j = 0; j < kWallThickness; j++) {
			image.at<cv::Vec3b>(cv::Point(j, drawRow)) = white;
		}

		for (int j = 0; j < (int)(imageWidth() - kWallThickness); j++) {
			col = j / kShift;
			drawCol = j + kWallThickness;

			if (i % kShift < kPassageThickness) {
				if (j % kShift < kPassageThickness)
					image.at<cv::Vec3b>(cv::Point(drawCol, drawRow)) = black;
				else if (getCell(row, col)->_eastLinked)
					image.at<cv::Vec3b>(cv::Point(drawCol, drawRow)) = black;
				else
					image.at<cv::Vec3b>(cv::Point(drawCol, drawRow)) = white;

			} else {
				if (j % kShift >= kPassageThickness)
					image.at<cv::Vec3b>(cv::Point(drawCol, drawRow)) = white;
				else if (getCell(row, col)->_southLinked)
					image.at<cv::Vec3b>(cv::Point(drawCol, drawRow)) = black;
				else
					image.at<cv::Vec3b>(cv::Point(drawCol, drawRow)) = white;
			}
		}

	}

	cv::imwrite(filename, image);

	return image;
}

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
