#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

#define VM_VRAM_ADDRESS 0xE0000000 // by running prog/lab5 with -args "mode 0x105"

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
int vg_square (unsigned long x, unsigned long y, unsigned long size, unsigned long color);

/**
 * @brief Swaps content of secondary_buffer with the video memory
 */
void vg_secondary_to_vmem();

void vg_clear_secondary();

/**
* @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
*
* @return 0 upon success, non-zero upon failure
*/
int vg_exit(void);

 
#endif /* __VIDEO_GR_H */
/** @} end of video_gr */
