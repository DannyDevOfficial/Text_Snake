/*
 * CursesUtils.h
 *
 *  Created on: Mar 24, 2020
 *      Author: danielgrieco
 */

#ifndef CURSESUTILS_H_
#define CURSESUTILS_H_

namespace CursesUtils {

	/*
	 * Initializes curses library.
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
	void InitCurses(bool hasLineBuffering = false, bool hasEcho = false, bool hasKeypad = true, bool isDynamic = true, int cursor = 0);

	/*
	 * Shuts down the curses library.
	 */
	void ShutdownCurses();

	/*
	 * Refreshes the screen, so everything can be displayed properly.
	 */
	void RefreshScreen();

	/*
	 * Returns the number of rows on the screen.
	 */
	int GetRows();

	/*
	 * Returns the number of columns on the screen.
	 */
	int GetColumns();

	/*
	 * Gets the max numbers of rows and cols on the screen.
	 * maxX: # of columns.
	 * maxY: # of rows.
	 */
	void GetWindowSize(int& maxX, int& maxY);

	/*
	 * Moves the cursor at the given position on the screen.
	 * x: Horizontal position on the screen.
	 * y: Vertical position on the screen.
	 */
	void MoveCursorAtPosition(const int x, const int y);

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
	 * cString: The formatted output (Make sure to use this the same way you use printf).
	 * x: Horizontal position on the screen.
	 * y: Vertical position on the screen.
	 */
	void PrintFormattedAtPosition(const int x, const int y, const char* cString);

	/*
	 * Prints a formatted output at the cursor's current position.
	 * cString: The formatted output (Make sure to use this the same way you use printf).
	 */
	void PrintFormatted(const char* cString);

}

#endif /* CURSESUTILS_H_ */
