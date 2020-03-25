/*
 * CursesUtils.cpp
 *
 *  Created on: Mar 24, 2020
 *      Author: danielgrieco
 */

#include "CursesUtils.h"

#include <ncurses.h>

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
