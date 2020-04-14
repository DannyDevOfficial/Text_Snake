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
		static const char ENTER_KEY = '\n';
		static const unsigned int BACKSPACE_KEY = 127;
		static const char SELECTED_BUTTON = '>';
		static const int X_MIN = 0;
		static const int Y_MIN = 2;
		static const unsigned short SCORE_HUD_WIDTH = 11;
		static const unsigned short OFFSET_FROM_MIDSCREEN = 5;
		static const unsigned int BASE_APPLE_POINTS = 10;
		static const unsigned int SCORE_MULTIPLIER = 10;
		static const unsigned short INTRO_TEXT_OFFSET = 7;
		static const unsigned short MENU_TEXT_DIST = 2;
		static const unsigned short FIRST_ENTRY_TEXT_OFFSET = 2;
		static const unsigned int TOTAL_MAIN_MENU_ENTRIES = 2;
		static const unsigned short TOTAL_DIGITS = 10;
		static const unsigned short TOTAL_LETTERS = 26;
		static const unsigned short START_CAP_LETTERS = 65;
		static const unsigned short START_LOW_LETTERS = 97;
		static const unsigned short START_DIGITS = 48;
		static const char* HIGH_SCORES_FILENAME = "HighScores.bin";
		static const unsigned short MAX_HIGH_SCORES_ON_SCREEN = 8;


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
		SHOW_MAIN_GAME,
		SHOW_GAME_OVER,
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
	 * Represents the directions taken by the menu selector.
	 */
	enum class SelectorDirection {
		STILL,
		UP,
		DOWN
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
	 * Menu entry used in main menu.
	 */
	struct MenuEntry {
		std::string text;
		Vector2D position;
		CursesUtils::Attribute attribute;
		bool isSelected;
		Screen relatedScreen;
	};

	/*
	 * Represents the game e.g. states, scores etc.
	 */
	struct Game {
		unsigned short lives;
		unsigned int currentScore;
		Score finalScore;
		Apple apple;
		bool isAppleOnScreen;
		std::vector<MenuEntry> mainMenuEntries;
		SelectorDirection selectorDirection;
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
	 * Initializes everything as a brand new instance.
	 * It's used when the game first runs.
	 * gm: Instance of the game.
	 * snk: Instance of the snake.
	 */
	void FirstInit(Game& gm, Snake& snk);

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
	 * Initializes all the menu entries.
	 * game: Instance of the game.
	 */
	void InitMenu(Game& game);

	/*
	 * Analyzes the input and act accordingly.
	 * inpt: Input to write to.
	 * g: Instance of the game.
	 * s: Instance of the snake.
	 */
	void HandleInput(int& inpt, Game& g, Snake& s);

	/*
	 * Decides what state to go to when the enter key is pressed.
	 * game: Game instance.
	 * snake: Snake instance.
	 */
	void EnterKeyPressed(Game& game, Snake& snake);

	/*
	 * Saves the high scores to a file.
	 * gm: Instance of the game.
	 */
	void SaveHighScores(const Game& gm);

	/*
	 * Compares two scores and returns the higher one.
	 * score1: First score.
	 * score2: Second score.
	 */
	bool GetHigher(const Score& score1, const Score& score2);

	/*
	 * Loads the high scores into the vector.
	 * gm: Instance of the game.
	 */
	void LoadHighScores(Game& gm);

	/*
	 * Tweaks the FPS based on the snake direction and returns it.
	 * snake: Instance of the snake.
	 */
	unsigned int AdjustFPSbasedOnDirection(const Snake& snake);

	/*
	 * Calls all the functions that deal with game updates.
	 * g: Instance of the current game.
	 * s: Instance of the snake.
	 * in: The current input from the user.
	 */
	void Update(Game& g, Snake& s, int in);

	/*
	 * Draws the game to the screen.
	 * g: Instance of the game.
	 * s: Instance of the snake.
	 */
	void Draw(const Game& g, const Snake& s);

	/*
	 * Changes the game's current screen based on the current state.
	 * game: Instance of the game.
	 */
	void UpdateScreen(Game& game);

	/*
	 * Runs the main menu related logic.
	 * game: Game instance.
	 */
	void UpdateMainMenu(Game& game);

	/*
	 * Runs the game over related logic.
	 * game: Instance of the game.
	 * input: User's input.
	 */
	void UpdateGameOver(Game& game, int input);

	/*
	 * Runs the main game related logic.
	 * game: Instance of the game.
	 * snake: Instance of the snake.
	 */
	inline void UpdateMainGame(Game& game, Snake& snake);

	/*
	 * Draws the main menu related things.
	 * game: Instance of the game.
	 */
	void DrawMainMenu(const Game& game);

	/*
	 * Draws the game related things.
	 * game: Instance of the game.
	 * snake: Instance of the snake.
	 */
	void DrawMainGame(const Game& game, const Snake& snake);

	/*
	 * Draws the game over screen.
	 * game: Instance of the game.
	 */
	void DrawGameOver(const Game& game);

	/*
	 * Draws the high scores screen.
	 * game: Instance of the game.
	 */
	void DrawHighScores(const Game& game);

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
	 * Calculates the score based on the snake's length.
	 * snake: Instance of the snake.
	 */
	unsigned int CalcScore(const Snake& snake);

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

	/*
	 * Draws the given text at the given position with the given attribute.
	 * text: The text to write.
	 * position: Where to draw the text.
	 * attribute: What attribute to use. (Use NORMAL for no attributes)
	 */
	inline void DrawText(const char* text, const Vector2D& position, const CursesUtils::Attribute attribute);

	/*
	 * Draws the given text at the given position underlined.
	 * It also draws a little blinking selection marker next to it.
	 * text: Text to write.
	 * position: Where to draw the text.
	 */
	void DrawSelectedText(const char* text, const Vector2D& position);


} /* namespace TextSnake */

#endif /* SNAKEUTILS_H_ */
