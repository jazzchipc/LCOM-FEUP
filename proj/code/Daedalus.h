#pragma once

/** @defgroup Deadalus Daedalus
 * @{ Functions that control the game
 */

#include "bitmap.h"
#include "sprite.h"
#include "mouse.h"
#include "keyboard.h"
#include "rtc.h"
#include "date.h"
#include "video_gr.h"
#include "maze.h"
#include "timer.h"

typedef struct
{
	int KBD_IRQ_SET;
	int MOUSE_IRQ_SET;
	int TIMER_IRQ_SET;
	int RTC_IRQ_SET;

	/**
	 * 	This array is used to store the keys that are "active". I.e. if a key has been pressed, but not released, the corresponding
	 * number will be 1. When it's released, it'll be 0.
	 * [0] - Ignore for practical use
	 * [1] - W Key
	 * [2] - S Key
	 * [3] - A Key
	 * [4] - D Key
	 */
	char activeKeys[5];

	long int visibleArea[4];

	unsigned long pressed_key;

	int stop;
	int draw;
	int win;
	int boss;
	int year;
	int day;
	int month;

	Crosshair* ch;
	Maze* maze;
	Doll* doll;

}Daedalus;

/**
 * @brief It creates a Daedalus' struct object, that initializes the game base on a maze.
 * @return A Daedalus object, that is basically the beginning of the game.
 */
Daedalus* initDaedalus();

/**
 * @brief Prints the starting game screen, with the game's title.
 * @param dae Daedalus instance in course.
 * @return Has no return.
 */
void showStartScreen(Daedalus* dae);

/**
 * @brief It refreshes every member of the Daedalus struct, using the IRQ interruptions
 * @param dae Daedalus instance in course.
 * @return Has no return.
 */
void updateDaedalus(Daedalus* dae);

/**
 * @brief Draws the whole game and its objects, according to the time that has passed since the last draw
 * @param dae Daedalus instance in course.
 * @param elapsedTime The time of one tick (1/60s = 60Hz = 60 fps)
 * @return Has no return.
 */
void drawDaedalus(Daedalus* dae, float elapsedTime);

/**
 * @brief Function that either ends the whole game, or make the transition to the next maze.
 * @param dae Daedalus instance in course.
 * @return 1 if the game should end. 0 if the maze is changing.
 */
int stopDaedalus(Daedalus* dae);

/**
 * @brief It refreshes the "pressed_key" member of the Daedalus instance. This is how we receive keyboard controls.
 * @param dae Daedalus instance in course.
 * @return Has no return.
 */
void updateDaedalusKey(Daedalus *dae);

/**
 * @brief This function is a way of knowing which "WASD" key is being pressed. We update the "activeKeys" array accordingly.
 * @param dae Daedalus instance in course.
 * @return 1 for W, -1 for W break. 2 for S, -2 for S break. 3 for A, -3 for A break. 4 for D, -4 for D break.
 */
int pressedKeyDaedalus(Daedalus *dae);

/**
 * @brief Updates doll's position according to keyboard input.
 * @param dae Daedalus instance in course.
 * @return Has no return.
 */
void updateDaedalusDoll(Daedalus* dae);

/**
 * @brief Checks if the user has reached the end of the maze (green area). Will set dae->win to 1.
 * @param dae Daedalus instance in course.
 * @return Has no return.
 */
void updateDaedalusWin(Daedalus* dae);

/**
 * @brief If the doll collides with a maze wall, it returns the doll to the start position.
 * @param dae Daedalus instance in course.
 * @return Has no return.
 */
void updateDaedalusCollision(Daedalus* dae);

//void updateDaedalusBoss(Daedalus* dae);

/**
 * For debug purposes.
 */
int testDaedalus(Daedalus *dae);


//int questOne();
