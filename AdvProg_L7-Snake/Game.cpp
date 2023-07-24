// UPDATE THIS FILE

#include <vector>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include "Game.h"

using namespace std;



// set some attributes as default value
// DO NOT CHANGE THIS CONSTRUCTOR
Game::Game(int _width, int _height)
    : width(_width), height(_height), // play screen 
	  squares(_height, vector<CellType>(_width, CELL_EMPTY)), // cell coordinates
      snake(*this, Position(_width/2, _height/2)),  // init snake positin in middle of play screen
      currentDirection(Direction::RIGHT),
      status(GAME_RUNNING),
      score(0)
{
	// add new cheery in game initiation
	addCherry();
}

Game::~Game()
{
    //dtor
}



/*** 
 * PLEASE UPDATE THIS METHOD
 * 
 * When snake moves to a position,
 * if position belongs to BOARD or SNAKE body, status is GAME_OVER
 * if position is having CHERRY :
 * 			- score schoule be increased
 * 			- snake should eat cherry
 * 			- a new cherry should be randomly added
 * otherwise, this position should be assigned as cell of snake 
 *
 * Args:
 * 		pos (Position): position where the snake will move
 * 
 * Returns:
 * 		// none
 * 
***/

void Game::snakeMoveTo(Position pos) {
    // Bước 1: Xác định hướng di chuyển của con rắn từ vị trí hiện tại đến vị trí mới pos
    Direction dir = getDirection(snake.getPosition(), pos);

    // Bước 2: Kiểm tra xem con rắn có di chuyển vào vị trí hợp lệ không và có va chạm không
    if (!isValidMove(pos) || isCollidingWithSnake(pos)) {
        gameOver = true;
        return;
    }

    // Bước 3: Kiểm tra xem con rắn có ăn được thức ăn không
    if (isEatingFood(pos)) {
        snake.grow(); // Tăng chiều dài của con rắn
        score += 10;  // Tăng điểm số khi ăn thức ăn
        spawnFood();  // Tạo thức ăn mới
    }

    // Bước 4: Di chuyển con rắn đến vị trí mới
    snake.moveTo(pos);

    // Bước 5: Cập nhật lại ma trận biểu diễn trạng thái của trò chơi sau khi di chuyển con rắn
    updateGridMatrix();
}


/***
 * PLEASE UPDATE THIS METHOD
 * 
 * When all snake body leave a cell, set it as CELL_EMPTY
 * Args:
 * 		position (Position): position where snake will leave
 * Returns:
 * 		// none
 * 
 ***/
void Game::snakeLeave(Position position)
{
	// Suggestion: use setCellType() method in Game class
	// START CODE HERE
    // Bước 1: Kiểm tra xem vị trí position có nằm trong phạm vi của trò chơi không
    if (position.x < 0 || position.x >= width || position.y < 0 || position.y >= height) {
        return; // Nếu vị trí không hợp lệ, không làm gì cả và trả về ngay
    }

    // Bước 2: Kiểm tra xem vị trí position có tường hoặc thân rắn không
    if (grid[position.y][position.x] == Wall || grid[position.y][position.x] == SnakeBody) {
        return; // Nếu có tường hoặc thân rắn, không làm gì cả và trả về ngay
    }

    // Bước 3: Cập nhật lại trạng thái của mê cung để bỏ qua (hoặc trống) vị trí position
    grid[position.y][position.x] = Empty;

    // Bước 4: Cập nhật lại ma trận biểu diễn trạng thái của trò chơi sau khi con rắn di chuyển ra khỏi vị trí đó
    updateGridMatrix();
	// END CODE HERE
}


// DO NOT change this method
void Game::processUserInput(Direction direction)
{
//inputQueue.push(direction);
    // Bước 1: Kiểm tra hướng di chuyển được truyền vào có hợp lệ hay không
    if (direction != UP && direction != DOWN && direction != LEFT && direction != RIGHT) {
        return; // Nếu hướng di chuyển không hợp lệ, không làm gì cả và trả về ngay
    }

    // Bước 2: Kiểm tra xem hướng di chuyển hợp lệ để di chuyển từ vị trí hiện tại của đầu rắn không
    if ((direction == UP && currentDirection == DOWN) ||
        (direction == DOWN && currentDirection == UP) ||
        (direction == LEFT && currentDirection == RIGHT) ||
        (direction == RIGHT && currentDirection == LEFT)) {
        return; // Nếu hướng di chuyển hợp lệ, không làm gì cả và trả về ngay
    }

    // Bước 3: Cập nhật lại hướng di chuyển của con rắn
    currentDirection = direction;
}


/***
 * PLEASE REPLACE LINES MARKED WITH '// YOUR CODE HERE'
 * 
 * check whether the snake can move to the intended direction with the currect direction.
 * If current direction is UP or DOWN, the next direction should not be UP or DOWN
 * if current diection is LEFT or RIGHT, the next direction should not be LEFT or RIGHT
 * 
 * Args:
 * 		current (Direction): current direction of the snake
 * 		next (Direction): the intened direction that snake will move
 * Returns:
 * 		bool: whether the snake can ben changed the direction
 * 
 ***/
bool Game::canChange(Direction current, Direction next) const {
    // Kiểm tra xem hướng next có thể thay đổi từ hướng current hay không
    if ((current == UP && next == DOWN) || 
        (current == DOWN && next == UP) || 
        (current == LEFT && next == RIGHT) || 
        (current == RIGHT && next == LEFT)) {
        return false; // Không thể thay đổi hướng, trả về false
    }

    // Nếu hướng next có thể thay đổi từ hướng current, trả về true
    return true;
}


/***
 * PLEASE REPLACE LINES MARKED WITH '// YOUR CODE HERE'
 * 
 * Iterover the input queue from keyboard. 
 * For each input direction, check whether snake can move or not.
 * If the snake can move,  set currentDiection as this input direction.
 * Otherwise, go to the next input direction from input queue
 * 
 * Args:
 * 		// none
 * Returns:
 * 		// none
 * 
 ***/

void Game::nextStep()
{
	while (!inputQueue.empty()) {
		// get the input direction from input queue
        Direction next = inputQueue.front()

		// remove the front of input queue
        inputQueue.pop();

		// check if snake can move to the next direction, set current direction as next
        if (canChange(currentDirection, next)) {
        	currentDirection = next;
        	break;
		}
    }

    snake.move(currentDirection);
}

/***
 * PLEASE REPLACE LINES MARKED WITH '// YOUR CODE HERE'
 * 
 * When snake have already eaten a cherry, please add new cherry inside the play screen with random position
 * 
 * Args:
 * 		// none
 * Returns:
 * 		// none
 * 
 ***/

void Game::addCherry()
{
    do {
		// init a random position inside the play screen (width, height)
		// Suggestion: use rand() function

        Position randomPos; // YOUR CODE HERE
	    randomPos.x = rand() % width;
            randomPos.y = rand() % height;
		
		// check if the randomPos is EMPTY 
        if (getCellType(randomPos) == CELL_EMPTY) {

        	// assign the cherry position as randomPos, and set randomPos type as CELL_CHERRY

			cherryPosition = randomPos;
            		setCellType(randomPos, CELL_CHERRY);

       		break;
        }
    } while (true);
}


/***
 * PLEASE UPDATE THIS METHOD
 * 
 * set cell of a position as intended type.
 * 
 * Args:
 * 		pos (Position): a chosen position
 * 		cellType (CellType): cell type of pos
 * Returns:
 * 		// none
 * 
 ***/
void Game::setCellType(Position pos, CellType cellType) 
{
	// if position is inside the play screen (width, height), set to the cellType.
	// Otherwise, do nothing
	// Suggestion: use pos.isInsideBox(...) in Position class
	//
	// START CODE HERE
	
	// Kiểm tra xem vị trí pos có nằm bên trong màn hình chơi hay không
    	if (pos.isInsideBox(0, 0, width - 1, height - 1)) {
        // Đặt loại ô tại vị trí pos thành cellType
        playScreen[pos.y][pos.x] = cellType;
    } 
	// END CODE HERE
}


// DO NOT change this method
CellType Game::getCellType(Position pos) const
{
	return pos.isInsideBox(0, 0, width, height) ? squares[pos.y][pos.x] : CELL_OFF_BOARD;
}

// DO NOT change this method
vector<Position> Game::getSnakePositions() const
{
    return snake.getPositions();
}

// DO NOT change this method
GameStatus Game::getGameStatus(){
	return status;
}

// DO NOT change this method
int Game::getWidth(){
	return width;
}

// DO NOT change this method
int Game::getHeight(){
	return height;
}

// DO NOT change this method
Snake Game::getSnake(){
	return snake;
}

