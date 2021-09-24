#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include "glut.h"
#include "game.h"
#include "lodepng.h"

// variables
static unsigned int win_width = 640;
static unsigned int win_height = 640;
static unsigned int time_interval = 33;
static float time_step = 0.1f;
static float red = 0, green = 0, blue = 0;

// glut callback functions
static void reshape( int w, int h );
static void display();
static void timer( int timer_id );
static void special( int key, int x, int y );
static void keyboard( unsigned char key, int x, int y );
static void mouse(int button, int state, int x, int y);
static void motion(int x, int y);
static void passive(int x, int y);

// external functions to be called back
extern void initialize();
extern void update();
extern void draw();
extern void finalize();

extern void handleSpecialKey( int key, int x, int y );
extern void handleGeneralKey( unsigned char key, int x, int y );
extern void handleMouseButton(int button, int state, int x, int y);
extern void handleMouseMove(int x, int y, int is_button_pressed);

extern void GetCameraPos(int* x, int* y);

// ---------------------- BASIC -------------------------

static void initGLUT()
{
	// window initialization
	glutInitWindowSize( win_width, win_height );
	glutInitWindowPosition( 0, 0 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
	glutCreateWindow( "Game Framework" );
	glutFullScreen();

	// cursor
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

	// call-back initialization
	glutReshapeFunc( reshape );
	glutDisplayFunc( display );
	glutIdleFunc(display);
	glutKeyboardFunc( keyboard );
	glutSpecialFunc( special );

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passive);

	glutTimerFunc( time_interval, timer, 1 );
}

static void initGL()
{
	glDisable( GL_NORMALIZE );
	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );

	glClearColor( 1.0, 1.0, 1.0, 1.0 );
	glClearDepth( 1.0 );
	glPointSize( 10.0 );
	glLineWidth( 5.0 );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glShadeModel( GL_FLAT );
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, win_width, 0, win_height, -100, 100 );
}

static void reshape( int w, int h )
{
	win_width = w;
	win_height = h;

	glViewport( 0, 0, w, h );
}

static void timer( int id )
{
	static int is_first = 1;

	if( is_first )
	{
		initialize();
		is_first = 0;
	}
	else
	{
		update();
	}
	glutPostRedisplay();

	glutTimerFunc( time_interval, timer, 1 );
}

static void display()
{
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glTranslatef( 0, 0, 0 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static int cameraX, cameraY;
	GetCameraPos(&cameraX, &cameraY);
	gluLookAt(cameraX, cameraY, 10.0f, cameraX, cameraY, 0.0f, 0.0f, 1.0f, 0.0f);

	draw();

	glPopMatrix();
	glutSwapBuffers();
}

static void special( int key, int x, int y )
{
	handleSpecialKey( key, x, win_height - y );
}

static void keyboard( unsigned char key, int x, int y )
{
	handleGeneralKey( key, x, win_height - y );
}

static void mouse(int button, int state, int x, int y)
{
	handleMouseButton(button, state, x - win_width / 2, win_height / 2 - y);
}

static void motion(int x, int y)
{
	handleMouseMove(x - win_width / 2, win_height / 2 - y, 1);
}

static void passive(int x, int y)
{
	handleMouseMove(x - win_width / 2, win_height / 2 - y, 0);
}


//


void initVisualApp( unsigned int width, unsigned int height )
{
	win_width = width;
	win_height = height;

	initGLUT();
	initGL();
	glutMainLoop();
}

void setColor( float r, float g, float b )
{
	red = r;
	green = g;
	blue = b;
}

void strokeLine( float x0, float y0, float x1, float y1 )
{
	glDisable( GL_TEXTURE_2D );
	glColor3f( red, green, blue );

	glBegin( GL_LINES );
	glVertex3f( x0, y0, 0 );
	glVertex3f( x1, y1, 0 );
	glEnd();
}

void strokeLine(float x0, float y0, float x1, float y1, float alpha) {
	glDisable(GL_TEXTURE_2D);
	glColor4f(red, green, blue, alpha);

	glBegin(GL_LINES);
	glVertex3f(x0, y0, 0);
	glVertex3f(x1, y1, 0);
	glEnd();
}

void fillCircle( float cx, float cy, float r )
{
	glDisable( GL_TEXTURE_2D );
	glColor3f(red, green, blue);

	unsigned int i = 0, num_sides = 30;
	float angle = 0, da = 2 * PI / (float)num_sides;

	glBegin( GL_TRIANGLE_FAN );
	glVertex3f( cx, cy, 0 );
	for( i=0; i < num_sides+1; i++ )
	{
		float x = cx + r * cosf( angle );
		float y = cy + r * sinf( angle );
		glVertex3f( x, y, 0 );

		angle += da;
	}
	glEnd();
}

void fillCircle(float cx, float cy, float r, float alpha)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(red, green, blue, alpha);

	unsigned int i = 0, num_sides = 30;
	float angle = 0, da = 2 * PI / (float)num_sides;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(cx, cy, 0);
	for (i = 0; i < num_sides + 1; i++)
	{
		float x = cx + r * cosf(angle);
		float y = cy + r * sinf(angle);
		glVertex3f(x, y, 0);

		angle += da;
	}
	glEnd();
}

void strokeCircle( float cx, float cy, float r )
{
	glDisable( GL_TEXTURE_2D );
	glColor3f( red, green, blue );

	unsigned int i = 0, num_sides = 36;
	float angle = 0, da = 2 * PI / (float)num_sides;

	glBegin( GL_LINE_STRIP );
	for( i=0; i < num_sides+1; i++ )
	{
		float x = cx + r * cosf( angle );
		float y = cy + r * sinf( angle );
		glVertex3f( x, y, 0 );

		angle += da;
	}
	glEnd();
}

void fillTriangle( float x0, float y0, float x1, float y1, float x2, float y2)
{
	glDisable( GL_TEXTURE_2D );
	glColor3f(red, green, blue);

	glBegin( GL_TRIANGLES );
	glVertex3f( x0, y0, 0 );
	glVertex3f( x1, y1, 0 );
	glVertex3f( x2, y2, 0 );
	glEnd();
}

void fillTriangle(float x0, float y0, float x1, float y1, float x2, float y2, float alpha)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(red, green, blue, alpha);

	glBegin(GL_TRIANGLES);
	glVertex3f(x0, y0, 0);
	glVertex3f(x1, y1, 0);
	glVertex3f(x2, y2, 0);
	glEnd();
}

void strokeTriangle( float x0, float y0, float x1, float y1, float x2, float y2 )
{
	glDisable( GL_TEXTURE_2D );
	glColor3f( red, green, blue );

	glBegin( GL_LINE_STRIP );
	glVertex3f( x0, y0, 0 );
	glVertex3f( x1, y1, 0 );
	glVertex3f( x2, y2, 0 );
	glVertex3f( x0, y0, 0 );
	glEnd();
}

void fillRectangle( float cx, float cy, float w, float h, float a)
{
	glDisable( GL_TEXTURE_2D );
	glColor3f(red, green, blue);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glTranslatef( cx, cy, 0 );
	glRotatef( a, 0, 0, 1 );
	glScalef( w/2, h/2, 1 );
	
	glBegin( GL_QUADS );
	glVertex3f( 1, 1, 0 );
	glVertex3f( -1, 1, 0 );
	glVertex3f( -1, -1, 0 );
	glVertex3f( 1, -1, 0 );
	glEnd();

	glPopMatrix();
}

void fillRectangle(float cx, float cy, float w, float h, float a, float alpha)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(red, green, blue, alpha);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(cx, cy, 0);
	glRotatef(a, 0, 0, 1);
	glScalef(w / 2, h / 2, 1);

	glBegin(GL_QUADS);
	glVertex3f(1, 1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, -1, 0);
	glEnd();

	glPopMatrix();
}

void strokeRectangle( float cx, float cy, float w, float h, float a)
{
	glDisable( GL_TEXTURE_2D );
	glColor3f(red, green,blue);

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glTranslatef( cx, cy, 0 );
	glRotatef( a, 0, 0, 1 );
	glScalef( w/2, h/2, 1 );
	
	glBegin( GL_LINE_STRIP );
	glVertex3f( 1, 1, 0 );
	glVertex3f( -1, 1, 0 );
	glVertex3f( -1, -1, 0 );
	glVertex3f( 1, -1, 0 );
	glVertex3f( 1, 1, 0 );
	glEnd();

	glPopMatrix();
}

void strokeRectangle(float cx, float cy, float w, float h, float a, float alpha)
{
	glDisable(GL_TEXTURE_2D);
	glColor4f(red, green, blue, alpha);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(cx, cy, 0);
	glRotatef(a, 0, 0, 1);
	glScalef(w / 2, h / 2, 1);

	glBegin(GL_LINE_STRIP);
	glVertex3f(1, 1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(-1, -1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(1, 1, 0);
	glEnd();

	glPopMatrix();
}

void drawText( float x, float y, void* font, char* string )
{
	glDisable( GL_TEXTURE_2D );
	glColor3f( red, green, blue );

	char* c;
	glRasterPos3f( x, y, 0 );
	for( c = string; *c != '\0'; c++ )
	{
		glutBitmapCharacter( font, *c );
	}
}

unsigned int getWinWidth()
{
	return win_width;
}

unsigned int getWinHeight()
{
	return win_height;
}

int isKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000) ? 1 : 0;
}

int isKeyUp(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000) ? 0 : 1;
}

int getMousePosition(int * px, int * py)
{
	POINT p;
	if (GetCursorPos(&p))
	{
		HWND hwnd = GetActiveWindow();
		BOOL is_good = ScreenToClient(hwnd, &p);
		if (is_good)
		{
			*px = p.x;
			*py = win_height - p.y;
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}


// ---------------------- BITMAP -------------------------


unsigned char* loadBMP( char* path, unsigned int* w, unsigned int* h )
{
	unsigned char header[54];
	unsigned int dataPos, width, height, imageSize;
	unsigned char* data = 0;

	FILE* fp = fopen( path, "rb" );
	if( !fp )
	{
		return 0;
	}

	int bytes_read = fread( header, 1, 54, fp );
	if( bytes_read != 54 )
	{
		return 0;
	}

	if( header[0] != 'B' || header[1] != 'M' )
	{
		return 0;
	}

	dataPos		= *(unsigned int*)&( header[0x0A] );
	imageSize	= *(unsigned int*)&( header[0x22] );
	width		= *(unsigned int*)&( header[0x12] );
	height		= *(unsigned int*)&( header[0x16] );

	if( imageSize == 0 )
	{
		imageSize = width * height * 3;
	}
	if( dataPos == 0 )
	{
		dataPos = 54;
	}

	data = (unsigned char *)malloc( sizeof(unsigned char) *imageSize );
	bytes_read = fread( data, 1, imageSize, fp );
	fclose( fp );

	if( bytes_read != imageSize )
	{
		delete data;
		return 0;
	}

	*w = width;
	*h = height;

	return data;
}

unsigned char* loadPNG(char* path, unsigned int* w, unsigned int* h)
{
	unsigned char* image;
	unsigned int error = lodepng_decode32_file(&image, w, h, path);
	if (error)
	{
		if (image != NULL) free(image);
		return NULL;
	}
	else
	{
		return image;
	}
}

static const char *getFilenameExtension(const char *filename) {
	const char *dot = strrchr(filename, '.');
	if (!dot || dot == filename) return "";
	return dot + 1;
}


//
static std::vector<Bitmap*> bitmap_list;

Bitmap* createBitmap( char* path )
{
	int bitmap_format = UNKNOWN_FORMAT;

	const char* ext = getFilenameExtension(path);
	if (strcmp(ext, "bmp") == 0 || strcmp(ext, "BMP") == 0) bitmap_format = BMP_FORMAT;
	else if (strcmp(ext, "png") == 0 || strcmp(ext, "PNG") == 0) bitmap_format = PNG_FORMAT;
	else return NULL;

	//
	Bitmap* bitmap = findBitmap(path);
	if (bitmap)
	{
		bitmap->ref_count++;
	}
	else
	{
		bitmap = (Bitmap *)malloc(sizeof(Bitmap));
		bitmap->ref_count = 0;
		bitmap_list.push_back(bitmap);

		bitmap->format = bitmap_format;
		switch (bitmap_format) {
		case BMP_FORMAT:
			bitmap->data = loadBMP(path, &bitmap->width, &bitmap->height);
			bitmap->num_channels = 3;
			break;

		case PNG_FORMAT:
			bitmap->data = loadPNG(path, &bitmap->width, &bitmap->height);
			bitmap->num_channels = 4;
			break;
		}

		if (bitmap->data == NULL)
		{
			bitmap->num_channels = 0;
			bitmap->format = UNKNOWN_FORMAT;
			free(bitmap);
			bitmap = NULL;
		}
		else
		{
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &bitmap->texture_id);
			glBindTexture(GL_TEXTURE_2D, bitmap->texture_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			switch (bitmap_format) {
			case BMP_FORMAT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap->width, bitmap->height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bitmap->data);
				break;

			case PNG_FORMAT:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->data);
				break;
			}
		}
	}
	return bitmap;
}

Bitmap* findBitmap(char* path)
{
	std::vector<Bitmap*>::iterator itor_bmp = bitmap_list.begin();
	while (itor_bmp != bitmap_list.end())
	{
		Bitmap* bmp = (*itor_bmp++);
		if (strcmp(bmp->path, path) == 0)
		{
			return bmp;
		}
	}
	return NULL;
}

void destroyBitmap( Bitmap* bitmap )
{
	if( bitmap != NULL )
	{
		bitmap->ref_count--;
		
		if (bitmap->ref_count == 0)
		{
			std::vector<Bitmap*>::iterator itor_bmp = bitmap_list.begin();
			while (itor_bmp != bitmap_list.end())
			{
				Bitmap* bmp = (*itor_bmp);
				if (bmp == bitmap)
				{
					bitmap_list.erase(itor_bmp);
					break;
				}
				itor_bmp++;
			}

			if (bitmap->data != NULL)
			{
				free(bitmap->data);
			}
			free(bitmap);
		}
	}
}

void drawBitmap( Bitmap* bitmap, float cx, float cy, float sx, float sy, float a )
{
	if( !bitmap )
	{
		return;
	}
	if( !bitmap->data )
	{
		return;
	}
	if( bitmap->format == UNKNOWN_FORMAT )
	{
		return;
	}

	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, bitmap->texture_id );

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();

	glTranslatef( cx, cy, 0 );
	glRotatef( a, 0, 0, 1 );
	glScalef( sx/2, sy/2, 1 );
	
	if( bitmap->format == BMP_FORMAT )
	{
		glBegin(GL_QUADS);
		glColor3f(red, green, blue);

		glTexCoord2f(0, 0);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, -1, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 1);
		glVertex3f(-1, 1, 0);

		glEnd();
	}
	else if (bitmap->format == PNG_FORMAT)
	{
		glBegin(GL_QUADS);
		glColor3f(red, green, blue);

		glTexCoord2f(0, 1);
		glVertex3f(-1, -1, 0);

		glTexCoord2f(1, 1);
		glVertex3f(1, -1, 0);

		glTexCoord2f(1, 0);
		glVertex3f(1, 1, 0);

		glTexCoord2f(0, 0);
		glVertex3f(-1, 1, 0);

		glEnd();
	}

	glPopMatrix();
}

// ---------------------- OBJECTS POOL -------------------------

GameObject* object_pool = NULL;
int pool_size = 0;

void initializePool( int size )
{
	object_pool = (GameObject *)malloc( sizeof(GameObject) * size );
	if( !object_pool )
	{
		pool_size = 0;
		return;
	}
	else
	{
		pool_size = size;

		int i;
		for (i = 0; i < pool_size; i++)
		{
			GameObject* game_object = &object_pool[i];
			game_object->is_alive = 0;
			game_object->id = -1;
			game_object->type = -1;
			game_object->age = -1;
		
			game_object->bitmap = NULL;
			game_object->x = game_object->y = game_object->a = 0.0f;
			game_object->width = game_object->height = 0.0f;
		}
	}
}

GameObject* newObject()
{
	if( !object_pool )
	{
		return NULL;
	}
	else
	{
		int i;
		for (i = 0; i < pool_size; i++)
		{
			GameObject* new_object = &object_pool[i];
			if (!new_object->is_alive)
			{
				new_object->is_alive = 1;
				new_object->id = i;
				new_object->age = 0;

				return new_object;
			}
		}
		return NULL;
	}
}

void deleteObject(GameObject* obj)
{
	if( object_pool && obj->is_alive ) 
	{
		GameObject* game_object = &object_pool[obj->id];
		
		game_object->is_alive = 0;
		game_object->id = -1;
		game_object->type = -1;
		game_object->age = -1;

		game_object->x = game_object->y = game_object->a = 0.0f;
		game_object->width = game_object->height = 0.0f;

		game_object->move_x = game_object->move_y = 0.0f;
		game_object->delay = 0;

	}
}

void cleanupPool()
{
	int i;
	for (i = 0; i < pool_size; i++)
	{
		GameObject* game_object = &object_pool[i];
		deleteObject(game_object);
	}
}

void finalizePool()
{
	if( object_pool )
	{
		int i;
		for (i = 0; i < pool_size; i++)
		{
			GameObject* game_object = &object_pool[i];
			deleteObject(game_object);
		}

		free( object_pool );
		object_pool = NULL;

		pool_size = 0;
	}
}

GameObject* getObject( int i )
{
	if( object_pool && i >=0 && i < pool_size )
	{
		return &object_pool[ i ];
	}
	else
	{
		return NULL;
	}
}

// ---------------------- HELPER -------------------------

void getRectanglePoints( GameObject* obj, float x_arr[4], float y_arr[4] )
{
	x_arr[0] = -obj->width/2;
	y_arr[0] = -obj->height/2;

	x_arr[1] = +obj->width/2;
	y_arr[1] = -obj->height/2;

	x_arr[2] = +obj->width/2;
	y_arr[2] = +obj->height/2;

	x_arr[3] = -obj->width/2;
	y_arr[3] = +obj->height/2;

	float theta = obj->a * PI / 180.0f;

	int i;
	for( i=0; i < 4; i++ )
	{
		float x = obj->x + cosf(theta) * x_arr[i] - sinf(theta) * y_arr[i];
		float y = obj->y + sinf(theta) * x_arr[i] + cosf(theta) * y_arr[i];

		x_arr[i] = x;
		y_arr[i] = y;
	}
}

int isIntersecting( GameObject* obj1, GameObject* obj2 )
{
	if( !obj1 || !obj2 )
	{
		return 0;
	}

	float x1_arr[4], y1_arr[4];
	float x2_arr[4], y2_arr[4];

	getRectanglePoints( obj1, x1_arr, y1_arr );
	getRectanglePoints( obj2, x2_arr, y2_arr );

	//
	int i, j;

	for( i=0; i < 4; i++ )
	{
		float sx = x1_arr[ (i+0)%4 ], sy = y1_arr[ (i+0)%4 ];
		float ex = x1_arr[ (i+1)%4 ], ey = y1_arr[ (i+1)%4 ];

		float nx = ey - sy;
		float ny = sx - ex;

		float min1 = +FLT_MAX, max1 = -FLT_MAX, proj1 = 0.0f;
		float min2 = +FLT_MAX, max2 = -FLT_MAX, proj2 = 0.0f;

		for( j=0; j < 4; j++ )
		{
			proj1 = nx * x1_arr[j] + ny * y1_arr[j];
			if( proj1 < min1 )	min1 = proj1;
			if( proj1 > max1 )	max1 = proj1;

			proj2 = nx * x2_arr[j] + ny * y2_arr[j];
			if( proj2 < min2 )	min2 = proj2;
			if( proj2 > max2 )	max2 = proj2;
		}

		if( max1 < min2 || max2 < min1 )
		{
			return 0;
		}
	}

	for( i=0; i < 4; i++ )
	{
		float sx = x2_arr[ (i+0)%4 ], sy = y2_arr[ (i+0)%4 ];
		float ex = x2_arr[ (i+1)%4 ], ey = y2_arr[ (i+1)%4 ];

		float nx = ey - sy;
		float ny = sx - ex;

		float min1 = +FLT_MAX, max1 = -FLT_MAX, proj1 = 0.0f;
		float min2 = +FLT_MAX, max2 = -FLT_MAX, proj2 = 0.0f;

		for( j=0; j < 4; j++ )
		{
			proj1 = nx * x1_arr[j] + ny * y1_arr[j];
			if( proj1 < min1 )	min1 = proj1;
			if( proj1 > max1 )	max1 = proj1;

			proj2 = nx * x2_arr[j] + ny * y2_arr[j];
			if( proj2 < min2 )	min2 = proj2;
			if( proj2 > max2 )	max2 = proj2;
		}

		if( max1 < min2 || max2 < min1 )
		{
			return 0;
		}
	}

	return 1;
}

float randomFloat( float min, float max )
{
	return (float)rand() / (float)RAND_MAX * (max-min) + min;
}
