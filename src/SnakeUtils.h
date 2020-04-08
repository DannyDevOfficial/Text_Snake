/*
 * SnakeUtils.h
 *
 *  Created on: Apr 2, 2020
 *      Author: danielgrieco
 */

#ifndef SNAKEUTILS_H_
#define SNAKEUTILS_H_

// Macro used to enable debugging functionalities in game.
//#define SNAKE_UTILS_IN_GAME_DEBUG

#include <vector>
#include <string>

#include "CursesUtils.h"

namespace TextSnake {

	/*
	 * Defining all constants here.
	 */
	namespace Constants {

		static const char SPR_SNAKE_HEAD = '@';
		static const char SPR_SNAKE_TAIL = '*';
		static const char SPR_APPLE = 'o';

		static const unsigned int DEFAULT_FPS = 6;
		static const unsigned int SNAKE_DEFAULT_SPEED = 1;
		static const CursesUtils::Color DEFAULT_COLOR = CursesUtils::Color::WHITE;
		static const unsigned short TOTAL_LIVES = 3;
		static const char QUIT_BUTTON = 'q';

#ifdef SNAKE_UTILS_IN_GAME_DEBUG
		static const char ADD_SNAKE_PIECE_BUTTON = 'a';
#endif

	} /* namespace Constants */


	///////////////////////////// Data /////////////////////////////

	/*
	 * Enumeration for screens.
	 */
	enum class Screen {
		MAIN_MENU,
		MAIN_GAME,
		GAME_OVER,
		HIGH_SCORES
	};

	/*
	 * Enumeration for game states.
	 */
	enum class State {
		SHOW_MAIN_MENU,
		START_GAME,
		SHOW_MAIN_GAME,
		END_GAME,
		SHOW_GAME_OVER,
		LOAD_HIGH_SCORES,
		SHOW_HIGH_SCORES
	};

	/*
	 * Represents the directions for the snake movement.
	 */
	enum class Direction {
		UP,
		RIGHT,
		DOWN,
		LEFT
	};

	/*
	 * Represents a position (x and y) in 2D space.
	 */
	struct Vector2D {
		int x;
		int y;
	};

	/*
	 * Represents a piece of the snake's tail.
	 */
	struct TailPiece {
		Vector2D currentPosition;
		Vector2D previousPosition;
		Direction currentDirection;
		Direction previousDirection;
		char sprite;
		CursesUtils::Color color;
	};

	/*
	 * Represents the snake.
	 */
	struct Snake {
		Vector2D currentPosition;
		Vector2D previousPosition;
		Direction currentDirection;
		Direction previousDirection;
		unsigned int speed;
		char sprite;
		CursesUtils::Color color;
		std::vector<TailPiece> tail;
	};

	/*
	 * Represents an apple, which is the fruit eaten by the snake to grow bigger.
	 */
	struct Apple {
		Vector2D position;
		char sprite;
		CursesUtils::Color color;
	};

	/*
	 * Represents a score.
	 */
	struct Score {
		unsigned int score;
		std::string name;
	};

	/*
	 * Represents the game e.g. states, scores etc.
	 */
	struct Game {
		unsigned short lives;
		unsigned int applesEaten;
		unsigned int currentScore;
		std::vector<Score> highScores;
		State currentState;
		Screen currentScreen;
	};


	///////////////////////////// Functions /////////////////////////////

	/*
	 * Starts up the game.
	 */
	void Start();

	/*
	 * Initializes the snake's data.
	 * s: snake to initialize.
	 */
	void InitSnake(Snake& s);

	/*
	 * Initializes an apple's data
	 * a: apple to initialize.
	 * p: horizontal and vertical position on the screen.
	 */
	void InitApple(Apple& a, const Vector2D& p);

	/*
	 * Initializes the game's data.
	 * g: game to initialize.
	 */
	void InitGame(Game& g);

	/*
	 * Analyzes the input and act accordingly.
	 * inpt: Input to write to.
	 * g: Instance of the game.
	 * s: Instance of the snake.
	 */
	void HandleInput(int& inpt, const Game& g, Snake& s);

	/*
	 * Tweaks the FPS based on the snake direction and returns it.
	 * snake: Instance of the snake.
	 */
	unsigned int AdjustFPSbasedOnDirection(const Snake& snake);

	/*
	 * Calls all the functions that deal with game updates.
	 * g: Instance of the current game.
	 * s: Instance of the snake.
	 */
	void UpdateGame(Game& g, Snake& s);

	/*
	 * Draws the game to the screen.
	 * g: Instance of the game.
	 * s: Instance of the snake.
	 */
	void DrawGame(const Game& g, const Snake& s);

	/*
	 * Updates the position of every piece of the snake's tail so they're ready for the next frame.
	 * snake: Instance of the snake.
	 */
	void UpdateTailPiecesPosition(Snake& snake);

	/*
	 * Calls the snake movement function picking its x and y coords based on the given direction.
	 * snake: The snake of which to update the position.
	 */
	void TellSnakeToMove(Snake& snake);

	/*
	 * Sets the snake current position based on the given direction.
	 * snake: The snake of which to update the position.
	 * x: The new position on the x axis.
	 * y: The new position on the y axis.
	 */
	void MoveSnake(Snake& snake, const int x, const int y);

	/*
	 * Instantiates a new tail piece at the right position and initializes its values.
	 * s: Instance of the snake.
	 */
	void MakeTailPiece(Snake& s);

	/*
	 * Sets the direction and position of a newly created tail piece.
	 * snake: Instance of the snake.
	 * tailPiece: Newly instantiated tail piece.
	 */
	void SetNewTailPieceDirAndPos(const Snake& snake, TailPiece& tailPiece);

	/*
	 * Draws the snake's head.
	 * snake: Instance of the snake.
	 */
	void DrawHead(const Snake& snake);

	/*
	 * Draws the tail pieces.
	 * snake: Instance of the snake.
	 */
	void DrawTail(const Snake& snake);


} /* namespace TextSnake */

#endif /* SNAKEUTILS_H_ */