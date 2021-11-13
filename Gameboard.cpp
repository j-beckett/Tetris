#pragma once
#include <vector>
#include "Point.h"
#include "Gameboard.h"
#include <assert.h>
#include < iomanip >

Point Gameboard::getSpawnLoc() {
	return spawnLoc;
}

	// return the content at a given point (assert the point is valid)
int Gameboard::getContent(const Point& pt) const {
	assert(isValidPoint(pt));

	return grid[pt.getY()][pt.getX()];
}
	// return the content at an x,y grid loc (assert the point is valid)
int Gameboard::getContent(int x, int y) const {
	assert(isValidPoint(x, y));
		
	return grid[y][x];                                                    
}

	// set the content at a given point (only if the point is valid)
void Gameboard::setContent(const Point& pt, int content) {
	assert(isValidPoint(pt));

	setContent(pt.getX(), pt.getY(), content);                              // keep it as x and y because set content below flips them?
}
	// set the content at an x,y position (only if the point is valid)
void Gameboard::setContent(int x, int y, int content) {                      //because I check the content before, do i need to check it again here?
	assert(isValidPoint(x, y));

	grid[y][x] = content;
}
	// set the content for a set of points (only if the points are valid)
void Gameboard::setContent(const std::vector<Point>& locs, int content) {
	
	for (const Point& pt : locs) {
		if (isValidPoint(pt)) {
			setContent(pt, content);
		}
	}
}

	// return true if the content at ALL (valid) points is empty
	//   *** IMPORTANT NOTE: invalid x,y values can be passed to this method.
	//   Invalid meaning outside the bounds of the grid.
	//   * ONLY TEST VALID POINTS (disregard the others - and ensure you
	//   don't use them to index into the grid).  Use isValidPoint() 
	//   Testing invalid points would likely result in an out of bounds
	//     error or segmentation fault!
																				//   If none of the points are valid, return true WHY ???
bool Gameboard::areLocsEmpty(std::vector<Point> locs) const {
	

	for (const Point pt :locs) {
		if (isValidPoint(pt)) {
			if (getContent(pt) != EMPTY_BLOCK) {
				return false;
			}
		}
	}

	return true;
}

	// removes all completed rows from the board
	//   use getCompletedRowIndices() and removeRows() 
	//   return the # of completed rows removed
int Gameboard::removeCompletedRows() {

	std::vector<int> rowsComplete{ getCompletedRowIndices() };
	removeRows(rowsComplete);
	return rowsComplete.size();
}

	// fill the board with EMPTY_BLOCK 
	//   (iterate through each rowIndex and fillRow() with EMPTY_BLOCK))
void Gameboard::empty() {
	for (int y = 0; y < MAX_Y; y++) {             //This loops on the rows.
		for (int x = 0; x < MAX_X; x++) {         //This loops on the columns. we want a board tall and skinny
			grid[y][x] = EMPTY_BLOCK;
		}
	}
}

	// print the grid contents to the console (for debugging purposes)
	//   use std::setw(2) to space the contents out (#include <iomanip>).
void Gameboard::printToConsole() const {

	for (int y = 0; y < MAX_Y; y++) {
		std::cout << "\n";//This loops on the rows.
		for (int x = 0; x < MAX_X; x++) {         //This loops on the columns
			std::cout<< std::setw(2)<<Gameboard::grid[y][x] << " ";
		}
	}
	 
}


																			//PRIVATE FUNCTIONS START HERE....... is declaring them private in the .h enough? //

// return a bool indicating if a given row is full (no EMPTY_BLOCK in the row)
bool Gameboard::isRowCompleted(int rowIndex) const {
	assert((rowIndex >= 0) && (rowIndex < MAX_Y));                                 //check the row passed is valid

	for (auto& col : grid[rowIndex]) {
		if (col == EMPTY_BLOCK)
			return false;
	}

	return true;
}

// scan the board for completed rows.
//   Iterate through grid rows and use isRowCompleted(rowIndex)
//   return a vector of completed row indices.
std::vector<int> Gameboard::getCompletedRowIndices() const {
	std::vector<int> fullRows{};

	for (int i = 0; i < MAX_Y; i++) {
		if (isRowCompleted(i)) {
			fullRows.push_back(i);
			//std::cout << "ROW COMPLETE: " << i << "\n";
		}
	
	}

	return fullRows;
}

// In gameplay, when a full row is completed (filled with content)
// it gets "removed".  To be exact, the row itself is not removed
// but the content from the row above it is copied into it.
// This continues all the way up the grid until the first row
// is copied into the second row.  Finally, the first row is 
// filled with EMPTY_BLOCK
// given a row index:
//   1) Starting at rowIndex, copy each row above the removed
//     row "one-row-downwards" in the grid.
//     (loop from y=rowIndex down to 0, and copyRowIntoRow(y-1, y)).
//   2) call fillRow() on the first row (and place EMPTY_BLOCKs in it).
void Gameboard::removeRow(int rowIndex) {
	assert(rowIndex >= 0 && rowIndex < MAX_Y && "Row index out of bounds");
	while (rowIndex > 0) {
			copyRowIntoRow(rowIndex - 1, rowIndex);                          
		rowIndex--;
	}
	fillRow(0, EMPTY_BLOCK);            //fill the top row with the -1
}

// given a vector of row indices, remove them 
//   (iterate through the vector and and call removeRow()
//   on each row index). 
void Gameboard::removeRows(const std::vector<int>& rowIndices) {

	for (auto& rowIndex : rowIndices) {
		removeRow(rowIndex);
	}
}

// fill a given grid row with specified content
void Gameboard::fillRow(int rowIndex, int content) {
	
	for (int i = 0; i < MAX_X; i++) {
		setContent(i, rowIndex, content);
	}
}

// copy a source row's contents into a target row.
void Gameboard::copyRowIntoRow(int sourceRowIndex, int targetRowIndex) {

	for (int i = 0; i < MAX_X; i++) {
		grid[targetRowIndex][i]= grid[sourceRowIndex][i];
	}
}

// return true if the point is on the grid, false otherwise
bool Gameboard::isValidPoint(const Point& p) const {

	return ((p.getX() < MAX_X) && (p.getY() < MAX_Y) && (p.getX() >= 0) ) && (p.getY() >=0 );
}

// return true if the x,y location is on the grid, false otherwise
bool Gameboard::isValidPoint(int x, int y) const {

	return (x < MAX_X) && (y < MAX_Y) && (x >=0) && (y >= 0);
}


