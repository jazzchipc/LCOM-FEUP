/*
*Creates a new sprite with pixmap "pic", random speeds
*(not zero) and position (within the screen limits), and
*draws it in memory whose address is "base";
*Returns NULL on invalid pixmap.
*/

#include "read_xpm.h"
#include "sprite.h"
#include "video_gr.h"

#define H_RES 1024
#define V_RES 768

Sprite* create_sprite(char* pic[], int x, int y, int xspeed, int yspeed)
{
	//allocate space for the "object"
	Sprite* sp = (Sprite*) malloc (sizeof(Sprite));
	if( sp == NULL )
		return NULL;
	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if( sp->map == NULL ) {
		free(sp);
		return NULL;
	}

	sp->xi =x;
	sp->yi =y;

	sp->xspeed = xspeed;
	sp->yspeed = yspeed;

	return sp;
}

void destroy_sprite(Sprite* sp)
{
	if( sp == NULL )
		return;
	free(sp->map);
	free(sp);
	sp = NULL;     // hopeless: pointer is passed by value
}
void animate_sprite(Sprite* sp, float time)
{
	sp->xf = sp->xi + (sp->xspeed * time);
	sp->yf = sp->yi + (sp->yspeed * time);

	draw_sprite (sp);
}

/*Some useful non-visible functions*/

int check_collision(Sprite* sp, char* base)
{
	//...
}
void draw_sprite(Sprite* sp)
{
	vg_store_pixmap (sp->xf, sp->yf, sp->width, sp->height, sp->map);
}
