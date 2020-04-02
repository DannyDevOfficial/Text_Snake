/*
 * SnakeUtils.h
 *
 *  Created on: Apr 2, 2020
 *      Author: danielgrieco
 */

#ifndef SNAKEUTILS_H_
#define SNAKEUTILS_H_

#include <vector>

#include "CursesUtils.h"

namespace TextSnake {

	/*
	 * Defining all constants here.
	 */
	namespace Constants {

		const char SPR_SNAKE_HEAD = '@';
		const char SPR_SNAKE_TAIL = '*';
		const char SPR_APPLE = 'o';

		const unsigned int FPS = 20;

	} /* namespace Constants */

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
	};

	/*
	 * Represents the game e.g. states, scores etc.
	 */
	struct Game {
		unsigned short lives;
		unsigned int applesEaten;
		unsigned int score;
		// TODO Add variable for current game state and current screen.
	};

} /* namespace TextSnake */

#endif /* SNAKEUTILS_H_ */
