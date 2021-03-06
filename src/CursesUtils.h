/*
 * CursesUtils.h
 *
 *  Created on: Mar 24, 2020
 *      Author: danielgrieco
 */

#ifndef CURSESUTILS_H_
#define CURSESUTILS_H_

#include <ncurses.h>

namespace CursesUtils {

	/*
	 * Arrow keys.
	 */
	enum class ArrowKey {
		UP = KEY_UP,
		RIGHT = KEY_RIGHT,
		DOWN = KEY_DOWN,
		LEFT = KEY_LEFT
	};

	/*
	 * Attributes.
	 */
	enum class Attribute {
		NORMAL = A_NORMAL,			// Normal display.
		STANDOUT = A_STANDOUT,		// Best highlighting mode of the terminal.
		UNDERLINE = A_UNDERLINE,	// Underlining.
		REVERSE = A_REVERSE,		// Reverse video.
		BLINK = A_BLINK,			// Blinking.
		DIM = A_DIM,				// Half bright.
		BOLD = A_BOLD,				// Extra bright or bold.
		PROTECT = A_PROTECT,		// Protected mode.
		INVIS = A_INVIS,			// Invisible or blank mode.
		ALTCHARSET = A_ALTCHARSET,	// Alternate character set.
		CHARTEXT = A_CHARTEXT		// Bit-mask to extract a character.
	};

	/*
	 * Colors.
	 */
	enum class Color {
		BLACK = COLOR_BLACK,
		RED = COLOR_RED,
		GREEN = COLOR_GREEN,
		YELLOW = COLOR_YELLOW,
		BLUE = COLOR_BLUE,
		MAGENTA = COLOR_MAGENTA,
		CYAN = COLOR_CYAN,
		WHITE = COLOR_WHITE
	};

	/*
	 * Initializes curses library.
	 * hasColors: If true, then curses will be set to use colors if possible. False, otherwise.
	 * hasLineBuffering: If set to true, it will be the default terminal mode.
	 * If set to false, every character will be passed to the program before hitting the newline.
	 * hasEcho: If true, characters will be displayed as they're typed.
	 * If false, characters typed won't be visible.
	 * hasKeypad: True to enable all special key function e.g. keypad.
	 * False will disable them.
	 * isDynamic: If true, curses won't wait for the user to type characters before executing other commands.
	 * If false, it will be the opposite.
	 * cursor: 0 for invisible, 1 for normal visibility, 2 for very visible.
	 */
	void InitCurses(bool hasColors = false, bool hasLineBuffering = false,
	                bool hasEcho = false, bool hasKeypad = true,
	                bool isDynamic = true, int cursor = 0);

	/*
	 * Shuts down the curses library.
	 */
	inline void ShutdownCurses() {
		// Deallocate curses screen/window
		endwin();
	}

	/*
	 * Refreshes the screen, so everything can be displayed properly.
	 */
	inline void RefreshScreen() {
		refresh();
	}

	/*
	 * Returns the number of rows on the screen.
	 */
	inline int GetRows() {
		return LINES;
	}

	/*
	 * Returns the number of columns on the screen.
	 */
	inline int GetColumns() {
		return COLS;
	}

	/*
	 * Gets the current cursor's position on the screen.
	 */
	inline void GetCursorPosition(int& x, int& y) {
		// Store the x and y position of the cursor in the given variables.
		getyx(stdscr, y, x);
	}

	/*
	 * Gets the max numbers of rows and cols on the screen.
	 * maxX: # of columns.
	 * maxY: # of rows.
	 */
	inline void GetWindowSize(int& maxX, int& maxY) {
		// Store the max x and y position of the given window.
		getmaxyx(stdscr, maxY, maxX);
	}

	/*
	 * Clears the screen from any output.
	 */
	inline void ClearScreen() {
		clear();
	}

	/*
	 * Moves the cursor at the given position on the screen.
	 * x: Horizontal position on the screen.
	 * y: Vertical position on the screen.
	 */
	inline void MoveCursorAtPosition(const int x, const int y) {
		// Set the position of the cursor to the given position.
		move(y, x);
	}

	/*
	 * Moves the cursor to the given position and prints a character (attributes can be OR'd with the character) at that position.
	 * character: The character to print.
	 * x: Horizontal position on the screen.
	 * y: Vertical position on the screen.
	 * If those are left blank then it won't move the character at a position.
	 */
	void PrintCharAtPosition(const char character, const int x = -1, const int y = -1);

	/*
	 * Moves the cursor to the given position and prints the given string at that position.
	 * cString: The string to print.
	 * x: Horizontal position on the screen.
	 * y: Vertical position on the screen.
	 * If those are left blank then it won't move the character at a position.
	 */
	void PrintStringAtPosition(const char* cString, const int x = -1, const int y = -1);

	/*
	 * Moves the cursor to the given position and prints a formatted output at that position.
	 * cString: The formatted output.
	 * x: Horizontal position on the screen.
	 * y: Vertical position on the screen.
	 */
	inline void PrintFormattedAtPosition(const int x, const int y, const char* cString) {
		// Move the cursor at the given position and print the formatted output.
		mvprintw(y, x, "%s", cString);
	}

	/*
	 * Prints a formatted output at the cursor's current position.
	 * cString: The formatted output.
	 */
	inline void PrintFormatted(const char* cString) {
		// Print the formatted output.
		printw("%s", cString);
	}

	/*
	 * Returns the key pressed on the keyboard.
	 */
	inline int GetCharacter() {
		return getch();
	}

	/*
	 * Sets the given string to the typed character sequence.
	 * cString: The string to set.
	 */
	inline void GetString(char* cString) {
		getstr(cString);
	}

	/*
	 * Toggles one or more attributes (for more attributes at the same time, bitwise OR is needed).
	 * attr: Attribute or bit mask of attributes to toggle.
	 * isOn: Should the attribute/s be active or disabled.
	 */
	void ToggleAttribute(Attribute attr, bool isOn);

	/*
	 * Makes a color pair, which consists of an ID, a foreground and a background color.
	 * id: Identifier for the pair, to be able to use it later. (Make sure to remember this id).
	 * fg: Foreground color.
	 * bg: Background color.
	 */
	inline void MakeColorPair(const short id, Color fg, Color bg) {
		init_pair(id, static_cast<short>(fg), static_cast<short>(bg));
	}

	/*
	 * Gets the color content of a pair defined by the given id.
	 * id: Identifier of the pair to get the colors of.
	 * fg: The foreground color will be stored in here.
	 * bg: The background color will be stored in here.
	 * (These are going to be short values equivalent to the values stored in the pair, they can be put back in a variable of type Color);
	 */
	inline void GetPairColors(const short id, short& fg, short& bg) {
		pair_content(id, &fg, &bg);
	}

	/*
	 * Toggles a color pair.
	 * id: Identifier of the pair to toggle on/off.
	 * isOn: Should the color pair be active or disabled.
	 */
	void ToggleColorPair(const short id, bool isOn);

}

#endif /* CURSESUTILS_H_ */
