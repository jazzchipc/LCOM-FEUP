#include <minix/syslib.h>
#include <minix/drivers.h>

#include "test5.h"
#include "video_gr.h"
#include "timer.h"
#include "keyboard.h"
#include "xpm.h"
#include "read_xpm.h"
#include "sprite.h"

void *test_init(unsigned short mode, unsigned short delay) {
	
	unsigned int c; // c = counter
	unsigned int freq = 60; // frequency

	const int id = TIMER_HOOK_ID;

	int ipc_status;
	message msg;
	int r;

	int irq_set = timer_subscribe_int();

	vg_init(mode);

	while (c < delay * freq) // it iterates 60 times per second
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf ("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{
					c++;
				}

				break;

			default: break;

			}
		}

	}
	vg_exit();
	timer_unsubscribe_int();
}

int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color)
{
	lm_init();

	unsigned short mode = VBE_GRAPHICS_1024_768_256;

	if(vg_init(mode) == NULL)
	{
		printf ("video_mem is a null pointer. vg_init() may have failed.\n");
		return 1;
	}

	vg_store_square(x, y, size, color);

	vg_secondary_to_video_mem();

	/*Beginning of the cycle to press ESC to exit*/

	int irq_set = kbc_subscribe();

	int ipc_status;
	message msg;
	int r;

	unsigned long key;

	while (key != ESC_KEY)
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
				{
					printf ("driver_receive failed with: %d", r);
					continue;
				}

				if (is_ipc_notify(ipc_status))
				{
					switch (_ENDPOINT_P(msg.m_source))
					{
					case HARDWARE:
						if (msg.NOTIFY_ARG & irq_set)
						{

							key = kbc_read_key();

						}

						break;

					default: break;

					}
				}

			}

	kbc_unsubscribe();

	if (vg_exit())
	{
		printf ("vg_exit() failed.\n");
		return 1;
	}

	return 0;
	
}

int test_line(unsigned short xi, unsigned short yi, 
		           unsigned short xf, unsigned short yf, unsigned long color) {

	lm_init();

	unsigned short mode = VBE_GRAPHICS_1024_768_256;

	if (vg_init(mode) == NULL)
	{
		printf ("video_mem is a null pointer. vg_init() may have failed.\n");
		return 1;
	}

	vg_store_line (xi,yi,xf,yf,color);

	vg_secondary_to_video_mem();

	/*Beginning of the cycle to press ESC to exit*/

	int irq_set = kbc_subscribe();

	int ipc_status;
	message msg;
	int r;

	unsigned long key;

	while (key != ESC_KEY)
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf ("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{

					key = kbc_read_key();

				}

				break;

			default: break;

			}
		}

	}

	kbc_unsubscribe();

	if (vg_exit())
	{
		printf ("vg_exit() failed.\n");
		return 1;
	}

	return 0;

}

char** pixmap_id(int id) // function that'll convert 1, 2... to the pointer of a pixmap in xpm.h
{
	if (id == 1)
		return pic1;
	if (id == 2)
		return pic2;
	if (id == 3)
		return pic3;
	if (id == 4)
		return cross;
	if (id == 5)
		return penguin;

	else
	{
		printf ("Invalid pixmap_id(int id)\n");
		return NULL;
	}
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[])
{

	lm_init();

	unsigned short mode = VBE_GRAPHICS_1024_768_256;

	if (vg_init(mode) == NULL)
	{
		printf ("video_mem is a null pointer. vg_init() may have failed.\n");
		return 1;
	}

	int width, height;

	char* pixmap = read_xpm(xpm, &width, &height); //TO DO: ASK THE PROFESSOR IF THIS NEEDS TO SUIT THE RESOLUTION OF EVERY MODE

	vg_store_pixmap(xi, yi, width, height, pixmap); // the function that'll store the pixmap on the secondary buffer

	vg_secondary_to_video_mem();

	free(pixmap); // to avoid memory problems from the allocation made in read_xpm()

	//Beginning of the cycle to press ESC to exit

	int irq_set = kbc_subscribe();

	int ipc_status;
	message msg;
	int r;

	unsigned long key;

	while (key != ESC_KEY)
	{
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf ("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set)
				{

					key = kbc_read_key();

				}

				break;

			default: break;

			}
		}

	}

	kbc_unsubscribe();

	if (vg_exit())
	{
		printf ("vg_exit() failed.\n");
		return 1;
	}

	return 0;
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
				unsigned short hor, short delta, unsigned short time)

{

	// Initializing graphics mode
	lm_init();

	char* v_mem;
	unsigned short mode = VBE_GRAPHICS_1024_768_256;

	if ((v_mem = vg_init(mode)) == NULL)
	{
		printf ("video_mem is a null pointer. vg_init() may have failed.\n");
		return 1;
	}

	float yspeed, xspeed;

	if (hor == 0)
	{
		yspeed = delta / time;
		xspeed = 0;
	}

	else
	{
		xspeed = delta / time;
		yspeed = 0;
	}

	Sprite* sprite;
	sprite = create_sprite(xpm, xi, yi, xspeed, yspeed);
	
	// Beginning of the cycle

	unsigned int c = 0; // c = counter
	unsigned int freq = 60; // frequency
	float time_elapsed = c / 60; // calculates the time that has passed in seconds since the beginning of the cycle

	const int id = TIMER_HOOK_ID;

	int irq_set_keyboard = kbc_subscribe();
	int irq_set_timer = timer_subscribe_int();

	int ipc_status;
	message msg;
	int r;

	unsigned long key;

	while ((key != ESC_KEY) && (time_elapsed < time))
		{
			if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
			{
				printf ("driver_receive failed with: %d", r);
				continue;
			}

			if (is_ipc_notify(ipc_status))
			{
				switch (_ENDPOINT_P(msg.m_source))
				{
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set_keyboard)
					{

						key = kbc_read_key();

					}

					if (msg.NOTIFY_ARG & irq_set_timer)
					{
						animate_sprite(sprite, time_elapsed);
						vg_secondary_to_video_mem();


						c++;
						time_elapsed = c / 60;
					}

					break;

				default: break;

				}
			}

		}

	destroy_sprite(sprite);

	kbc_unsubscribe();
	timer_unsubscribe_int();

	if (vg_exit())
	{
		printf ("vg_exit() failed.\n");
		return 1;
	}

	return 0;

}					

int test_controller() {
	
	/* To be completed */
	
}					
	
