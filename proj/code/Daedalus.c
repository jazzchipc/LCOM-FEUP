#include "Daedalus.h"

#define VISIBLE_AREA 30 // visible area in pixels

int mazeCounter = 2;
int currentMaze = 0;
int win = 0;

Daedalus* initDaedalus()
{
	Daedalus* dae = (Daedalus* ) malloc(sizeof(Daedalus)); // creates new variable that'll represent the state of the game

	//Subscribing interrupts on the devices
	dae->RTC_IRQ_SET = rtc_subscribe();
	dae->TIMER_IRQ_SET = timer_subscribe_int();
	dae->KBD_IRQ_SET = kbc_subscribe();

	//Initializing low memory and graphics
	lm_init();
	Date* data = createDate();
	dae->day = data->day;
	dae->month = data->month;
	dae->year = data->year;
	char* v_mem;
	unsigned short mode = VBE_GRAPHICS_1024_768_16B;

	if ((v_mem = vg_init(mode)) == NULL)
	{
		printf ("video_mem is a null pointer. vg_init() may have failed.\n");
	}

	//Setting object definitions
	dae->draw = 1;
	dae->stop = 0;
	dae->win = win;
	dae->boss = 0;

	dae->pressed_key = 0;

	/*Setting up the crosshair*/
	dae->ch = newCrosshair();

	/*Setting up the maze*/

	Maze* maze1 = createMaze("/home/lcom/lcom1516-t2g03/proj/images/Maze1.bmp", 60, 600);
	Maze* maze2 = createMaze("/home/lcom/lcom1516-t2g03/proj/images/maz2.bmp", 60, 600);

	Maze* mazes[2] = {maze1, maze2};

	dae->maze = mazes[currentMaze]; // starts with Maze1.bmp

	//Useless without mouse
	/*Sprite* boss = createSprite(loadBitmap(eggMan), 560, 210, PURE_GREEN);
	Boss* eggman = createBoss(boss, 524, boss->yi, 763, boss->yf);
	addMazeBoss(dae->maze, eggman);*/

	/*Creating the doll*/
	dae->doll = createDoll(dae->maze->xiPos, dae->maze->yiPos);

	return dae;
}

void showStartScreen(Daedalus* dae)
{

	Bitmap* startScreen = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/StartScreen.bmp");
	drawBitmap(startScreen, 0, 0, ALIGN_LEFT);
	vg_secondary_to_video_mem();

	tickdelay(micros_to_ticks(3000000));

	deleteBitmap(startScreen);
	vg_clear_secondary();
	vg_secondary_to_video_mem();
}

void updateDaedalusKey(Daedalus *dae)
{
	//This receives the makecode of the key, provided by the KBC
	dae->pressed_key = kbc_read_key();

	//End game if ESC is pressed
	if (dae->pressed_key == ESC_KEY)
		dae->stop = 2;
}

/**
 * This next function will set the respetive activeKeys char to 1 or 0.
 */
int pressedKeyDaedalus(Daedalus *dae)
{
	if (dae->pressed_key == KEY_W)
	{
		dae->activeKeys[1] = 1;
		return 1;
	}

	if (dae->pressed_key == BREAK_KEY_W)
	{
		dae->activeKeys[1] = 0;
		return -1;
	}

	if (dae->pressed_key == KEY_S)
	{
		dae->activeKeys[2] = 1;
		return 2;
	}

	if (dae->pressed_key == BREAK_KEY_S)
	{
		dae->activeKeys[2] = 0;
		return -2;
	}

	if (dae->pressed_key == KEY_A)
	{
		dae->activeKeys[3] = 1;
		return 3;
	}

	if (dae->pressed_key == BREAK_KEY_A)
	{
		dae->activeKeys[3] = 0;
		return -3;
	}

	if (dae->pressed_key == KEY_D)
	{
		dae->activeKeys[4] = 1;
		return 4;
	}

	if (dae->pressed_key == BREAK_KEY_D)
	{
		dae->activeKeys[4] = 0;
		return -4;
	}

	return 0;

}

void updateDaedalusDoll(Daedalus* dae)
{
	int isWASD = pressedKeyDaedalus(dae);

	if (isWASD)
	{
		changeDollDirection(dae->doll, dae->activeKeys);
	}
}

void updateDaedalusWin(Daedalus* dae)
{
	int x = dae->doll->sprite->xi;
	int y = dae->doll->sprite->yi;

	int victory = isSameColor(x, y, getVideoMem(), GREEN);

	if(victory == 1)
	{
		win++;
		dae->win = dae->win + 1;
		dae->stop = 1;
	}
}

void updateDaedalusCollision(Daedalus* dae)
{
	//If any of the corners of the doll touches a white wall
	if (mazeColorInCoord(dae->maze, dae->doll->sprite->xi, dae->doll->sprite->yi) > 63000 ||
			mazeColorInCoord(dae->maze, dae->doll->sprite->xi, dae->doll->sprite->yf) > 63000 ||
			mazeColorInCoord(dae->maze, dae->doll->sprite->xf, dae->doll->sprite->yi) > 63000 ||
			mazeColorInCoord(dae->maze, dae->doll->sprite->xf, dae->doll->sprite->yf) > 63000)
		dae->doll = createDoll(dae->maze->xiPos, dae->maze->yiPos);

	/*	For some reason, the walls on the maze bitmap are a shade of white that's not even near to RGB (255, 255, 255),
	 * probably because of some details in the conversion to 16-bit color depth, so we had to consider all "shades" of white
	 * above 63000 to be a wall.*/
}

/*void updateDaedalusBoss(Daedalus* dae)
{
	if (isBoss(dae->maze, dae->doll->sprite->xi, dae->doll->sprite->yi))
	{
		dae->boss = dae->boss + 1;
	}

	if (dae->boss == 1)
	{
		questOne(dae);
	}

}*/

void updateDaedalus(Daedalus* dae)
{
	int ipc_status;
	message msg;
	int r;


	float seconds = 0.1; // Speed will be in pixels/second

	unsigned long timerIntCounter = 0;

	while(1)
	{
		if(dae->stop)
			break;

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0)
		{
			printf ("driver_receive failed with: %d", r);
			return;
		}

		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & dae->KBD_IRQ_SET)
				{
					updateDaedalusKey(dae);
					updateDaedalusDoll(dae);
				}

				if (msg.NOTIFY_ARG & dae->TIMER_IRQ_SET)
				{
					drawDaedalus(dae, seconds);
					updateDaedalusWin(dae);
					updateDaedalusCollision(dae);

					timerIntCounter++;

				}

				break;

			default: break;

			}
		}
	}
}

void drawDaedalus(Daedalus* dae, float elapsedTime)
{
	moveDoll(dae->doll, elapsedTime);
	drawMaze(dae->maze);

	//These next memsets serve the purpose to restrain the player from seeing all the maze
	//Each visibleArea is a corner of the area to be visualized
	//I'm sorry this code is confusing, but we couldn't figure out a better way to do this
	unsigned long int xTL, xTR, xBL, xBR;
	unsigned long int yTL, yTR, yBL, yBR;

	// top left
	if (dae->doll->sprite->xi - VISIBLE_AREA < 0)
		xTL = 0;
	else
		xTL = (dae->doll->sprite->xi - VISIBLE_AREA) * 2;

	if (dae->doll->sprite->yi - VISIBLE_AREA < 0)
		yTL = 0;
	else
		yTL = (dae->doll->sprite->yi - VISIBLE_AREA) * 1024 * 2;

	dae->visibleArea[0] = xTL + yTL;

	// top right
	if (dae->doll->sprite->xf + VISIBLE_AREA > 1024)
		xTR = 1024 * 2;
	else
		xTR = (dae->doll->sprite->xf + VISIBLE_AREA) * 2;

	if (dae->doll->sprite->yi - VISIBLE_AREA < 0)
		yTR = 0;
	else
		yTR = (dae->doll->sprite->yi - VISIBLE_AREA) * 1024 * 2;

	dae->visibleArea[1] = xTR+ yTR;

	// bottom left
	if (dae->doll->sprite->xi - VISIBLE_AREA < 0)
		xBL = 0;
	else
		xBL = (dae->doll->sprite->xi - VISIBLE_AREA) * 2;

	if (dae->doll->sprite->yf + VISIBLE_AREA > 765)
		yBL = 765 * 2 * 1024;
	else
		yBL = (dae->doll->sprite->yf + VISIBLE_AREA) * 1024 * 2;

	dae->visibleArea[2] = xBL + yBL;

	// bottom right
	if (dae->doll->sprite->xf + VISIBLE_AREA > 1024)
		xBR = 1024 * 2;
	else
		xBR = (dae->doll->sprite->xf + VISIBLE_AREA) * 2;

	if (dae->doll->sprite->yf + VISIBLE_AREA > 765)
		yBR = 765 * 2 * 1024;
	else
		yBR = (dae->doll->sprite->yf + VISIBLE_AREA) * 1024 * 2;

	dae->visibleArea[3] = xBR + yBR;

	// hides everything from top left maze corner to top left sprite corner
	memset(getSecondaryVideoMem(), 0, dae->visibleArea[0]);

	long int h;

	if (dae->doll->sprite->yi - VISIBLE_AREA < 0)
		h = dae->doll->sprite->yf + VISIBLE_AREA;
	else
	{
		if (dae->doll->sprite->yf + VISIBLE_AREA > 765)
			h = 768 - (dae->doll->sprite->yi - VISIBLE_AREA);
		else
			h = dae->doll->sprite->yf + VISIBLE_AREA - (dae->doll->sprite->yi - VISIBLE_AREA);
	}


	int i;

	for (i = 0; i <= h; i++)	// hides left side
	{
		memset(getSecondaryVideoMem() + yTL + i * 1024 * 2, 0, xTL);
	}

	for (i = 0; i <= h; i++)	// hides right side
	{
		memset(getSecondaryVideoMem() + dae->visibleArea[1] + i * 1024 * 2, 0, 1024 * 2 - xTR);
	}

	memset(getSecondaryVideoMem() + dae->visibleArea[3], 0, (1024 * 768 * 2) - dae->visibleArea[3]);

	drawDoll(dae->doll);
	char str[15];
	int day = dae->day;
	sprintf(str, "%d", day);
	writeSentece(20,20, str);
	char str3[15];
	int month = dae->month;
	sprintf(str3, "%d", month);
	writeSentece(60,20, str3);
	char str2[15];
	int year = dae->year;
	sprintf(str2, "%d", year);
	writeSentece(100,20, str2);

	vg_secondary_to_video_mem();

}

int testDaedalus(Daedalus *dae)
{
	updateDaedalus(dae);
	return 0;
}

int stopDaedalus(Daedalus* dae)
{
	destroyCrosshair(dae->ch);
	destroyMaze(dae->maze);
	destroyDoll(dae->doll);

	if(dae->stop == 2)
	{
		free(dae); // frees the memory occupied by the game

		if (vg_exit())
		{
			printf ("vg_exit() failed.\n");
		}

		kbc_unsubscribe(); // unsubscribes keyboard
		timer_unsubscribe_int();
		rtc_unsubscribe();


		return 1;
	}

	if (dae->win == mazeCounter && dae->stop == 1)
	{
		Bitmap* victory = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/VicScreen.bmp");
		drawBitmap(victory,0,0,ALIGN_LEFT);
		vg_secondary_to_video_mem();
		deleteBitmap(victory);
		unsigned long k = 0;
		while (k != ESC_KEY)
		{
			k = kbc_read_key();
		}

		free(dae); // frees the memory occupied by the game

		if (vg_exit())
		{
			printf ("vg_exit() failed.\n");
		}

		kbc_unsubscribe(); // unsubscribes keyboard
		timer_unsubscribe_int();
		rtc_unsubscribe();

		return 1;
	}

	else
	{
		free(dae); // frees the memory occupied by the game

		if (vg_exit())
		{
			printf ("vg_exit() failed.\n");
		}

		kbc_unsubscribe(); // unsubscribes keyboard
		timer_unsubscribe_int();
		rtc_unsubscribe();

		currentMaze++;

		return 0;
	}

}
/*
#define IS_BIT_SET(n, bit)	(((n) & BIT(bit)) ? 1 : 0)

unsigned char info[3];	// Array that'll store the 3 bytes sent by the mouse
//memset(info, 0, sizeof(info)); // sets the packet all to 0 (cleans it)
int currentByte;	// Tells which byte of the "packet" array to analyze next
m_packet* packet;	// Mouse packet that'll store changes to the mouse position/input

int questOne()
{
	vg_exit();

	//First we need to subscribe the mouse interrupts

	int irq_set = mouse_subscribe();

	mouse_stream_mode(1);

	mouse_enable_packets();

	packet = (m_packet*) malloc (sizeof (m_packet));
	Crosshair* c = newCrosshair();

	//Then a cycle to detect interruptions aimed for the mouse

	int ipc_status;
	message msg;
	int r;

	int i = 0;

	int error_get_packet;

	while (i < 200 * 3)
	{
		unsigned long data;
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
					//packet->bytes[i % 3] = mouse_get_byte();
					//packet_print (*packet);

					info[i%3] = mouse_get_byte();

					if (i % 3 == 0)
					{
						if (!(info[i%3] & BIT(3)))	//to sync
							continue;

						mouse_set_packet(packet, info);
						packet_print(packet);
						updateCrosshair(c, packet);
						printf("%d %d", c->sprite->xi, c->sprite->yi);
					}

					i++;
				}

				break;

			default: {
				printf("nao deu\n");
				i++;
				break;}

			}
		}

	}


	printf ("out of loop\n");


	destroyCrosshair(c);


	int a = mouse_stream_mode(0);
	printf ("%d\n", a);

	int j = mouse_unsubscribe();
	printf ("%d\n", j);


	return 0;
}*/
