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

		static const unsigned int FPS = 20;

	} /* namespace Constants */

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
		int speed;
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

} /* namespace TextSnake */

#endif /* SNAKEUTILS_H_ */
