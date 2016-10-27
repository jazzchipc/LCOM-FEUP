#ifndef _MAZE_H
#define _MAZE_H

/** @defgroup maze maze
 * @{ Functions related to a maze struct used in the game
 */

#include "keyboard.h"
#include "video_gr.h"
#include "bitmap.h"
#include "sprite.h"

typedef struct
{
	Bitmap* map;
	int xiPos;
	int yiPos;

	int bossCounter;
	Boss* bosses[];
}Maze;

/**
 * @brief Creates a new Maze object from a bitmap and an initial position (0,0)
 * @param bitmapPath The path to the bmp that is the maze.
 * @param xi 0 for 1024x768 mazes
 * @param yi 0 for 1024x768 mazes
 * @return Maze object created.
 */
Maze* createMaze(const char* bitmapPath, int xi, int yi);

/**
 * Useless.
 */
Maze* addMazeBoss(Maze* m, Boss* boss);
int isBoss(Maze* m, int x, int y);

/**
 * @brief Stores the maze in the secondary video memory.
 * @param m Maze to be drawn.
 * @return Has no return.
 */
void drawMaze(Maze* m);

/**
 * @brief Detects which color is the maze in given coordinates. Serves the purpose of knowing where the doll is.
 * @param m Maze in question.
 * @param x Horizontal pixel coordinate.
 * @param y Vertical pixel coordinate.
 * @return The decimal value of the color.
 */
int mazeColorInCoord(Maze* m, unsigned x, unsigned y);

/**
 * @brief Deletes a maze and frees up memory.
 * @param m Maze to be deleted.
 * @return Has no return.
 */
void destroyMaze(Maze* m);

#endif

