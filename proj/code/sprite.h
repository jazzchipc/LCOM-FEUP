#ifndef _SPRITE2_H_
#define _SPRITE2_H_

/** @defgroup sprite sprite
 * @{ Functions related to the sprites used in the game
 */


#include "bitmap.h"

/****************
 * BASE SPRITES *
 ****************/

/**
 * Our Sprite struct is a bit different, because we're working with bitmaps.
 */
typedef struct {
  int xi,yi, xf, yf;	/**< current sprite position */
  int width, height;	/**< sprite dimensions */
  float xspeed, yspeed;	/**< current speeds in the x and y direction */
  Bitmap* map;			/**< the bitmap (see "bitmap.h") */

  int transparentColor; // This color will be considered transparent for this sprite
} Sprite;

/**
 * 	@brief Creates a sprite from a pointer to a Bitmap "map". "map" has been created with loadBitmap() function.
 * The beginning speeds are equal to 0. The dimension of the sprite is the same as the bitmap.
 */
Sprite* createSprite(Bitmap* map, int x, int y, int transparentColor);

/**
 *  @brief Draws a sprite. Essentially, it draws it's bitmap, using drawBitmapIg().
 */
void drawSprite(Sprite* sp);

/**
 *  @brief Updates sprites (x,y) coordinates according to its speed and elapsedTime
 */
void moveSprite(Sprite *sp, float elapsedTime);

/**
 *  @brief Frees the memory space used by the sprite
 */
void destroySprite(Sprite* sp);

/****************
 * DOLL SPRITES *
 ****************/

//Paths to bitmaps that'll be used for the doll
static const char* dollN = "/home/lcom/lcom1516-t2g03/proj/images/Sprite6.bmp";	//doll facing North
static const char* dollS = "/home/lcom/lcom1516-t2g03/proj/images/Sprite3.bmp";	//doll facing South
static const char* dollW1 ="/home/lcom/lcom1516-t2g03/proj/images/Sprite1.bmp";	//doll facing West standing still
static const char* dollW2 ="/home/lcom/lcom1516-t2g03/proj/images/Sprite2.bmp";	//doll facing West walking
static const char* dollE1 ="/home/lcom/lcom1516-t2g03/proj/images/Sprite5.bmp";	//doll facing East standing still
static const char* dollE2 ="/home/lcom/lcom1516-t2g03/proj/images/Sprite4.bmp";	//doll facing East walking

/**
 * Doll struct
 */
typedef struct
{
	Sprite* sprite;	// sprite that characterizes the doll

	int direction;	// direction the doll is facing. 1 = North, 2 = South, 3 = West, 4 = East
	int collision;	// 1 if the doll collides with the walls. 0 otherwise
	int end;		// 1 if the doll has reached the end. 0 otherwise
} Doll;

/**
 * Doll Functions
 */

/**
 * @brief Creates a doll in the given coordinates. The sprite for the doll is one from the bitmaps above.
 */
Doll* createDoll(int xi, int yi);

/**
 * @brief Uses drawSprite to draw the doll.
 */
void drawDoll(Doll* doll);

/**
 * @brief According to the keys being pressed, it updates the doll's speed and direction.
 * @param doll Doll instance that'll be changed.
 * @param directions Char array that contains the directions that the doll should follow. This results from the active keys array.
 */
void changeDollDirection(Doll* doll, char* directions);

/**
 * @brief When the doll changes direction, this changes its sprite, to face the direction it's going.
 */
void changeDollSprite(Doll* doll, Sprite* sp);

/**
 * @brief Updates doll's position according to it's speed and the time that has passed since the last update.
 * @param doll Doll instance that'll be changed.
 * @param elaspedTime Cycle time.
 */
void moveDoll (Doll* doll, float elapsedTime);

/**
 * @brief Frees up memory by deleting a doll instance.
 */
void destroyDoll(Doll* doll);

/****************
 * BOSS SPRITES *
 ****************/

//Path to bitmaps
static const char* eggMan = "/home/lcom/lcom1516-t2g03/proj/images/Eggman.bmp";	//doll facing North

typedef struct
{
	Sprite* sprite; // sprite that'll be used to characterize the boss

	int xi, yi, xf, yf; // these are the coordinates that'll define the are that triggers the boss
}Boss;

Boss* createBoss(Sprite* sp, int xi, int yi, int xf, int yf);

void drawBoss(Boss* boss);

void destroyBoss(Boss* boss);

void writeSentece(int x, int y, char sentence[]);

#endif
