/*
 * CursesUtils.cpp
 *
 *  Created on: Mar 24, 2020
 *      Author: danielgrieco
 */

#include "CursesUtils.h"

namespace CursesUtils {

	void InitCurses(bool hasColors, bool hasLineBuffering,
	                bool hasEcho, bool hasKeypad,
	                bool isDynamic, int cursor) {
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

		// Set the usage of colors if possible.
		if (hasColors && has_colors())	start_color();
	}


	void PrintCharAtPosition(const char character, const int x, const int y) {
		// Don't move the cursor if any of the coordinates aren't set.
		if (x == -1 || y == -1) {
			addch(character);
			return;
		}

		// Move the cursor at the given position and print the character.
		mvaddch(y, x, character);
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


	void ToggleAttribute(Attribute attr, bool isOn) {
		// Attribute/s is/are set/unset based on the given flag.
		if (isOn)	attron(static_cast<int>(attr));
		else		attroff(static_cast<int>(attr));
	}


	void ToggleColorPair(const short id, bool isOn) {
		// Color pair is set/unset based on the given flag.
		if (isOn)	attron(COLOR_PAIR(id));
		else		attroff(COLOR_PAIR(id));
	}

}
