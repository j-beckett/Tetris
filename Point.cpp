#include "point.h"
#include <sstream>
#include "TestSuite.h"

void Point::setX(int usrX) {
	x = usrX;
}

void  Point::setY(int usrY) {
	y = usrY;
}

void  Point::setXY(int usrX, int usrY) {
	x = usrX;
	y = usrY;
}

//swaps the coordinates of x and y
void  Point::swapXY() {
	int temp{ x };
	x = y;
	y = temp;
}

// multiply x by some factor
void  Point::multiplyX(int factor) {
	setX (x * factor);

}

// multiply y by some factor
void  Point::multiplyY(int factor) {
	setY(y * factor);
}


// return a string in the form "[x,y]" to represent the state of the Point instance
// (for debugging) You could use stringstream if you want to try a forming a string in
// the same way you use cout http://stev.org/post/cstringstreamexample for an example
std::string  Point::toString() const {
	std::stringstream os;
	os << "[" << x << " , " << y << "]\n";
	std::cout << os.str();

	return os.str();
}
