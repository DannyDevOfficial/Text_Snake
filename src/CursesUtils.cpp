/*
 * CursesUtils.cpp
 *
 *  Created on: Mar 24, 2020
 *      Author: danielgrieco
 */

#include "CursesUtils.h"

#include <ncurses.h>

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


	void ShutdownCurses() {
		// Deallocate curses screen/window
		endwin();
	}


	void RefreshScreen() {
		refresh();
	}


	int GetRows() {
		return LINES;
	}


	int GetColumns() {
		return COLS;
	}


	void GetWindowSize(int& maxX, int& maxY) {
		// Store the max x and y position of the given window.
		getmaxyx(stdscr, maxY, maxX);
	}


	void MoveCursorAtPosition(const int x, const int y) {
		// Set the position of the cursor to the given position.
		move(y, x);
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


	void PrintFormattedAtPosition(const int x, const int y, const char* cString) {
		// Move the cursor at the given position and print the formatted output.
		mvprintw(y, x, cString);
	}


	void PrintFormatted(const char* cString) {
		// Print the formatted output.
		printw(cString);
	}

}
