#include "sprite.h"
#include "video_gr.h"

/**
 * SPRITE STRUCT related functions
 */
/*
 * Creates Sprite
 */
Sprite* createSprite(Bitmap* map, int x, int y, int transparentColor)
{
	//allocate space for the "object"
	Sprite* sp = (Sprite*) malloc (sizeof(Sprite));
	if( sp == NULL )
		return NULL;

	// read the sprite bitmap

	sp->map = map;

	if( sp->map == NULL )
	{
		free(sp);
		return NULL;
	}

	sp->xi =x;
	sp->yi =y;

	sp->width = map->bitmapInfoHeader.width;
	sp->height = map->bitmapInfoHeader.height;

	sp->xf = sp->xi + sp->width;
	sp->yf = sp->yi + sp->height;

	sp->transparentColor = transparentColor;

	sp->xspeed = 0;
	sp->yspeed = 0;

	return sp;
}
/*
 * Draws Sprite
 */
void drawSprite(Sprite* sp)
{
	drawBitmapIg(sp->map, sp->xi, sp->yi, ALIGN_LEFT, sp->transparentColor);
}
/*
 * moves Sprite
 */
void moveSprite(Sprite *sp, float elapsedTime)
{
	sp->xi = sp->xi + (sp->xspeed * elapsedTime);
	sp->yi = sp->yi + (sp->yspeed * elapsedTime);

	sp->xf = sp->xi + sp->width;
	sp->yf = sp->yi + sp->height;
}

/*
 * Destroys the Sprite
 */
void destroySprite(Sprite* sp)
{
	if( sp == NULL )
		return;

	deleteBitmap(sp->map);
	free(sp);
	sp = NULL;     // hopeless: pointer is passed by value
}

/**
 * DOLL SPRITE related functions
 */

/*
 * Creates Doll
 */
Doll* createDoll(int xi, int yi)
{
	//allocate space for the "object"
	Doll* doll = (Doll*) malloc (sizeof(Doll));
	if( doll == NULL )
		return NULL;

	Sprite* sp = createSprite (loadBitmap(dollS), xi, yi, RED);

	doll->sprite = sp;

	doll->collision = 0;
	doll->direction = 2;
	doll->end = 0;

	return doll;
}

/*
 * Checks if doll is colliding
 */
int collidingDoll(Doll* doll)
{/*
	int ret = 0;

	int j = getColorFromCoord( 62, 572, getSecondaryVideoMem());
	if (isSameColor(doll->sprite->xi, doll->sprite->yi, getSecondaryVideoMem(), j) && isSameColor(doll->sprite->xi, doll->sprite->yi-9, getSecondaryVideoMem(), j))
		{
			doll->sprite->xi = doll->sprite->xi;
			doll->sprite->yi = doll->sprite->yi + 5;

			ret = 1;
		}
	if (isSameColor(doll->sprite->xi, doll->sprite->yi, getSecondaryVideoMem(), j) && isSameColor(doll->sprite->xi, doll->sprite->yi-9, getSecondaryVideoMem(), j))
			{
				doll->sprite->xi = doll->sprite->xi + 5;
				doll->sprite->yi = doll->sprite->yi;

				ret = 1;
			}
	if (isSameColor(doll->sprite->xf, doll->sprite->yi, getSecondaryVideoMem(), j) && isSameColor(doll->sprite->xf+9, doll->sprite->yi, getSecondaryVideoMem(), j))
				{
					doll->sprite->xi = doll->sprite->xi - 5;
					doll->sprite->yi = doll->sprite->yi;

					ret = 1;
				}
	if (isSameColor(doll->sprite->xi, doll->sprite->yf, getSecondaryVideoMem(), j) && isSameColor(doll->sprite->xi, doll->sprite->yf+9, getSecondaryVideoMem(), j))
					{
						doll->sprite->xi = doll->sprite->xi;
						doll->sprite->yi = doll->sprite->yi - 5;

						ret = 1;
					}
	return ret;*/
}

/*
 * Draws Doll
 */
void drawDoll(Doll* doll)
{
	collidingDoll(doll);
	drawSprite(doll->sprite);
}
/*
 * Changes the direction sprite of te dol
 */
void changeDollDirection(Doll* doll, char* directions)
{
	doll->sprite->xspeed = 0;
	doll->sprite->yspeed = 0;

	if (directions[1])
	{
		doll->sprite->map = loadBitmap(dollN);
		doll->sprite->yspeed = doll->sprite->yspeed -10;
	}

	if (directions[2])
	{
		doll->sprite->map = loadBitmap(dollS);
		doll->sprite->yspeed = doll->sprite->yspeed + 10;
	}

	if (directions[3])
	{
		doll->sprite->map = loadBitmap(dollW1);
		doll->sprite->xspeed = doll->sprite->xspeed - 10;
	}

	if (directions[4])
	{
		doll->sprite->map = loadBitmap(dollE1);
		doll->sprite->xspeed = doll->sprite->xspeed + 10;
	}

	if (!(directions[1] || directions[2] || directions[3] || directions[4]))
	{
		doll->sprite->xspeed = 0;
		doll->sprite->yspeed = 0;
	}

}
/*
 * Changes the doll sprite
 */
void changeDollSprite(Doll* doll, Sprite* sp)
{
	doll->sprite = sp;
}
/*
 * Moves the doll's sprite
 */
void moveDoll (Doll* doll, float elapsedTime)
{
	if(collidingDoll(doll) != 1){
	moveSprite (doll->sprite, elapsedTime);}
	//collidingDoll(doll);
	//winning_screen(doll->sprite);

}
/*
 * Destroys the doll's sprite
 */
void destroyDoll(Doll* doll)
{
	if( doll == NULL )
		return;

	destroySprite(doll->sprite);
	free(doll);
	doll = NULL;     // hopeless: pointer is passed by value
}
/*
 * writes sentences using bitmaps
 */
void writeSentece(int x, int y, char sentence[]){
int i;
	for(i = 0; i < strlen(sentence); i++){
		if(sentence[i] != ' '){
		writeletter(x,y,sentence[i]);
		x = x+20;
		}
		else{
			x = x+20;
		}
	}
}
/*
 * Draws a certain letter at a certain position
 */
void writeletter(int x, int y, char letter){
	if(letter == '0'){
			Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/0.bmp");
			drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
			deleteBitmap(le);
		}
	if(letter == '1'){
				Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/1.bmp");
				drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
				deleteBitmap(le);
			}
	if(letter == '2'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/2.bmp");
					drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == '3'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/3.bmp");
					drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == '4'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/4.bmp");
					drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == '5'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/5.bmp");
					drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == '6'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/6.bmp");
					drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == '7'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/7.bmp");
					drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == '8'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/8.bmp");
					drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == '9'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/9.bmp");
					drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	if(letter == 'a'){
		Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/a2.bmp");
		drawBitmapIg(le,x,y+3,ALIGN_LEFT,MUSTARD);
		deleteBitmap(le);
	}
	else if(letter == 'A'){
		Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/A.bmp");
		drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
		deleteBitmap(le);
	}
	else if(letter == 'b'){
			Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/b2.bmp");
			drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
			deleteBitmap(le);
		}
	else if(letter == 'B'){
			Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/B.bmp");
			drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
			deleteBitmap(le);
		}
	else if(letter == 'c'){
				Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/c2.bmp");
				drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
				deleteBitmap(le);
			}
	else if(letter == 'C'){
					Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/C.bmp");
					drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
					deleteBitmap(le);
				}
	else if(letter == 'd'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/d2.bmp");
						drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'D'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/D.bmp");
						drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'e'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/e2.bmp");
						drawBitmapIg(le,x,y+4,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'E'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/E.bmp");
						drawBitmapIg(le,x,y-2,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'f'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/f2.bmp");
						drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'F'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/F.bmp");
						drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'g'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/g2.bmp");
						drawBitmapIg(le,x,y+4,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'G'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/G.bmp");
						drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'h'){
						Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/h2.bmp");
						drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
						deleteBitmap(le);
					}
	else if(letter == 'H'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/H.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'i'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/i2.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
						}
	else if(letter == 'I'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/I.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'j'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/j2.bmp");
							drawBitmapIg(le,x,y+4,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'J'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/J.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'k'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/k2.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'K'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/K.bmp");
							drawBitmapIg(le,x,y-2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'l'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/l2.bmp");
							drawBitmapIg(le,x,y-3,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'L'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/L.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'm'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/m2.bmp");
							drawBitmapIg(le,x,y+2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'M'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/M.bmp");
							drawBitmapIg(le,x,y-3,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'n'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/n2.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'N'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/N.bmp");
							drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'o'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/o2.bmp");
							drawBitmapIg(le,x,y+2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'O'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/O.bmp");
							drawBitmapIg(le,x,y-2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'p'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/p2.bmp");
							drawBitmapIg(le,x,y+3,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'P'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/P.bmp");
							drawBitmapIg(le,x,y-2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'q'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/q2.bmp");
							drawBitmapIg(le,x,y+5,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'Q'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/Q.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'r'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/r2.bmp");
							drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'R'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/R.bmp");
							drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 's'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/s2.bmp");
							drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'S'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/S.bmp");
							drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 't'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/t2.bmp");
							drawBitmapIg(le,x,y,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'T'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/T.bmp");
							drawBitmapIg(le,x,y-2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'u'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/u2.bmp");
							drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'U'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/U.bmp");
							drawBitmapIg(le,x,y-2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'v'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/v2.bmp");
							drawBitmapIg(le,x,y+3,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'V'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/V.bmp");
							drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'w'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/w2.bmp");
							drawBitmapIg(le,x,y+1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'W'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/W.bmp");
							drawBitmapIg(le,x,y-3,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'x'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/x2.bmp");
							drawBitmapIg(le,x,y+3,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'X'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/X.bmp");
							drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'y'){
								Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/y2.bmp");
								drawBitmapIg(le,x,y+6,ALIGN_LEFT,MUSTARD);
								deleteBitmap(le);
							}
	else if(letter == 'Y'){
									Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/Y.bmp");
									drawBitmapIg(le,x,y-2,ALIGN_LEFT,MUSTARD);
									deleteBitmap(le);
								}

	else if(letter == 'z'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/z2.bmp");
							drawBitmapIg(le,x,y+2,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
	else if(letter == 'Z'){
							Bitmap* le = loadBitmap("/home/lcom/lcom1516-t2g03/proj/images/Z.bmp");
							drawBitmapIg(le,x,y-1,ALIGN_LEFT,MUSTARD);
							deleteBitmap(le);
						}
}

/***************
 * BOSS SPRITES *
 ***************/


/*
 * Creates Boss Struct
 */
Boss* createBoss(Sprite* sp, int xi, int yi, int xf, int yf)
{
	Boss* boss = (Boss*) malloc (sizeof(Boss));
	if( boss == NULL )
		return NULL;

	boss->sprite = sp;

	boss->xi = xi;
	boss->yi = yi;
	boss->xf = xf;
	boss->yf = yf;

	return boss;
}
/*
 * Draws Boss's sprite
 */
void drawBoss(Boss* boss)
{
	drawSprite(boss->sprite);
}
/*
 * Destroys boss's sprite
 */
void destroyBoss(Boss* boss)
{
	if( boss == NULL )
		return;

	destroySprite(boss->sprite);
	free(boss);
	boss = NULL;
}
