/*
 * CursesUtils.h
 *
 *  Created on: Mar 24, 2020
 *      Author: danielgrieco
 */

#ifndef CURSESUTILS_H_
#define CURSESUTILS_H_

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
void ShutdownCurses();


#endif /* CURSESUTILS_H_ */
