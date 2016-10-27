#include "maze.h"
/*
 * Creates Maze Struct
 */
Maze* createMaze(const char* bitmapPath, int xi, int yi)
{
	Maze* m = (Maze*) malloc(sizeof(Maze));
	if( m == NULL )
		return NULL;

	m->map = loadBitmap(bitmapPath);
	m->xiPos = xi;
	m->yiPos = yi;

	m->bossCounter = 0;

	return m;
}
/*
 *Adds Boss to game
 */
Maze* addMazeBoss(Maze* m, Boss* boss)
{
	m->bosses[m->bossCounter] = boss;
	m->bossCounter++;
}
/*
 * Checks it that pixel has a boss
 */
int isBoss(Maze* m, int x, int y)
{
	int i;

	for (i = 0; i < m->bossCounter; i++)
	{
		if(x >= m->bosses[i]->xi && x <= m->bosses[i]->xf && y >= m->bosses[i]->yi && y <= m->bosses[i]->yf)
		{
			return i;
		}
	}

	return -1;
}
/*
 * Draws labirynth
 */
void drawMaze(Maze* m)
{
	drawBitmap(m->map, 0, 0, ALIGN_LEFT);
}
/*
 * finds color in certain pixel of maze
 */
int mazeColorInCoord(Maze* m, unsigned x, unsigned y)
{
	y = m->map->bitmapInfoHeader.height - y; 	// the bitmapData[] array in a bitmap obeys to a referential
												//whose origin is the BOTTOM left corner of an image
												// for practical usage, we'll keep using (0,0) as being the TOP left corner

	int pixel = (y * m->map->bitmapInfoHeader.width + x) * 2;
	int firstByte = m->map->bitmapData[pixel+1] << 8;
	int secondByte = m->map->bitmapData[pixel+2];

	int ret = (firstByte + secondByte);

	return ret;
}
/*
 * Destroys the maze
 */
void destroyMaze(Maze* m)
{
	if (m == NULL)
		return;

	deleteBitmap(m->map);
	free(m);
}
