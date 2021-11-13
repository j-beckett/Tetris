#pragma once
#include <vector>
#include "point.h"
#include <time.h>
//#include "TestSuite.h"

enum class TetColor {
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE_LIGHT,
	BLUE_DARK,
	PURPLE
};

enum class TetShape {
	S,
	Z,
	L,
	J,
	O,
	I,
	T,
	count
};


class Tetromino {
	friend class TestSuite;
	friend class GridTetromino;   //					do I need the whole class as a friend class? Or just the one friend function?

private:
	TetColor color;
	TetShape shape;
protected:
	std::vector<Point> blockLocs;

public:
	Tetromino() { //tell it to set to something 
		
		//setShape(TetShape::O);
		setShape(getRandomShape()); //choose a random shape to generate. Random from 0 to count of TetShapes (7). random num chosen is then set to a TetShape (enum)
	}

	TetColor getColor()const {
		return color;
	}

	TetShape getShape() const{
		return shape;
	}

	static TetShape getRandomShape();

	void setShape(TetShape shape);

	void rotateClockwise();


	void printToConsole() const;



};


