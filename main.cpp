#include "simple-grid.h"
#include <opencv2/highgui.hpp>

void binaryTreeTransform(AbstractGrid *grid) {
	Cell *cell;
	for (uint i = 0; i < grid->_rows; i++) {
		for (uint j = 0; j < grid->_cols; j++) {
			cell = grid->getCell(i, j);
			std::cout << "Row: " << i << " Column: " << j << "\n";
			if (cell->_north != nullptr && cell->_east != nullptr) {
				uint chance = grid->rand(2);
				std::cout << "Chance: " << chance << "\n";
				if (chance)
					cell->link(cell->_north);
				else
					cell->link(cell->_east);
			} else if (cell->_east != nullptr)
				cell->link(cell->_east);
			else if (cell->_north != nullptr)
				cell->link(cell->_north);
		}
	}
	std::cout << "\n";
}

int main() {
	SimpleGrid x1(4, 4);
	SimpleGrid x2(4, 4);
	SimpleGrid x3(4, 4);
	SimpleGrid x4(4, 4);
	binaryTreeTransform(&x1);
	binaryTreeTransform(&x2);
	binaryTreeTransform(&x3);
	binaryTreeTransform(&x4);

	std::cout << x1;

	cv::namedWindow("Display");
	cv::imshow("Display", x1.saveImage("output.png"));

	cv::waitKey(0);

	return 0;
}