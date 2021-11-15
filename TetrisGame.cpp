// This class encapsulates the tetris game and its drawing routines, gameplay, & control logic.
// This class was designed so with the idea of potentially instantiating 2 of them
// and have them run side by side (player vs player).
// So, anything you would need for an individual tetris game has been included here.
// Anything you might use between games (like the background, or the sprite used for 
// rendering a tetromino block) was left in main.cpp
// 
// This class is responsible for:
//   - setting up the board,
//	 - drawing game elements to the screen
//   - spawning tetrominoes,
//   - handling user input,
//   - moving and placing tetrominoes 
//
//  [expected .cpp size: ~ 275 lines]

#include "TetrisGame.h";
#include <assert.h>
#include <string>

TetrisGame::TetrisGame(sf::RenderWindow& window, sf::Sprite& blockSprite, Point gameboardOffset, Point nextShapeOffset)
	:window{ window }, blockSprite{ blockSprite },gameboardOffset{ gameboardOffset },nextShapeOffset{ nextShapeOffset }, score{0}      //I thought I would have to use this here...  //but it seems to work without? why?
{
	reset();
	//board.setContent(5, 5, 5);                                             //could be fun to make a challenge board with the blocks in the middle             

	// setup our font for drawing the score
	if (!scoreFont.loadFromFile("fonts/RedOctober.ttf"))
	{
		assert(false && "Missing font: RedOctober.ttf");
	};
	scoreText.setFont(scoreFont);
	scoreText.setCharacterSize(24);
	scoreText.setFillColor(sf::Color::White);
	scoreText.setPosition(435, 325);
};																																


	// Draw anything to do with the game, 
	//   includes the board, currentShape, nextShape, score
	//   called every game loop
void TetrisGame::draw() {
	drawGameboard();
	drawTetromino(currentShape, gameboardOffset);
	drawTetromino(nextShape, nextShapeOffset);

	drawTetromino(ghostShape, gameboardOffset, 100);
	updateGhostShape();
	window.draw(scoreText);

	//drawGameboard();
}


	// Event and game loop processing
	// handles keypress events (up, left, right, down, space)
void TetrisGame::onKeyPressed(sf::Event event) {                            //would it be more appropriate to use a switch statement here?

	if (event.key.code == sf::Keyboard::Up) {
		attemptRotate(currentShape);
		updateGhostShape();
	}
	else if (event.key.code == sf::Keyboard::Left) {
		attemptMove(currentShape, -1, 0);
		updateGhostShape();
	}
	else if (event.key.code == sf::Keyboard::Right) {
		attemptMove(currentShape, 1, 0);
		updateGhostShape();
	}
	else if (event.key.code == sf::Keyboard::Down) {
		if (!attemptMove(currentShape, 0, 1)) {
			updateGhostShape();
			lock(currentShape);
			shapePlacedSinceLastGameLoop = true;
		};
	}
	else if (event.key.code == sf::Keyboard::Space) {
		drop(currentShape);
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}

	// called every game loop to handle ticks & tetromino placement (locking)
void TetrisGame::processGameLoop(float secondsSinceLastLoop) {                    //as far as i can tell this is working well, might be a bit slow but idk
	 
	secondsSinceLastTick = secondsSinceLastTick + secondsSinceLastLoop;
	//double secsPerTick = MAX_SECONDS_PER_TICK;

	if (secondsSinceLastTick >= secondsPerTick){
		tick();
		secondsSinceLastTick =- secondsPerTick;
	}

	if (shapePlacedSinceLastGameLoop) {
		if (!spawnNextShape()) {
			reset();
			return;
		}

		calculatePoints(board.removeCompletedRows());
		updateScoreDisplay();
		determineSecondsPerTick();                                                     //FIXME: this logic needs to be updated. speed is constant atm
	}

	shapePlacedSinceLastGameLoop = false;
}


//This function calculates the bonus points for removing more than one line
//with a tetromino block.
//1 line removed = 1 points
//2 lines removed = 3 points
//3 lines removed = 5 points
//4 lines removed =  8 points
//the white font is updated in processGameLoop

void TetrisGame::calculatePoints(int rowsRemovedAtOnce) {           //Yes.. these are magic numbers. We also assum that the max bloock size will always be four
																	//If you wanted to be fussy, you could do some math on blocklocks.length() or something but this is good enough for now :)
	if (rowsRemovedAtOnce == 0)
		return;

	if (rowsRemovedAtOnce == 1) {
		score = score + 1;
	}
	else if (rowsRemovedAtOnce == 2) {
		score = score + 3;
	}
	else if (rowsRemovedAtOnce == 3) {
		score = score + 5;
	}
	else if (rowsRemovedAtOnce == 4) {
		score = score + 8;
	}

}



	// A tick() forces the currentShape to move (if there were no tick,
	// the currentShape would float in position forever). This should
	// call attemptMove() on the currentShape.  If not successful, lock() 
	// the currentShape (it can move no further), and record the fact that a
	// shape was placed (using shapePlacedSinceLastGameLoop)
void TetrisGame::tick() {

	//std::cout << "TICK TOK \n";

	if (!attemptMove(currentShape, 0, 1)) {
		updateGhostShape();
		lock(currentShape);
		shapePlacedSinceLastGameLoop = true;
	}
}


	// reset everything for a new game (use existing functions) 
	//  - set the score to 0 and call updateScoreDisplay()
	//  - call determineSecondsPerTick() to determine the tick rate.
	//  - clear the gameboard,
	//  - pick & spawn next shape
	//  - pick next shape again (for the "on-deck" shape)
void TetrisGame::reset() {

	score = 0;
	updateScoreDisplay();

	determineSecondsPerTick();

	board.empty();

	currentShape.setShape(Tetromino::getRandomShape());
	currentShape.setGridLoc(board.getSpawnLoc());

	spawnNextShape();
}



void TetrisGame::updateGhostShape() {
	ghostShape = currentShape;

	drop(ghostShape);
}



	// assign nextShape.setShape a new random shape  
void TetrisGame::pickNextShape() {

	nextShape.setShape(Tetromino::getRandomShape());
	//nextShape.setGridLoc(nextShapeOffset);
}

	// copy the nextShape into the currentShape (through assignment)
	//   position the currentShape to its spawn location.
	//	 - return true/false based on isPositionLegal()
bool TetrisGame::spawnNextShape() {                                                        //FIXME: this might be spawning next tet in wrong place 
	currentShape = nextShape;
	currentShape.setGridLoc(board.getSpawnLoc());

	pickNextShape();
	return isPositionLegal(currentShape);
}

	// Test if a rotation is legal on the tetromino and if so, rotate it. 
	//  To do this:
	//	 1) create a (local) temporary copy of the tetromino
	//	 2) rotate it (shape.rotateClockwise())
	//	 3) test if temp rotation was legal (isPositionLegal()), 
	//      if so - rotate the original tetromino.
	//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptRotate(GridTetromino& shape) {
	GridTetromino localShape;
	   
	localShape = shape; //copy the shape of the tet passed in 
	localShape.rotateClockwise();  //if legal, rotate the orig tet
	
	if (isPositionLegal(localShape)) {
		shape.rotateClockwise();
		return true;
	}

}

	// test if a move is legal on the tetromino, if so, move it.
	//  To do this:
	//	 1) create a (local) temporary copy of the tetromino
	//	 2) move it (temp.move())
	//	 3) test if temp move was legal (isPositionLegal(),
	//      if so - move the original.
	//	 4) return true/false to indicate successful movement
bool TetrisGame::attemptMove(GridTetromino& shape, int x, int y) {

	GridTetromino localShape;
	localShape = shape;									//copy the shape of the tet passed in 											
	localShape.move(x, y);

	if (isPositionLegal(localShape)) {
		shape.move(x, y);
		return true;
	}
	return false;
	
}

	// drops the tetromino vertically as far as it can 
	//   legally go.  Use attemptMove(). This can be done in 1 line.
void TetrisGame::drop(GridTetromino& shape) {

	while (attemptMove(shape, 0, 1)){}

}

	// copy the contents (color) of the tetromino's mapped block locs to the grid.
	//	 1) get current blockshape locs via tetromino.getBlockLocsMappedToGrid()
	//	 2) copy the content (color) to the grid (via gameboard.setContent()) 
void TetrisGame::lock(const GridTetromino& shape) {

	board.setContent(shape.getBlockLocsMappedToGrid(), static_cast<int>(shape.getColor()));
}

	// Graphics methods ==============================================

	// Draw a tetris block sprite on the canvas		
	// The block position is specified in terms of 2 offsets: 
	//    1) the top left (of the gameboard in pixels)
	// 
	//    2) an x & y offset into the gameboard - in blocks (not pixels)
	//       meaning they need to be multiplied by BLOCK_WIDTH and BLOCK_HEIGHT
	//       to get the pixel offset.
	// 
	// 
	//	 1) set the block color using blockSprite.setTextureRect()
	//   2) set the block location using blockSprite.setPosition()   
	//	 3) draw the block using window.draw()
	//   For details/instructions on these 3 operations see:
	//       www.sfml-dev.org/tutorials/2.5/graphics-sprite.php
	//       use member variables: window and blockSprite (assigned in constructor)
void TetrisGame::drawBlock(const Point& topLeft, int xOffset, int yOffset, TetColor color, int alpha) {

	int pixelOffset = BLOCK_WIDTH * static_cast<int>(color);

	blockSprite.setTextureRect(sf::IntRect(pixelOffset, 0, BLOCK_WIDTH, BLOCK_HEIGHT)); //block 32 px wide - sets the color of the block 

	blockSprite.setPosition(sf::Vector2f(topLeft.getX() + (xOffset * BLOCK_WIDTH), topLeft.getY()+ (yOffset * BLOCK_HEIGHT)));

	blockSprite.setColor(sf::Color(255, 255, 255, alpha));

	window.draw(blockSprite);
}

	// Draw the gameboard blocks on the window
	//   Iterate through each row & col, use drawBlock() to 
	//   draw a block if it isn't empty.
void TetrisGame::drawGameboard() {
	for (int y = 0; y < board.MAX_Y; y++) {             //This loops on the rows.
		for (int x = 0; x < board.MAX_X; x++) {         //This loops on the columns. we want a board tall and skinny
			if (board.getContent(x,y) != board.EMPTY_BLOCK) {
				drawBlock(Point{ gameboardOffset.getX(),gameboardOffset.getY() }, x, y, static_cast<TetColor>(board.getContent(x, y)));
			}
		}

	}
}

	// Draw a tetromino on the window
	//	 Iterate through each mapped loc & drawBlock() for each.
	//   The topLeft determines a 'base point' from which to calculate block offsets
	//      If the Tetromino is on the gameboard: use gameboardOffset
void TetrisGame::drawTetromino(const GridTetromino& tetromino, const Point& topLeft, int alpha) {
	std::vector<Point> currLocs = tetromino.getBlockLocsMappedToGrid();

	for (const auto& loc : currLocs) {          //top left is currently gameboard offset 
		//Point spawn = board.getSpawnLoc();																	//spawn Loc should be 5,0 for now, but it needs to get fixed

		drawBlock(topLeft, loc.getX(),  loc.getY(), tetromino.getColor(), alpha);  //move the tetromino points to the middle by offsetting by spawn loc
	}													//am I supposed to be calling another function here to do the moving?? Work with grid loc?
}

	// update the score display
	// form a string "score: ##" to display the current score
	// user scoreText.setString() to display it.
void TetrisGame::updateScoreDisplay() {
	
	
	std::string curScoreStr = std::to_string(score);
	std::string scoreTxt = "Score is: " + curScoreStr;

	scoreText.setString(scoreTxt);

}

	// State & gameplay/logic methods ================================



	// return true if shape is within borders (isShapeWithinBorders())
	//	 and the shape's mapped board locs are empty.
	//   Make use of Gameboard's areLocsEmpty() and pass it the shape's mapped locs.
bool TetrisGame::isPositionLegal(const GridTetromino& shape) const {

	return (isShapeWithinBorders(shape) && board.areLocsEmpty(shape.getBlockLocsMappedToGrid()));
	
}



	// return true if the shape is within the left, right, and lower border of
	//	 the grid, but *NOT* the top border. (return false otherwise)
	//   * Ignore the upper border because we want shapes to be able to drop
	//     in from the top of the gameboard.
	//   All of a shape's blocks must be inside these 3 borders to return true
bool TetrisGame::isShapeWithinBorders(const GridTetromino& shape) const {

	std::vector<Point> mappedLocs = shape.getBlockLocsMappedToGrid();

	//std::cout << "mapped locs are: \n";
	for (auto& pnt : mappedLocs) {
		//std::cout << pnt.toString();

		//trying to figure out the prob here
		//int leftBoundry = (board.MAX_X / 2) * -1;
		//int rightBoundry = (board.MAX_X / 2);

		//

		//std::cout << shape.getGridLoc().toString() << " is the grid loc in isShapeWithinBorders \n";
		if (pnt.getX() < 0 || pnt.getX() >= board.MAX_X || pnt.getY() >= board.MAX_Y) {  //I think I have to re write this functionality because is Valid Point checks top border?
			return false;   //This current shape is only getting the BASIC block loc
		}
	}
	return true;
}


	// set secsPerTick 
	//   - basic: use MAX_SECS_PER_TICK
	//   - advanced: base it on score (higher score results in lower secsPerTick)
void TetrisGame::determineSecondsPerTick() {
	secondsPerTick = MAX_SECONDS_PER_TICK;
	
													//FIXME: ADD in more complex logic for this 
	
	if (score > 0) {
		//double speed{ 0 };
		int speedFactor = score / 5;     
		double speed = (speedFactor * .30);     //for every 5 points, increase the speed of tick by .1
		secondsPerTick = MAX_SECONDS_PER_TICK - speed;

		if (secondsPerTick < MIN_SECONDS_PER_TICK)
			secondsPerTick = MIN_SECONDS_PER_TICK;
	}

	

	
}


