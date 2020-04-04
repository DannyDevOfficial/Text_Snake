/*
 * SnakeUtils.cpp
 *
 *  Created on: Apr 2, 2020
 *      Author: danielgrieco
 */

#include "SnakeUtils.h"

#include <ctime>

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


		// Flag that tells the game loop when to quit.
		bool quit = false;

		// Take the time at the start of the game.
		clock_t lastTime = clock();

		while (!quit) {
			// Take the time at the start of every loop.
			clock_t currentTime = clock();
			// Delta time is the time elapsed to run the last frame.
			clock_t deltaTime = currentTime - lastTime;

			// Only run game logic in respect to the wanted frame rate.
			if (deltaTime > (CLOCKS_PER_SEC / Constants::FPS)) {
				// Update the last time to be the current time.
				lastTime = currentTime;

				// TODO Handle the input from the user.
				// TODO Whenever the user hits the quit button the game ends, otherwise it goes on normally.
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

} /* namespace TextSnake */
