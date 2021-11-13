#pragma once
#include <iostream>

class Point {
	friend class TestSuite;
private:
	int x;
	int y;

public: 

	Point()  : Point{0, 0}
	{}

	Point(int x, int y) {
		setXY(x,y);
	}

	int getX() const{
		return x;
	};

	int getY() const{
		return y;
	}

	void setX(int usrX);

	void setY(int usrY);

	void setXY(int usrX, int usrY);

	void swapXY();

	// multiply x by some factor
	void multiplyX(int factor);

	// multiply y by some factor
	void multiplyY(int factor);


	// return a string in the form "[x,y]" to represent the state of the Point instance
	// (for debugging) You could use stringstream if you want to try a forming a string in
	// the same way you use cout http://stev.org/post/cstringstreamexample for an example
	std::string toString() const;



};