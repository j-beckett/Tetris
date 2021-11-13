#pragma once
#include "Tetromino.h"
#include "point.h"
//#include "TestSuite.h"

// set the shape
// - clear any blockLocs set previously
// - set the blockLocs for the shape
// - set the color for the shape
void Tetromino::setShape(TetShape shape) {

	blockLocs.clear();

	switch (shape) {

	case TetShape::S:
		color = TetColor::RED;
		blockLocs = { Point(-1, 0), Point(0, 0),Point(0,1), Point(1,1) };
		break;
	case TetShape::Z:
		color = TetColor::ORANGE;
		blockLocs = {Point(-1,1), Point(0,1), Point(0,0), Point(1,0) };
		break;

	case TetShape::L:
		color = TetColor::YELLOW;
		blockLocs = {Point(0,1), Point(0,0), Point(0,-1), Point(1,-1) };
		break;

	case TetShape::J:
		color = TetColor::GREEN;
		blockLocs = {Point(0,1), Point(0,0), Point(0,-1), Point(-1,-1) };
		break;

	case TetShape::O:
		color = TetColor::BLUE_LIGHT;
		blockLocs = {Point(0,0), Point(0,1), Point(1,1), Point(1,0) };
		break;

	case TetShape::I:
		color = TetColor::BLUE_DARK;
		blockLocs = { Point(0,-1), Point(0,0), Point(0,1), Point(0,2)};
		break;

	case TetShape::T:
		color = TetColor::PURPLE;
		blockLocs = { Point(0,0), Point(0,-1), Point(-1,0), Point(1,0) };
		break;
	}

}


// rotate the shape 90 degrees around [0,0] (clockwise)
// to do this:
// - iterate through blockLocs
// - rotate each Point 90 degrees around [0,0]
// hint: rotate the point [1,2] clockwise around [0,0] and note
// how the x,y values change. There are 2 functions in the Point
// class that can be used to accomplish a rotation.
void Tetromino::rotateClockwise() {

	/*for (int i = 0; i < blockLocs.size(); i++) {
		blockLocs[i].toString();
	}
	std::cout << "\n";*/

	for (int i = 0; i < blockLocs.size(); i++) {
		blockLocs[i].swapXY();

		if (blockLocs[i].getY() != 0)
			blockLocs[i].multiplyY(-1);

		//blockLocs[i].toString();
	}
	//std::cout << "\n";

}


// print a grid to display the current shape
// to do this: print out a “grid” of text to represent a co-ordinate
// system. Start at top left [-3,3] go to bottom right [3,-3]
// (use nested for loops)
// for each [x,y] point, loop through the blockLocs and if the point exists
// in the list, print an 'x' instead of a '.'. You should end up with something
// like this: (results will vary depending on shape and rotation, eg: this
// one shows a T shape rotated clockwise once)
// .......
// .......
// ...x...
// ..xx...
// ...x...
// .......
void Tetromino::printToConsole() const {

	int y = 3;
	int x = -3;
	
	for (int i = 1; i < 50; i++) {     
		bool wrote = false;
		for (int j = 0; j < blockLocs.size(); j++) {
			//std::cout << " x is " << x << " and y is " << y;
			if (blockLocs[j].getX() == x && blockLocs[j].getY() == y) {
				std::cout << " x ";
				wrote = true;
				break;
			}
		}
			if (!wrote)             //So i don't double write for one interation of the for loop
				std::cout << " . ";

			x++;
			if (i % 7 == 0) {       //start a new line of dots to print
				std::cout << "\n";
				y--;
				x = -3;
			}
		
	}

}

TetShape Tetromino::getRandomShape() {

	return static_cast<TetShape>(rand() % static_cast <int>(TetShape::count));
}