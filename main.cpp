#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "glut.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

Bitmap* bitmap = 0;

void main()
{
	initVisualApp( WIN_WIDTH, WIN_HEIGHT );
}

void initialize()
{
	bitmap = createBitmap( "ari2.bmp" );
}

void finalize()
{
	destroyBitmap( bitmap );
}

void draw()
{
	drawBitmap( bitmap, 400, 300, 100, 100, 0 );
}

void update()
{
}

void handleSpecialKey( int key, int x, int y )
{
	switch( key ) {
	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	case GLUT_KEY_UP:
		break;

	case GLUT_KEY_DOWN:
		break;
	}
}

void handleGeneralKey( unsigned char key, int x, int y )
{
	switch( key ) {
	case 'A':
	case 'a':
		break;

	case 8:
		break;

	case 13:
		break;

	case 32:
		break;
	}
}
