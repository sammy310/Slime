#pragma once

#define PI	(3.141592)

//
void initVisualApp( unsigned int width, unsigned int height );
void setColor( float r, float g, float b );

void strokeLine( float x0, float y0, float x1, float y1 );
void strokeLine(float x0, float y0, float x1, float y1, float alpha);
void strokeCircle( float cx, float cy, float r );
void strokeTriangle( float x0, float y0, float x1, float y1, float x2, float y2 );
void strokeRectangle( float cx, float cy, float w, float h, float a );
void strokeRectangle(float cx, float cy, float w, float h, float a, float alpha);

void fillCircle( float cx, float cy, float r );
void fillCircle(float cx, float cy, float r, float alpha);
void fillTriangle( float x0, float y0, float x1, float y1, float x2, float y2 );
void fillTriangle(float x0, float y0, float x1, float y1, float x2, float y2, float alpha);
void fillRectangle( float cx, float cy, float w, float h, float a );
void fillRectangle(float cx, float cy, float w, float h, float a, float alpha);

void drawText( float x, float y, void* font, char* string );

unsigned int getWinWidth();
unsigned int getWinHeight();

int isKeyDown(int vKey);
int isKeyUp(int vKey);
int getMousePosition(int* px, int* py);

// (1) BITMAP
enum {
	UNKNOWN_FORMAT = -1,
	BMP_FORMAT,
	PNG_FORMAT,
};

typedef struct {
	char path[128];
	unsigned int width;
	unsigned int height;
	unsigned int num_channels;
	unsigned char* data;
	unsigned int texture_id;
	int format;
	int ref_count;
} Bitmap;

Bitmap* createBitmap( char* path );
Bitmap* findBitmap( char* path );
void drawBitmap( Bitmap* bitmap, float cx, float cy, float sx, float sy, float a );
void destroyBitmap( Bitmap* bitmap );

// (2) OBJECT POOL
typedef struct
{
	int is_alive;
	int id;
	int type;
	int age;
	int health;

	Bitmap* bitmap;
	float x, y;
	float a;
	float width;
	float height;
	float size;

	float move_x, move_y;
	int delay;
	int corner;
} GameObject;

typedef struct {
	int year, month, day;
	int hour, minute, second;
	int stage;
	int score;
	float time;
} Score_Mode_1;

typedef struct {
	int year, month, day;
	int hour, minute, second;
	int score;
	float time;
} Score_Mode_2;

void initializePool( int size );
void cleanupPool();
void finalizePool();

GameObject* newObject();
GameObject* getObject( int i );
void deleteObject( GameObject* obj );

extern GameObject* object_pool;

// (3) HELPER
int isIntersecting( GameObject* obj1, GameObject* obj2 );
float randomFloat( float min, float max );
