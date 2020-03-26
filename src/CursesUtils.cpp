/*
 * CursesUtils.cpp
 *
 *  Created on: Mar 24, 2020
 *      Author: danielgrieco
 */

#include "CursesUtils.h"

namespace CursesUtils {

	void InitCurses(bool hasLineBuffering, bool hasEcho, bool hasKeypad, bool isDynamic, int cursor) {
		// Initialize curses screen
		initscr();

		// Remove line buffering
		if (!hasLineBuffering)	raw();

		// Remove characters display
		if (!hasEcho)	noecho();

		// Enable/Disable special keys
		keypad(stdscr, hasKeypad);

		// Enable/Disable the wait time normally included when waiting for user's input
		nodelay(stdscr, isDynamic);

		// Set the cursor to visible or invisible
		curs_set(cursor);
	}


	void PrintCharAtPosition(const char character, const int x, const int y) {
		// Don't move the cursor if any of the coordinates aren't set.
		if (x == -1 || y == -1) {
			addch(character);
			return;
		}

		// Move the cursor at the given position and print the character.
		mvaddch(character, y, x);
	}


	void PrintStringAtPosition(const char* cString, const int x, const int y) {
		// Don't move the cursor if any of the coordinates aren't set.
		if (x == -1 || y == -1) {
			addstr(cString);
			return;
		}

		// Move the cursor at the given position and print the string.
		mvaddstr(y, x, cString);
	}

}
