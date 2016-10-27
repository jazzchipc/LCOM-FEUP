#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @{ Functions related to the video_graphics
 */


#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

#define VM_VRAM_ADDRESS 0xE0000000 // by running prog/lab5 with -args "mode 0x105"

/******************
 * LIST OF COLORS *
 ******************/

#define RED 		(getIntFromRGB(255, 0, 0))		// for the charcter sprite
#define WHITE 		(getIntFromRGB(255, 255, 255))	// for the maze walls
#define MUSTARD		(getIntFromRGB(224,192,64))		// for the fonts sprite
#define GREEN		(getIntFromRGB(0, 207, 49))		// for the end zone
#define PURE_GREEN	(getIntFromRGB(0, 255, 0))		// for the boss sprites

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

/**
 * @brief Colors pixel in coordinates (x,y) with color given
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_color_pixel(unsigned long x, unsigned long y, unsigned long color);

/**
 * @brief Colors a size*size square on (x,y) coordinates with given color on the screen
 *
 * @return 0 upon success, non-zero upon failure
 */
int vg_store_square (unsigned long x, unsigned long y, unsigned long size, unsigned long color);

/**
 * @brief Swaps content of secondary_buffer with the video memory
 */
void vg_secondary_to_video_mem();

/**
 * @brief Sets all secondary_buffer to 0's, creating an empty array (totally black output)
 */
void vg_clear_secondary();

/**
 * @brief Returns horizontal resolution of the mode in which the graphics were initialized
 *
 * @return Number of Pixels of a line in the current graphics mode (probably 1024)
 */
int getHorResolution();

/**
 * @brief Returns vertical resolution of the mode in which the graphics were initialized
 *
 * @return Number of Pixels of a column in the current graphics mode (probably 768)
 */
int getVerResolution();

/**
 * @return The first address of the secondary_buffer
 */
char* getSecondaryVideoMem();

/**
 * @return The first address of the video memory
 */
char* getVideoMem();

/**
 * @brief Converts a color on RGB to the corresponding INT that's stored in the video memory, according to the 16-bit (5:6:5) standard
 *
 * @return The INT value corresponding to RGB (r, g, b). -1 if the values are not valid.
 */
int getIntFromRGB (int r, int g, int b);

/**
 * @brief Gets the color that's store in (x,y) coordinates of given buffer (char array)
 *
 * @param x Horizontal coordinate
 * @param y Vertical coordinate
 * @param buffer Buffer to search
 *
 * @return Value of the color store on that pixel
 */
int getColorFromCoord(int x, int y, char* buffer);

int compareColors(int color1, int color2);

/**
 * @brief Since while we were testing colors, there were some differences between the RGB code our image creating software (GIMP)
 * was giving, and the color that was actually stored in the video memory, this function serves the purpose of giving a 100 error
 * to detect if a color is the same or not. Basically, it considers some shades of a color the same color, for practical purposes.
 *
 * @param x Horizontal pixel coordinate
 * @param y Vertical pixel coordinate
 * @param buffer Buffer to test
 * @param color Value of the color (extracted from getIntFromRGB())
 */
int isSameColor(int x, int y, char* buffer, int color);

/**
 * @brief Stores a XPM in the secondary_buffer
 *
 * @return 0 if it succeeds
 */
int storeXPM(unsigned short xi, unsigned short yi, char* xpm[]);

/**
* @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
*
* @return 0 upon success, non-zero upon failure
*/
int vg_exit(void);

 
#endif /* __VIDEO_GR_H */
/** @} end of video_gr */
