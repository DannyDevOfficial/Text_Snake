/*
 * SnakeUtils.h
 *
 *  Created on: Apr 2, 2020
 *      Author: danielgrieco
 */

#ifndef SNAKEUTILS_H_
#define SNAKEUTILS_H_

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

		static const unsigned int FPS = 10;
		static const unsigned int SNAKE_DEFAULT_SPEED = 1;
		static const CursesUtils::Color DEFAULT_COLOR = CursesUtils::Color::WHITE;
		static const unsigned short TOTAL_LIVES = 3;
		static const char QUIT_BUTTON = 'q';

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
		Vector2D position;
		char sprite;
		CursesUtils::Color color;
	};

	/*
	 * Represents the snake.
	 */
	struct Snake {
		Vector2D position;
		Direction direction;
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


} /* namespace TextSnake */

#endif /* SNAKEUTILS_H_ */
