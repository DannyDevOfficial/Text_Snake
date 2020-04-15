/*
 * SnakeUtils.cpp
 *
 *  Created on: Apr 2, 2020
 *      Author: danielgrieco
 */

#include "SnakeUtils.h"

#include <ctime>
#include <cmath>
#include <fstream>
#include <algorithm>

namespace TextSnake {

	void Start() {
		// Seed the random number generator.
		srand(time(0));

		// Initialize Curses.
		CursesUtils::InitCurses(true, false, false, true, true, 0);

		// Initializations.
		Game mainGame;
		Snake theSnake;

		FirstInit(mainGame, theSnake);

		// Initialize all menu entries.
		InitMenu(mainGame);

		// Load high scores only once when the application is run.
		LoadHighScores(mainGame);

		// Make color pairs.
		InitColors();

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
					Update(mainGame, theSnake, input);

					// Draw the game.
					Draw(mainGame, theSnake);

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


	void FirstInit(Game& gm, Snake& snk) {
		// Initialize everything.
		InitGame(gm);
		InitSnake(snk);
		SpawnApple(gm, snk);
	}


	void InitColors() {
		// Make a green for the snake.
		CursesUtils::MakeColorPair(Constants::GREEN_ON_BLACK_ID, CursesUtils::Color::GREEN, CursesUtils::Color::BLACK);
		// Make a red for the apple.
		CursesUtils::MakeColorPair(Constants::RED_ON_BLACK_ID, CursesUtils::Color::RED, CursesUtils::Color::BLACK);
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


	void InitGame(Game& g) {
		// Total lives available.
		g.lives = Constants::TOTAL_LIVES;

		// No apples on the screen during initialization.
		g.isAppleOnScreen = false;

		// Score is 0 at the start.
		g.currentScore = 0;

		// There's no final score when initializing.
		g.finalScore.name = "PLAYER";
		g.finalScore.score = 0;

		// Selector is standing still.
		g.selectorDirection = SelectorDirection::STILL;

		// Set the selected entry to the first one in the vector.
		if (!g.mainMenuEntries.empty()) {
			g.mainMenuEntries[0].isSelected = true;

			// All the other entries are deselected.
			for (std::size_t i = 1; i < g.mainMenuEntries.size(); i++)
				g.mainMenuEntries[i].isSelected = false;
		}

		// Screen will be set to main menu at first.
		g.currentScreen = Screen::MAIN_MENU;

		// Game state initially set to show main menu.
		g.currentState = State::SHOW_MAIN_MENU;
	}


	void InitMenu(Game& game) {
		// Position to use.
		Vector2D pos;
		// Set the y position.
		// Take into account the position of the intro.
		pos.y = static_cast<int>(CursesUtils::GetRows() / 2) - Constants::INTRO_TEXT_OFFSET;

		// Entries.
		MenuEntry entries[Constants::TOTAL_MAIN_MENU_ENTRIES];

		// Entries texts.
		// Play entry.
		entries[0].text = "Play the game";
		entries[0].relatedScreen = Screen::MAIN_GAME;
		// High scores entry.
		entries[1].text = "High Scores";
		entries[1].relatedScreen = Screen::HIGH_SCORES;

		// Set all the entries.
		for (std::size_t i = 0; i < Constants::TOTAL_MAIN_MENU_ENTRIES; i++) {
			// Reset the x position to void the previous movement.
			pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
			// Center the entry based on the string's length.
			pos.x -= static_cast<int>(std::strlen(entries[i].text.c_str()) / 2);

			// The first entry will have a special offset.
			// The first entry gets automatically selected.
			if (i == 0) {
				pos.y += Constants::MENU_TEXT_DIST + Constants::FIRST_ENTRY_TEXT_OFFSET;

				entries[i].isSelected = true;
			} else {
				pos.y += Constants::MENU_TEXT_DIST;

				entries[i].isSelected = false;
			}

			// Set the entry.
			entries[i].position.x = pos.x;
			entries[i].position.y = pos.y;
			entries[i].attribute = CursesUtils::Attribute::NORMAL;
		}

		// Add the entries to the vector.
		for (std::size_t i = 0; i < Constants::TOTAL_MAIN_MENU_ENTRIES; i++)
			game.mainMenuEntries.push_back(entries[i]);
	}


	void HandleInput(int& inpt, Game& g, Snake& s) {
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
				} else if (g.currentState == State::SHOW_MAIN_MENU) {
					// Move through the entry list upwards.

					// Selector is moving up.
					g.selectorDirection = SelectorDirection::UP;
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
				} else if (g.currentState == State::SHOW_MAIN_MENU) {
					// Move through the entry list downwards.

					// Selector moves down.
					g.selectorDirection = SelectorDirection::DOWN;
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
			case static_cast<int>(Constants::ENTER_KEY): {
				// Enter_key press is valid in every screen but the main game.
				if (g.currentState != State::SHOW_MAIN_GAME)	EnterKeyPressed(g, s);
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


	void EnterKeyPressed(Game& game, Snake& snake) {
		// Change the current state to high scores when the user pressed enter from within the game over screen.
		if (game.currentScreen == Screen::GAME_OVER) {
			// Add new high score.
			game.highScores.push_back(game.finalScore);

			// Sort them out in descending order.
			std::sort(game.highScores.begin(), game.highScores.end(), GetHigher);

			// Save high scores.
			SaveHighScores(game);

			game.currentState = State::SHOW_HIGH_SCORES;

			// Don't run the next instructions.
			return;
		} else if (game.currentScreen == Screen::HIGH_SCORES) {
			// Initialize the game as if it was a brand new instance.
			FirstInit(game, snake);

			// Don't run the next instructions.
			return;
		}

		// Selected entry related screen.
		Screen selectedRelatedScreen = Screen::MAIN_GAME;

		// Find the selected entry in the menu.
		for (std::size_t i = 0; i < game.mainMenuEntries.size(); i++) {
			// Once the selected entry is found.
			if (game.mainMenuEntries[i].isSelected) {
				// Set related screen.
				selectedRelatedScreen = game.mainMenuEntries[i].relatedScreen;

				// Done. Don't keep looking for entries.
				break;
			}
		}

		// Check what entry that was and change state based on that.
		switch (selectedRelatedScreen) {
			case Screen::MAIN_MENU:
				break;
			case Screen::MAIN_GAME:
				// Show the game.
				game.currentState = State::SHOW_MAIN_GAME;
				break;
			case Screen::HIGH_SCORES:
				// Show the high scores.
				game.currentState = State::SHOW_HIGH_SCORES;
				break;
			case Screen::GAME_OVER:
				break;
		}
	}


	void SaveHighScores(const Game& gm) {
		// Make an output file stream for binary.
		std::ofstream writeFile;

		// Open the file or make a new one if it didn't exist before.
		writeFile.open(Constants::HIGH_SCORES_FILENAME, std::ios_base::binary);

		// Only write to the file if it was opened.
		if (writeFile.is_open()) {
			// Store the values of the vector into an array.
			Score hScores[gm.highScores.size()];
			std::copy(gm.highScores.begin(), gm.highScores.end(), hScores);

			// Write high scores to the file.
			writeFile.write(reinterpret_cast<char*>(hScores), sizeof(hScores));

			// Close the file.
			writeFile.close();
		}
	}


	bool GetHigher(const Score& score1, const Score& score2) {
		return (score1.score > score2.score) ? true : false;
	}


	void LoadHighScores(Game& gm) {
		// Make an input file stream for binary.
		std::ifstream readFile;

		// Open the file in binary mode.
		readFile.open(Constants::HIGH_SCORES_FILENAME, std::ios_base::binary);

		// Only read if the file was opened.
		if (readFile.is_open()) {
			// Figure out the size of the file in bytes.
			readFile.seekg(0, readFile.end);
			long fileSizeInBytes = readFile.tellg();
			readFile.seekg(0);

			// Figure out how many high scores are in the file.
			int numberOfHighScores = fileSizeInBytes / sizeof(Score);
			// Allocate a buffer for the high scores.
			Score hScores[numberOfHighScores];

			// Read the high scores into the buffer.
			readFile.read(reinterpret_cast<char*>(hScores), fileSizeInBytes);

			// Put the high scores back into the vector.
			for (std::size_t i = 0; i < numberOfHighScores; i++)
				gm.highScores.push_back(hScores[i]);

			// Close the file.
			readFile.close();
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


	void Update(Game& g, Snake& s, int in) {
		// Updates the current screen when the state changes.
		UpdateScreen(g);

		// Run the correct update logic based on the current state.
		switch (g.currentState) {
			// Run the main menu logic.
			case State::SHOW_MAIN_MENU:
				UpdateMainMenu(g);
				break;
			// Run main game logic.
			case State::SHOW_MAIN_GAME:
				UpdateMainGame(g, s);
				break;
			// Run game over logic.
			case State::SHOW_GAME_OVER:
				UpdateGameOver(g, in);
				break;
			// Run high scores logic.
			case State::SHOW_HIGH_SCORES:
				break;
		}
	}


	void Draw(const Game& g, const Snake& s) {
		// Draw the game depending on what screen the game is on.
		switch (g.currentScreen) {
			// Draw the main menu screen.
			case Screen::MAIN_MENU:
				DrawMainMenu(g);
				break;
			// Draw the in game screen.
			case Screen::MAIN_GAME:
				DrawMainGame(g, s);
				break;
			// Draw game over screen.
			case Screen::GAME_OVER:
				DrawGameOver(g);
				break;
			// Draw the high scores screen.
			case Screen::HIGH_SCORES:
				DrawHighScores(g);
				break;
		}
	}


	void UpdateScreen(Game& game) {
		// Change the current screen based on the current state.
		switch (game.currentState) {
			case State::SHOW_MAIN_MENU:
				game.currentScreen = Screen::MAIN_MENU;
				break;
			case State::SHOW_MAIN_GAME:
				game.currentScreen = Screen::MAIN_GAME;
				break;
			case State::SHOW_GAME_OVER:
				game.currentScreen = Screen::GAME_OVER;
				break;
			case State::SHOW_HIGH_SCORES:
				game.currentScreen = Screen::HIGH_SCORES;
				break;
		}
	}


	void UpdateMainMenu(Game& game) {
		// This function won't run when the selector isn't moving.
		if (game.selectorDirection == SelectorDirection::STILL)
			return;

		// Find the currently selected entry.
		for (std::size_t i = 0; i < game.mainMenuEntries.size(); i++) {
			// When the selected entry is found.
			if (game.mainMenuEntries[i].isSelected) {
				// Deselect the currently selected one.
				game.mainMenuEntries[i].isSelected = false;

				// Pick next index based on the selection direction.
				int nextIndex = 0;
				if (game.selectorDirection == SelectorDirection::UP)		nextIndex = abs(static_cast<int>(i - 1));
				else if (game.selectorDirection == SelectorDirection::DOWN)	nextIndex = static_cast<int>(i + 1);

				// Wrap around the entries when the next one isn't available.
				int nextSelectedIndex = nextIndex % game.mainMenuEntries.size();

				// Select the next entry.
				game.mainMenuEntries[nextSelectedIndex].isSelected = true;

				// No need to look for another entry, we've already found it.
				break;
			}
		}

		// Selector has done moving.
		game.selectorDirection = SelectorDirection::STILL;
	}


	void UpdateMainGame(Game& game, Snake& snake) {
		// Update snake's position.
		TellSnakeToMove(snake, game);

		// Update the tail's position.
		UpdateTailPiecesPosition(snake);
	}


	void UpdateGameOver(Game& game, int input) {
		// If the user entered a backspace, delete the last character in the string.
		if (input == Constants::BACKSPACE_KEY &&
				game.finalScore.name.length() > 0)
			game.finalScore.name.erase(game.finalScore.name.length() - 1);

		// Make sure the input is either a letter or a digit.
		bool isCapitalLetter = (input >= Constants::START_CAP_LETTERS) &&
				(input < (Constants::START_CAP_LETTERS + Constants::TOTAL_LETTERS));

		bool isLowercaseLetter = (input >= Constants::START_LOW_LETTERS) &&
				(input < (Constants::START_LOW_LETTERS + Constants::TOTAL_LETTERS));

		bool isDigit = (input >= Constants::START_DIGITS) &&
				(input < (Constants::START_DIGITS + Constants::TOTAL_DIGITS));

		// Add the valid input to the string.
		if (isCapitalLetter || isLowercaseLetter || isDigit)
			game.finalScore.name += std::toupper(static_cast<char>(input));
	}


	void DrawMainMenu(const Game& game) {
		// Position.
		Vector2D pos;
		// Initially set to the middle of the screen.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		pos.y = static_cast<int>(CursesUtils::GetRows() / 2);

		// String to draw.
		std::string menuString = "";

		// Intro.

		// Set the intro string.
		menuString = "TEXT SNAKE";
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(menuString.c_str()) / 2);
		// Lift the intro string up a little.
		pos.y -= Constants::INTRO_TEXT_OFFSET;
		// Draw the text.
		DrawText(menuString.c_str(), pos, CursesUtils::Attribute::BOLD);

		// Draw the menu entries.
		for (std::size_t i = 0; i < game.mainMenuEntries.size(); i++) {
			// Draw a selected entry differently.
			if (game.mainMenuEntries[i].isSelected)
				DrawSelectedText(game.mainMenuEntries[i].text.c_str(),
				                 game.mainMenuEntries[i].position);
			else
				DrawText(game.mainMenuEntries[i].text.c_str(),
				         game.mainMenuEntries[i].position,
				         game.mainMenuEntries[i].attribute);
		}

		// Quit text.
		menuString = "You can press (q) at any point in the game to quit.";
		// Reset the x position to void the previous movement.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(menuString.c_str()) / 2);
		// Move the string down a bit.
		// Added more offset cause it's not part of the menu, just info.
		// Taken into account all the entries before it.
		pos.y += (Constants::MENU_TEXT_DIST + 7) +
				(game.mainMenuEntries.size() * Constants::MENU_TEXT_DIST) +
				Constants::FIRST_ENTRY_TEXT_OFFSET;

		// Draw the text.
		DrawText(menuString.c_str(), pos, CursesUtils::Attribute::STANDOUT);
	}


	void DrawMainGame(const Game& game, const Snake& snake) {
		// Draw the HUD.
		DrawHUD(game);

		// Draw the snake in green.
		CursesUtils::ToggleColorPair(Constants::GREEN_ON_BLACK_ID, true);

		DrawHead(snake);
		DrawTail(snake);

		// Turn off the color.
		CursesUtils::ToggleColorPair(Constants::GREEN_ON_BLACK_ID, false);

		// Draw the apple if there's one on screen.
		if (game.isAppleOnScreen) {
			// Make it red.
			CursesUtils::ToggleColorPair(Constants::RED_ON_BLACK_ID, true);

			DrawApple(game.apple);

			// Turn off the color.
			CursesUtils::ToggleColorPair(Constants::RED_ON_BLACK_ID, false);
		}
	}


	void DrawGameOver(const Game& game) {
		// Position.
		Vector2D pos;
		// Initially set to the middle of the screen.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		pos.y = static_cast<int>(CursesUtils::GetRows() / 2);

		// String to draw.
		std::string gameOverString = "";

		// Intro text.
		gameOverString = "GAME OVER";
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(gameOverString.c_str()) / 2);
		// Lift the intro string up a little.
		pos.y -= Constants::INTRO_TEXT_OFFSET;
		// Draw the text.
		DrawText(gameOverString.c_str(), pos, CursesUtils::Attribute::BOLD);

		// Set the high score string.
		gameOverString = game.finalScore.name + "   ";
		// Get the length of the above string.
		int goLength = std::strlen(gameOverString.c_str());
		// Recenter the position.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(goLength / 2);
		// Move the string down.
		pos.y += Constants::FIRST_ENTRY_TEXT_OFFSET + Constants::MENU_TEXT_DIST;
		// Draw name.
		DrawText(gameOverString.c_str(), pos, CursesUtils::Attribute::BLINK);
		// Draw score.
		pos.x += goLength;
		DrawText(std::to_string(game.finalScore.score).c_str(), pos, CursesUtils::Attribute::NORMAL);

		// Enter text.
		gameOverString = "Press (enter) to confirm.";
		// Reset the x position to void the previous movement.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(gameOverString.c_str()) / 2);
		// Move the string down a bit.
		// Added more offset cause it's not part of the menu, just info.
		// Take into account all entries before this.
		pos.y += Constants::MENU_TEXT_DIST + 7;
		// Draw the text.
		DrawText(gameOverString.c_str(), pos, CursesUtils::Attribute::UNDERLINE);

		// Quit text.
		gameOverString = "You can press (q) at any point in the game to quit.";
		// Reset the x position to void the previous movement.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(gameOverString.c_str()) / 2);
		// Move the string down a bit.
		// Added more offset cause it's not part of the menu, just info.
		pos.y += Constants::MENU_TEXT_DIST;
		// Draw the text.
		DrawText(gameOverString.c_str(), pos, CursesUtils::Attribute::STANDOUT);
	}


	void DrawHighScores(const Game& game) {
		// Position.
		Vector2D pos;
		// Initially set to the middle of the screen.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		// Set this to the very top of the screen.
		pos.y = 0;

		// String to draw.
		std::string highScoresString = "";

		// Intro text.
		highScoresString = "HIGH SCORES";
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(highScoresString.c_str()) / 2);
		// Draw the text.
		DrawText(highScoresString.c_str(), pos, CursesUtils::Attribute::BOLD);

		// High scores.
		std::string highScoreStr = "";
		for (std::size_t i = 0; i < game.highScores.size(); i++) {
			// Don't draw more than the max to the screen.
			if (i >= Constants::MAX_HIGH_SCORES_ON_SCREEN)
				break;

			// Set the string.
			highScoreStr = game.highScores[i].name + "   " + std::to_string(game.highScores[i].score);

			// Center the x position.
			pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
			// Align the string with the center based on the length of the string.
			pos.x -= static_cast<int>(std::strlen(highScoreStr.c_str()) / 2);
			// Lower the string a little.
			pos.y += Constants::MENU_TEXT_DIST;

			// Draw high score.
			DrawText(highScoreStr.c_str(), pos, CursesUtils::Attribute::NORMAL);
		}

		// Enter text.
		highScoresString = "Press (enter) to go back to main menu.";
		// Reset the x position to void the previous movement.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(highScoresString.c_str()) / 2);
		// Move the string down a bit.
		// Added more offset cause it's not part of the menu, just info.
		// Take into account all entries before this.
		pos.y += Constants::MENU_TEXT_DIST + 2;
		// Draw the text.
		DrawText(highScoresString.c_str(), pos, CursesUtils::Attribute::UNDERLINE);

		// Quit text.
		highScoresString = "You can press (q) at any point in the game to quit.";
		// Reset the x position to void the previous movement.
		pos.x = static_cast<int>(CursesUtils::GetColumns() / 2);
		// Center the intro based on the string's length.
		pos.x -= static_cast<int>(std::strlen(highScoresString.c_str()) / 2);
		// Move the string down a bit.
		// Added more offset cause it's not part of the menu, just info.
		pos.y += Constants::MENU_TEXT_DIST;
		// Draw the text.
		DrawText(highScoresString.c_str(), pos, CursesUtils::Attribute::STANDOUT);
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


	void TellSnakeToMove(Snake& snake, Game& game) {
		// Check the snake current direction.
		switch (snake.currentDirection) {
			case Direction::UP:
				MoveSnake(snake, 0, -1, game);
				break;
			case Direction::RIGHT:
				MoveSnake(snake, 1, 0, game);
				break;
			case Direction::DOWN:
				MoveSnake(snake, 0, 1, game);
				break;
			case Direction::LEFT:
				MoveSnake(snake, -1, 0, game);
				break;
		}
	}


	void MoveSnake(Snake& snake, const int x, const int y, Game& game) {
		// Set the previous position before we set the new one.
		snake.previousPosition.x = snake.currentPosition.x;
		snake.previousPosition.y = snake.currentPosition.y;

		// Take the speed into account when changing the position.
		unsigned int newX = x * snake.speed;
		unsigned int newY = y * snake.speed;

		// Set the new position.
		snake.currentPosition.x += newX;
		snake.currentPosition.y += newY;

		// Check whether the snake hits a wall or itself.
		DieOnCollision(snake, game);

		// Check whether the snake ate an apple.
		EatAppleOnCollision(snake, game);
	}


	void DieOnCollision(Snake& snk, Game& gm) {
		// Wall collisions.
		// Snake position is the same as either border of the screen.
		bool vWallCollision = (snk.currentPosition.y < Constants::Y_MIN) ||
				(snk.currentPosition.y > CursesUtils::GetRows());
		bool hWallCollision = (snk.currentPosition.x < Constants::X_MIN) ||
				(snk.currentPosition.x > CursesUtils::GetColumns());

		// Tail Collision.
		bool tailCollision = false;
		if (snk.tail.size() > 0) {
			for (std::size_t i = 0; i < snk.tail.size(); i++) {
				// Head position is the same as the tail piece position.
				if ((snk.currentPosition.x == snk.tail[i].currentPosition.x) &&
						(snk.currentPosition.y == snk.tail[i].currentPosition.y)) {
					tailCollision = true;

					// No need to check for other pieces since this one already collided.
					break;
				}
			}
		}

		// If a collision happened, make sure to lose one life or die.
		if (vWallCollision || hWallCollision || tailCollision) {
			// Lose a life.
			gm.lives--;

			// When the snake has at least one life left, then reset it.
			// On the other hand, when the snake has no more lives left, move onto the
			// game over screen.
			if (gm.lives > 0) {
				ResetSnake(snk, gm);
			} else	{
				// Set the lives count to 0.
				gm.lives = 0;

				// Set the final score.
				gm.finalScore.score = gm.currentScore;

				// Change state to game over.
				gm.currentState = State::SHOW_GAME_OVER;
			}
		}
	}


	void ResetSnake(Snake& snake, const Game& game) {
		// Middle of the screen.
		int xMid = static_cast<int>(CursesUtils::GetColumns() / 2);
		int yMid = static_cast<int>(CursesUtils::GetRows() / 2);

		// Reset the snake position to the center of the screen.
		int xPos = 0;
		int yPos = 0;

		// Random offset from the center.
		int randomOffset = (rand() % Constants::OFFSET_FROM_MIDSCREEN) + 1;

		if ((game.apple.position.x == xMid) && (game.apple.position.y == yMid)) {
			// If an apple is located in the center, put the snake somewhere else.
			xPos = xMid + randomOffset;
			yPos = yMid + randomOffset;
		} else {
			// Snake is positioned in the center.
			xPos = xMid;
			yPos = yMid;
		}

		snake.currentPosition.x = xPos;
		snake.currentPosition.y = yPos;

		// Reset previous position.
		snake.previousPosition.x = snake.currentPosition.x;
		snake.previousPosition.y = snake.currentPosition.y;

		// Reset direction.
		snake.currentDirection = static_cast<Direction>(rand() % 4);
		snake.previousDirection = snake.currentDirection;

		// Clear the tail.
		if (snake.tail.size() > 0)	snake.tail.clear();
	}


	void EatAppleOnCollision(Snake& snk, Game& gm) {
		// Snake didn't collide with an apple.
		if ((snk.currentPosition.x != gm.apple.position.x) || (snk.currentPosition.y != gm.apple.position.y))
			return;

		// Apple is no longer on the screen when the snake eats it.
		gm.isAppleOnScreen = false;

		// Increase length of the snake's tail.
		MakeTailPiece(snk);

		// Increase the score whenever the snake eats an apple.
		gm.currentScore += CalcScore(snk);

		// Spawn a new apple.
		SpawnApple(gm, snk);
	}


	unsigned int CalcScore(const Snake& snake) {
		// My score increase formula.
		unsigned int scoreAddition = static_cast<unsigned int>(ceil(snake.tail.size() / 2) * Constants::SCORE_MULTIPLIER);

		// Return the base points in case it's the first apple the snake eats.
		return (snake.tail.size() > 1) ? scoreAddition : Constants::BASE_APPLE_POINTS;
	}


	void SpawnApple(Game& game, const Snake& snake) {
		// Can't spawn an apple if there's already one on the screen.
		if (game.isAppleOnScreen)	return;

		// Calculate its position.
		Vector2D randomPos;
		PickRandomApplePos(snake, randomPos);

		// Initialize this apple.
		InitApple(game.apple, randomPos);

		// Since the apple has been created, the flag will be updated.
		game.isAppleOnScreen = true;
	}


	void PickRandomApplePos(const Snake& s, Vector2D& p) {
		// Flag to indicate whether the random spot on the screen is free or not.
		bool isFree = false;

		int randomX = 0;
		int randomY = 0;

		// Keep generating a random position until we find a free spot on the screen.
		do {
			// Get the random position between min and max.
			randomX = (rand() % (CursesUtils::GetColumns() - Constants::X_MIN)) + Constants::X_MIN;
			randomY = (rand() % (CursesUtils::GetRows() - Constants::Y_MIN)) + Constants::Y_MIN;

			// Check every single piece of the snake, including the head, to see
			// if it happens to be in the same spot as the random one.

			// Head.
			if ((s.currentPosition.x == randomX) && (s.currentPosition.y == randomY)) {
				isFree = false;

				// No need to check for the tail if the head is already in the same position as the random one.
				break;
			} else {
				isFree = true;
			}

			// Tail.
			for (std::size_t i = 0; i < s.tail.size(); i++) {
				if ((s.tail[i].currentPosition.x == randomX) && (s.tail[i].currentPosition.y == randomY)) {
					isFree = false;

					// Don't check other pieces if this one already matches up with the random one.
					break;
				} else {
					isFree = true;
				}
			}
		} while (!isFree);

		// If we got here then we know a good random position was found.
		p.x = randomX;
		p.y = randomY;
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


	void DrawHUD(const Game& game) {
		// Lives.
		Vector2D livesPos;
		livesPos.x = 0;
		livesPos.y = 0;
		DrawLives(game, livesPos);

		// Score.
		Vector2D scorePos;
		scorePos.x = CursesUtils::GetColumns() - Constants::SCORE_HUD_WIDTH;
		scorePos.y = 0;
		DrawScore(game, scorePos);
	}


	void DrawScore(const Game& g, const Vector2D& pos) {
		std::string scoreHUD = "Score: " + std::to_string(g.currentScore);
		CursesUtils::PrintFormattedAtPosition(pos.x, pos.y, scoreHUD.c_str());
	}


	void DrawLives(const Game& g, const Vector2D& pos) {
		std::string livesHUD = "Lives: " + std::to_string(g.lives);
		CursesUtils::PrintFormattedAtPosition(pos.x, pos.y, livesHUD.c_str());
	}


	void DrawHead(const Snake& snake) {
		CursesUtils::PrintCharAtPosition(snake.sprite, snake.currentPosition.x, snake.currentPosition.y);
	}


	void DrawTail(const Snake& snake) {
		for (int i = 0; i < snake.tail.size(); i++)
			CursesUtils::PrintCharAtPosition(snake.tail[i].sprite, snake.tail[i].currentPosition.x, snake.tail[i].currentPosition.y);
	}


	void DrawApple(const Apple& appl) {
		CursesUtils::PrintCharAtPosition(appl.sprite, appl.position.x, appl.position.y);
	}


	void DrawText(const char* text, const Vector2D& position, const CursesUtils::Attribute attribute) {
		// Turn on the attribute.
		CursesUtils::ToggleAttribute(attribute, true);

		// Print the text at the position.
		CursesUtils::PrintFormattedAtPosition(position.x, position.y, text);

		// Turn off the attribute.
		CursesUtils::ToggleAttribute(attribute, false);
	}


	void DrawSelectedText(const char* text, const Vector2D& position) {
		// Turn on the underline.
		CursesUtils::ToggleAttribute(CursesUtils::Attribute::UNDERLINE, true);

		// Draw the text.
		CursesUtils::PrintFormattedAtPosition(position.x, position.y, text);

		// Turn underline off.
		CursesUtils::ToggleAttribute(CursesUtils::Attribute::UNDERLINE, false);

		// Turn on the blink.
		CursesUtils::ToggleAttribute(CursesUtils::Attribute::BLINK, true);

		// Draw the selection marker to the left of the text.
		CursesUtils::PrintCharAtPosition(Constants::SELECTED_BUTTON, position.x - 1, position.y);

		// Turn blink off.
		CursesUtils::ToggleAttribute(CursesUtils::Attribute::BLINK, false);
	}

} /* namespace TextSnake */
