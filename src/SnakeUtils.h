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
		static const int X_MIN = 0;
		static const int Y_MIN = 2;
		static const unsigned short SCORE_HUD_WIDTH = 11;

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
		unsigned int currentScore;
		Apple apple;
		bool isAppleOnScreen;
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
	void TellSnakeToMove(Snake& snake, Game& game);

	/*
	 * Sets the snake current position based on the given direction.
	 * snake: The snake of which to update the position.
	 * x: The new position on the x axis.
	 * y: The new position on the y axis.
	 */
	void MoveSnake(Snake& snake, const int x, const int y, Game& game);

	/*
	 * Checks whether the snake collided with a wall or with itself
	 * and if it did, it will lose one life.
	 * snk: Instance of the snake.
	 * gm: Instance of the game.
	 */
	void DieOnCollision(Snake& snk, Game& gm);

	/*
	 * Resets the snake when it collides with something other than apples
	 * and it has at least one life left.
	 * snake: Instance of the snake.
	 * game: Instance of the game.
	 */
	void ResetSnake(Snake& snake, const Game& game);

	/*
	 * On collision with an apple, the snake will eat it and increase its score.
	 * snk: Instance of the snake.
	 * gm: Instance of the game.
	 */
	void EatAppleOnCollision(Snake& snk, Game& gm);

	/*
	 * Spawns an apple whenever it's possible.
	 * game: Instance of the game.
	 * snake: Instance of the snake.
	 */
	void SpawnApple(Game& game, const Snake& snake);

	/*
	 * Picks a random position on the screen free of any obstacles
	 * and assigns it to the given argument.
	 * s: Instance of the snake.
	 * p: Position to fill in.
	 */
	void PickRandomApplePos(const Snake& s, Vector2D& p);

	/*
	 * Initializes an apple's data
	 * a: apple to initialize.
	 * p: horizontal and vertical position on the screen.
	 */
	void InitApple(Apple& a, const Vector2D& p);

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
	 * Draws the HUD.
	 * game: Instance of the game.
	 */
	inline void DrawHUD(const Game& game);

	/*
	 * Draws the score counter.
	 * g: Instance of the game.
	 * pos: Where to draw the counter.
	 */
	inline void DrawScore(const Game& g, const Vector2D& pos);

	/*
	 * Draws the lives counter.
	 * g: Instance of the game.
	 * pos: Where to draw the counter.
	 */
	inline void DrawLives(const Game& g, const Vector2D& pos);

	/*
	 * Draws the snake's head.
	 * snake: Instance of the snake.
	 */
	inline void DrawHead(const Snake& snake);

	/*
	 * Draws the tail pieces.
	 * snake: Instance of the snake.
	 */
	inline void DrawTail(const Snake& snake);

	/*
	 * Draws an apple.
	 * appl: Apple to draw.
	 */
	inline void DrawApple(const Apple& appl);

} /* namespace TextSnake */

#endif /* SNAKEUTILS_H_ */
