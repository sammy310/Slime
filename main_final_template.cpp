#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "game.h"
#include "glut.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define POOL_SIZE					100

#define GAME_UPDATE_INTERVAL		1
#define READY_INTERVAL				50
#define ENEMY_SPAWN_INTERVAL		100

#define BULLET_PROBABILITY			0.025f
#define ITEM_PROBABILITY			0.002f

#define PLAYER_SPEED				20.0f
#define ENEMY_SPEED					5.0f
#define BULLET_SPEED				10.0f
#define ITEM_SPEED					5.0f

#define INITIAL_LIFE				1
#define INITIAL_ENERGY				10
#define HIT_DAMAGE					10
#define HIT_SCORE					5
#define ITEM_ENERGY					20

enum
{
	UNDEFINED = -1,
	PLAYER,
	ENEMY1,
	ENEMY2,
	PLAYER_BULLET,
	ENEMY_BULLET,
	PARTICLE,
	ITEM,
};

GameObject* player_obj = NULL;
Bitmap* background = NULL;
Bitmap* title_image = NULL;
Bitmap* button_start = NULL;
Bitmap* button_exit = NULL;
Bitmap* ending_image = NULL;

void createPlayer();
void createEnemy();
void createPlayerBullet();
void createEnemyBullet( GameObject* enemy );
void createParticles( GameObject* obj );
void createItem();

void moveEnemy( GameObject* enemy );
void movePlayerBullet( GameObject* bullet );
void moveEnemyBullet( GameObject* bullet );
void moveParticle( GameObject* particle );
void moveItem(GameObject* item);

void checkCollisions();

// Player state
int num_life = 0;
int energy = 0;
int score = 0;

int ready_count = 0;
int menu_selection = 0;

void drawMenu();
void drawPlayerStates();
void drawGameOver();

// Game state
enum {
	OUT_OF_GAME = -1,
	PRE_GAME,
	READY_GAME,
	IN_GAME,
	POST_GAME
};
// TO BE ADDED
int game_state = OUT_OF_GAME;
void changeGameState(int new_state);

//
void main()
{
	initVisualApp( WIN_WIDTH, WIN_HEIGHT );
}

void initialize()
{
	initializePool( POOL_SIZE );

	changeGameState(PRE_GAME);
}

void finalize()
{
	// TO BE ADDED
	changeGameState(OUT_OF_GAME);

	finalizePool();
}

void draw()
{
	static int count = 0;

	if (background)
	{
		drawBitmap(background, WIN_WIDTH / 2, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT, 0);
	}

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* game_obj = getObject(i);
		if (game_obj && game_obj->is_alive && game_obj->bitmap)
		{
			// TO BE ADDED
			if (ready_count > 0 && game_obj == player_obj && ready_count % 5 == 0) {
				continue;
			}
			drawBitmap(game_obj->bitmap, game_obj->x, game_obj->y, game_obj->width, game_obj->height, game_obj->a);
		}
	}

	// TO BE ADDED
	switch (game_state) {
	case PRE_GAME:
		drawMenu();
		break;

	case READY_GAME:
	case IN_GAME:
		drawPlayerStates();
		break;

	case POST_GAME:
		drawPlayerStates();
		drawGameOver();
		break;
	}

	count++;
}

void update()
{
	static int count = 0;
	static int num_frames = 0;

	if (count % GAME_UPDATE_INTERVAL == 0)
	{
		int i;
		for (i = 0; i < POOL_SIZE; i++)
		{
			GameObject* game_obj = getObject(i);
			if (game_obj && game_obj->is_alive)
			{
				switch (game_obj->type) {
				case ENEMY1:
				case ENEMY2:
					moveEnemy(game_obj);
					if (game_obj->age > 10)
					{
						if (randomFloat(0, 1) < BULLET_PROBABILITY)
						{
							createEnemyBullet(game_obj);
						}
					}
					break;

				case PLAYER_BULLET:
					movePlayerBullet(game_obj);
					break;

				case ENEMY_BULLET:
					moveEnemyBullet(game_obj);
					break;

				case PARTICLE:
					moveParticle(game_obj);
					break;

				// TO BE ADDED
				}

				game_obj->age++;
			}
		}

		// TO BE MODIFIED
		if (game_state == IN_GAME) {
			if (num_frames % ENEMY_SPAWN_INTERVAL == 0)
			{
				createEnemy();
			}

			checkCollisions();
		}

		if (game_state == READY_GAME) {
			ready_count--;
			if (ready_count == 0) {
				changeGameState(IN_GAME);
			}
		}
		num_frames++;
	}

	count++;
}

void handleSpecialKey( int key, int x, int y )
{
	switch (key)
	{
		case GLUT_KEY_LEFT:
		{
			// TO BE MODIFIED
			if (game_state = IN_GAME) {
				if (player_obj)
				{
					if (player_obj->x - player_obj->width / 2 > PLAYER_SPEED)
					{
						player_obj->x -= PLAYER_SPEED;
					}
				}
			}
		}
		break;

		case GLUT_KEY_RIGHT:
		{
			// TO BE MODIFIED
			if (game_state = IN_GAME) {
				if (player_obj)
				{
					if (player_obj->x + player_obj->width / 2 < WIN_WIDTH - PLAYER_SPEED)
					{
						player_obj->x += PLAYER_SPEED;
					}
				}
			}
		}
		break;

		// TO BE ADDED
		case GLUT_KEY_UP:
			if (game_state == PRE_GAME) {
				menu_selection--;
				if (menu_selection < 0) menu_selection = 1;
			}
			break;

		case GLUT_KEY_DOWN:
			if (game_state == PRE_GAME) {
				menu_selection++;
				if (menu_selection > 1) menu_selection = 0;
			}
			break;
	}
}

void handleGeneralKey( unsigned char key, int x, int y )
{
	switch (key) 
	{
		// TO BE ADDED
	case 13: //ENTER
		if (game_state == PRE_GAME) {
			if (menu_selection == 0) {
				changeGameState(READY_GAME);
			}
			else {
				finalize();
				exit(0);
			}
		}
		else if (game_state == POST_GAME) {
			changeGameState(PRE_GAME);
		}
		break;

		case 32:		// spacebar
		{
			// TO BE MODIFIED
			if (game_state = IN_GAME) {
				if (player_obj)
				{
					createPlayerBullet();
				}
			}
		}
		break;
	}
}

void handleMouseButton(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
		}
		else // state == GLUT_UP
		{
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
		}
		else // state == GLUT_UP
		{
		}
		break;
	}
}

void handleMouseMove(int x, int y, int is_button_pressed)
{
	if (is_button_pressed)
	{

	}
	else
	{

	}
}


//

void createPlayer()
{
	if (player_obj)
	{
		deleteObject(player_obj);
	}

	player_obj = newObject();
	if (player_obj)
	{
		player_obj->type = PLAYER;
		player_obj->bitmap = createBitmap("alienblaster.png");
		player_obj->x = WIN_WIDTH / 2.0f;
		player_obj->y = 100.0f;
		player_obj->width = 100.0f;
		player_obj->height = 100.0f;
		player_obj->a = 180.0f;
	}
}

void createEnemy()
{
	GameObject* enemy_obj = newObject();
	if (enemy_obj)
	{
		enemy_obj->type = ( (float)rand() / (float)RAND_MAX < 0.5f ? ENEMY1 : ENEMY2 );
		enemy_obj->bitmap = createBitmap("kspaceduel.png");
		enemy_obj->x = (float)rand() / (float)RAND_MAX * (WIN_WIDTH - 100.0f) + 50.0f;
		enemy_obj->y = (float)WIN_HEIGHT + 50.0f;
		enemy_obj->width = 100.0f;
		enemy_obj->height = 100.0f;
		enemy_obj->a = 0.0f;
	}
}

void createPlayerBullet()
{
	if (!player_obj)
	{
		return;
	}

	GameObject* bullet_obj = newObject();
	if( bullet_obj )
	{
		bullet_obj->type = PLAYER_BULLET;
		bullet_obj->bitmap = createBitmap("missile.png");
		bullet_obj->width = 25.0f;
		bullet_obj->height = 50.0f;
		bullet_obj->x = player_obj->x;
		bullet_obj->y = player_obj->y + player_obj->height / 2 + bullet_obj->height / 2;
		bullet_obj->a = 0.0f;
	}
}

void createEnemyBullet( GameObject* enemy )
{
	if (!enemy)
	{
		return;
	}

	GameObject* bullet_obj = newObject();
	if( bullet_obj )
	{
		bullet_obj->type = ENEMY_BULLET;
		bullet_obj->bitmap = createBitmap("rocket.png");
		bullet_obj->width = 50.0f;
		bullet_obj->height = 25.0f;
		bullet_obj->x = enemy->x;
		bullet_obj->y = enemy->y - enemy->height / 2 - bullet_obj->width / 2;
		bullet_obj->a = -90.0f;
	}
}

void createItem()
{
	GameObject* item = newObject();
	if (item)
	{
		item->type = ITEM;
		item->bitmap = createBitmap("item3.png");
		item->x = (float)rand() / (float)RAND_MAX * (WIN_WIDTH - 100.0f) + 50.0f;
		item->y = (float)WIN_HEIGHT + 50.0f;
		item->width = 50.0f;
		item->height = 50.0f;
		item->a = 0.0f;
	}
}

void createParticles( GameObject* obj )
{
	if (!obj)
	{
		return;
	}

	int i;
	for( i=0; i < 10; i++ )
	{
		GameObject* particle = newObject();
		if( particle )
		{
			particle->type = PARTICLE;
			particle->bitmap = createBitmap( "particle.png" );
			particle->width = particle->height = randomFloat( 25.0f, 50.0f );
			particle->x = obj->x + randomFloat( -25.0f, +25.0f );
			particle->y = obj->y + randomFloat( -25.0f, +25.0f );
			particle->a = 0.0f;
		}
	}
}

void moveEnemy( GameObject* enemy )
{
	if( enemy )
	{
		switch( enemy->type )	{
		case ENEMY1:
			enemy->y -= ENEMY_SPEED;
			enemy->x += ENEMY_SPEED;
			break;

		case ENEMY2:
			enemy->y -= ENEMY_SPEED;
			enemy->x -= ENEMY_SPEED;
			break;
		}

		if (enemy->y + enemy->height / 2 < 0)
		{
			deleteObject(enemy);
		}
	}
}

void movePlayerBullet( GameObject* bullet )
{
	if( bullet )
	{
		bullet->y += BULLET_SPEED;

		if (bullet->y - bullet->height / 2 > WIN_HEIGHT)
		{
			deleteObject(bullet);
		}
	}
}

void moveEnemyBullet( GameObject* bullet )
{
	if( bullet )
	{
		bullet->y -= BULLET_SPEED;

		if (bullet->y + bullet->height / 2 < 0.0f)
		{
			deleteObject(bullet);
		}
	}
}

void moveItem(GameObject* item)
{
	if (item)
	{
		item->y -= ITEM_SPEED;
		item->a += 1.0f;

		if (item->y + item->height / 2 < 0.0f)
		{
			deleteObject(item);
		}
	}
}

void moveParticle( GameObject* particle )
{
	if( particle )
	{
		particle->x += randomFloat( -5.0f, +5.0f );
		particle->y += randomFloat( -5.0f, +5.0f );
		particle->width *= randomFloat( 0.75f, 0.95f );
		particle->height *= randomFloat( 0.75f, 0.95f );

		if( particle->age > 50 )
		{
			if( randomFloat( 0.0f, 1.0f ) < 0.1f )
			{
				deleteObject( particle );
			}
		}
	}
}

void checkCollisions()
{
	int i, j;
	for( i=0; i < POOL_SIZE; i++ )
	{
		GameObject* obj1 = getObject( i );

		if( obj1->type == ENEMY_BULLET )
		{
			if( isIntersecting( player_obj, obj1 ) )
			{
				createParticles( player_obj );
				deleteObject( obj1 );

				// TO BE ADDED
				energy -= HIT_DAMAGE;
				if (energy <= 0) {
					num_life--;
					if (num_life == 0) {
						changeGameState(POST_GAME);
					}
					else {
						changeGameState(READY_GAME);
					}
				}
				return;
			}
		}
		else if (obj1->type == ITEM)
		{
			// TO BE ADDED
		}
		else if( obj1->type == PLAYER_BULLET )
		{
			for( j=0; j < POOL_SIZE; j++ )
			{
				GameObject* obj2 = getObject( j );
				if( obj2->type == ENEMY1 || obj2->type == ENEMY2 )
				{
					if( isIntersecting( obj1, obj2 ) )
					{
						createParticles( obj2 );
						deleteObject( obj1 );
						deleteObject( obj2 );

						// TO BE ADDED
						score += HIT_SCORE;
						return;
					}
				}
			}
		}
	}
}

void changeGameState(int new_state)
{
	// TO BE ADDED
	switch (game_state) {
	case OUT_OF_GAME:
		background = createBitmap("space.bmp");
		break;

	case PRE_GAME:
		destroyBitmap(title_image);
		destroyBitmap(button_start);
		destroyBitmap(button_exit);
		title_image = NULL;
		button_start = NULL;
		button_exit = NULL;
		break;

	case READY_GAME:
		break;

	case IN_GAME:
		deleteObject(player_obj);
		player_obj = NULL;
		break;

	case POST_GAME:
		cleanupPool();
		destroyBitmap(ending_image);
		ending_image = NULL;
		break;
	}

	switch (new_state) {
	case OUT_OF_GAME:
		destroyBitmap(background);
		background = NULL;
		break;

	case PRE_GAME:
		num_life = INITIAL_LIFE;
		score = 0;

		title_image = createBitmap("title.png");
		button_start = createBitmap("button_start.bmp");
		button_exit = createBitmap("button_exit.bmp");
		menu_selection = 0;
		break;

	case READY_GAME:
		energy = INITIAL_ENERGY;
		ready_count = READY_INTERVAL;
		createPlayer();
		break;

	case IN_GAME:
		break;

	case POST_GAME:
		ending_image = createBitmap("gameover.png");
		break;
	}

	game_state = new_state;
}

void drawMenu()
{
	// title
	if (title_image)
	{
		drawBitmap(title_image, WIN_WIDTH / 2, WIN_HEIGHT - title_image->height, title_image->width, title_image->height, 0);
	}
	// button 0: start
	if( button_start )
	{
		drawBitmap(button_start, WIN_WIDTH / 2, WIN_HEIGHT / 2 + button_start->height, button_start->width, button_start->height, 0);

		if (menu_selection == 0)
		{
			setColor(1, 0, 0);
			strokeRectangle(WIN_WIDTH / 2, WIN_HEIGHT / 2 + button_start->height, button_start->width, button_start->height, 0);
		}
	}
	// button 1: exit
	if (button_exit)
	{
		drawBitmap(button_exit, WIN_WIDTH / 2, WIN_HEIGHT / 2 - button_exit->height, button_exit->width, button_exit->height, 0);

		if (menu_selection == 1)
		{
			setColor(1, 0, 0);
			strokeRectangle(WIN_WIDTH / 2, WIN_HEIGHT / 2 - button_start->height, button_exit->width, button_exit->height, 0);
		}
	}
}

void drawPlayerStates()
{
	// energy
	setColor(1, 1, 1);
	strokeRectangle(60, WIN_HEIGHT - 30, 100, 30, 0);

	if (energy <= 20)
	{
		setColor(1, 0, 0);
	}
	else
	{
		setColor(0, 0, 1);
	}
	fillRectangle(60 - (100-energy) / 2, WIN_HEIGHT - 30, energy-2, 28, 0);

	// score
	char score_str[10];
	sprintf(score_str, "%3d", score);
	setColor(1, 1, 1);
	drawText(WIN_WIDTH / 2, WIN_HEIGHT - 45, GLUT_BITMAP_TIMES_ROMAN_24, score_str);

	// life
	int i;
	for (i = 0; i < num_life; i++)
	{
		setColor(0, 1, 0);
		fillCircle(WIN_WIDTH - 30 - i * 30, WIN_HEIGHT - 30, 10);
	}
}

void drawGameOver()
{
	if (ending_image)
	{
		drawBitmap(ending_image, WIN_WIDTH / 2, WIN_HEIGHT / 2, ending_image->width, ending_image->height, 0);
	}
}
