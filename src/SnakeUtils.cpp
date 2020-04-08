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

		// Set the snake's current and previous positions to be the middle of the screen.
		s.currentPosition.x = midX;
		s.currentPosition.y = midY;

		s.previousPosition.x = midX;
		s.previousPosition.y = midY;

		// Random number between 0 and 3.
		int randDir = rand() % 4;

		// Set the direction to be a random one among the 4 available ones.
		s.currentDirection = static_cast<Direction>(randDir);

		// Previous direction is currently the same as the current one.
		s.previousDirection = s.currentDirection;

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
				if (g.currentState == State::SHOW_MAIN_GAME) {
					// Don't need to update directions when we're already going in the same direction.
					if (s.currentDirection != Direction::UP) {
						s.previousDirection = s.currentDirection;
						s.currentDirection = Direction::UP;
					}
				}
			}
				break;
			case static_cast<int>(CursesUtils::ArrowKey::RIGHT): {
				// Set the snake direction to the right whenever in game.
				if (g.currentState == State::SHOW_MAIN_GAME) {
					// Don't need to update directions when we're already going in the same direction.
					if (s.currentDirection != Direction::RIGHT) {
						s.previousDirection = s.currentDirection;
						s.currentDirection = Direction::RIGHT;
					}
				}
			}
				break;
			case static_cast<int>(CursesUtils::ArrowKey::DOWN): {
				// Set the snake direction to downwards whenever in game.
				if (g.currentState == State::SHOW_MAIN_GAME) {
					// Don't need to update directions when we're already going in the same direction.
					if (s.currentDirection != Direction::DOWN) {
						s.previousDirection = s.currentDirection;
						s.currentDirection = Direction::DOWN;
					}
				}
			}
				break;
			case static_cast<int>(CursesUtils::ArrowKey::LEFT): {
				// Set the snake direction to the left whenever in game.
				if (g.currentState == State::SHOW_MAIN_GAME) {
					// Don't need to update directions when we're already going in the same direction.
					if (s.currentDirection != Direction::LEFT) {
						s.previousDirection = s.currentDirection;
						s.currentDirection = Direction::LEFT;
					}
				}
			}
				break;
#ifdef SNAKE_UTILS_IN_GAME_DEBUG
			case Constants::ADD_SNAKE_PIECE_BUTTON: {
				// When in game, add a piece to the snake's tail.
				if (g.currentState == State::SHOW_MAIN_GAME)	MakeTailPiece(s);
			}
			break;
#endif
		}
	}


	unsigned int AdjustFPSbasedOnDirection(const Snake& snake) {
		// True when the direction is up or down.
		bool isMovingVertically = (snake.currentDirection == Direction::UP) || (snake.currentDirection == Direction::DOWN);

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
		// Only update the snake if in game.
		if (g.currentState == State::SHOW_MAIN_GAME) {
			// Update snake's position.
			TellSnakeToMove(s);

			// Update the tail's position.
			UpdateTailPiecesPosition(s);
		}
	}


	void DrawGame(const Game& g, const Snake& s) {
		// Only draw the snake if in game.
		if (g.currentState == State::SHOW_MAIN_GAME) {
			// Draw the snake.
			DrawHead(s);
			DrawTail(s);
		}
	}


	void UpdateTailPiecesPosition(Snake& snake) {
		// Get out if there is no tail.
		if (snake.tail.size() == 0)
			return;

		// Update all the pieces' position and direction.
		for (std::size_t i = 0; i < snake.tail.size(); i++) {
			// The first tail piece follows the head.
			if (i == 0) {
				// Update previous, current position and direction.
				snake.tail[i].previousDirection = snake.tail[i].currentDirection;
				snake.tail[i].currentDirection = snake.previousDirection;

				snake.tail[i].previousPosition.x = snake.tail[i].currentPosition.x;
				snake.tail[i].previousPosition.y = snake.tail[i].currentPosition.y;
				snake.tail[i].currentPosition.x = snake.previousPosition.x;
				snake.tail[i].currentPosition.y = snake.previousPosition.y;

				// Move onto the next piece.
				continue;
			}

			// Tail pieces normally follow their predecessors.
			// Update previous, current position and direction.
			snake.tail[i].previousDirection = snake.tail[i].currentDirection;
			snake.tail[i].currentDirection = snake.tail[i - 1].previousDirection;

			snake.tail[i].previousPosition.x = snake.tail[i].currentPosition.x;
			snake.tail[i].previousPosition.y = snake.tail[i].currentPosition.y;
			snake.tail[i].currentPosition.x = snake.tail[i - 1].previousPosition.x;
			snake.tail[i].currentPosition.y = snake.tail[i - 1].previousPosition.y;
		}
	}


	void TellSnakeToMove(Snake& snake) {
		// Check the snake current direction.
		switch (snake.currentDirection) {
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
		// Set the previous position before we set the new one.
		snake.previousPosition.x = snake.currentPosition.x;
		snake.previousPosition.y = snake.currentPosition.y;

		// Take the speed into account when changing the position.
		unsigned int newX = x * snake.speed;
		unsigned int newY = y * snake.speed;

		// Set the new position.
		snake.currentPosition.x += newX;
		snake.currentPosition.y += newY;

		// TODO Make sure the snake loses one life/dies if it hits its body.
		// TODO Make sure the snake loses one life/dies if it hits the wall.

		// TODO Make sure to call MakeTailPiece() whenever the snake eats an apple.
		// TODO Make sure the user gains score whenever the snake eats an apple.
	}


	void MakeTailPiece(Snake& s) {
		// Instantiate a new TailPiece and initialize it.
		TailPiece tp;
		tp.color = s.color;
		tp.sprite = Constants::SPR_SNAKE_TAIL;

		// Set its direction and position.
		SetNewTailPieceDirAndPos(s, tp);

		// Add the tail piece to the vector.
		s.tail.push_back(tp);
	}


	void SetNewTailPieceDirAndPos(const Snake& snake, TailPiece& tailPiece) {
		// Used to set the position.
		bool isFirstPiece = false;

		// First piece will follow the head.
		if (snake.tail.size() == 0) {
			tailPiece.currentDirection = snake.currentDirection;
			tailPiece.previousDirection = snake.previousDirection;

			isFirstPiece = true;
		} else {
			tailPiece.currentDirection = snake.tail.back().currentDirection;
			tailPiece.previousDirection = snake.tail.back().previousDirection;

			isFirstPiece = false;
		}

		// Set position based on direction.
		// We want to place the new piece one spot behind the preceding piece/head.
		// We find the spot behind the preceding piece by knowing what direction it's moving towards.
		switch (tailPiece.currentDirection) {
			case Direction::UP: {
				if (isFirstPiece) {
					tailPiece.currentPosition.x = snake.currentPosition.x;
					tailPiece.currentPosition.y = snake.currentPosition.y + 1;
				} else {
					tailPiece.currentPosition.x = snake.tail.back().currentPosition.x;
					tailPiece.currentPosition.y = snake.tail.back().currentPosition.y + 1;
				}

				// Since this piece was just created it doesn't have a previous position yet.
				tailPiece.previousPosition.x = tailPiece.currentPosition.x;
				tailPiece.previousPosition.y = tailPiece.currentPosition.y;
			}
				break;
			case Direction::RIGHT: {
				if (isFirstPiece) {
					tailPiece.currentPosition.x = snake.currentPosition.x - 1;
					tailPiece.currentPosition.y = snake.currentPosition.y;
				} else {
					tailPiece.currentPosition.x = snake.tail.back().currentPosition.x - 1;
					tailPiece.currentPosition.y = snake.tail.back().currentPosition.y;
				}

				// Since this piece was just created it doesn't have a previous position yet.
				tailPiece.previousPosition.x = tailPiece.currentPosition.x;
				tailPiece.previousPosition.y = tailPiece.currentPosition.y;
			}
				break;
			case Direction::DOWN: {
				if (isFirstPiece) {
					tailPiece.currentPosition.x = snake.currentPosition.x;
					tailPiece.currentPosition.y = snake.currentPosition.y - 1;
				} else {
					tailPiece.currentPosition.x = snake.tail.back().currentPosition.x;
					tailPiece.currentPosition.y = snake.tail.back().currentPosition.y - 1;
				}

				// Since this piece was just created it doesn't have a previous position yet.
				tailPiece.previousPosition.x = tailPiece.currentPosition.x;
				tailPiece.previousPosition.y = tailPiece.currentPosition.y;
			}
				break;
			case Direction::LEFT: {
				if (isFirstPiece) {
					tailPiece.currentPosition.x = snake.currentPosition.x + 1;
					tailPiece.currentPosition.y = snake.currentPosition.y;
				} else {
					tailPiece.currentPosition.x = snake.tail.back().currentPosition.x + 1;
					tailPiece.currentPosition.y = snake.tail.back().currentPosition.y;
				}

				// Since this piece was just created it doesn't have a previous position yet.
				tailPiece.previousPosition.x = tailPiece.currentPosition.x;
				tailPiece.previousPosition.y = tailPiece.currentPosition.y;
			}
				break;
		}
	}


	void DrawHead(const Snake& snake) {
		CursesUtils::PrintCharAtPosition(snake.sprite, snake.currentPosition.x, snake.currentPosition.y);
	}


	void DrawTail(const Snake& snake) {
		for (int i = 0; i < snake.tail.size(); i++)
			CursesUtils::PrintCharAtPosition(snake.tail[i].sprite, snake.tail[i].currentPosition.x, snake.tail[i].currentPosition.y);
	}

} /* namespace TextSnake */