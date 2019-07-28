#include "simple-grid.h"
#include <opencv2/highgui.hpp>

int main() {
	SimpleGrid x(4, 4);

	Cell *ptr = x.getCell(1, 1);

	ptr->link(ptr->_north);

	std::cout << x;

	cv::namedWindow("Display");
	cv::imshow("Display", x.saveImage("output.png"));

	cv::waitKey(0);

	return 0;
}