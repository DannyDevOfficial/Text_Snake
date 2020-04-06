/*
 * SnakeUtils.cpp
 *
 *  Created on: Apr 2, 2020
 *      Author: danielgrieco
 */

#include "SnakeUtils.h"

#include <ctime>
#include <cmath>

namespace TextSnake {

	void Start() {
		// Seed the random number generator.
		srand(time(0));

		// Initialize Curses.
		CursesUtils::InitCurses(true, false, false, true, true, 0);

		// Initializations.
		Game mainGame;
		Snake theSnake;

		InitGame(mainGame);
		InitSnake(theSnake);

		// TODO Remove the following line after the basic game is implemented.
		mainGame.currentState = State::SHOW_MAIN_GAME;

		// Flag that tells the game loop when to quit.
		bool quit = false;

		// Used for the input handling.
		int input = 0;

		// FPS needs to be adjusted because the screen is larger than longer,
		// which means that the snake is faster when moving vertically.
		unsigned int adjustedFPS = Constants::DEFAULT_FPS;

		// Take the time at the start of the game.
		clock_t lastTime = clock();

		// Game loop.
		while (!quit) {
			// Take the time at the start of every loop.
			clock_t currentTime = clock();
			// Delta time is the time elapsed to run the last frame.
			clock_t deltaTime = currentTime - lastTime;

			// Only run game logic in respect to the wanted frame rate.
			if (deltaTime > (CLOCKS_PER_SEC / adjustedFPS)) {
				// Update the last time to be the current time.
				lastTime = currentTime;

				// Handle the input from the user.
				HandleInput(input, mainGame, theSnake);

				// Whenever the user hits the quit button the game ends, otherwise it goes on normally.
				if (input != Constants::QUIT_BUTTON) {
					// Clear the screen before updating and drawing the next frame.
					CursesUtils::ClearScreen();

					// Adjusting FPS.
					adjustedFPS = AdjustFPSbasedOnDirection(theSnake);

					// Update the game logic.
					UpdateGame(mainGame, theSnake);

					// Draw the game.
					DrawGame(mainGame, theSnake);

					// Refresh the screen to show the up to date game.
					CursesUtils::RefreshScreen();
				} else {
					// Quitting...
					quit = true;
				}
			}
		}

		// Make sure Curses gets shut down.
		CursesUtils::ShutdownCurses();
	}


	void InitSnake(Snake& s) {
		// Get the middle point of the screen.
		unsigned int midX = CursesUtils::GetColumns() / 2;
		unsigned int midY = CursesUtils::GetRows() / 2;

		// Set the snake's position to be the middle of the screen.
		s.position.x = midX;
		s.position.y = midY;

		// Random number between 0 and 3.
		int randDir = rand() % 4;

		// Set the direction to be a random one among the 4 available ones.
		s.direction = static_cast<Direction>(randDir);

		// Set the snake's head sprite.
		s.sprite = Constants::SPR_SNAKE_HEAD;

		// Default speed.
		s.speed = Constants::SNAKE_DEFAULT_SPEED;

		// Default color.
		s.color = Constants::DEFAULT_COLOR;

		// Tail vector should be empty.
		if (s.tail.size() > 0)	s.tail.clear();
	}


	void InitApple(Apple& a, const Vector2D& p) {
		// Set the apple's position.
		a.position.x = p.x;
		a.position.y = p.y;

		// Set the apple's sprite.
		a.sprite = Constants::SPR_APPLE;

		// Set the apple's default color;
		a.color = Constants::DEFAULT_COLOR;
	}


	void InitGame(Game& g) {
		// Total lives available.
		g.lives = Constants::TOTAL_LIVES;

		// No apples eaten at the beginning.
		g.applesEaten = 0;

		// Score is 0 at the start.
		g.currentScore = 0;

		// TODO High scores should be loaded in when the game starts.


		// Screen will be set to main menu at first.
		g.currentScreen = Screen::MAIN_MENU;

		// Game state initially set to show main menu.
		g.currentState = State::SHOW_MAIN_MENU;
	}


	void HandleInput(int& inpt, const Game& g, Snake& s) {
		// Store the current input.
		inpt = CursesUtils::GetCharacter();

		// Check what kind of input the user entered.
		switch (inpt) {
			case static_cast<int>(CursesUtils::ArrowKey::UP): {
				// Set the snake direction to upwards whenever in game.
				if (g.currentState == State::SHOW_MAIN_GAME)	s.direction = Direction::UP;
			}
				break;
			case static_cast<int>(CursesUtils::ArrowKey::RIGHT): {
				// Set the snake direction to the right whenever in game.
				if (g.currentState == State::SHOW_MAIN_GAME)	s.direction = Direction::RIGHT;
			}
				break;
			case static_cast<int>(CursesUtils::ArrowKey::DOWN): {
				// Set the snake direction to downwards whenever in game.
				if (g.currentState == State::SHOW_MAIN_GAME)	s.direction = Direction::DOWN;
			}
				break;
			case static_cast<int>(CursesUtils::ArrowKey::LEFT): {
				// Set the snake direction to the left whenever in game.
				if (g.currentState == State::SHOW_MAIN_GAME)	s.direction = Direction::LEFT;
			}
				break;
		}
	}


	unsigned int AdjustFPSbasedOnDirection(const Snake& snake) {
		// True when the direction is up or down.
		bool isMovingVertically = (snake.direction == Direction::UP) || (snake.direction == Direction::DOWN);

		// Get the screen width and height.
		int width = 0;
		int height = 0;
		CursesUtils::GetWindowSize(width, height);

		// Get the width/height ratio and make some calculations on it to get a more precise number.
		int whRatio =  static_cast<int>(ceil(width / height)) + 2;

		// Calculate the new fps for the vertical movement, which is based on the height of the screen.
		int vFPS = static_cast<int>(ceil(height / whRatio));

		// When the snake is moving vertically, change the frame rate to adjust the movement speed to be equal
		// on both horizontal and vertical directions.
		return isMovingVertically ? vFPS : Constants::DEFAULT_FPS;
	}


	void UpdateGame(Game& g, Snake& s) {
		// Update snake's position.
		TellSnakeToMove(s);
	}


	void DrawGame(const Game& g, const Snake& s) {
		// Draw the snake.
		CursesUtils::PrintCharAtPosition(s.sprite, s.position.x, s.position.y);
	}


	void TellSnakeToMove(Snake& snake) {
		// Check the snake current direction.
		switch (snake.direction) {
			case Direction::UP:
				MoveSnake(snake, 0, -1);
				break;
			case Direction::RIGHT:
				MoveSnake(snake, 1, 0);
				break;
			case Direction::DOWN:
				MoveSnake(snake, 0, 1);
				break;
			case Direction::LEFT:
				MoveSnake(snake, -1, 0);
				break;
		}
	}


	void MoveSnake(Snake& snake, const int x, const int y) {
		// Take the speed into account when changing the position.
		unsigned int newX = x * snake.speed;
		unsigned int newY = y * snake.speed;

		// Set the new position.
		snake.position.x += newX;
		snake.position.y += newY;

		// TODO Make sure all the tail pieces move alongside the head.

		// TODO Make sure the snake loses one life/dies if it hits its body.
		// TODO Make sure the snake loses one life/dies if it hits the wall.

		// TODO Make sure the snake has its body lengthened if it eats an apple.
		// TODO Make sure the user gains score whenever the snake eats an apple.
	}

} /* namespace TextSnake */
