#include "video_gr.h"
#include <math.h>
/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES            1024
#define V_RES		  	768
#define BITS_PER_PIXEL	  8

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

static char *secondary_buffer; 	/* This will be a pointer to a chars (bytes) in memory where changes to
 	 	 	 	 	 	 	 	 pixels will be stored.
 	 	 	 	 	 	 	 	 Then it will be used to apply the double buffering technique.
 	 	 	 	 	 	 	 	 It has to be freed when it's no longer needed.*/
/*
 * Initiates Vgmode
 */
void *vg_init(unsigned short mode){

	struct reg86u reg;

	reg.u.b.intno = VBE_INT;
	reg.u.w.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
	reg.u.w.bx = 1<<14 | mode; // set bit 14: linear framebuffer

	vbe_mode_info_t vbe_info;

	if (sys_int86(&reg) != OK)
	{
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	if (reg.u.w.ax != VBE_SUPP | VBE_SUCCESS) // ax output is the VBE return status
	{
		printf ("function call is not supported by VBE\n");
		return NULL;
	}

	if (vbe_get_mode_info(mode, &vbe_info) != 0)
	{
		printf ("vbe_get_mode_info returned non-zero value\n");
		return NULL;
	}


	int r;
	struct mem_range mr;
	unsigned int mem_size;

	// see vbe.h for definition of vbe_mode_info_t struct

	h_res = vbe_info.XResolution;
	v_res = vbe_info.YResolution;
	bits_per_pixel = vbe_info.BitsPerPixel;

	mem_size = h_res * v_res * bits_per_pixel;

	/* Allow memory mapping */

	mr.mr_base =  vbe_info.PhysBasePtr;
	mr.mr_limit = mr.mr_base + mem_size;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void * )mr.mr_base, mem_size);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");

	secondary_buffer = malloc(h_res * v_res * bits_per_pixel / 8);

	if (secondary_buffer == NULL)
	{
		printf ("buffer could not be created.\n");
	}

	/* Save text mode resolution */

	return video_mem;

}
/*
 * Colours a certain pixel
 */
int vg_color_pixel(unsigned long x, unsigned long y, unsigned long color)
{
	if (x > h_res || y > v_res || y < 0 || x < 0)
	{
		printf ("Invalid (x,y) coordinates.\n");
		return 1;
	}

	*(secondary_buffer + (x + y * h_res) * bits_per_pixel / 8) = color; /* the byte in the (x,y) linear position
																		in secondary_buffer will store the color code*/

	return 0;

}
/*
 * Creates square for testing purposes
 */
int vg_store_square (unsigned long x, unsigned long y, unsigned long size, unsigned long color)
{
	unsigned long i, j;
	for (i = x; i < x + size; i++)
	{
		for (j = y; j < y + size; j++)
		{
			if(vg_color_pixel(i, j, color))
			{
				printf ("vg_color_pixel() failed.\n");
				return 1;
			}
		}
	}
}
/*
 * Creates line for testing purposes
 */
int vg_store_line (unsigned short xi, unsigned short yi,
		           unsigned short xf, unsigned short yf, unsigned long color)
{
	double m = (yf - yi)/(xf - xi);	// slope of the line

	long i, j;
	long yn = yi;
	long xn = xi;

	if(vg_color_pixel(xn = xn , yn, color))
	{
		printf("vg_color_pixel() failed. \n");
		return 1;
	}

	if ((xi == xf) && (yi == yf))
		return 0;	// just colors one pixel

	// if the line is vertical
	if ((xi == xf) && (yi != yf))
	{
		int i, j;

		if (yi < yf)
		{
			i = yi;
			j = yf;
		}

		else
		{
			i = yf;
			j = yi;
		}

		for (i; i < j; i++)
			if(vg_color_pixel(xi, i, color))
			{
				printf ("vg_color_pixel() failed.\n");
				return 1;
			}

		return 0;
	}

	// if the line is horizontal
	if (m == 0)
	{
		int i, j;

		if (xi < xf)
		{
			i = xi;
			j = xf;
		}

		else
		{
			i = xf;
			j = xi;
		}

		for (i; i < j; i++)
			if(vg_color_pixel(i, yi, color))
			{
				printf ("vg_color_pixel() failed.\n");
				return 1;
			}
	}

	/*if (m > 1)
	{
		if(yi < yf){
			for(j = yi; j < yf;j++ ){
				float xn_f =xn;

				xn_f = xn_f + 1/m;
				long pixel_x = lroundf(xn_f);

				if(vg_color_pixel( pixel_x, j, color)){
					printf("vg_color_pixel() failed. \n");
					return 1;
				}
			}
		}
		else{
			for(j = yi; j > yf;j-- ){
				if(vg_color_pixel(xn = xn - 1/m, j, color)){
					printf("vg_color_pixel() failed. \n");
					return 1;
				}
			}
		}
	}*/

	else if(m > 0)
	{

		if(xi <  xf){
			for(i = xi;i < xf; i++ ){
				if(vg_color_pixel(i, yn = yn + m, color))
				{
					printf ("vg_color_pixel() failed.\n");
					return 1;
				}
			}
		}
		else{

			for(i = xi;i >= xf; i-- ){
				if(vg_color_pixel(i, yn = yn + m, color))
				{
					printf ("vg_color_pixel() failed.\n");
					return 1;

				}
			}
		}
	}
/*
	else if(m < -1){
		if(yi < yf){
				for(j = yi; j < yf;j++ ){
					if(vg_color_pixel(xn = xn - 1/m, j, color)){
						printf("vg_color_pixel() failed. \n");
						return 1;
					}
				}
			}
				else{
					for(j = yi; j > yf;j-- ){
								if(vg_color_pixel(xn = xn + 1/m, j, color)){
									printf("vg_color_pixel() failed. \n");
									return 1;
								}
				}
			}


	}*/
	else if(m < 0)
	{
		if(xi <  xf)
		{
			for(i = xi;i < xf; i++ ){
				if(vg_color_pixel(i, yn = yn + m, color))
				{
					printf ("vg_color_pixel() failed.\n");
					return 1;
				}
			}
		}
		else
		{
			for(i = xi;i >= xf; i-- ){
				if(vg_color_pixel(i,yn = yn + m, color))
				{
					printf ("vg_color_pixel() failed.\n");
					return 1;

				}
			}
		}
	}

	return 0;
}
/*
 * Stores pixelmap
 */
int vg_store_pixmap (unsigned short xi, unsigned short yi, unsigned long width,
		unsigned long height, char *pixmap)
{
	unsigned long i, j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			vg_color_pixel(j + xi, i + yi, *(pixmap + ((i * width + j) * bits_per_pixel /8)));
			//(i) and (j) are the coordinates in the pixmap if we consider the top left corner (0,0)
		}
	}
}

void vg_secondary_to_video_mem()
{
	memcpy (video_mem, secondary_buffer, h_res * v_res * bits_per_pixel / 8);

	// the buffer with the changes made to the color of the pixel will be switched with
	// the video memory
}

void vg_clear_secondary()
{
	memset (secondary_buffer, 0 ,h_res*v_res*bits_per_pixel / 8);

	// this way, the secondary buffer represents all BLACK
}

int getHorResolution()
{
	return h_res;
}

int getVerResolution()
{
	return v_res;
}

char* getSecondaryVideoMem()
{
	return secondary_buffer;
}

char* getVideoMem()
{
	return video_mem;
}
/*
 * Having R, G and B turns into usable INT
 */
int getIntFromRGB(int r, int g, int b)
{
	if (r < 0 || g < 0 || b < 0)
		return -1;

	if (r > 255 || g > 255 || b > 255)
		return -1;

	//Red has 5 bits so it's on a scale from 0 to 31
	//r is to 255, as red is to 31
	r = r * 31 / 255;

	//Green has 1 extra bit, because humans' eyes are more sensitive to green shades

	g = g * 63 / 255;

	//Blue has 5 bits

	b = b * 31 / 255;

	//This is how they're stored in memory (5:6:5)
	int red = r << (6+5);
	int green = g << (5);
	int blue = b;

	return (red+green+blue);

}
/*
 * Gets color Int form a certain pixel
 */
int getColorFromCoord(int x, int y, char* buffer)
{
	x = x * 2;	//as each pixel has 2 bytes
	y = y * 2;

	int firstByte = buffer[(y * h_res) + x + 1] << 8;	// most significant byte, so it's shifted 8 cases to the left
	int secondByte = buffer[(y * h_res) + x + 2];		// least significant byte

	return (firstByte+secondByte);
}
/*
 * Checks if two colours are the same
 */
int compareColors(int color1, int color2)
{
	int diff = color1 - color2;

	if (diff < 100 && diff > -100)
		return 1;

	else
		return 0;
}
/*
 * Checks if a certain pixel is of a certain color
 */
int isSameColor(int x, int y, char* buffer, int color)
{
	int diff = getColorFromCoord(x, y, buffer) - color;

	if (diff < 100 && diff > -100)
		return 1;

	else
		return 0;
}
/*
 * Stores XPM
 */
/*int storeXPM(unsigned short xi, unsigned short yi, char* xpm[])
{
	int width, height;

	char* pixmap = read_xpm(xpm, &width, &height); //TO DO: ASK THE PROFESSOR IF THIS NEEDS TO SUIT THE RESOLUTION OF EVERY MODE

	vg_store_pixmap(xi, yi, width, height, pixmap); // the function that'll store the pixmap on the secondary buffer

	free(pixmap); // to avoid memory problems from the allocation made in read_xpm()

	return 0;
}*/
/*
 * Exits video mode
 */
int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  free(secondary_buffer);

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}
