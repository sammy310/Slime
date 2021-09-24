#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>

#include "game.h"
#include "glut.h"

#define _USE_MATH_DEFINES
#include <math.h>

#define ALPHA						0.8f

#define POOL_SIZE					1500

#define GAME_UPDATE_INTERVAL		1
#define READY_INTERVAL				20
#define ENEMY_SPAWN_INTERVAL		10

#define BULLET_PROBABILITY			0.025f

#define PLAYER_SPEED				10.0f
#define PLAYER_SPEED_SLOW			2
#define PLAYER_INVINCIBILITY_DELAY	15
#define PET_SPEED					5.0f
#define ENEMY_SPEED					10.0f

#define INITIAL_LIFE				1
#define INITIAL_ENERGY				10
#define HIT_DAMAGE					10
#define HIT_SCORE					10


const int WIN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
const int WIN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);

int MAPSIZE_WIDTH;
int MAPSIZE_HEIGHT = MAPSIZE_WIDTH = 4500;

enum
{
	UNDEFINED = -1,
	PLAYER,
	PET1,
	PET2,
	PET3,
	PET4,
	ENEMY1,
	ENEMY2,
	ENEMY3,
	ENEMY4,
	ENEMY5,
	ENEMY6,
	BOSS,
	PLAYER_BULLET,
	PET_BULLET,
	ENEMY_BULLET,
	BOSS_BULLET1,
	BOSS_BULLET2,
	BOSS_BULLET3,
	BOSS_BULLET4,
	BOSS_BULLET5,
	BOSS_BULLET6,
	PARTICLE,
};

// Image
Bitmap* image_background_menu = NULL;
Bitmap* image_background_game = NULL;
Bitmap* image_title = NULL;
// Menu
Bitmap* image_menu_start = NULL;
Bitmap* image_menu_ranking = NULL;
Bitmap* image_menu_option = NULL;
Bitmap* image_menu_exit = NULL;
// Mode Select
//Bitmap* image_select_mode_1 = NULL;
Bitmap* image_select_mode_2 = NULL;
Bitmap* image_select_mode_3 = NULL;
Bitmap* image_select_mode_4 = NULL;
Bitmap* image_select_mode_4_select = NULL;
Bitmap* image_select_exit = NULL;
// Ranking
Bitmap* image_ranking_allkill = NULL;
Bitmap* image_ranking_infinite = NULL;
Bitmap* image_ranking_boss = NULL;
Bitmap* image_ranking_rank = NULL;
Bitmap* image_ranking_date = NULL;
Bitmap* image_ranking_stage = NULL;
Bitmap* image_ranking_score = NULL;
Bitmap* image_ranking_time = NULL;
// Option
Bitmap* image_minimapType_1 = NULL;
Bitmap* image_minimapType_2 = NULL;
Bitmap* image_minimapType_3 = NULL;
Bitmap* image_minimapType_4 = NULL;
Bitmap* image_explanation = NULL;
Bitmap* image_explanation_image = NULL;
Bitmap* image_option_exit = NULL;

Bitmap* image_0 = NULL;
Bitmap* image_1 = NULL;
Bitmap* image_2 = NULL;
Bitmap* image_3 = NULL;
Bitmap* image_4 = NULL;
Bitmap* image_5 = NULL;
Bitmap* image_6 = NULL;
Bitmap* image_7 = NULL;
Bitmap* image_8 = NULL;
Bitmap* image_9 = NULL;

Bitmap* image_mode_2_stage_clear = NULL;
Bitmap* image_game_over = NULL;
Bitmap* image_press_enter = NULL;

Bitmap* image_player1 = NULL;
Bitmap* image_player2 = NULL;
Bitmap* image_pet = NULL;
Bitmap* image_enemy1 = NULL;
Bitmap* image_enemy2 = NULL;
Bitmap* image_enemy3 = NULL;
Bitmap* image_enemy4 = NULL;
Bitmap* image_enemy5 = NULL;
Bitmap* image_enemy6 = NULL;
Bitmap* image_boss = NULL;
Bitmap* image_playerBullet = NULL;
Bitmap* image_petBullet = NULL;
Bitmap* image_particle = NULL;

void createPlayer(int collision);
void createPet();
void createEnemy();
void spawnEnemy(int enemyType);
void createPlayerBullet();
void createPetBullet(GameObject* pet);
void createEnemy4Bullet( GameObject* enemy );
void createParticles( GameObject* obj );

void movePlayer();
void movePet(GameObject* pet);
void moveEnemy( GameObject* enemy );
void movePlayerBullet( GameObject* bullet );
void movePetBullet(GameObject* bullet);
void moveEnemyBullet( GameObject* bullet );
void moveParticle( GameObject* particle );

void shootBullet();

void checkCollisions();

void InitImage();
void DestroyImage();

// Player
GameObject* player_obj = NULL;
GameObject player_obj_collision = { 0 };
int num_life = 0;
int energy = 0;
int score = 0;
int invincibility_delay = 0;

float player_collision_ratio = 0.6f;

// Bullet
int playerBullet_damage = 10;
int playerBullet_delay = 20;
int playerBullet_reach = 40;
int playerBullet_speed = 20;

// Draw
void drawMenu();
void drawModeSelection();
void drawOption();
void drawExplanation();
void drawMenuMob(int mobNum);
void drawUI();
void drawStageClear(int mode);
void drawGameOver();
void drawMinimap(int minimapType);
void drawRanking();

// Pet
int petCount = 0;

int petBullet_damage = 10;
int petBullet_delay = 30;
int petBullet_reach = 30;
int petBullet_speed = 20;

// Enemy
int enemyInitSize = 400;
int enemySpawnSize = 400;
int enemyCount = 0;

int enemyHealth = 10;
int enemyBullet_reach = 20;
int enemyBullet_speed = 10;

int isEnemy6 = 0;

int enemyProbability_1 = 50;
int enemyProbability_2 = 0;
int enemyProbability_3 = 0;
int enemyProbability_4 = 0;
int enemyProbability_5 = 0;
int enemyProbability_6 = 0;

// Game state
enum
{
	OUT_OF_GAME = -1,
	GAME_MENU,
	GAME_MODE_SELECT,
	GAME_RANKING,
	GAME_OPTION,
	GAME_EXPLANATION,
	GAME_MODE_1_INIT,
	GAME_MODE_2_INIT,
	GAME_MODE_3_INIT,
	GAME_MODE_4_INIT,
	GAME_MODE_1,
	GAME_MODE_2,
	GAME_MODE_3,
	GAME_MODE_4,
	GAME_MODE_2_STAGE_CLEAR,
	GAME_MODE_4_STAGE_CLEAR,
	GAME_MODE_1_END,
	GAME_MODE_2_END,
	GAME_MODE_3_END,
	GAME_MODE_4_END
};
int game_mode = OUT_OF_GAME;
void changeGameMode(int new_mode);

// Mode
int mode_2_Stage = 1;
int mode_4_Stage = 1;

void mode_1();
void mode_2();
void mode_3();
void mode_4();

void mode_2_Init();
void mode_3_Init();
void mode_4_Init();

// Pattern
int pattern = 0;
float patternStartTime = 0;

void Pattern_Spread();
void Pattern_1();
void Pattern_2();
void Pattern_3();
void Pattern_4();
void Pattern_5();
void Pattern_6(int size);
void Pattern_7();
void Pattern_8();
void Pattern_9();

int patternTargetTime_1 = 60;
int patternTargetTime_2 = 120;
int patternTargetTime_3 = 180;
int patternTargetTime_4 = 240;
int patternTargetTime_5 = 300;
int patternTargetTime_6 = 360;
int patternTargetTime_7 = 420;
int patternTargetTime_8 = 480;
int patternTargetTime_9 = 540;

int patternTime_1 = 20;
int patternTime_2 = 20;
int patternTime_3 = 20;
int patternTime_4 = 20;
int patternTime_5 = 20;
int patternTime_6 = 10;
int patternTime_7 = 0;
int patternTime_8 = 0;
int patternTime_9 = 0;

// Boss
GameObject* boss_obj = NULL;
int bossHealth = 5000;
int bossBulletSpeed = 4;
int bossBulletReach = 60;

void createBoss();
void createBossBullet(int bulletType, float bulletSpeed, int reach, float to_x, float to_y);
void createBossBullet_Custom(int bulletType, float bulletSpeed, int reach, float from_x, float from_y, float to_x, float to_y);
void moveBossBullet(GameObject* bullet);
void bulletPattern_1(int bulletType, int bulletNum, float bulletSpeed, int reach, float degree);
void bulletPattern_1_Custom(int bulletType, int bulletNum, float bulletSpeed, int reach, float degree, float from_x, float from_y);

// Boss Bullet Pattern
int bossPattern_1(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed);
int bossPattern_2(float startingTime, int delay, int loop, int bulletNum, float bulletSpeeda);
int bossPattern_3(float startingTime, int delay, int loop, int bulletNum, float bulletSpeed, int reach);
int bossPattern_4(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed);
int bossPattern_5(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed);
int bossPattern_6(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed, int reach);
int bossPattern_7(float startingTime);

// Time
clock_t startTime;
float elapsedTime;
float totalTime;

// Ranking
int ranking_page;
int ranking_sortType;
int ranking_print_min;
int ranking_print_max;

int SetCutSize();

// Score
int score_Mode_2_Size = 0;
int score_Mode_3_Size = 0;
int score_Mode_4_Size = 0;

int* score_Mode_2_Sequence = NULL;
int* score_Mode_3_Sequence = NULL;
int* score_Mode_4_Sequence = NULL;

Score_Mode_1* score_Mode_2 = NULL;
Score_Mode_2* score_Mode_3 = NULL;
Score_Mode_1* score_Mode_4 = NULL;

void initializeScoreArr(int mode);
void finalizeScore();
void SaveScore();
void LoadScore();
void appendScoreArr(int mode2_size, int mode3_size, int mode4_size);
void NewScore(int mode);

void SortScore_Stage(int mode, int sort_type);
void SortScore_Score(int mode);
void SortScore_Score_Custom(int mode, int from, int to);
void SortScore_Time(int mode);
void SortScore_Time_Custom(int mode, int from, int to);

// etc
int ready_count = 0;
int menu_selection = 1;
int mode_selection = 1;
int option_selection = 1;

int menu_mobNum;

int isLMouseDown;
int mouse_x, mouse_y;
float camera_x, camera_y;

float vector_x, vector_y, vector_sum;
float degree;

void GetCameraPos(int* x, int* y);
void ConvertCollision(GameObject* obj, GameObject* obj_collision);

int minimap_type = 2;
int collision_mark = 0;


//
void main()
{
	initVisualApp( WIN_WIDTH, WIN_HEIGHT );
}

void initialize()
{
	initializePool( POOL_SIZE );
	InitImage();
	changeGameMode(GAME_MENU);
}

void finalize()
{
	changeGameMode(OUT_OF_GAME);
	DestroyImage();
	finalizePool();
	finalizeScore();
}

void draw()
{
	static int count = 0;


	switch (game_mode) {
	case GAME_MENU:
	case GAME_MODE_SELECT:
	case GAME_RANKING:
	case GAME_OPTION:
	case GAME_EXPLANATION:
		if (image_background_menu) {
			drawBitmap(image_background_menu, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0);
		}
		break;
	case GAME_MODE_1_INIT:
	case GAME_MODE_2_INIT:
	case GAME_MODE_3_INIT:
	case GAME_MODE_4_INIT:
	case GAME_MODE_1:
	case GAME_MODE_2:
	case GAME_MODE_3:
	case GAME_MODE_4:
	case GAME_MODE_2_STAGE_CLEAR:
	case GAME_MODE_4_STAGE_CLEAR:
	case GAME_MODE_1_END:
	case GAME_MODE_2_END:
	case GAME_MODE_3_END:
	case GAME_MODE_4_END:
		if (image_background_game) {
			drawBitmap(image_background_game, 0, 0, MAPSIZE_WIDTH, MAPSIZE_HEIGHT, 0);
		}
		break;
	}
	
	GameObject game_obj_collision = { 0 };
	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* game_obj = getObject(i);

		if (game_obj && game_obj->is_alive && game_obj->bitmap)
		{
			if( ready_count > 0 && game_obj == player_obj && ready_count % 5 == 0 )
			{
				continue;
			}
			else if (invincibility_delay > 0 && game_obj == player_obj && invincibility_delay % 5 == 0) {
				continue;
			}
			drawBitmap(game_obj->bitmap, game_obj->x, game_obj->y, game_obj->width, game_obj->height, game_obj->a);

			if (collision_mark) {
				if (game_obj->type != PLAYER) {
					ConvertCollision(game_obj, &game_obj_collision);
					setColor(1, 1, 1);
					strokeRectangle(game_obj_collision.x, game_obj_collision.y, game_obj_collision.width, game_obj_collision.height, game_obj_collision.a, ALPHA / 2);
				}
			}
		}
	}

	switch (game_mode)
	{
	case GAME_MENU:	
		drawMenu();
		break;
	case GAME_MODE_SELECT:
		drawModeSelection();
		break;
	case GAME_RANKING:
		drawRanking();
		break;
	case GAME_OPTION:
		drawOption();
		break;
	case GAME_EXPLANATION:
		drawExplanation();
		break;

	case GAME_MODE_1_INIT:
	case GAME_MODE_2_INIT:
	case GAME_MODE_3_INIT:
	case GAME_MODE_4_INIT:
	case GAME_MODE_1:
	case GAME_MODE_2:
	case GAME_MODE_3:
	case GAME_MODE_4:
		drawUI();

		// player collision
		if (player_obj) {
			setColor(1, 0, 1);
			strokeRectangle(player_obj_collision.x, player_obj_collision.y, player_obj_collision.width, player_obj_collision.height, player_obj_collision.a);
		}

		drawMinimap(minimap_type);
		break;

	case GAME_MODE_2_STAGE_CLEAR:
	case GAME_MODE_4_STAGE_CLEAR:
		drawUI();

		// player collision
		if (player_obj) {
			setColor(1, 0, 1);
			strokeRectangle(player_obj_collision.x, player_obj_collision.y, player_obj_collision.width, player_obj_collision.height, player_obj_collision.a);
		}

		if (game_mode == GAME_MODE_2_STAGE_CLEAR) {
			drawStageClear(2);
		}
		else if (game_mode == GAME_MODE_4_STAGE_CLEAR) {
			drawStageClear(4);
		}
		drawMinimap(minimap_type);
		break;

	case GAME_MODE_1_END:
	case GAME_MODE_2_END:
	case GAME_MODE_3_END:
	case GAME_MODE_4_END:
		drawUI();
		drawGameOver();
		drawMinimap(minimap_type);
		break;
	}

	// mouse cursor
	setColor(1, 0, 0);
	strokeCircle(camera_x + mouse_x, camera_y + mouse_y, 10);

	count++;
}

void update()
{
	static int count = 1;
	static int num_frames = 1;

	if (count % GAME_UPDATE_INTERVAL == 0)
	{
		int i;
		for (i = 0; i < POOL_SIZE; i++)
		{
			GameObject* game_obj = getObject(i);
			if (game_obj->is_alive)
			{
				switch (game_obj->type) {
				case PET1:
				case PET2:
				case PET3:
				case PET4:
					movePet(game_obj);
					if (game_obj->delay == 0) {
						createPetBullet(game_obj);
						game_obj->delay = petBullet_delay;
					}
					else if (game_obj->delay != 0) {
						game_obj->delay--;
					}
					break;
				case ENEMY1:
				case ENEMY2:
				case ENEMY3:
				case ENEMY4:
				case ENEMY5:
				case ENEMY6:
					moveEnemy(game_obj);
					break;

				case PLAYER_BULLET:
					movePlayerBullet(game_obj);
					break;

				case PET_BULLET:
					movePetBullet(game_obj);
					break;

				case ENEMY_BULLET:
					moveEnemyBullet(game_obj);
					break;

				case BOSS_BULLET1:
				case BOSS_BULLET2:
				case BOSS_BULLET3:
				case BOSS_BULLET4:
				case BOSS_BULLET5:
				case BOSS_BULLET6:
					moveBossBullet(game_obj);
					break;

				case PARTICLE:
					moveParticle(game_obj);
					break;
				}

				game_obj->age++;
			}
		}

		switch (game_mode) {
		case GAME_MODE_1_INIT:
			ready_count--;
			if (ready_count == 0) {
				changeGameMode(GAME_MODE_1);
			}
			elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;
			break;
		case GAME_MODE_2_INIT:
			ready_count--;
			if (ready_count == 0) {
				changeGameMode(GAME_MODE_2);
			}
			elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;
			break;
		case GAME_MODE_3_INIT:
			ready_count--;
			if (ready_count == 0) {
				changeGameMode(GAME_MODE_3);
			}
			elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;
			break;
		case GAME_MODE_4_INIT:
			ready_count--;
			if (ready_count == 0) {
				changeGameMode(GAME_MODE_4);
			}
			elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;
			break;

		case GAME_MODE_1:
			mode_1();
			break;
		case GAME_MODE_2:
			mode_2();
			break;
		case GAME_MODE_3:
			mode_3();
			break;
		case GAME_MODE_4:
			mode_4();
			break;
		}

		num_frames++;
	}

	count++;
}

void handleSpecialKey( int key, int x, int y )
{
	switch (key)
	{
	case GLUT_KEY_UP:
		switch (game_mode) {
		case GAME_MENU:
			menu_selection--;
			if (menu_selection < 0) menu_selection = 3;
			break;

		case GAME_MODE_SELECT:
			mode_selection--;
			if (mode_selection < 0) mode_selection = 3;
			break;

		case GAME_RANKING:
			if (ranking_print_min > 0) {
				ranking_print_min--;
				ranking_print_max--;
			}
			break;

		case GAME_OPTION:
			option_selection--;
			if (option_selection < 0) option_selection = 2;
			break;
		}
		break;

	case GLUT_KEY_DOWN:
		switch (game_mode) {
		case GAME_MENU:
			menu_selection++;
			if (menu_selection > 3) menu_selection = 0;
			break;

		case GAME_MODE_SELECT:
			mode_selection++;
			if (mode_selection > 3) mode_selection = 0;
			break;

		case GAME_RANKING:
			if (ranking_page == 0) {
				if (ranking_print_max < score_Mode_2_Size - 1) {
					ranking_print_min++;
					ranking_print_max++;
				}
			}
			else if (ranking_page == 1) {
				if (ranking_print_max < score_Mode_3_Size - 1) {
					ranking_print_min++;
					ranking_print_max++;
				}
			}
			else if (ranking_page == 2) {
				if (ranking_print_max < score_Mode_4_Size - 1) {
					ranking_print_min++;
					ranking_print_max++;
				}
			}
			break;

		case GAME_OPTION:
			option_selection++;
			if (option_selection > 2) option_selection = 0;

			break;
		}
		break;

	case GLUT_KEY_LEFT:
		if (game_mode == GAME_RANKING) {
			if (ranking_page == 1) {
				ranking_page = 0;
				ranking_print_min = 0;
				ranking_print_max = SetCutSize();

				SortScore_Stage(2, 1);
				ranking_sortType = 1;
			}
			else if (ranking_page == 2) {
				ranking_page = 1;
				ranking_print_min = 0;
				ranking_print_max = SetCutSize();

				SortScore_Time(3);
				ranking_sortType = 0;
			}
		}
		break;

	case GLUT_KEY_RIGHT:
		if (game_mode == GAME_RANKING) {
			if (ranking_page == 0) {
				ranking_page = 1;
				ranking_print_min = 0;
				ranking_print_max = SetCutSize();

				SortScore_Time(3);
				ranking_sortType = 0;
			}
			else if (ranking_page == 1) {
				ranking_page = 2;
				ranking_print_min = 0;
				ranking_print_max = SetCutSize();

				SortScore_Stage(4, 1);
				ranking_sortType = 1;
			}
		}
		break;

	case GLUT_KEY_F1:
		if (game_mode == GAME_RANKING) {
			ranking_sortType = 0;
			if (ranking_page == 0) {
				SortScore_Stage(2, 0);
			}
			else if (ranking_page == 1) {
				SortScore_Time(3);
			}
			else if (ranking_page == 2) {
				SortScore_Stage(4, 0);
			}
		}
		break;

	case GLUT_KEY_F2:
		if (game_mode == GAME_RANKING) {
			ranking_sortType = 1;
			if (ranking_page == 0) {
				SortScore_Stage(2, 1);
			}
			else if (ranking_page == 1) {
				SortScore_Score(3);
			}
			else if (ranking_page == 2) {
				SortScore_Stage(4, 1);
			}
		}
		break;
		
	case GLUT_KEY_F11:
		if (game_mode == GAME_OPTION) {
			collision_mark = 0;
		}
		break;
	case GLUT_KEY_F12:
		if (game_mode == GAME_OPTION) {
			collision_mark = 1;
		}
		break;
	}
}

void handleGeneralKey( unsigned char key, int x, int y )
{
	switch (key) 
	{
		case 13:		// enter
		{
			switch (game_mode) {
			case GAME_MENU:
				switch (menu_selection) {
				case 0:
					finalize();
					PostQuitMessage(0);
					exit(0);
					break;
				case 1:
					changeGameMode(GAME_MODE_SELECT);
					break;
				case 2:
					changeGameMode(GAME_RANKING);
					break;
				case 3:
					changeGameMode(GAME_OPTION);
				}
				break;

			case GAME_MODE_SELECT:
				switch (mode_selection) {
				case 0:
					changeGameMode(GAME_MENU);
					break;
					/*
				case 1:
					changeGameMode(GAME_MODE_1_INIT);
					break;
					*/
				case 1:
					changeGameMode(GAME_MODE_2_INIT);
					break;
				case 2:
					changeGameMode(GAME_MODE_3_INIT);
					break;
				case 3:
					changeGameMode(GAME_MODE_4_INIT);
					break;
				}
				break;

			case GAME_OPTION:
				switch (option_selection) {
				case 0:
					changeGameMode(GAME_MENU);
					break;
				case 1:
					minimap_type++;
					if (minimap_type > 3) minimap_type = 0;
					break;
				case 2:
					changeGameMode(GAME_EXPLANATION);
					break;
				}
				break;

			case GAME_EXPLANATION:
				changeGameMode(GAME_OPTION);
				break;

			case GAME_MODE_2_STAGE_CLEAR:
				changeGameMode(GAME_MODE_2_INIT);
				break;

			case GAME_MODE_1_END:
			case GAME_MODE_2_END:
			case GAME_MODE_3_END:
			case GAME_MODE_4_END:
			case GAME_MODE_4_STAGE_CLEAR:
				changeGameMode(GAME_MENU);
				break;
			}
		}
		break;

		case 32:		// spacebar
		{
			if( game_mode == GAME_MODE_1 )
			{
				
			}
		}
		break;

		case 27: //ESC
			switch (game_mode) {
			case GAME_MODE_SELECT:
				changeGameMode(GAME_MENU);
				break;

			case GAME_RANKING:
				changeGameMode(GAME_MENU);
				break;

			case GAME_OPTION:
				changeGameMode(GAME_MENU);
				break;

			case GAME_EXPLANATION:
				changeGameMode(GAME_OPTION);
				break;

				/*
			case GAME_MODE_1:
				changeGameMode(GAME_MODE_1_END);
				changeGameMode(GAME_MENU);
				break;
				*/
			case GAME_MODE_2:
				changeGameMode(GAME_MODE_2_END);
				changeGameMode(GAME_MENU);
			case GAME_MODE_3:
				changeGameMode(GAME_MODE_3_END);
				changeGameMode(GAME_MENU);
				break;
			case GAME_MODE_4:
				changeGameMode(GAME_MODE_4_END);
				changeGameMode(GAME_MENU);
				break;
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
			isLMouseDown = 1;
		}
		else // state == GLUT_UP
		{
			isLMouseDown = 0;
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
	mouse_x = x;
	mouse_y = y;

	if (is_button_pressed)
	{

	}
	else
	{

	}
}


//

void createPlayer(int collision)
{
	if (player_obj)
	{
		deleteObject(player_obj);
	}

	player_obj = newObject();
	if (player_obj)
	{
		player_obj->type = PLAYER;
		player_obj->bitmap = image_player1;
		player_obj->width = 100.0f;
		player_obj->height = 100.0f;

		// collision
		if (collision == 1) {
			player_obj_collision.x = 0;
			player_obj_collision.y = -(MAPSIZE_HEIGHT / 8) * 3;
			player_obj_collision.a = 0;
			player_obj_collision.width = 3;
			player_obj_collision.height = 3;
		}
		else {
			player_obj_collision.x = 0;
			player_obj_collision.y = 0;
			player_obj_collision.a = 0;
			player_obj_collision.width = (player_obj->width / 2) * player_collision_ratio;
			player_obj_collision.height = player_obj_collision.width;
		}

		player_obj->x = player_obj_collision.x + 15;
		player_obj->y = player_obj_collision.y + 10;
		player_obj->a = 0;
		player_obj->size = sqrt(pow(player_obj->width, 2) + pow(player_obj->height, 2)) / 2;

		player_obj_collision.size = player_obj->size + 15;

		invincibility_delay = 0;

	}
}

void createPet() {
	GameObject* pet_obj = newObject();

	if (pet_obj && player_obj){
		switch (petCount) {
		case 0:
			pet_obj->type = PET1;
			pet_obj->x = player_obj_collision.x - player_obj_collision.size;
			pet_obj->y = player_obj_collision.y;
			break;
		case 1:
			pet_obj->type = PET2;
			pet_obj->x = player_obj_collision.x + player_obj_collision.size;
			pet_obj->y = player_obj_collision.y;
			break;
		case 2:
			pet_obj->type = PET3;
			pet_obj->x = player_obj_collision.x;
			pet_obj->y = player_obj_collision.y + player_obj_collision.size;
			break;
		case 3:
			pet_obj->type = PET4;
			pet_obj->x = player_obj_collision.x;
			pet_obj->y = player_obj_collision.y - player_obj_collision.size;
			break;
		}
		pet_obj->bitmap = image_pet;
		pet_obj->width = 60.0f;
		pet_obj->height = 60.0f;
		pet_obj->size = sqrt(pow(pet_obj->width, 2) + pow(pet_obj->height, 2)) / 2;

		pet_obj->a = 0;

		pet_obj->delay = 0;


		petCount++;
	}
}

void createEnemy()
{
	GameObject* enemy_obj = newObject();

	if (enemy_obj)
	{
		enemy_obj->type = ENEMY1;
		enemy_obj->bitmap = image_enemy1;
		enemy_obj->x = ((float)rand() / (float)RAND_MAX < 0.5f ? -((float)MAPSIZE_WIDTH / 4) : (float)MAPSIZE_WIDTH / 4);
		enemy_obj->y = ((float)rand() / (float)RAND_MAX < 0.5f ? -((float)MAPSIZE_HEIGHT / 4) : (float)MAPSIZE_HEIGHT / 4);
		enemy_obj->width = 100.0f;
		enemy_obj->height = 100.0f;
		enemy_obj->size = sqrt(pow(enemy_obj->width, 2) + pow(enemy_obj->height, 2)) / 2;

		enemy_obj->move_x = randomFloat(-1.0f, 1.0f);
		enemy_obj->move_y = randomFloat(-1.0f, 1.0f);
		float vector_total = sqrt(pow(enemy_obj->move_x, 2) + pow(enemy_obj->move_y, 2));
		enemy_obj->move_x = (enemy_obj->move_x / vector_total) * ENEMY_SPEED;
		enemy_obj->move_y = (enemy_obj->move_y / vector_total) * ENEMY_SPEED;
		
		enemy_obj->a = atan2(enemy_obj->move_y, enemy_obj->move_x) * (180 / M_PI) + 90;

		enemy_obj->health = 10;
		enemy_obj->corner = 0;
		enemy_obj->delay = 0;

		enemyCount++;
	}
}

void spawnEnemy(int enemyType) {
	GameObject* enemy_obj = newObject();

	if (enemy_obj)
	{
		switch (enemyType) {
		case 1:
			enemy_obj->type = ENEMY1;
			enemy_obj->bitmap = image_enemy1;

			enemy_obj->width = 100.0f;
			enemy_obj->height = 100.0f;
			enemy_obj->health = enemyHealth;
			break;
		case 2:
			enemy_obj->type = ENEMY2;
			enemy_obj->bitmap = image_enemy2;

			enemy_obj->width = 100.0f;
			enemy_obj->height = 100.0f;
			enemy_obj->health = enemyHealth * 2;
			break;
		case 3:
			enemy_obj->type = ENEMY3;
			enemy_obj->bitmap = image_enemy3;

			enemy_obj->width = 100.0f;
			enemy_obj->height = 100.0f;
			enemy_obj->health = enemyHealth;
			break;
		case 4:
			enemy_obj->type = ENEMY4;
			enemy_obj->bitmap = image_enemy4;

			enemy_obj->width = 100.0f;
			enemy_obj->height = 100.0f;
			enemy_obj->health = enemyHealth;
			break;
		case 5:
			enemy_obj->type = ENEMY5;
			enemy_obj->bitmap = image_enemy5;

			enemy_obj->width = 100.0f;
			enemy_obj->height = 100.0f;
			enemy_obj->health = enemyHealth * 3;
			break;
		case 6:
			enemy_obj->type = ENEMY6;
			enemy_obj->bitmap = image_enemy6;

			enemy_obj->width = 100.0f;
			enemy_obj->height = 100.0f;
			enemy_obj->health = enemyHealth;

			isEnemy6 = 1;
			break;
		}

		enemy_obj->size = sqrt(pow(enemy_obj->width, 2) + pow(enemy_obj->height, 2)) / 2;
		enemy_obj->delay = 0;

		enemy_obj->corner = ((int)rand() % 4) + 1;
		switch (enemy_obj->corner) {
		case 1:
			enemy_obj->x = ((float)rand() / (float)RAND_MAX * MAPSIZE_WIDTH) * 0.9 - MAPSIZE_WIDTH * 0.9 / 2;
			enemy_obj->y = -(MAPSIZE_HEIGHT / 2);
			enemy_obj->move_x = randomFloat(-1.0f, 1.0f);
			enemy_obj->move_y = randomFloat(0.3f, 1.0f);
			break;
		case 2:
			enemy_obj->x = MAPSIZE_WIDTH / 2;
			enemy_obj->y = ((float)rand() / (float)RAND_MAX * MAPSIZE_HEIGHT) * 0.9 - MAPSIZE_HEIGHT * 0.9 / 2;
			enemy_obj->move_x = randomFloat(-1.0f, -0.3f);
			enemy_obj->move_y = randomFloat(-1.0f, 1.0f);
			break;
		case 3:
			enemy_obj->x = ((float)rand() / (float)RAND_MAX * MAPSIZE_WIDTH) * 0.9 - MAPSIZE_WIDTH * 0.9 / 2;
			enemy_obj->y = MAPSIZE_HEIGHT / 2;
			enemy_obj->move_x = randomFloat(-1.0f, 1.0f);
			enemy_obj->move_y = randomFloat(-1.0f, -0.3f);
			break;
		case 4:
			enemy_obj->x = -(MAPSIZE_WIDTH / 2);
			enemy_obj->y = ((float)rand() / (float)RAND_MAX * MAPSIZE_HEIGHT) * 0.9 - MAPSIZE_HEIGHT * 0.9 / 2;
			enemy_obj->move_x = randomFloat(0.3f, 1.0f);
			enemy_obj->move_y = randomFloat(-1.0f, 1.0f);
			break;
		}

		float vector_total = sqrt(pow(enemy_obj->move_x, 2) + pow(enemy_obj->move_y, 2));
		enemy_obj->move_x = (enemy_obj->move_x / vector_total) * ENEMY_SPEED;
		enemy_obj->move_y = (enemy_obj->move_y / vector_total) * ENEMY_SPEED;

		enemy_obj->a = atan2(enemy_obj->move_y, enemy_obj->move_x) * (180 / M_PI) + 90;

		if (enemy_obj->type != ENEMY6) {
			enemyCount++;
		}
	}
}

void createPlayerBullet()
{
	GameObject* bullet_obj = newObject();

	if( bullet_obj )
	{
		bullet_obj->type = PLAYER_BULLET;
		bullet_obj->bitmap = image_playerBullet;
		bullet_obj->health = 1;
		bullet_obj->width = 25.0f;
		bullet_obj->height = 50.0f;
		bullet_obj->x = player_obj_collision.x + (player_obj_collision.size * cos(degree));
		bullet_obj->y = player_obj_collision.y + (player_obj_collision.size * sin(degree));
		bullet_obj->a = degree * (180 / M_PI) - 90;
		bullet_obj->move_x = (vector_x / vector_sum) * playerBullet_speed;
		bullet_obj->move_y = (vector_y / vector_sum) * playerBullet_speed;
	}
}

void createPetBullet(GameObject* pet) {
	float enemy_x, enemy_y;
	float lenth_min = MAPSIZE_WIDTH, lenth;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* obj = getObject(i);

		if (obj->type == ENEMY1 || obj->type == ENEMY2) {
			lenth = sqrt(pow((pet->x - obj->x), 2) + pow((pet->y - obj->y), 2));
			if (lenth_min > lenth) {
				lenth_min = lenth;
				enemy_x = obj->x;
				enemy_y = obj->y;
			}
		}
	}
	if (lenth_min > petBullet_reach * petBullet_speed) {
		return;
	}

	GameObject* bullet_obj = newObject();
	if (bullet_obj){
		bullet_obj->type = PET_BULLET;
		bullet_obj->bitmap = image_petBullet;
		bullet_obj->health = 1;
		bullet_obj->width = 50.0f;
		bullet_obj->height = 50.0f;
		bullet_obj->x = pet->x;
		bullet_obj->y = pet->y;

		bullet_obj->move_x = enemy_x - pet->x;
		bullet_obj->move_y = enemy_y - pet->y;
		float vector_total = sqrt(pow(bullet_obj->move_x, 2) + pow(bullet_obj->move_y, 2));
		bullet_obj->move_x = (bullet_obj->move_x / vector_total) * petBullet_speed;
		bullet_obj->move_y = (bullet_obj->move_y / vector_total) * petBullet_speed;

		bullet_obj->a = atan2(bullet_obj->move_y, bullet_obj->move_x) * (180 / M_PI);
	}
}

void createEnemy4Bullet(GameObject* enemy) {
	int i;
	for (i = 0; i < 4; i++) {
		GameObject* bullet_obj = newObject();

		if (bullet_obj)
		{
			bullet_obj->type = ENEMY_BULLET;
			bullet_obj->bitmap = image_enemy4;
			bullet_obj->width = 25.0f;
			bullet_obj->height = 25.0f;
			bullet_obj->x = enemy->x;
			bullet_obj->y = enemy->y;

			switch (i) {
			case 0:
				bullet_obj->move_x = enemyBullet_speed;
				bullet_obj->move_y = 0;
				bullet_obj->a = -90.0f;
				break;
			case 1:
				bullet_obj->move_x = 0;
				bullet_obj->move_y = enemyBullet_speed;
				bullet_obj->a = 0.0f;
				break;
			case 2:
				bullet_obj->move_x = -enemyBullet_speed;
				bullet_obj->move_y = 0;
				bullet_obj->a = 90.0f;
				break;
			case 3:
				bullet_obj->move_x = 0;
				bullet_obj->move_y = -enemyBullet_speed;
				bullet_obj->a = 180.0f;
				break;
			}


		}
	}

}

void createParticles( GameObject* obj )
{
	int i;
	for( i=0; i < 10; i++ )
	{
		GameObject* particle = newObject();

		if( particle )
		{
			particle->type = PARTICLE;
			particle->bitmap = image_particle;
			particle->width = particle->height = randomFloat( 25.0f, 50.0f );
			particle->x = obj->x + randomFloat( -25.0f, +25.0f );
			particle->y = obj->y + randomFloat( -25.0f, +25.0f );
			particle->a = 0.0f;
		}
	}
}

void movePlayer() {
	if (player_obj) {
		int slow;
		if (isKeyDown(VK_SHIFT)) {
			slow = PLAYER_SPEED_SLOW;
		}
		else {
			slow = 1;
		}
		if (isKeyDown(0x57) || isKeyDown(VK_UP)) { //w
			if (player_obj_collision.y + player_obj_collision.height / 2 < MAPSIZE_HEIGHT / 2 - 20.0f) {
				player_obj_collision.y += PLAYER_SPEED / slow;
				player_obj->y = player_obj_collision.y + 10;
			}
		}
		if (isKeyDown(0x53) || isKeyDown(VK_DOWN)) { //s
			if (player_obj_collision.y - player_obj_collision.height / 2 > -(MAPSIZE_HEIGHT / 2) + 20.0f) {
				player_obj_collision.y -= PLAYER_SPEED / slow;
				player_obj->y = player_obj_collision.y + 10;
			}
		}
		if (isKeyDown(0x41) || isKeyDown(VK_LEFT)) { //a
			if (player_obj_collision.x - player_obj_collision.width > -(MAPSIZE_WIDTH / 2) + 20.0f) {
				player_obj_collision.x -= PLAYER_SPEED / slow;
			}
		}
		if (isKeyDown(0x44) || isKeyDown(VK_RIGHT)) { //d
			if (player_obj_collision.x + player_obj_collision.width < MAPSIZE_WIDTH / 2 - 20.0f) {
				player_obj_collision.x += PLAYER_SPEED / slow;
			}
		}

		if (camera_x + mouse_x > player_obj_collision.x) {
			player_obj->bitmap = image_player1;
			player_obj->x = player_obj_collision.x + 15;
		}
		else {
			player_obj->bitmap = image_player2;
			player_obj->x = player_obj_collision.x - 15;
		}

		if ((game_mode == GAME_MODE_1 || game_mode == GAME_MODE_1_INIT) || (game_mode == GAME_MODE_2 || game_mode == GAME_MODE_2_INIT) || (game_mode == GAME_MODE_3 || game_mode == GAME_MODE_3_INIT) || (game_mode == GAME_MODE_4 || game_mode == GAME_MODE_4_INIT)) {
			vector_x = (camera_x + mouse_x) - player_obj_collision.x;
			vector_y = (camera_y + mouse_y) - player_obj_collision.y;

			if (vector_x != 0 || vector_y != 0) {
				vector_sum = sqrt(vector_x * vector_x + vector_y * vector_y);
			}
			else {
				vector_x = 0;
				vector_y = 1;
				vector_sum = 1;
			}

			degree = atan2(vector_y, vector_x);
		}


		if (invincibility_delay != 0) {
			invincibility_delay--;
		}
	}
}

void movePet(GameObject* pet) {
	if (pet && player_obj) {
		switch (pet->type) {
		case PET1:
			pet->x = player_obj_collision.x - player_obj_collision.size;
			pet->y = player_obj_collision.y;
			break;
		case PET2:
			pet->x = player_obj_collision.x + player_obj_collision.size;
			pet->y = player_obj_collision.y;
			break;
		case PET3:
			pet->x = player_obj_collision.x;
			pet->y = player_obj_collision.y + player_obj_collision.size;
			break;
		case PET4:
			pet->x = player_obj_collision.x;
			pet->y = player_obj_collision.y - player_obj_collision.size;
			break;
		}
	}
}

void moveEnemy( GameObject* enemy )
{
	if( enemy )
	{
		if (enemy->delay != 0) {
			enemy->delay--;
			if (enemy->delay == 1) {
				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
		}
		else if (enemy->delay == 0) {
			switch (enemy->type) {
			case ENEMY1:
				enemy->x += enemy->move_x;
				enemy->y += enemy->move_y;
				break;
			case ENEMY2:
				enemy->x += enemy->move_x * 0.75;
				enemy->y += enemy->move_y * 0.75;
				break;

			case ENEMY3:
				enemy->x += enemy->move_x * 1.5;
				enemy->y += enemy->move_y * 1.5;
				break;

			case ENEMY4:
				enemy->x += enemy->move_x;
				enemy->y += enemy->move_y;
				break;

			case ENEMY5:
				enemy->x += enemy->move_x;
				enemy->y += enemy->move_y;
				if (enemy->age > 30 && randomFloat(0.0f, 1.0f) < 0.01f) {
					enemy->move_x = enemy->move_y = 0;
				}
				break;

			case ENEMY6:
				enemy->move_x = player_obj->x - enemy->x;
				enemy->move_y = player_obj->y - enemy->y;
				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;
				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;

				enemy->x += enemy->move_x * 0.5;
				enemy->y += enemy->move_y * 0.5;
				break;
			}
		}

		if (enemy->type == ENEMY6) {
			return;
		}

		switch(pattern) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
			if (enemy->corner >= 10) {
				return;
			}
			break;
		case 7:
		case 8:
			break;
		case 9:
			return;
		}


		if (enemy->delay == 0) {
			if (enemy->y - enemy->height / 2 - 30 <= -(MAPSIZE_HEIGHT / 2)) {
				if (enemy->corner != 1) {
					enemy->corner = 1;
					enemy->move_x = randomFloat(-1.0f, 1.0f);
					enemy->move_y = randomFloat(0.1f, 1.0f);
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->delay = 30;
				}
			}
			else if (enemy->x + enemy->width / 2 + 30 >= MAPSIZE_WIDTH / 2) {
				if (enemy->corner != 2) {
					enemy->corner = 2;
					enemy->move_x = randomFloat(-1.0f, -0.1f);
					enemy->move_y = randomFloat(-1.0f, 1.0f);
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->delay = 30;
				}
			}
			else if (enemy->y + enemy->height / 2 + 30 >= MAPSIZE_HEIGHT / 2) {
				if (enemy->corner != 3) {
					enemy->corner = 3;
					enemy->move_x = randomFloat(-1.0f, 1.0f);
					enemy->move_y = randomFloat(-1.0f, -0.1f);
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->delay = 30;
				}
			}
			else if (enemy->x - enemy->width / 2 - 30 <= -(MAPSIZE_WIDTH / 2)) {
				if (enemy->corner != 4) {
					enemy->corner = 4;
					enemy->move_x = randomFloat(0.1f, 1.0f);
					enemy->move_y = randomFloat(-1.0f, 1.0f);
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->delay = 30;
				}
			}

			if (enemy->x < -(MAPSIZE_WIDTH / 2) - 50 || enemy->x > MAPSIZE_WIDTH / 2 + 50 || enemy->y < -(MAPSIZE_HEIGHT / 2) - 50 || enemy->y > MAPSIZE_HEIGHT / 2 + 50) {
				deleteObject(enemy);
				enemyCount--;
			}
		}
	}
}

void movePlayerBullet( GameObject* bullet )
{
	if( bullet )
	{
		bullet->x += bullet->move_x;
		bullet->y += bullet->move_y;

		if (game_mode != GAME_MODE_4 && bullet->age >= playerBullet_reach) {
			deleteObject(bullet);
		}
		else if (bullet->x - bullet->width / 2 < -(MAPSIZE_WIDTH / 2) || bullet->x + bullet->width / 2 > MAPSIZE_WIDTH / 2 || bullet->y - bullet->height / 2 > MAPSIZE_HEIGHT / 2 || bullet->y + bullet->height / 2 < -(MAPSIZE_HEIGHT / 2))
		{
			deleteObject(bullet);
		}
	}
}

void movePetBullet(GameObject* bullet) {
	if (bullet)
	{
		bullet->x += bullet->move_x;
		bullet->y += bullet->move_y;

		if (bullet->age >= petBullet_reach) {
			deleteObject(bullet);
		}
		else if (bullet->x - bullet->width / 2 < -(MAPSIZE_WIDTH / 2) || bullet->x + bullet->width / 2 > MAPSIZE_WIDTH / 2 || bullet->y - bullet->height / 2 > MAPSIZE_HEIGHT / 2 || bullet->y + bullet->height / 2 < -(MAPSIZE_HEIGHT / 2))
		{
			deleteObject(bullet);
		}
	}
}

void moveEnemyBullet( GameObject* bullet )
{
	if( bullet )
	{
		bullet->x += bullet->move_x;
		bullet->y += bullet->move_y;

		if (bullet->age >= enemyBullet_reach) {
			deleteObject(bullet);
		}
		else if (bullet->x - bullet->width / 2 < -(MAPSIZE_WIDTH / 2) || bullet->x + bullet->width / 2 > MAPSIZE_WIDTH / 2 || bullet->y - bullet->height / 2 > MAPSIZE_HEIGHT / 2 || bullet->y + bullet->height / 2 < -(MAPSIZE_HEIGHT / 2))
		{
			deleteObject(bullet);
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

void shootBullet() {
	static int bulletCount = 0;
	if (player_obj) {
		if (bulletCount == 0 && (isKeyDown(VK_SPACE) || isLMouseDown == 1)) {
			bulletCount = 1;
			createPlayerBullet();
		}
	}
	if (bulletCount != 0) {
		bulletCount++;
		if (bulletCount == playerBullet_delay) {
			bulletCount = 0;
		}
	}
}

void checkCollisions()
{
	GameObject obj1_collision = { 0 };

	int i, j;
	for( i=0; i < POOL_SIZE; i++ )
	{
		GameObject* obj1 = getObject( i );
		ConvertCollision(obj1, &obj1_collision);

		if (obj1->type == ENEMY1 || obj1->type == ENEMY2 || obj1->type == ENEMY3 || obj1->type == ENEMY4 || obj1->type == ENEMY5 || obj1->type == ENEMY6) {
			if (invincibility_delay == 0 && isIntersecting(&player_obj_collision, &obj1_collision)) {
				createParticles(player_obj);

				energy -= HIT_DAMAGE;
				invincibility_delay = PLAYER_INVINCIBILITY_DELAY;

				if (energy <= 0)
				{
					num_life--;

					if (num_life == 0)
					{
						switch (game_mode) {
						case GAME_MODE_1:
							changeGameMode(GAME_MODE_1_END);
							break;
						case GAME_MODE_2:
							changeGameMode(GAME_MODE_2_END);
							break;
						case GAME_MODE_3:
							changeGameMode(GAME_MODE_3_END);
							break;
						}
					}
					else
					{
						energy = INITIAL_ENERGY;
					}
				}
				return;
			}
		}
		else if (obj1->type == BOSS_BULLET1 || obj1->type == BOSS_BULLET2 || obj1->type == BOSS_BULLET3 || obj1->type == BOSS_BULLET4 || obj1->type == BOSS_BULLET5 || obj1->type == BOSS_BULLET6) {
			if (invincibility_delay == 0 && isIntersecting(&player_obj_collision, &obj1_collision))
			{
				createParticles(obj1);
				deleteObject(obj1);

				energy -= HIT_DAMAGE;
				invincibility_delay = PLAYER_INVINCIBILITY_DELAY;

				if (energy <= 0)
				{
					num_life--;

					if (num_life == 0){
						if (game_mode == GAME_MODE_4) {
							changeGameMode(GAME_MODE_4_END);
						}
					}
					else{
						energy = INITIAL_ENERGY;
					}
				}
				return;
			}
		}
		else if( obj1->type == ENEMY_BULLET )
		{
			if(invincibility_delay == 0 && isIntersecting( &player_obj_collision, &obj1_collision ) )
			{
				createParticles( player_obj );
				deleteObject( obj1 );

				energy -= HIT_DAMAGE;
				invincibility_delay = PLAYER_INVINCIBILITY_DELAY;
				if (energy <= 0)
				{
					num_life--;

					if (num_life == 0)
					{
						switch (game_mode) {
						case GAME_MODE_1:
							changeGameMode(GAME_MODE_1_END);
							break;
						case GAME_MODE_2:
							changeGameMode(GAME_MODE_2_END);
							break;
						case GAME_MODE_3:
							changeGameMode(GAME_MODE_3_END);
							break;
						}
					}
					else
					{
						energy = INITIAL_ENERGY;
					}
				}
				return;
			}
		}

		else if( obj1->type == PLAYER_BULLET || obj1->type == PET_BULLET )
		{
			for( j=0; j < POOL_SIZE; j++ )
			{
				GameObject* obj2 = getObject( j );
				if( obj2->type == ENEMY1 || obj2->type == ENEMY2 || obj2->type == ENEMY3 || obj2->type == ENEMY4 || obj2->type == ENEMY5 )
				{
					if( isIntersecting( obj1, obj2 ) )
					{
						switch (obj1->type) {
						case PLAYER_BULLET:
							obj2->health -= playerBullet_damage;
							break;
						case PET_BULLET:
							obj2->health -= petBullet_damage;
							break;
						}

						createParticles( obj1 );
						obj1->health--;
						if (obj1->health == 0) {
							deleteObject(obj1);
						}

						if (obj2->health <= 0) {
							switch (obj2->type) {
							case ENEMY1:
								score += HIT_SCORE;
								break;
							case ENEMY2:
								score += HIT_SCORE * 2;
								break;
							case ENEMY3:
								score += HIT_SCORE;
								break;
							case ENEMY4:
								score += HIT_SCORE;
								createEnemy4Bullet(obj2);
								break;
							case ENEMY5:
								score += HIT_SCORE * 3;
								break;
							}
							deleteObject(obj2);
							enemyCount--;
						}
						return;
					}
				}
			}

			if (isIntersecting(obj1, boss_obj)) {
				boss_obj->health -= playerBullet_damage;
				createParticles(obj1);
				obj1->health--;
				if (obj1->health == 0) {
					deleteObject(obj1);
				}
				
				score += HIT_SCORE;
				if (boss_obj->health <= 0) {
					deleteObject(boss_obj);
					enemyCount--;
				}
			}
		}
	}
}

void changeGameMode(int new_mode)
{
	// clean up the existing mode
	switch (game_mode)
	{
	case OUT_OF_GAME:
		menu_mobNum = rand() % 7 + 1;
		break;

	case GAME_MENU:
		break;

	case GAME_MODE_SELECT:
		break;

	case GAME_RANKING:
		break;

	case GAME_OPTION:
		break;

	case GAME_EXPLANATION:
		break;

	case GAME_MODE_1_INIT:
	case GAME_MODE_2_INIT:
	case GAME_MODE_3_INIT:
	case GAME_MODE_4_INIT:
		break;

	case GAME_MODE_1:
		deleteObject(player_obj);
		player_obj = NULL;
		break;

	case GAME_MODE_2:
		totalTime += elapsedTime;
		if (new_mode == GAME_MODE_2_END) {
			NewScore(2);
			mode_2_Stage = 1;
		}
		break;

	case GAME_MODE_3:
		totalTime = elapsedTime;
		NewScore(3);
		deleteObject(player_obj);
		player_obj = NULL;
		break;

	case GAME_MODE_4:
		totalTime = elapsedTime;
		NewScore(4);
		deleteObject(player_obj);
		player_obj = NULL;
		break;

	case GAME_MODE_2_STAGE_CLEAR:
		mode_2_Stage++;
		cleanupPool();
		break;

	case GAME_MODE_4_STAGE_CLEAR:
		cleanupPool();
		break;

	case GAME_MODE_1_END:
	case GAME_MODE_2_END:
	case GAME_MODE_3_END:
	case GAME_MODE_4_END:
		cleanupPool();
		break;
	}

	// initialize the new mode
	switch (new_mode)
	{
	case OUT_OF_GAME:
		break;

	case GAME_MENU:
		menu_selection = 1;
		break;

	case GAME_MODE_SELECT:
		num_life = INITIAL_LIFE;
		score = 0;

		mode_selection = 1;
		break;

	case GAME_RANKING:
		ranking_page = 0;
		ranking_print_min = 0;
		ranking_print_max = SetCutSize();

		LoadScore();
		SortScore_Stage(2, 1);
		ranking_sortType = 1;

		menu_mobNum = rand() % 7 + 1;
		break;

	case GAME_OPTION:
		option_selection = 1;
		break;

	case GAME_EXPLANATION:
		break;

	case GAME_MODE_1_INIT:
		break;

	case GAME_MODE_2_INIT:
		mode_2_Init();
		break;

	case GAME_MODE_3_INIT:
		mode_3_Init();
		break;

	case GAME_MODE_4_INIT:
		mode_4_Init();
		break;

	case GAME_MODE_1:
	case GAME_MODE_2:
	case GAME_MODE_3:
	case GAME_MODE_4:
		break;

	case GAME_MODE_2_STAGE_CLEAR:
	case GAME_MODE_4_STAGE_CLEAR:
		break;

	case GAME_MODE_1_END:
	case GAME_MODE_2_END:
	case GAME_MODE_3_END:
	case GAME_MODE_4_END:
		break;
	}

	game_mode = new_mode;
}

// Mode

void mode_1() {

}

void mode_2() {
	if (enemyCount == 0) {
		changeGameMode(GAME_MODE_2_STAGE_CLEAR);
	}

	elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;

	movePlayer();
	shootBullet();

	checkCollisions();
}

void mode_3() {
	elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;
	static int patternCheck = 0;
	static int spawnCheck = 0;

	movePlayer();
	shootBullet();

	if (int(elapsedTime) % 60 == 0 && spawnCheck == 0) {
		if (int(elapsedTime) < 720) {
			if (elapsedTime < patternTargetTime_3) {
				spawnCheck = 1;
				enemyProbability_2 += 10;
			}
			else if (elapsedTime < patternTargetTime_6) {
				spawnCheck = 1;
				enemyProbability_2 += 10;
				enemyProbability_3 += 10;
			}
			else if (elapsedTime < patternTargetTime_9) {
				spawnCheck = 1;
				enemyProbability_3 += 10;
				enemyProbability_4 += 10;
			}
			else if (elapsedTime > patternTargetTime_9) {
				spawnCheck = 1;
				enemyProbability_4 += 10;
				enemyProbability_5 += 10;
			}
		}
		else {
			spawnCheck = 1;

			enemyProbability_1 += 1;
			enemyProbability_2 += 2;
			enemyProbability_3 += 3;
			enemyProbability_4 += 4;
			enemyProbability_5 += 5;
			enemyProbability_6 += 1;

			enemySpawnSize += 10;
		}
	}
	if (spawnCheck == 1 && int(elapsedTime) % 60 == 1) {
		spawnCheck = 0;
	}

	if (enemyCount < enemySpawnSize)
	{
		float enemyProbabilityTotal = enemyProbability_1 + enemyProbability_2 + enemyProbability_3 + enemyProbability_4 + enemyProbability_5;

		int i;
		for (i = 0; i < enemySpawnSize - enemyCount; i++) {
			float spawn_probability = randomFloat(0, 1);

			if (spawn_probability < enemyProbability_1 / enemyProbabilityTotal) {
				spawnEnemy(1);
			}
			else if (spawn_probability < (enemyProbability_1 + enemyProbability_2) / enemyProbabilityTotal) {
				spawnEnemy(2);
			}
			else if (spawn_probability < (enemyProbability_1 + enemyProbability_2 + enemyProbability_3) / enemyProbabilityTotal) {
				spawnEnemy(3);
			}
			else if (spawn_probability < (enemyProbability_1 + enemyProbability_2 + enemyProbability_3 + enemyProbability_4) / enemyProbabilityTotal) {
				spawnEnemy(4);
			}
			else {
				spawnEnemy(5);
			}
			if (isEnemy6 == 0 && enemyProbability_6 > 0) {
				spawnEnemy(6);
			}
		}
	}

	if (score == 500 * pow(2, (petCount + 1)) && petCount <= 3) {
		createPet();
	}

	// Pattern
	if (int(elapsedTime) > patternTargetTime_9 + patternTime_9) {
		if (int(elapsedTime) % 30 == 0 && int(elapsedTime) % 60 != 0 && patternCheck == 0) {
			patternCheck = 1;
			if (randomFloat(0, 1) < 0.3f) {
				Pattern_Spread();
			}
		}
		else if (int(elapsedTime) % 60 == 0 && patternCheck == 0) {
			patternCheck = 1;
			pattern = ((int)rand() % 9) + 11;
		}

		if (patternCheck == 1 && int(elapsedTime) % 30 == 1) {
			patternCheck = 0;
		}
	}

	if (int(elapsedTime) == patternTargetTime_1 || pattern == 11) {
		patternStartTime = elapsedTime;
		Pattern_1();
	}
	else if (int(elapsedTime) == patternTargetTime_2 || pattern == 12) {
		patternStartTime = elapsedTime;
		Pattern_2();
	}
	else if (int(elapsedTime) == patternTargetTime_3 || pattern == 13) {
		patternStartTime = elapsedTime;
		Pattern_3();
	}
	else if (int(elapsedTime) == patternTargetTime_4 || pattern == 14) {
		patternStartTime = elapsedTime;
		Pattern_4();
	}
	else if (int(elapsedTime) == patternTargetTime_5 || pattern == 15) {
		patternStartTime = elapsedTime;
		Pattern_5();
	}
	else if (int(elapsedTime) == patternTargetTime_6 || pattern == 16) {
		patternStartTime = elapsedTime;
		Pattern_6(2);
	}
	else if (int(elapsedTime) == patternTargetTime_7 || pattern == 17) {
		patternStartTime = elapsedTime;
		Pattern_7();
	}
	else if (int(elapsedTime) == patternTargetTime_8 || pattern == 18) {
		patternStartTime = elapsedTime;
		Pattern_8();
	}
	else if (int(elapsedTime) == patternTargetTime_9 || pattern == 19) {
		patternStartTime = elapsedTime;
		Pattern_9();
	}

	switch (pattern) {
	case 1:
		Pattern_1();
		if (int(elapsedTime - patternStartTime) >= patternTime_1) {
			Pattern_Spread();
		}
		break;
	case 2:
		Pattern_2();
		if (int(elapsedTime - patternStartTime) >= patternTime_2) {
			Pattern_Spread();
		}
		break;
	case 3:
		Pattern_3();
		if (int(elapsedTime - patternStartTime) >= patternTime_3) {
			Pattern_Spread();
		}
		break;
	case 4:
		Pattern_4();
		if (int(elapsedTime - patternStartTime) >= patternTime_4) {
			Pattern_Spread();
		}
		break;
	case 5:
		Pattern_5();
		if (int(elapsedTime - patternStartTime) >= patternTime_5) {
			Pattern_Spread();
		}
		break;
	case 6:
		Pattern_6(2);
		if (int(elapsedTime - patternStartTime) >= patternTime_6) {
			Pattern_Spread();
		}
		break;
	case 7:
	case 8:
		break;
	case 9:
		Pattern_9();
		break;
	}

	checkCollisions();
}

void mode_4() {
	static int patternCheck, loopDelayTime;
	static int delay, loop, bulletType, bulletNum, bulletSpeed, reach;
	static float startingTime;

	if (enemyCount == 0) {
		changeGameMode(GAME_MODE_4_STAGE_CLEAR);
	}
	if (bulletNum != 0 && int(elapsedTime) <= 1) {
		patternCheck = 0;
		pattern = 0;
		loopDelayTime = rand() % 11 + 5;
		delay = 0;
		loop = 0;
		bulletType = 0;
		bulletNum = 0;
		bulletSpeed = 0;
		reach = 0;
	}

	elapsedTime = float(clock() - startTime) / CLOCKS_PER_SEC;
	
	if (patternCheck == 2 && int(startingTime * 10) + loopDelayTime < int(elapsedTime * 10)) {
		patternCheck = 0;
		loopDelayTime = rand() % 11 + 5;
	}

	if (int(elapsedTime) > 3 && patternCheck == 0) {
		patternCheck = 1;
		if (randomFloat(0.0f, 1.0f) < 0.3f) {
			pattern = 7;
		}
		else {
			pattern = rand() % 6 + 1;
		}
		startingTime = elapsedTime;

		switch (pattern) {
		case 1:
			delay = rand() % 20 + 31;
			loop = rand() % 15 + 6;
			bulletType = rand() % 3 + 1;
			bulletNum = rand() % 30 + 20;
			bulletSpeed = bossBulletSpeed * randomFloat(1, 1.5);
			break;

		case 2:
			delay = rand() % 10 + 11;
			loop = rand() % 30 + 31;
			bulletNum = rand() % 6 + 4;
			bulletSpeed = bossBulletSpeed * randomFloat(1, 1.5);
			break;

		case 3:
			delay = rand() % 100 + 51;
			loop = rand() % 3 + 1;
			bulletNum = rand() % 5 + 3;
			bulletSpeed = bossBulletSpeed * randomFloat(0.8, 2);
			reach = bossBulletReach * randomFloat(0.5, 2.0);
			break;

		case 4:
			delay = rand() % 30 + 20;
			loop = rand() % 15 + 6;
			bulletType = rand() % 2 + 1;
			bulletNum = rand() % 30 + 20;
			bulletSpeed = bossBulletSpeed * randomFloat(1, 1.5);
			break;

		case 5:
			delay = rand() % 20 + 20;
			loop = rand() % 20 + 10;
			bulletType = rand() % 3 + 1;
			bulletNum = rand() % 20 + 10;
			if (bulletType == 3) {
				bulletSpeed = bossBulletSpeed * randomFloat(2, 3);
			}
			else {
				bulletSpeed = bossBulletSpeed * randomFloat(1, 1.8);
			}
			break;

		case 6:
			if (randomFloat(0.0f, 1.0f) < 0.7) {
				delay = rand() % 30 + 15;
				loop = rand() % 20 + 5;
				bulletType = 5;
				bulletNum = rand() % 20 + 6;
				bulletSpeed = bossBulletSpeed * randomFloat(1, 1.5);
				reach = bossBulletReach * randomFloat(0.5, 1.5);
			}
			else {
				delay = rand() % 50 + 20;
				loop = rand() % 5 + 1;
				bulletType = 6;
				bulletNum = rand() % 5 + 3;
				bulletSpeed = bossBulletSpeed * randomFloat(1, 1.5);
				reach = bossBulletReach * randomFloat(1.0, 2.0);
			}
			break;

		case 7:
			break;
		}
	}

	switch (pattern) {
	case 1:
		if (bossPattern_1(startingTime, delay, loop, bulletType, bulletNum, bulletSpeed) == 1) {
			patternCheck = 2;
			pattern = 0;
			startingTime = elapsedTime;
		}
		break;
	case 2:
		if (bossPattern_2(startingTime, delay, loop, bulletNum, bulletSpeed) == 1) {
			patternCheck = 2;
			pattern = 0;
			startingTime = elapsedTime;
		}
		break;
	case 3:
		if (bossPattern_3(startingTime, delay, loop, bulletNum, bulletSpeed, reach) == 1) {
			patternCheck = 2;
			pattern = 0;
			startingTime = elapsedTime;
		}
		break;
	case 4:
		if (bossPattern_4(startingTime, delay, loop, bulletType, bulletNum, bulletSpeed) == 1) {
			patternCheck = 2;
			pattern = 0;
			startingTime = elapsedTime;
		}
		break;
	case 5:
		if (bossPattern_5(startingTime, delay, loop, bulletType, bulletNum, bulletSpeed) == 1) {
			patternCheck = 2;
			pattern = 0;
			startingTime = elapsedTime;
		}
		break;
	case 6:
		if (bossPattern_6(startingTime, delay, loop, bulletType, bulletNum, bulletSpeed, reach) == 1) {
			patternCheck = 2;
			pattern = 0;
			startingTime = elapsedTime;
		}
		break;
	case 7:
		if (bossPattern_7(startingTime) == 1) {
			patternCheck = 2;
			pattern = 0;
			startingTime = elapsedTime;
		}
		break;
	}

	movePlayer();
	shootBullet();

	checkCollisions();
}

void mode_2_Init() {
	energy = INITIAL_ENERGY;
	ready_count = READY_INTERVAL;
	enemyCount = 0;
	createPlayer(0);

	if (mode_2_Stage == 1) {
		enemyProbability_1 = 50;
		enemyProbability_2 = 0;
		enemyProbability_3 = 0;
		enemyProbability_4 = 0;
		enemyProbability_5 = 0;
		enemyProbability_6 = 0;
		isEnemy6 = 0;

		totalTime = 0;
	}
	else if(mode_2_Stage <= 6){
		switch (mode_2_Stage) {
		case 2:
			enemyProbability_2 += 20;
			break;
		case 3:
			enemyProbability_2 += 30;
			enemyProbability_3 += 20;
			break;
		case 4:
			enemyProbability_2 += 10;
			enemyProbability_3 += 30;
			enemyProbability_4 += 20;
			break;
		case 5:
			enemyProbability_3 += 10;
			enemyProbability_4 += 30;
			enemyProbability_5 += 20;
			break;
		case 6:
			enemyProbability_4 += 20;
			enemyProbability_5 += 10;
			enemyProbability_6 += 5;
			break;
		}
	}
	else {
		enemyProbability_1 += 1;
		enemyProbability_2 += 2;
		enemyProbability_3 += 3;
		enemyProbability_4 += 4;
		enemyProbability_5 += 5;
		enemyProbability_6 += 1;
	}

	MAPSIZE_WIDTH = MAPSIZE_HEIGHT = 2000 + mode_2_Stage * 200;

	int enemy_size = 30 + mode_2_Stage * 20;
	float enemyProbabilityTotal = enemyProbability_1 + enemyProbability_2 + enemyProbability_3 + enemyProbability_4 + enemyProbability_5;
	int i;
	for (i = 0; i < enemy_size; i++) {
		float spawn_probability = randomFloat(0, 1);

		if (spawn_probability < enemyProbability_1 / enemyProbabilityTotal) {
			spawnEnemy(1);
		}
		else if (spawn_probability < (enemyProbability_1 + enemyProbability_2) / enemyProbabilityTotal) {
			spawnEnemy(2);
		}
		else if (spawn_probability < (enemyProbability_1 + enemyProbability_2 + enemyProbability_3) / enemyProbabilityTotal) {
			spawnEnemy(3);
		}
		else if (spawn_probability < (enemyProbability_1 + enemyProbability_2 + enemyProbability_3 + enemyProbability_4) / enemyProbabilityTotal) {
			spawnEnemy(4);
		}
		else {
			spawnEnemy(5);
		}
		if (isEnemy6 == 0 && enemyProbability_6 > 0) {
			spawnEnemy(6);
		}
	}
	startTime = clock();
	elapsedTime = 0;
}

void mode_3_Init() {
	energy = INITIAL_ENERGY;
	ready_count = READY_INTERVAL;
	pattern = 0;
	patternStartTime = 0;
	enemyInitSize = 400;
	enemySpawnSize = 400;
	enemyCount = 0;
	petCount = 0;
	createPlayer(0);

	enemyProbability_1 = 50;
	enemyProbability_2 = 0;
	enemyProbability_3 = 0;
	enemyProbability_4 = 0;
	enemyProbability_5 = 0;
	enemyProbability_6 = 0;
	isEnemy6 = 0;

	MAPSIZE_WIDTH = MAPSIZE_HEIGHT = 4500;

	int i;
	for (i = 0; i < enemyInitSize; i++) {
		createEnemy();
	}
	startTime = clock();
	elapsedTime = 0;
	totalTime = 0;
}

void mode_4_Init() {
	energy = INITIAL_ENERGY;
	ready_count = READY_INTERVAL;
	enemyCount = 0;

	MAPSIZE_WIDTH = MAPSIZE_HEIGHT = 2000;
	createPlayer(1);
	createBoss();
	mode_4_Stage = 1;

	startTime = clock();
	elapsedTime = 0;
	totalTime = 0;
}

//

// Pattern

void Pattern_Spread() {
	pattern = 0;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			enemy->corner = 0;

			enemy->move_x = randomFloat(-1.0f, 1.0f);
			enemy->move_y = randomFloat(-1.0f, 1.0f);
			float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
			enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
			enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

			enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
		}
	}
}

void Pattern_1() {
	pattern = 1;
	int pos = 200;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->move_x != 0 && enemy->move_y != 0) {
				if ((pos - 10 < enemy->x && enemy->x < pos + 10) && (pos - 10 < enemy->y && enemy->y < pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}
				else if ((-pos - 10 < enemy->x && enemy->x < -pos + 10) && (pos - 10 < enemy->y && enemy->y < pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}
				else if ((-pos - 10 < enemy->x && enemy->x < -pos + 10) && (-pos - 10 < enemy->y && enemy->y < -pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}
				else if ((pos - 10 < enemy->x && enemy->x < pos + 10) && (-pos - 10 < enemy->y && enemy->y < -pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}

				else {
					if (enemy->corner != 11 && (enemy->x >= 0 && enemy->y > 0)) {
						enemy->corner = 11;

						enemy->move_x = pos - enemy->x;
						enemy->move_y = pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
					else if (enemy->corner != 12 && (enemy->x < 0 && enemy->y >= 0)) {
						enemy->corner = 12;

						enemy->move_x = -pos - enemy->x;
						enemy->move_y = pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
					else if (enemy->corner != 13 && (enemy->x <= 0 && enemy->y < 0)) {
						enemy->corner = 13;

						enemy->move_x = -pos - enemy->x;
						enemy->move_y = -pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
					else if (enemy->corner != 14 && (enemy->x > 0 && enemy->y <= 0)) {
						enemy->corner = 14;

						enemy->move_x = pos - enemy->x;
						enemy->move_y = -pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
				}
			}
		}
	}
}

void Pattern_2() {
	pattern = 2;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->corner != 11 && (enemy->x >= 0 && enemy->y > 0)) {
				enemy->corner = 11;

				enemy->move_x = 0 - enemy->x;
				enemy->move_y = MAPSIZE_HEIGHT / 4 - enemy->y;
				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
			else if (enemy->corner != 12 && (enemy->x < 0 && enemy->y >= 0)) {
				enemy->corner = 12;

				enemy->move_x = -(MAPSIZE_HEIGHT / 4) - enemy->x;
				enemy->move_y = 0 - enemy->y;
				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
			else if (enemy->corner != 13 && (enemy->x <= 0 && enemy->y < 0)) {
				enemy->corner = 13;

				enemy->move_x = 0 - enemy->x;
				enemy->move_y = -(MAPSIZE_HEIGHT / 4) - enemy->y;
				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
			else if (enemy->corner != 14 && (enemy->x > 0 && enemy->y <= 0)) {
				enemy->corner = 14;

				enemy->move_x = MAPSIZE_HEIGHT / 4 - enemy->x;
				enemy->move_y = 0 - enemy->y;
				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
		}
	}
}

void Pattern_3() {
	pattern = 3;
	int mapsize = MAPSIZE_WIDTH / 8 * 3;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->corner == 11) {
				float xPos = 0;
				float yPos = mapsize;

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 13;

					enemy->move_x = (mapsize * cos(234 * (M_PI / 180))) - enemy->x;
					enemy->move_y = (mapsize * sin(234 * (M_PI / 180))) - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (enemy->corner == 12) {
				float xPos = mapsize * cos(162 * (M_PI / 180));
				float yPos = mapsize * sin(162 * (M_PI / 180));

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 14;

					enemy->move_x = (mapsize * cos(306 * (M_PI / 180))) - enemy->x;
					enemy->move_y = (mapsize * sin(306 * (M_PI / 180))) - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (enemy->corner == 13) {
				float xPos = mapsize * cos(234 * (M_PI / 180));
				float yPos = mapsize * sin(234 * (M_PI / 180));

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 15;

					enemy->move_x = (mapsize * cos(18 * (M_PI / 180))) - enemy->x;
					enemy->move_y = (mapsize * sin(18 * (M_PI / 180))) - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (enemy->corner == 14) {
				float xPos = mapsize * cos(306 * (M_PI / 180));
				float yPos = mapsize * sin(306 * (M_PI / 180));

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 11;

					enemy->move_x = 0 - enemy->x;
					enemy->move_y = mapsize - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (enemy->corner == 15) {
				float xPos = mapsize * cos(18 * (M_PI / 180));
				float yPos = mapsize * sin(18 * (M_PI / 180));

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 12;

					enemy->move_x = (mapsize * cos(162 * (M_PI / 180))) - enemy->x;
					enemy->move_y = (mapsize * sin(162 * (M_PI / 180))) - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}

			else {
				if (enemy->x >= 0 && enemy->y > 0) {
					if (enemy->y > enemy->x) {
						enemy->corner = 11;

						enemy->move_x = 0 - enemy->x;
						enemy->move_y = mapsize - enemy->y;
					}
					else {
						enemy->corner = 15;

						enemy->move_x = (mapsize * cos(18 * (M_PI / 180))) - enemy->x;
						enemy->move_y = (mapsize * sin(18 * (M_PI / 180))) - enemy->y;
					}
				}
				else if (enemy->x < 0 && enemy->y >= 0) {
					if (enemy->y > -enemy->x) {
						enemy->corner = 11;

						enemy->move_x = 0 - enemy->x;
						enemy->move_y = mapsize - enemy->y;
					}
					else {
						enemy->corner = 12;

						enemy->move_x = (mapsize * cos(162 * (M_PI / 180))) - enemy->x;
						enemy->move_y = (mapsize * sin(162 * (M_PI / 180))) - enemy->y;
					}
				}
				else if (enemy->x <= 0 && enemy->y < 0) {
					enemy->corner = 13;

					enemy->move_x = (mapsize * cos(234 * (M_PI / 180))) - enemy->x;
					enemy->move_y = (mapsize * sin(234 * (M_PI / 180))) - enemy->y;
				}
				else if (enemy->x > 0 && enemy->y <= 0) {
					enemy->corner = 14;

					enemy->move_x = (mapsize * cos(306 * (M_PI / 180))) - enemy->x;
					enemy->move_y = (mapsize * sin(306 * (M_PI / 180))) - enemy->y;
				}

				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
		}
	}
}

void Pattern_4() {
	pattern = 4;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->corner == 11) {
				float xPos = -(MAPSIZE_WIDTH / 4);
				float yPos = MAPSIZE_HEIGHT / 4;

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 13;

					enemy->move_x = -xPos - enemy->x;
					enemy->move_y = -yPos - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (enemy->corner == 12) {
				float xPos = -(MAPSIZE_WIDTH / 4);
				float yPos = -(MAPSIZE_HEIGHT / 4);

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 14;

					enemy->move_x = -xPos - enemy->x;
					enemy->move_y = -yPos - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (enemy->corner == 13) {
				float xPos = MAPSIZE_WIDTH / 4;
				float yPos = -(MAPSIZE_HEIGHT / 4);

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 12;

					enemy->move_x = -xPos - enemy->x;
					enemy->move_y = yPos - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (enemy->corner == 14) {
				float xPos = MAPSIZE_WIDTH / 4;
				float yPos = MAPSIZE_HEIGHT / 4;

				if ((enemy->x - 10 < xPos && xPos < enemy->x + 10) && (enemy->y - 10 < yPos && yPos < enemy->y + 10)) {
					enemy->corner = 11;

					enemy->move_x = -xPos - enemy->x;
					enemy->move_y = yPos - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}

			else {
				if (enemy->x >= 0 && enemy->y > 0) {
					enemy->corner = 14;

					enemy->move_x = MAPSIZE_WIDTH / 4 - enemy->x;
					enemy->move_y = MAPSIZE_HEIGHT / 4 - enemy->y;
				}
				else if (enemy->x < 0 && enemy->y >= 0) {
					enemy->corner = 11;

					enemy->move_x = -(MAPSIZE_WIDTH / 4) - enemy->x;
					enemy->move_y = MAPSIZE_HEIGHT / 4 - enemy->y;
				}
				else if (enemy->x <= 0 && enemy->y < 0) {
					enemy->corner = 12;

					enemy->move_x = -(MAPSIZE_WIDTH / 4) - enemy->x;
					enemy->move_y = -(MAPSIZE_HEIGHT / 4) - enemy->y;
				}
				else if (enemy->x > 0 && enemy->y <= 0) {
					enemy->corner = 13;

					enemy->move_x = MAPSIZE_WIDTH / 4 - enemy->x;
					enemy->move_y = -(MAPSIZE_HEIGHT / 4) - enemy->y;
				}

				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
		}
	}
}

void Pattern_5() {
	pattern = 5;
	int pos = 200;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->move_x != 0 && enemy->move_y != 0) {
				if ((MAPSIZE_WIDTH / 2 - pos - 10 < enemy->x && enemy->x < MAPSIZE_WIDTH / 2 - pos + 10) && (MAPSIZE_HEIGHT / 2 - pos - 10 < enemy->y && enemy->y < MAPSIZE_HEIGHT / 2 - pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}
				else if ((-MAPSIZE_WIDTH / 2 + pos - 10 < enemy->x && enemy->x < -MAPSIZE_WIDTH / 2 + pos + 10) && (MAPSIZE_HEIGHT / 2 - pos - 10 < enemy->y && enemy->y < MAPSIZE_HEIGHT / 2 - pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}
				else if ((-MAPSIZE_WIDTH / 2 + pos - 10 < enemy->x && enemy->x < -MAPSIZE_WIDTH / 2 + pos + 10) && (-MAPSIZE_HEIGHT / 2 + pos - 10 < enemy->y && enemy->y < -MAPSIZE_HEIGHT / 2 + pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}
				else if ((MAPSIZE_WIDTH / 2 - pos - 10 < enemy->x && enemy->x < MAPSIZE_WIDTH / 2 - pos + 10) && (-MAPSIZE_HEIGHT / 2 + pos - 10 < enemy->y && enemy->y < -MAPSIZE_HEIGHT / 2 + pos + 10)) {
					enemy->move_x = 0;
					enemy->move_y = 0;
				}
			
				else {
					if (enemy->corner != 11 && (enemy->x >= 0 && enemy->y > 0)) {
						enemy->corner = 11;

						enemy->move_x = MAPSIZE_WIDTH / 2 - pos - enemy->x;
						enemy->move_y = MAPSIZE_HEIGHT / 2 - pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
					else if (enemy->corner != 12 && (enemy->x < 0 && enemy->y >= 0)) {
						enemy->corner = 12;

						enemy->move_x = -MAPSIZE_WIDTH / 2 + pos - enemy->x;
						enemy->move_y = MAPSIZE_HEIGHT / 2 - pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
					else if (enemy->corner != 13 && (enemy->x <= 0 && enemy->y < 0)) {
						enemy->corner = 13;

						enemy->move_x = -MAPSIZE_WIDTH / 2 + pos - enemy->x;
						enemy->move_y = -MAPSIZE_HEIGHT / 2 + pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
					else if (enemy->corner != 14 && (enemy->x > 0 && enemy->y <= 0)) {
						enemy->corner = 14;

						enemy->move_x = MAPSIZE_WIDTH / 2 - pos - enemy->x;
						enemy->move_y = -MAPSIZE_HEIGHT / 2 + pos - enemy->y;
						float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
						enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
						enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

						enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
					}
				}
			}
		}
	}
}

void Pattern_6(int size) {
	pattern = 6;

	int i;
	int w, h;
	int len = 500 * size;
	int loopExit;

	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			loopExit = 0;

			if (enemy->corner != 12) {
				for (h = int(MAPSIZE_HEIGHT / len); h > -int(MAPSIZE_HEIGHT / len); h--) {
					for (w = -int(MAPSIZE_WIDTH / len); w < int(MAPSIZE_WIDTH / len); w++) {

						float xPos = (((MAPSIZE_WIDTH / 2) / int(MAPSIZE_WIDTH / len)) * w + (((MAPSIZE_WIDTH / 2) / int(MAPSIZE_WIDTH / len)) * (w + 1))) / 2;
						float yPos = (((MAPSIZE_HEIGHT / 2) / int(MAPSIZE_HEIGHT / len)) * h + (((MAPSIZE_HEIGHT / 2) / int(MAPSIZE_HEIGHT / len)) * (h - 1))) / 2;

						if ((xPos - 10 <= enemy->x && enemy->x <= xPos + 10) && (yPos - 10 <= enemy->y && enemy->y <= yPos + 10)) {
							enemy->corner = 12;

							enemy->move_x = 0;
							enemy->move_y = 0;

							loopExit = 1;
							break;
						}

						if (enemy->corner != 11) {
							float len_1 = len / 2;

							if (((xPos - len_1 <= enemy->x) && (enemy->x <= xPos + len_1)) && ((yPos - len_1 <= enemy->y) && (enemy->y <= yPos + len_1))) {
								enemy->corner = 11;

								enemy->move_x = xPos - enemy->x;
								enemy->move_y = yPos - enemy->y;
								float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
								enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
								enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

								enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;

								loopExit = 1;
								break;
							}
						}
					}
					if (loopExit == 1) {
						break;
					}
				}
			}
		}
	}
}

void Pattern_7() {
	pattern = 7;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->corner < 11) {
				if (enemy->x >= 0 && enemy->y > 0) {
					enemy->corner = 11;

					enemy->move_x = -MAPSIZE_WIDTH;
					enemy->move_y = -MAPSIZE_HEIGHT;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
				else if (enemy->x < 0 && enemy->y >= 0) {
					enemy->corner = 12;

					enemy->move_x = MAPSIZE_WIDTH;
					enemy->move_y = -MAPSIZE_HEIGHT;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
				else if (enemy->x <= 0 && enemy->y < 0) {
					enemy->corner = 13;

					enemy->move_x = MAPSIZE_WIDTH;
					enemy->move_y = MAPSIZE_HEIGHT;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
				else if (enemy->x > 0 && enemy->y <= 0) {
					enemy->corner = 14;

					enemy->move_x = -MAPSIZE_WIDTH;
					enemy->move_y = MAPSIZE_HEIGHT;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
		}
	}
}

void Pattern_8() {
	pattern = 8;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->corner < 11) {
				if (enemy->x >= 0 && enemy->y > 0) {
					enemy->corner = 11;

					enemy->move_x = 0 - enemy->x;
					enemy->move_y = 0 - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
				else if (enemy->x < 0 && enemy->y >= 0) {
					enemy->corner = 12;

					enemy->move_x = 0 - enemy->x;
					enemy->move_y = 0 - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
				else if (enemy->x <= 0 && enemy->y < 0) {
					enemy->corner = 13;

					enemy->move_x = 0 - enemy->x;
					enemy->move_y = 0 - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
				else if (enemy->x > 0 && enemy->y <= 0) {
					enemy->corner = 14;

					enemy->move_x = 0 - enemy->x;
					enemy->move_y = 0 - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
		}
	}
}

void Pattern_9() {
	pattern = 9;

	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* enemy = getObject(i);
		if (enemy->type == ENEMY1 || enemy->type == ENEMY2 || enemy->type == ENEMY3 || enemy->type == ENEMY4 || enemy->type == ENEMY5) {
			if (enemy->corner == 11) {
				if (enemy->x < -(MAPSIZE_WIDTH / 8 * 3) || enemy->x > MAPSIZE_WIDTH / 8 * 3 || enemy->y < -(MAPSIZE_HEIGHT / 8 * 3) || enemy->y > MAPSIZE_HEIGHT / 8 * 3) {
					Pattern_Spread();
					return;
				}
			}
			else if (enemy->corner == 10) {
				if ((-10 < enemy->x && enemy->x < 10) && (-10 < enemy->y && enemy->y < 10)) {
					enemy->corner = 11;

					enemy->move_x = ((float)rand() / (float)RAND_MAX < 0.5f ? -((float)MAPSIZE_WIDTH / 8 * 3) : (float)MAPSIZE_WIDTH / 8 * 3) - enemy->x;
					enemy->move_y = ((float)rand() / (float)RAND_MAX < 0.5f ? -((float)MAPSIZE_HEIGHT / 8 * 3) : (float)MAPSIZE_HEIGHT / 8 * 3) - enemy->y;
					float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
					enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
					enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

					enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
				}
			}
			else {
				enemy->corner = 10;

				enemy->move_x = 0 - enemy->x;
				enemy->move_y = 0 - enemy->y;
				float vector_total = sqrt(pow(enemy->move_x, 2) + pow(enemy->move_y, 2));
				enemy->move_x = (enemy->move_x / vector_total) * ENEMY_SPEED;
				enemy->move_y = (enemy->move_y / vector_total) * ENEMY_SPEED;

				enemy->a = atan2(enemy->move_y, enemy->move_x) * (180 / M_PI) + 90;
			}
		}
	}
}

//

// Boss

void createBoss() {
	if (boss_obj){
		deleteObject(boss_obj);
	}

	boss_obj = newObject();
	if (boss_obj)
	{
		boss_obj->type = BOSS;
		boss_obj->bitmap = image_boss;
		boss_obj->x = 0;
		boss_obj->y = 0;
		boss_obj->width = 200.0f;
		boss_obj->height = 400.0f;
		boss_obj->size = sqrt(pow(boss_obj->width, 2) + pow(boss_obj->height, 2)) / 2;
		boss_obj->a = 0;

		boss_obj->health = bossHealth;

		enemyCount++;
	}
}

void createBossBullet(int bulletType, float bulletSpeed, int reach, float to_x, float to_y) {
	GameObject* bullet_obj = newObject();

	if (bullet_obj)
	{
		switch (bulletType) {
		case 1:
			bullet_obj->type = BOSS_BULLET1;
			bullet_obj->bitmap = image_enemy1;
			break;
		case 2:
			bullet_obj->type = BOSS_BULLET2;
			bullet_obj->bitmap = image_enemy1;
			break;
		case 3:
			bullet_obj->type = BOSS_BULLET3;
			bullet_obj->bitmap = image_enemy1;
			break;
		case 4:
			bullet_obj->type = BOSS_BULLET4;
			bullet_obj->bitmap = image_enemy4;
			break;
		case 5:
			bullet_obj->type = BOSS_BULLET5;
			bullet_obj->bitmap = image_enemy2;
			break;
		case 6:
			bullet_obj->type = BOSS_BULLET6;
			bullet_obj->bitmap = image_enemy6;
			break;
		}
		bullet_obj->health = 1;
		bullet_obj->width = 25.0f;
		bullet_obj->height = 25.0f;
		bullet_obj->x = boss_obj->x;
		bullet_obj->y = boss_obj->y;

		bullet_obj->move_x = to_x - boss_obj->x;
		bullet_obj->move_y = to_y - boss_obj->y;
		float vector_total = sqrt(pow(bullet_obj->move_x, 2) + pow(bullet_obj->move_y, 2));
		bullet_obj->move_x = (bullet_obj->move_x / vector_total) * bulletSpeed;
		bullet_obj->move_y = (bullet_obj->move_y / vector_total) * bulletSpeed;

		bullet_obj->a = atan2(bullet_obj->move_y, bullet_obj->move_x) * (180 / M_PI) + 90;

		bullet_obj->delay = reach;
	}
}

void createBossBullet_Custom(int bulletType, float bulletSpeed, int reach, float from_x, float from_y, float to_x, float to_y) {
	GameObject* bullet_obj = newObject();

	if (bullet_obj)
	{
		switch (bulletType) {
		case 1:
			bullet_obj->type = BOSS_BULLET1;
			bullet_obj->bitmap = image_enemy1;
			break;
		case 2:
			bullet_obj->type = BOSS_BULLET2;
			bullet_obj->bitmap = image_enemy1;
			break;
		case 3:
			bullet_obj->type = BOSS_BULLET3;
			bullet_obj->bitmap = image_enemy1;
			break;
		case 4:
			bullet_obj->type = BOSS_BULLET4;
			bullet_obj->bitmap = image_enemy4;
			break;
		case 5:
			bullet_obj->type = BOSS_BULLET5;
			bullet_obj->bitmap = image_enemy2;
			break;
		case 6:
			bullet_obj->type = BOSS_BULLET6;
			bullet_obj->bitmap = image_enemy6;
			break;
		}
		bullet_obj->health = 1;
		bullet_obj->width = 25.0f;
		bullet_obj->height = 25.0f;
		bullet_obj->x = to_x;
		bullet_obj->y = to_y;

		bullet_obj->move_x = to_x - from_x;
		bullet_obj->move_y = to_y - from_y;
		float vector_total = sqrt(pow(bullet_obj->move_x, 2) + pow(bullet_obj->move_y, 2));
		bullet_obj->move_x = (bullet_obj->move_x / vector_total) * bulletSpeed;
		bullet_obj->move_y = (bullet_obj->move_y / vector_total) * bulletSpeed;

		bullet_obj->a = atan2(bullet_obj->move_y, bullet_obj->move_x) * (180 / M_PI) + 90;

		bullet_obj->delay = reach;
	}
}

void moveBossBullet(GameObject* bullet) {
	if (bullet)
	{
		switch (bullet->type) {
		case BOSS_BULLET1:
			bullet->x += bullet->move_x;
			bullet->y += bullet->move_y;
			break;
		case BOSS_BULLET2:
			bullet->x += bullet->move_x;
			bullet->y += bullet->move_y;
			bullet->move_x *= 1.02;
			bullet->move_y *= 1.02;
			break;
		case BOSS_BULLET3:
			bullet->x += bullet->move_x;
			bullet->y += bullet->move_y;
			if (bullet->age < 150) {
				bullet->move_x *= 0.995;
				bullet->move_y *= 0.995;
			}
			break;
		case BOSS_BULLET4:
		case BOSS_BULLET5:
		case BOSS_BULLET6:
			bullet->x += bullet->move_x;
			bullet->y += bullet->move_y;
			break;
		}

		if (bullet->delay != 0 && bullet->age == bullet->delay) {
			if (bullet->type == BOSS_BULLET4) {
				if (randomFloat(0.0f, 1.0f) < 0.8) {
					bulletPattern_1_Custom(rand() % 3 + 1, 12, bossBulletSpeed, 0, 360 * randomFloat(0.0f, 1.0f), bullet->x, bullet->y);
				}
				else {
					bulletPattern_1_Custom(4, rand() % 3 + 1, bossBulletSpeed, bullet->delay * randomFloat(0.6f, 1.5f), 360 * randomFloat(0.0f, 1.0f), bullet->x, bullet->y);
				}
				deleteObject(bullet);
			}
			else if (bullet->type == BOSS_BULLET5) {
				if (player_obj) {
					bullet->type = BOSS_BULLET1;
					bullet->bitmap = image_enemy3;
					bullet->delay = 0;

					bullet->move_x = player_obj->x - bullet->x;
					bullet->move_y = player_obj->y - bullet->y;
					float vector_total = sqrt(pow(bullet->move_x, 2) + pow(bullet->move_y, 2));
					bullet->move_x = (bullet->move_x / vector_total) * (bossBulletSpeed * 2);
					bullet->move_y = (bullet->move_y / vector_total) * (bossBulletSpeed * 2);

					bullet->a = atan2(bullet->move_y, bullet->move_x) * (180 / M_PI) + 90;
				}
			}
			else if (bullet->type == BOSS_BULLET6) {
				bulletPattern_1_Custom(5, rand() % 6 + 3, bossBulletSpeed, bullet->delay * randomFloat(0.6f, 1.5f), 360 * randomFloat(0.0f, 1.0f), bullet->x, bullet->y);
				deleteObject(bullet);
			}
		}

		if (bullet->x - bullet->width / 2 < -(MAPSIZE_WIDTH / 2) || bullet->x + bullet->width / 2 > MAPSIZE_WIDTH / 2 || bullet->y - bullet->height / 2 > MAPSIZE_HEIGHT / 2 || bullet->y + bullet->height / 2 < -(MAPSIZE_HEIGHT / 2))
		{
			deleteObject(bullet);
		}
	}
}

void bulletPattern_1(int bulletType, int bulletNum, float bulletSpeed, int reach, float degree) {
	int i;

	for (i = 0; i < bulletNum; i++) {
		createBossBullet(bulletType, bulletSpeed, reach, boss_obj->x + boss_obj->size * cos(((360 / float(bulletNum)) * i + degree) * (M_PI / 180)), boss_obj->y + boss_obj->size * sin(((360 / float(bulletNum)) * i + degree) * (M_PI / 180)));
	}
}

void bulletPattern_1_Custom(int bulletType, int bulletNum, float bulletSpeed, int reach, float degree, float from_x, float from_y) {
	int i;

	for (i = 0; i < bulletNum; i++) {
		createBossBullet_Custom(bulletType, bulletSpeed, reach, from_x, from_y, from_x + cos(((360 / float(bulletNum)) * i + degree) * (M_PI / 180)), from_y + sin(((360 / float(bulletNum)) * i + degree) * (M_PI / 180)));
	}
}

// Boss Bullet Pattern

// 0.01 * delay -> but maximum : 0.03 ~ 0.04
// straight
int bossPattern_1(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed) {
	static int i = -1;

	if (int(elapsedTime * 100) == int(startingTime * 100) && i == -1) {
		i = 0;
	}

	if (i >= loop) {
		i = -1;
		return 1;
	}

	if (int(elapsedTime * 100) >= (int(startingTime * 100) + delay * i)) {
		bulletPattern_1(bulletType, bulletNum, bulletSpeed, 0, 0);
		i++;
	}

	return 0;
}

// straight, start speed : default -> fast, move speed :default -> slow
int bossPattern_2(float startingTime, int delay, int loop, int bulletNum, float bulletSpeed) {
	static int i = -1;
	static int degree = 0;

	if (int(elapsedTime * 100) == int(startingTime * 100) && i == -1) {
		i = 0;
		degree = 360 * randomFloat(0.0f, 1.0f);
	}

	if (i >= loop) {
		i = -1;
		return 1;
	}

	if (int(elapsedTime * 100) >= (int(startingTime * 100) + delay * i)) {
		bulletPattern_1(3, bulletNum, bulletSpeed + (0.2 * i), 0, degree * i);
		i++;
	}

	return 0;
}

// boom, reach : spread start time
int bossPattern_3(float startingTime, int delay, int loop, int bulletNum, float bulletSpeed, int reach) {
	static int i = -1;

	if (int(elapsedTime * 100) == int(startingTime * 100) && i == -1) {
		i = 0;
	}

	if (i >= loop) {
		i = -1;
		return 1;
	}

	if (int(elapsedTime * 100) >= (int(startingTime * 100) + delay * i)) {
		bulletPattern_1(4, bulletNum, bulletSpeed, reach, (360 / float(bulletNum * 2) * (i % 2)));
		i++;
	}

	return 0;
}

// straight, move 1/2 degree each
int bossPattern_4(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed) {
	static int i = -1;

	if (int(elapsedTime * 100) == int(startingTime * 100) && i == -1) {
		i = 0;
	}

	if (i >= loop) {
		i = -1;
		return 1;
	}

	if (int(elapsedTime * 100) >= (int(startingTime * 100) + delay * i)) {
		bulletPattern_1(bulletType, bulletNum, bulletSpeed, 0, (360 / float(bulletNum * 2) * (i % 2)));
		i++;
	}

	return 0;
}

// straight, move right 1/4 degree each
int bossPattern_5(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed) {
	static int i = -1;

	if (int(elapsedTime * 100) == int(startingTime * 100) && i == -1) {
		i = 0;
	}

	if (i >= loop) {
		i = -1;
		return 1;
	}

	if (int(elapsedTime * 100) >= (int(startingTime * 100) + delay * i)) {
		bulletPattern_1(bulletType, bulletNum, bulletSpeed, 0, (360 / float(bulletNum * 4) * (i % 4)));
		i++;
	}

	return 0;
}

// tracking player, bulletType : 5 -> reach : tracking start time, 6 -> reach : spread start time
int bossPattern_6(float startingTime, int delay, int loop, int bulletType, int bulletNum, float bulletSpeed, int reach) {
	static int i = -1;

	if (int(elapsedTime * 100) == int(startingTime * 100) && i == -1) {
		i = 0;
	}

	if (i >= loop) {
		i = -1;
		return 1;
	}

	if (int(elapsedTime * 100) >= (int(startingTime * 100) + delay * i)) {
		bulletPattern_1(bulletType, bulletNum, bulletSpeed, reach, (360 / float(bulletNum * 2) * (i % 2)));
		i++;
	}

	return 0;
}

// random
int bossPattern_7(float startingTime) {
	static int i = -1;
	static int delay, loop;
	int bulletType;
	int bulletNum;
	int reach;
	float bulletSpeed;

	if (randomFloat(0.0f, 1.0f) < 0.1f) {
		bulletType = rand() % 3 + 4;
		bulletNum = rand() % 6 + 3;
		bulletSpeed = bossBulletSpeed * randomFloat(0.5, 2);
		reach = bossBulletReach * randomFloat(0.5, 2);
	}
	else {
		bulletType = rand() % 3 + 1;
		bulletNum = rand() % 40 + 4;
		bulletSpeed = bossBulletSpeed * randomFloat(0.5, 3);
		reach = 0;
	}

	if (int(elapsedTime * 100) == int(startingTime * 100) && i == -1) {
		i = 0;
		delay = rand() % 45 + 5;
		loop = rand() % 50 + 10;
	}

	if (i >= loop) {
		i = -1;
		return 1;
	}

	if (int(elapsedTime * 100) >= (int(startingTime * 100) + delay * i)) {
		bulletPattern_1(bulletType, bulletNum, bulletSpeed, reach, rand() % 360);
		i++;
	}

	return 0;
}

//

// Draw

void drawMenu()
{
	float cx = WIN_WIDTH / 4 + WIN_WIDTH / 16;
	float cy = -(WIN_HEIGHT / 2) + (WIN_HEIGHT / 16) * 4 + (WIN_HEIGHT / 32) * 4;

	if (image_title) {
		drawBitmap(image_title, 0, WIN_HEIGHT / 4, WIN_WIDTH / 2, WIN_HEIGHT / 8, 0);
	}

	if (image_menu_start) {
		drawBitmap(image_menu_start, cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (menu_selection == 1)
		{
			setColor(1, 0, 0);
			strokeRectangle(cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	if (image_menu_ranking) {
		drawBitmap(image_menu_ranking, cx, cy - (WIN_HEIGHT / 16) - WIN_HEIGHT / 32, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (menu_selection == 2) {
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) - WIN_HEIGHT / 32, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	if (image_menu_option) {
		drawBitmap(image_menu_option, cx, cy - (WIN_HEIGHT / 16) * 2 - (WIN_HEIGHT / 32) * 2, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (menu_selection == 3) {
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) * 2 - (WIN_HEIGHT / 32) * 2, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	if (image_menu_exit) {
		drawBitmap(image_menu_exit, cx, cy - (WIN_HEIGHT / 16) * 3 - (WIN_HEIGHT / 32) * 3, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (menu_selection == 0)
		{
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) * 3 - (WIN_HEIGHT / 32) * 3, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	drawMenuMob(menu_mobNum);
}

void drawModeSelection() {
	float cx = WIN_WIDTH / 4 + WIN_WIDTH / 16;
	float cy = -(WIN_HEIGHT / 2) + (WIN_HEIGHT / 16) * 4 + (WIN_HEIGHT / 32) * 4;

	if (image_title) {
		drawBitmap(image_title, 0, WIN_HEIGHT / 4, WIN_WIDTH / 2, WIN_HEIGHT / 8, 0);
	}

	/*
	if (image_select_mode_1) {
		drawBitmap(image_select_mode_1, cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (mode_selection == 1)
		{
			setColor(1, 0, 0);
			strokeRectangle(cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}
	*/

	if (image_select_mode_2) {
		drawBitmap(image_select_mode_2, cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (mode_selection == 1) {
			setColor(1, 0, 0);
			strokeRectangle(cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	if (image_select_mode_3) {
		drawBitmap(image_select_mode_3, cx, cy - (WIN_HEIGHT / 16) - (WIN_HEIGHT / 32), WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (mode_selection == 2) {
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) - (WIN_HEIGHT / 32), WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	if (image_select_mode_4) {
		drawBitmap(image_select_mode_4, cx, cy - (WIN_HEIGHT / 16) * 2 - (WIN_HEIGHT / 32) * 2, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (mode_selection == 3)
		{
			drawBitmap(image_select_mode_4_select, cx, cy - (WIN_HEIGHT / 16) * 2 - (WIN_HEIGHT / 32) * 2, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) * 2 - (WIN_HEIGHT / 32) * 2, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	if (image_select_exit)
	{
		drawBitmap(image_select_exit, cx, cy - (WIN_HEIGHT / 16) * 3 - (WIN_HEIGHT / 32) * 3, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (mode_selection == 0)
		{
			drawBitmap(image_select_exit, cx, cy - (WIN_HEIGHT / 16) * 3 - (WIN_HEIGHT / 32) * 3, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) * 3 - (WIN_HEIGHT / 32) * 3, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	drawMenuMob(menu_mobNum);
}

void drawOption() {
	float cx = WIN_WIDTH / 4 + WIN_WIDTH / 16;
	float cy = -(WIN_HEIGHT / 2) + (WIN_HEIGHT / 16) * 3 + (WIN_HEIGHT / 32) * 3;

	if (image_title) {
		drawBitmap(image_title, 0, WIN_HEIGHT / 4, WIN_WIDTH / 2, WIN_HEIGHT / 8, 0);
	}

	switch (minimap_type) {
	case 0:
		if (image_minimapType_1) {
			drawBitmap(image_minimapType_1, cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
		break;
	case 1:
		if (image_minimapType_2) {
			drawBitmap(image_minimapType_2, cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
		break;
	case 2:
		if (image_minimapType_3) {
			drawBitmap(image_minimapType_3, cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
		break;
	case 3:
		if (image_minimapType_4) {
			drawBitmap(image_minimapType_4, cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
		break;
	}
	if (option_selection == 1)
	{
		setColor(1, 0, 0);
		strokeRectangle(cx, cy, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
	}

	if (image_explanation) {
		drawBitmap(image_explanation, cx, cy - (WIN_HEIGHT / 16) - (WIN_HEIGHT / 32), WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (option_selection == 2) {
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) - (WIN_HEIGHT / 32), WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	if (image_option_exit)
	{
		drawBitmap(image_option_exit, cx, cy - (WIN_HEIGHT / 16) * 2 - (WIN_HEIGHT / 32) * 2, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);

		if (option_selection == 0) {
			setColor(1, 0, 0);
			strokeRectangle(cx, cy - (WIN_HEIGHT / 16) * 2 - (WIN_HEIGHT / 32) * 2, WIN_WIDTH / 4, WIN_HEIGHT / 16, 0);
		}
	}

	drawMenuMob(menu_mobNum);
}

void drawExplanation() {
	// background darkly
	setColor(0, 0, 0);
	fillRectangle(0, 0, WIN_WIDTH, WIN_HEIGHT, 0, ALPHA * 0.8);

	int imageSize;
	if (WIN_WIDTH > WIN_HEIGHT) {
		imageSize = WIN_HEIGHT;
	}
	else {
		imageSize = WIN_WIDTH;
	}
	imageSize *= 0.9;
	drawBitmap(image_explanation_image, 0, 0, imageSize, imageSize, 0);
}

void drawMenuMob(int mobNum) {
	switch (mobNum) {
	case 1:
		if (image_player1) {
			drawBitmap(image_player1, -(WIN_WIDTH / 4), -(WIN_HEIGHT / 4), (WIN_WIDTH / 8) * 3, (WIN_HEIGHT / 8) * 3, 0);
		}
		break;
	case 2:
		if (image_enemy1) {
			drawBitmap(image_enemy1, -(WIN_WIDTH / 4), -(WIN_HEIGHT / 4), (WIN_WIDTH / 8) * 3, (WIN_HEIGHT / 8) * 3, 0);
		}
		break;
	case 3:
		if (image_enemy2) {
			drawBitmap(image_enemy2, -(WIN_WIDTH / 4), -(WIN_HEIGHT / 4), (WIN_WIDTH / 8) * 3, (WIN_HEIGHT / 8) * 3, 0);
		}
		break;
	case 4:
		if (image_enemy3) {
			drawBitmap(image_enemy3, -(WIN_WIDTH / 4), -(WIN_HEIGHT / 4), (WIN_WIDTH / 8) * 3, (WIN_HEIGHT / 8) * 3, 0);
		}
		break;
	case 5:
		if (image_enemy4) {
			drawBitmap(image_enemy4, -(WIN_WIDTH / 4), -(WIN_HEIGHT / 4), (WIN_WIDTH / 8) * 3, (WIN_HEIGHT / 8) * 3, 0);
		}
		break;
	case 6:
		if (image_enemy5) {
			drawBitmap(image_enemy5, -(WIN_WIDTH / 4), -(WIN_HEIGHT / 4), (WIN_WIDTH / 8) * 3, (WIN_HEIGHT / 8) * 3, 0);
		}
		break;
	case 7:
		if (image_enemy6) {
			drawBitmap(image_enemy6, -(WIN_WIDTH / 4), -(WIN_HEIGHT / 4), (WIN_WIDTH / 8) * 3, (WIN_HEIGHT / 8) * 3, 0);
		}
		break;
	}
}

void drawUI()
{
	// score
	char score_str[18];
	sprintf(score_str, "Score : %d", score);
	setColor(1, 1, 1);
	if (game_mode == GAME_MODE_2_INIT || game_mode == GAME_MODE_2 || game_mode == GAME_MODE_2_STAGE_CLEAR || game_mode == GAME_MODE_2_END) {
		drawText(camera_x + 100, camera_y + WIN_HEIGHT / 2 - 45, GLUT_BITMAP_TIMES_ROMAN_24, score_str);
	}
	else if (game_mode == GAME_MODE_3_INIT || game_mode == GAME_MODE_3 || game_mode == GAME_MODE_3_END) {
		drawText(camera_x + 30, camera_y + WIN_HEIGHT / 2 - 45, GLUT_BITMAP_TIMES_ROMAN_24, score_str);
	}
	else if (game_mode == GAME_MODE_4_INIT || game_mode == GAME_MODE_4 || game_mode == GAME_MODE_4_STAGE_CLEAR || game_mode == GAME_MODE_4_END) {
		drawText(camera_x + 30, camera_y + WIN_HEIGHT / 2 - 80, GLUT_BITMAP_TIMES_ROMAN_24, score_str);
	}

	// time
	float calTime = elapsedTime;
	if (game_mode == GAME_MODE_2_INIT || game_mode == GAME_MODE_2) {
		calTime += totalTime;
	}
	else if (game_mode == GAME_MODE_2_STAGE_CLEAR || game_mode == GAME_MODE_4_STAGE_CLEAR || game_mode == GAME_MODE_1_END || game_mode == GAME_MODE_2_END || game_mode == GAME_MODE_3_END || game_mode == GAME_MODE_4_END) {
		calTime = totalTime;
	}

	int hour = 0, min = 0, sec = 0;
	hour = int(calTime) / 3600;
	min = int(calTime) / 60;
	if (min > 60) min -= 60;
	sec = int(calTime) % 60;

	char time_str[10];
	sprintf(time_str, "%02d:%02d:%02d", hour, min, sec);
	setColor(1, 1, 1);
	if (game_mode == GAME_MODE_2_INIT || game_mode == GAME_MODE_2 || game_mode == GAME_MODE_2_STAGE_CLEAR || game_mode == GAME_MODE_2_END) {
		drawText(camera_x - 200, camera_y + WIN_HEIGHT / 2 - 45, GLUT_BITMAP_TIMES_ROMAN_24, time_str);
	}
	else if (game_mode == GAME_MODE_3_INIT || game_mode == GAME_MODE_3 || game_mode == GAME_MODE_3_END) {
		drawText(camera_x - 120, camera_y + WIN_HEIGHT / 2 - 45, GLUT_BITMAP_TIMES_ROMAN_24, time_str);
	}
	else if (game_mode == GAME_MODE_4_INIT || game_mode == GAME_MODE_4 || game_mode == GAME_MODE_4_STAGE_CLEAR || game_mode == GAME_MODE_4_END) {
		drawText(camera_x - 120, camera_y + WIN_HEIGHT / 2 - 80, GLUT_BITMAP_TIMES_ROMAN_24, time_str);
	}

	// stage + enemy rest
	if (game_mode == GAME_MODE_2_INIT || game_mode == GAME_MODE_2 || game_mode == GAME_MODE_2_STAGE_CLEAR || game_mode == GAME_MODE_2_END) {
		char stage_str[15];
		sprintf(stage_str, "Stage : %2d", mode_2_Stage);
		setColor(1, 1, 1);
		drawText(camera_x - 50, camera_y + WIN_HEIGHT / 2 - 45, GLUT_BITMAP_TIMES_ROMAN_24, stage_str);


		char enemy_str[15];
		sprintf(enemy_str, "Rest : %3d", enemyCount);
		setColor(1, 1, 1);
		drawText(camera_x - 50, camera_y + WIN_HEIGHT / 2 - 45 - 40, GLUT_BITMAP_TIMES_ROMAN_24, enemy_str);
	}

	// boss energy
	if (game_mode == GAME_MODE_4_INIT || game_mode == GAME_MODE_4 || game_mode == GAME_MODE_4_STAGE_CLEAR || game_mode == GAME_MODE_4_END) {
		setColor(1, 1, 1);
		strokeRectangle(camera_x, camera_y + WIN_HEIGHT / 2 - 30, WIN_WIDTH / 2, 30, 0);

		if (boss_obj->health < bossHealth * 0.3)
		{
			setColor(1, 0, 0);
		}
		else
		{
			setColor(0, 0, 1);
		}
		if (boss_obj->health <= 0) {
			boss_obj->health = 0;
		}
		fillRectangle(camera_x + 1 - WIN_WIDTH / 4 * (1 - (boss_obj->health / float(bossHealth))), camera_y + WIN_HEIGHT / 2 - 30, (WIN_WIDTH / 2) * (boss_obj->health / float(bossHealth)) - 2, 28, 0);
	}
}

void drawStageClear(int mode) {
	int num, numSize = 0;
	int stage;
	float cx = camera_x - WIN_WIDTH / 6;
	float cy = camera_y;

	if (mode == 2) {
		stage = mode_2_Stage;
	}
	else if (mode == 4) {
		stage = mode_4_Stage;
	}
	else {
		return;
	}
	if (image_mode_2_stage_clear) {
		drawBitmap(image_mode_2_stage_clear, cx + 50 + WIN_WIDTH / 6, cy + WIN_HEIGHT / 8 + 30, WIN_WIDTH / 3, WIN_HEIGHT / 8, 0);
	}

	while (stage != 0) {
		num = stage % 10;
		stage /= 10;
		switch (num) {
		case 0:
			if (image_0) {
				drawBitmap(image_0, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 1:
			if (image_1) {
				drawBitmap(image_1, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 20, WIN_HEIGHT / 12, 0);
				numSize += 20;
			}
			break;
		case 2:
			if (image_2) {
				drawBitmap(image_2, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 3:
			if (image_3) {
				drawBitmap(image_3, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 4:
			if (image_4) {
				drawBitmap(image_4, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 5:
			if (image_5) {
				drawBitmap(image_5, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 6:
			if (image_6) {
				drawBitmap(image_6, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 7:
			if (image_7) {
				drawBitmap(image_7, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 8:
			if (image_8) {
				drawBitmap(image_8, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		case 9:
			if (image_9) {
				drawBitmap(image_9, cx - 50 - numSize, cy + WIN_HEIGHT / 8 + 30, 50, WIN_HEIGHT / 12, 0);
				numSize += 50;
			}
			break;
		}
	}

	if (image_press_enter) {
		drawBitmap(image_press_enter, camera_x, cy, WIN_WIDTH / 3, WIN_HEIGHT / 12, 0);
	}
}

void drawGameOver()
{
	if (image_game_over){
		drawBitmap(image_game_over, camera_x, camera_y + WIN_HEIGHT / 8 + 30, WIN_WIDTH / 2, WIN_HEIGHT / 8, 0);
	}
	if (image_press_enter) {
		drawBitmap(image_press_enter, camera_x, camera_y, WIN_WIDTH / 3, WIN_HEIGHT / 12, 0);
	}
}

void drawMinimap(int minimapType) {
	float mapsize;
	float minimap_cx;
	float minimap_cy;
	float minimap_w;
	float minimap_h;

	switch (minimapType) {
	case 0:
		minimap_cx = camera_x + (WIN_WIDTH / 8) * 3;
		minimap_cy = camera_y + (WIN_HEIGHT / 8) * 3;
		minimap_w = WIN_WIDTH / 4;
		minimap_h = WIN_HEIGHT / 4;
		break;
	case 1:
		if (WIN_WIDTH > WIN_HEIGHT) {
			mapsize = WIN_HEIGHT;
		}
		else {
			mapsize = WIN_WIDTH;
		}
		minimap_cx = camera_x + (WIN_WIDTH / 2) - (mapsize / 8);
		minimap_cy = camera_y + (WIN_HEIGHT / 2) - (mapsize / 8);
		minimap_w = mapsize / 4;
		minimap_h = mapsize / 4;
		break;
	case 2:
		mapsize = (WIN_WIDTH + WIN_HEIGHT) / 2;
		minimap_cx = camera_x + (WIN_WIDTH / 2) - (mapsize / 8);
		minimap_cy = camera_y + (WIN_HEIGHT / 2) - (mapsize / 8);
		minimap_w = mapsize / 4;
		minimap_h = mapsize / 4;
		break;
	case 3:
		if (WIN_WIDTH < WIN_HEIGHT) {
			mapsize = WIN_HEIGHT;
		}
		else {
			mapsize = WIN_WIDTH;
		}
		minimap_cx = camera_x + (WIN_WIDTH / 2) - (mapsize / 8);
		minimap_cy = camera_y + (WIN_HEIGHT / 2) - (mapsize / 8);
		minimap_w = mapsize / 4;
		minimap_h = mapsize / 4;
		break;
	}


	setColor(0.8, 0.8, 0.8);
	fillRectangle(minimap_cx, minimap_cy, minimap_w, minimap_h, 0, ALPHA);
	setColor(0.2, 0.2, 0.2);
	strokeRectangle(minimap_cx, minimap_cy, minimap_w, minimap_h, 0, ALPHA);


	float mCamera_cx = minimap_cx + camera_x * (minimap_w / float(MAPSIZE_WIDTH));
	float mCamera_cy = minimap_cy + camera_y * (minimap_h / float(MAPSIZE_HEIGHT));
	float mCamera_w = minimap_w * (WIN_WIDTH / float(MAPSIZE_WIDTH));
	float mCamera_h = minimap_h * (WIN_HEIGHT / float(MAPSIZE_HEIGHT));

	setColor(1, 1, 1);
	strokeRectangle(mCamera_cx, mCamera_cy, mCamera_w, mCamera_h, 0, ALPHA);


	if (player_obj) {
		float mPlayer_cx = minimap_cx + player_obj_collision.x * (minimap_w / float(MAPSIZE_WIDTH));
		float mPlayer_cy = minimap_cy + player_obj_collision.y * (minimap_h / float(MAPSIZE_HEIGHT));

		float mPlayer_x0 = mPlayer_cx + cos(degree) * 10;
		float mPlayer_y0 = mPlayer_cy + sin(degree) * 10;
		float mPlayer_x1 = mPlayer_cx + cos(degree + 120 * (M_PI / 180)) * 6;
		float mPlayer_y1 = mPlayer_cy + sin(degree + 120 * (M_PI / 180)) * 6;
		float mPlayer_x2 = mPlayer_cx + cos(degree + 240 * (M_PI / 180)) * 6;
		float mPlayer_y2 = mPlayer_cy + sin(degree + 240 * (M_PI / 180)) * 6;

		setColor(0, 0, 1);
		fillTriangle(mPlayer_x0, mPlayer_y0, mPlayer_x1, mPlayer_y1, mPlayer_x2, mPlayer_y2, ALPHA);
	}


	int i;
	for (i = 0; i < POOL_SIZE; i++)
	{
		GameObject* game_obj = getObject(i);
		if (game_obj && game_obj->is_alive && game_obj->bitmap)
		{
			float mGameObj_cx = minimap_cx + game_obj->x * (minimap_w / float(MAPSIZE_WIDTH));
			float mGameObj_cy = minimap_cy + game_obj->y * (minimap_h / float(MAPSIZE_HEIGHT));

			switch (game_obj->type) {
			case ENEMY1:
			case ENEMY2:
			case ENEMY3:
			case ENEMY4:
			case ENEMY5:
			case ENEMY6:
			case BOSS_BULLET1:
			case BOSS_BULLET2:
			case BOSS_BULLET3:
			case BOSS_BULLET4:
			case BOSS_BULLET5:
			case BOSS_BULLET6:
				setColor(1, 0, 0);
				fillCircle(mGameObj_cx, mGameObj_cy, 3, ALPHA);
				break;
			case ENEMY_BULLET:
				setColor(0.8, 0.4, 0.4);
				fillCircle(mGameObj_cx, mGameObj_cy, 3, ALPHA);
				break;
			case PLAYER_BULLET:
				setColor(0.4, 0.8, 0.4);
				fillCircle(mGameObj_cx, mGameObj_cy, 3, ALPHA);
				break;
			case PET_BULLET:
				setColor(0.4, 0.8, 0.4);
				fillCircle(mGameObj_cx, mGameObj_cy, 3, ALPHA);
				break;
			case BOSS:
				setColor(1, 1, 0);
				fillCircle(mGameObj_cx, mGameObj_cy, 5, ALPHA);
				break;
			}
		}
	}
}

void drawRanking() {
	// background darkly
	setColor(0, 0, 0);
	fillRectangle(0, 0, WIN_WIDTH, WIN_HEIGHT, 0, ALPHA * 0.8);

	setColor(0.6, 0.6, 0.6);
	fillRectangle(0, 0, 800, WIN_HEIGHT * 0.9, 0);


	int i;
	int hour, min;
	float sec;
	char score_str[100];

	if (ranking_page == 0) {
		if (image_ranking_allkill) drawBitmap(image_ranking_allkill, -80, WIN_HEIGHT / 2 - 75, 150, 40, 0);

		if (image_ranking_rank) drawBitmap(image_ranking_rank, -315, WIN_HEIGHT / 2 - 125, 40, 30, 0);
		if (image_ranking_date) drawBitmap(image_ranking_date, -120, WIN_HEIGHT / 2 - 125, 50, 30, 0);
		if (image_ranking_stage) drawBitmap(image_ranking_stage, 50, WIN_HEIGHT / 2 - 125, 63, 30, 0);
		if (image_ranking_score) drawBitmap(image_ranking_score, 143, WIN_HEIGHT / 2 - 125, 50, 30, 0);
		if (image_ranking_time) drawBitmap(image_ranking_time, 270, WIN_HEIGHT / 2 - 125, 50, 30, 0);

		setColor(0.4, 0.4, 0.4);
		strokeLine(-350, WIN_HEIGHT / 2 - 150, 350, WIN_HEIGHT / 2 - 150);
		strokeLine(-270, WIN_HEIGHT / 2 - 100, -270, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(-120, WIN_HEIGHT / 2 - 150, -120, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(10, WIN_HEIGHT / 2 - 100, 10, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(95, WIN_HEIGHT / 2 - 100, 95, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(190, WIN_HEIGHT / 2 - 100, 190, -(WIN_HEIGHT / 2) * 0.9 + 20);

		for (i = ranking_print_min; i < ranking_print_max; i++) {
			if (i < score_Mode_2_Size) {
				hour = int(score_Mode_2[score_Mode_2_Sequence[i]].time * 10) / 36000;
				min = int(score_Mode_2[score_Mode_2_Sequence[i]].time * 10) / 600;
				sec = score_Mode_2[score_Mode_2_Sequence[i]].time - hour * 3600 - min * 60;
				sprintf(score_str, "%03d        %d:%02d:%02d       %02d:%02d:%02d         %02d        %05d      %02d:%02d:%06.03f", i + 1, score_Mode_2[score_Mode_2_Sequence[i]].year, score_Mode_2[score_Mode_2_Sequence[i]].month, score_Mode_2[score_Mode_2_Sequence[i]].day, score_Mode_2[score_Mode_2_Sequence[i]].hour, score_Mode_2[score_Mode_2_Sequence[i]].minute, score_Mode_2[score_Mode_2_Sequence[i]].second, score_Mode_2[score_Mode_2_Sequence[i]].stage, score_Mode_2[score_Mode_2_Sequence[i]].score, hour, min, sec);
				setColor(0, 0, 0);
				drawText(-332, WIN_HEIGHT / 2 - 180 - (40 * (i - ranking_print_min)), GLUT_BITMAP_TIMES_ROMAN_24, score_str);

				setColor(0.4, 0.4, 0.4);
				strokeLine(-350, WIN_HEIGHT / 2 - 193 - (40 * (i - ranking_print_min)), 350, WIN_HEIGHT / 2 - 193 - (40 * (i - ranking_print_min)), ALPHA * 0.6);
			}
		}
	}
	
	else if (ranking_page == 1) {
		if (image_ranking_infinite) drawBitmap(image_ranking_infinite, -80, WIN_HEIGHT / 2 - 75, 150, 40, 0);

		if (image_ranking_rank) drawBitmap(image_ranking_rank, -315, WIN_HEIGHT / 2 - 125, 40, 30, 0);
		if (image_ranking_date) drawBitmap(image_ranking_date, -120, WIN_HEIGHT / 2 - 125, 50, 30, 0);
		if (image_ranking_score) drawBitmap(image_ranking_score, 100, WIN_HEIGHT / 2 - 125, 50, 30, 0);
		if (image_ranking_time) drawBitmap(image_ranking_time, 270, WIN_HEIGHT / 2 - 125, 50, 30, 0);
		
		setColor(0.4, 0.4, 0.4);
		strokeLine(-350, WIN_HEIGHT / 2 - 150, 350, WIN_HEIGHT / 2 - 150);
		strokeLine(-270, WIN_HEIGHT / 2 - 100, -270, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(-120, WIN_HEIGHT / 2 - 150, -120, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(10, WIN_HEIGHT / 2 - 100, 10, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(190, WIN_HEIGHT / 2 - 100, 190, -(WIN_HEIGHT / 2) * 0.9 + 20);

		for (i = ranking_print_min; i < ranking_print_max; i++) {
			if (i < score_Mode_3_Size) {
				hour = int(score_Mode_3[score_Mode_3_Sequence[i]].time * 10) / 36000;
				min = int(score_Mode_3[score_Mode_3_Sequence[i]].time * 10) / 600;
				sec = score_Mode_3[score_Mode_3_Sequence[i]].time - hour * 3600 - min * 60;
				sprintf(score_str, "%03d        %d:%02d:%02d       %02d:%02d:%02d              %05d             %02d:%02d:%06.03f", i + 1, score_Mode_3[score_Mode_3_Sequence[i]].year, score_Mode_3[score_Mode_3_Sequence[i]].month, score_Mode_3[score_Mode_3_Sequence[i]].day, score_Mode_3[score_Mode_3_Sequence[i]].hour, score_Mode_3[score_Mode_3_Sequence[i]].minute, score_Mode_3[score_Mode_3_Sequence[i]].second, score_Mode_3[score_Mode_3_Sequence[i]].score, hour, min, sec);
				setColor(0, 0, 0);
				drawText(-332, WIN_HEIGHT / 2 - 180 - (40 * (i - ranking_print_min)), GLUT_BITMAP_TIMES_ROMAN_24, score_str);

				setColor(0.4, 0.4, 0.4);
				strokeLine(-350, WIN_HEIGHT / 2 - 193 - (40 * (i - ranking_print_min)), 350, WIN_HEIGHT / 2 - 193 - (40 * (i - ranking_print_min)), ALPHA * 0.6);
			}
		}
	}

	else if (ranking_page == 2) {
		if (image_ranking_boss) drawBitmap(image_ranking_boss, -80, WIN_HEIGHT / 2 - 75, 150, 40, 0);

		if (image_ranking_rank) drawBitmap(image_ranking_rank, -315, WIN_HEIGHT / 2 - 125, 40, 30, 0);
		if (image_ranking_date) drawBitmap(image_ranking_date, -120, WIN_HEIGHT / 2 - 125, 50, 30, 0);
		if (image_ranking_stage) drawBitmap(image_ranking_stage, 50, WIN_HEIGHT / 2 - 125, 63, 30, 0);
		if (image_ranking_score) drawBitmap(image_ranking_score, 143, WIN_HEIGHT / 2 - 125, 50, 30, 0);
		if (image_ranking_time) drawBitmap(image_ranking_time, 270, WIN_HEIGHT / 2 - 125, 50, 30, 0);

		setColor(0.4, 0.4, 0.4);
		strokeLine(-350, WIN_HEIGHT / 2 - 150, 350, WIN_HEIGHT / 2 - 150);
		strokeLine(-270, WIN_HEIGHT / 2 - 100, -270, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(-120, WIN_HEIGHT / 2 - 150, -120, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(10, WIN_HEIGHT / 2 - 100, 10, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(95, WIN_HEIGHT / 2 - 100, 95, -(WIN_HEIGHT / 2) * 0.9 + 20);
		strokeLine(190, WIN_HEIGHT / 2 - 100, 190, -(WIN_HEIGHT / 2) * 0.9 + 20);

		for (i = ranking_print_min; i < ranking_print_max; i++) {
			if (i < score_Mode_4_Size) {
				hour = int(score_Mode_4[score_Mode_4_Sequence[i]].time * 10) / 36000;
				min = int(score_Mode_4[score_Mode_4_Sequence[i]].time * 10) / 600;
				sec = score_Mode_4[score_Mode_4_Sequence[i]].time - hour * 3600 - min * 60;
				sprintf(score_str, "%03d        %d:%02d:%02d       %02d:%02d:%02d         %02d        %05d      %02d:%02d:%06.03f", i + 1, score_Mode_4[score_Mode_4_Sequence[i]].year, score_Mode_4[score_Mode_4_Sequence[i]].month, score_Mode_4[score_Mode_4_Sequence[i]].day, score_Mode_4[score_Mode_4_Sequence[i]].hour, score_Mode_4[score_Mode_4_Sequence[i]].minute, score_Mode_4[score_Mode_4_Sequence[i]].second, score_Mode_4[score_Mode_4_Sequence[i]].stage, score_Mode_4[score_Mode_4_Sequence[i]].score, hour, min, sec);
				setColor(0, 0, 0);
				drawText(-332, WIN_HEIGHT / 2 - 180 - (40 * (i - ranking_print_min)), GLUT_BITMAP_TIMES_ROMAN_24, score_str);

				setColor(0.4, 0.4, 0.4);
				strokeLine(-350, WIN_HEIGHT / 2 - 193 - (40 * (i - ranking_print_min)), 350, WIN_HEIGHT / 2 - 193 - (40 * (i - ranking_print_min)), ALPHA * 0.6);
			}
		}
	}

	setColor(1, 1, 1);
	strokeLine(20, WIN_HEIGHT / 2 - 75, 40, WIN_HEIGHT / 2 - 75);
	if (ranking_sortType == 0) {
		if (image_ranking_time) drawBitmap(image_ranking_time, 85, WIN_HEIGHT / 2 - 75, 50, 30, 0);
	}
	else if (ranking_sortType == 1) {
		if (image_ranking_score) drawBitmap(image_ranking_score, 85, WIN_HEIGHT / 2 - 75, 50, 30, 0);
	}

	setColor(0, 0, 0);
	drawText(-330, WIN_HEIGHT / 2 - 82, GLUT_BITMAP_TIMES_ROMAN_24, "F1 : ");
	drawBitmap(image_ranking_time, -265, WIN_HEIGHT / 2 - 75, 40, 20, 0);

	drawText(250, WIN_HEIGHT / 2 - 82, GLUT_BITMAP_TIMES_ROMAN_24, "F2 : ");
	drawBitmap(image_ranking_score, 320, WIN_HEIGHT / 2 - 75, 40, 20, 0);
}

//

// etc

void GetCameraPos(int* x, int* y) {
	if (player_obj && ((game_mode == GAME_MODE_1 || game_mode == GAME_MODE_1_INIT) || (game_mode == GAME_MODE_2 || game_mode == GAME_MODE_2_INIT) || (game_mode == GAME_MODE_3 || game_mode == GAME_MODE_3_INIT) || game_mode == GAME_MODE_4)) {
		if (player_obj_collision.x - WIN_WIDTH / 2 > -(MAPSIZE_WIDTH / 2) && player_obj_collision.x + WIN_WIDTH / 2 < MAPSIZE_WIDTH / 2) {
			camera_x = player_obj_collision.x;
			*x = camera_x - WIN_WIDTH / 2;
		}
		if (player_obj_collision.y - WIN_HEIGHT / 2 > -(MAPSIZE_HEIGHT / 2) && player_obj_collision.y + WIN_HEIGHT / 2 < MAPSIZE_HEIGHT / 2) {
			camera_y = player_obj_collision.y;
			*y = camera_y - WIN_HEIGHT / 2;
		}
	}
	else if (game_mode == GAME_MODE_2_STAGE_CLEAR || game_mode == GAME_MODE_1_END || game_mode == GAME_MODE_2_END || game_mode == GAME_MODE_3_END || game_mode == GAME_MODE_4_END) {

	}
	else if (game_mode == GAME_MODE_4_INIT) {
		camera_x = 0;
		camera_y = -(MAPSIZE_HEIGHT / 2) + WIN_HEIGHT / 2;
		*x = -WIN_WIDTH / 2;
		*y = camera_y - WIN_HEIGHT / 2;
	}
	else {
		camera_x = 0;
		camera_y = 0;
		*x = -WIN_WIDTH / 2;
		*y = -WIN_HEIGHT / 2;
	}
}

void ConvertCollision(GameObject* obj, GameObject* obj_collision) {
	if (obj && obj_collision){
		obj_collision->x = obj->x;
		obj_collision->y = obj->y;
		obj_collision->width = obj->width * 0.9;
		obj_collision->height = obj->height * 0.9;
		obj_collision->a = obj->a;
	}
}

// Score

void SaveScore() {
	FILE *fp = fopen("score.dat", "w");
	if (fp != NULL) {
		fprintf(fp, "%d %d %d\n", score_Mode_2_Size, score_Mode_3_Size, score_Mode_4_Size);

		int i;
		for (i = 0; i < score_Mode_2_Size; i++) {
			fprintf(fp, "%d %d %d %d %d %d %d %d %f\n", score_Mode_2[i].year, score_Mode_2[i].month, score_Mode_2[i].day, score_Mode_2[i].hour, score_Mode_2[i].minute, score_Mode_2[i].second, score_Mode_2[i].stage, score_Mode_2[i].score, score_Mode_2[i].time);
		}
		for (i = 0; i < score_Mode_3_Size; i++) {
			fprintf(fp, "%d %d %d %d %d %d %d %f\n", score_Mode_3[i].year, score_Mode_3[i].month, score_Mode_3[i].day, score_Mode_3[i].hour, score_Mode_3[i].minute, score_Mode_3[i].second, score_Mode_3[i].score, score_Mode_3[i].time);
		}
		for (i = 0; i < score_Mode_4_Size; i++) {
			fprintf(fp, "%d %d %d %d %d %d %d %d %f\n", score_Mode_4[i].year, score_Mode_4[i].month, score_Mode_4[i].day, score_Mode_4[i].hour, score_Mode_4[i].minute, score_Mode_4[i].second, score_Mode_4[i].stage, score_Mode_4[i].score, score_Mode_4[i].time);
		}
		fclose(fp);
	}
}

void LoadScore() {
	FILE *fp = fopen("score.dat", "r");
	if (fp != NULL) {
		int mode2_Size, mode3_Size, mode4_Size;
		fscanf(fp, "%d %d %d", &mode2_Size, &mode3_Size, &mode4_Size);
		if (mode2_Size > score_Mode_2_Size) {
			if (score_Mode_2_Size == 0) {
				score_Mode_2_Size = mode2_Size;
				initializeScoreArr(2);
			}
			else {
				appendScoreArr(mode2_Size - score_Mode_2_Size, 0, 0);
				score_Mode_2_Size = mode2_Size;
			}
		}
		if (mode3_Size > score_Mode_3_Size) {
			if (score_Mode_3_Size == 0) {
				score_Mode_3_Size = mode3_Size;
				initializeScoreArr(3);
			}
			else {
				appendScoreArr(0, mode3_Size - score_Mode_3_Size, 0);
				score_Mode_3_Size = mode3_Size;
			}
		}
		if (mode4_Size > score_Mode_4_Size) {
			if (score_Mode_4_Size == 0) {
				score_Mode_4_Size = mode4_Size;
				initializeScoreArr(4);
			}
			else {
				appendScoreArr(0, 0, mode4_Size - score_Mode_4_Size);
				score_Mode_4_Size = mode4_Size;
			}
		}

		int i;
		for (i = 0; i < score_Mode_2_Size; i++) {
			fscanf(fp, "%d %d %d %d %d %d %d %d %f", &score_Mode_2[i].year, &score_Mode_2[i].month, &score_Mode_2[i].day, &score_Mode_2[i].hour, &score_Mode_2[i].minute, &score_Mode_2[i].second, &score_Mode_2[i].stage, &score_Mode_2[i].score, &score_Mode_2[i].time);
		}
		for (i = 0; i < score_Mode_3_Size; i++) {
			fscanf(fp, "%d %d %d %d %d %d %d %f", &score_Mode_3[i].year, &score_Mode_3[i].month, &score_Mode_3[i].day, &score_Mode_3[i].hour, &score_Mode_3[i].minute, &score_Mode_3[i].second, &score_Mode_3[i].score, &score_Mode_3[i].time);
		}
		for (i = 0; i < score_Mode_4_Size; i++) {
			fscanf(fp, "%d %d %d %d %d %d %d %d %f", &score_Mode_4[i].year, &score_Mode_4[i].month, &score_Mode_4[i].day, &score_Mode_4[i].hour, &score_Mode_4[i].minute, &score_Mode_4[i].second, &score_Mode_4[i].stage, &score_Mode_4[i].score, &score_Mode_4[i].time);
		}
		fclose(fp);
	}
}

void initializeScoreArr(int mode) {
	if (mode == 2 && score_Mode_2_Size) {
		score_Mode_2_Sequence = (int *)malloc(sizeof(int) * score_Mode_2_Size);
		score_Mode_2 = (Score_Mode_1 *)malloc(sizeof(Score_Mode_1) * score_Mode_2_Size);
	}

	else if (mode == 3 && score_Mode_3_Size) {
		score_Mode_3_Sequence = (int *)malloc(sizeof(int) * score_Mode_3_Size);
		score_Mode_3 = (Score_Mode_2 *)malloc(sizeof(Score_Mode_2) * score_Mode_3_Size);
	}

	else if (mode == 4 && score_Mode_4_Size) {
		score_Mode_4_Sequence = (int *)malloc(sizeof(int) * score_Mode_4_Size);
		score_Mode_4 = (Score_Mode_1 *)malloc(sizeof(Score_Mode_1) * score_Mode_4_Size);
	}
}

void appendScoreArr(int mode2_size, int mode3_size, int mode4_size) {
	if (mode2_size > 0) {
		if (score_Mode_2_Size == 0) {
			score_Mode_2_Size += mode2_size;
			initializeScoreArr(2);
		}
		else {
			score_Mode_2_Size += mode2_size;
			score_Mode_2_Sequence = (int *)realloc(score_Mode_2_Sequence, sizeof(int) * score_Mode_2_Size);
			score_Mode_2 = (Score_Mode_1 *)realloc(score_Mode_2, sizeof(Score_Mode_1) * score_Mode_2_Size);
		}
	}

	if (mode3_size > 0) {
		if (score_Mode_3_Size == 0) {
			score_Mode_3_Size += mode3_size;
			initializeScoreArr(3);
		}
		else {
			score_Mode_3_Size += mode3_size;
			score_Mode_3_Sequence = (int *)realloc(score_Mode_3_Sequence, sizeof(int) * score_Mode_3_Size);
			score_Mode_3 = (Score_Mode_2 *)realloc(score_Mode_3, sizeof(Score_Mode_2) * score_Mode_3_Size);
		}
	}

	if (mode4_size > 0) {
		if (score_Mode_4_Size == 0) {
			score_Mode_4_Size += mode4_size;
			initializeScoreArr(4);
		}
		else {
			score_Mode_4_Size += mode4_size;
			score_Mode_4_Sequence = (int *)realloc(score_Mode_4_Sequence, sizeof(int) * score_Mode_4_Size);
			score_Mode_4 = (Score_Mode_1 *)realloc(score_Mode_4, sizeof(Score_Mode_1) * score_Mode_4_Size);
		}
	}
}

void NewScore(int mode) {
	LoadScore();
	if (mode == 2) {
		appendScoreArr(1, 0, 0);
	}
	else if (mode == 3) {
		appendScoreArr(0, 1, 0);
	}
	else if (mode == 4) {
		appendScoreArr(0, 0, 1);
	}
	
	time_t now;
	time(&now);
	tm *currentTime = localtime(&now);
	if (mode == 2) {
		score_Mode_2[score_Mode_2_Size - 1].year = currentTime->tm_year + 1900;
		score_Mode_2[score_Mode_2_Size - 1].month = currentTime->tm_mon + 1;
		score_Mode_2[score_Mode_2_Size - 1].day = currentTime->tm_mday;
		score_Mode_2[score_Mode_2_Size - 1].hour = currentTime->tm_hour;
		score_Mode_2[score_Mode_2_Size - 1].minute = currentTime->tm_min;
		score_Mode_2[score_Mode_2_Size - 1].second = currentTime->tm_sec;
		score_Mode_2[score_Mode_2_Size - 1].stage = mode_2_Stage;
		score_Mode_2[score_Mode_2_Size - 1].score = score;
		score_Mode_2[score_Mode_2_Size - 1].time = totalTime;
	}
	else if (mode == 3) {
		score_Mode_3[score_Mode_3_Size - 1].year = currentTime->tm_year + 1900;
		score_Mode_3[score_Mode_3_Size - 1].month = currentTime->tm_mon + 1;
		score_Mode_3[score_Mode_3_Size - 1].day = currentTime->tm_mday;
		score_Mode_3[score_Mode_3_Size - 1].hour = currentTime->tm_hour;
		score_Mode_3[score_Mode_3_Size - 1].minute = currentTime->tm_min;
		score_Mode_3[score_Mode_3_Size - 1].second = currentTime->tm_sec;
		score_Mode_3[score_Mode_3_Size - 1].score = score;
		score_Mode_3[score_Mode_3_Size - 1].time = totalTime;
	}
	else if (mode == 4) {
		score_Mode_4[score_Mode_4_Size - 1].year = currentTime->tm_year + 1900;
		score_Mode_4[score_Mode_4_Size - 1].month = currentTime->tm_mon + 1;
		score_Mode_4[score_Mode_4_Size - 1].day = currentTime->tm_mday;
		score_Mode_4[score_Mode_4_Size - 1].hour = currentTime->tm_hour;
		score_Mode_4[score_Mode_4_Size - 1].minute = currentTime->tm_min;
		score_Mode_4[score_Mode_4_Size - 1].second = currentTime->tm_sec;
		score_Mode_4[score_Mode_4_Size - 1].stage = mode_4_Stage;
		score_Mode_4[score_Mode_4_Size - 1].score = score;
		score_Mode_4[score_Mode_4_Size - 1].time = totalTime;
	}

	SaveScore();
}

void finalizeScore() {
	if (score_Mode_2_Sequence) {
		free(score_Mode_2_Sequence);
		score_Mode_2_Sequence = NULL;
	}
	if (score_Mode_3_Sequence) {
		free(score_Mode_3_Sequence);
		score_Mode_3_Sequence = NULL;
	}
	if (score_Mode_4_Sequence) {
		free(score_Mode_4_Sequence);
		score_Mode_4_Sequence = NULL;
	}

	if (score_Mode_2) {
		free(score_Mode_2);
		score_Mode_2 = NULL;
	}
	if (score_Mode_3) {
		free(score_Mode_3);
		score_Mode_3 = NULL;
	}
	if (score_Mode_4) {
		free(score_Mode_4);
		score_Mode_4 = NULL;
	}
}

void SortScore_Stage(int mode, int sort_type) {
	int min, max;
	int min_pos, max_pos;
	int temp;
	int i, j, n, m;

	int stage, stage_pos;
	int score, score_pos;
	float time;
	int time_pos;

	if (mode == 2) {
		for (i = 0; i < score_Mode_2_Size; i++) {
			score_Mode_2_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_2_Size / 2; i++) {
			min = max = score_Mode_2[score_Mode_2_Sequence[score_Mode_2_Size / 2]].score;
			min_pos = max_pos = score_Mode_2_Size / 2;
			for (j = i; j < score_Mode_2_Size - i; j++) {
				if (score_Mode_2[score_Mode_2_Sequence[j]].score < min) {
					min = score_Mode_2[score_Mode_2_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_2[score_Mode_2_Sequence[j]].score > max) {
					max = score_Mode_2[score_Mode_2_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == i) {
				if (max_pos == score_Mode_2_Size - 1 - i) {
					temp = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_2_Sequence[score_Mode_2_Size - 1 - i];
					score_Mode_2_Sequence[score_Mode_2_Size - 1 - i] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;

					temp = score_Mode_2_Sequence[i];
					score_Mode_2_Sequence[i] = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_2_Sequence[i];
				score_Mode_2_Sequence[i] = score_Mode_2_Sequence[max_pos];
				score_Mode_2_Sequence[max_pos] = temp;

				temp = score_Mode_2_Sequence[score_Mode_2_Size - 1 - i];
				score_Mode_2_Sequence[score_Mode_2_Size - 1 - i] = score_Mode_2_Sequence[min_pos];
				score_Mode_2_Sequence[min_pos] = temp;
			}
		}


		for (i = 0; i < score_Mode_2_Size - 1; i++) {
			stage = score_Mode_2[score_Mode_2_Sequence[i]].stage;
			stage_pos = i;
			for (j = i + 1; j < score_Mode_2_Size; j++) {
				if (stage != score_Mode_2[score_Mode_2_Sequence[j]].stage) {
					min_pos = stage_pos;
					max_pos = j - 1;
					if (sort_type == 0) {
						SortScore_Time_Custom(2, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							time = score_Mode_2[score_Mode_2_Sequence[m]].time;
							time_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (time != score_Mode_2[score_Mode_2_Sequence[n]].time) {
									SortScore_Score_Custom(2, time_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Score_Custom(2, time_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					else if (sort_type == 1) {
						SortScore_Score_Custom(2, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							score = score_Mode_2[score_Mode_2_Sequence[m]].score;
							score_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (score != score_Mode_2[score_Mode_2_Sequence[n]].score) {
									SortScore_Time_Custom(2, score_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Time_Custom(2, score_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					i = j - 1;
					break;
				}
				else if (j == score_Mode_2_Size - 1) {
					min_pos = stage_pos;
					max_pos = j;
					if (sort_type == 0) {
						SortScore_Time_Custom(2, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							time = score_Mode_2[score_Mode_2_Sequence[m]].time;
							time_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (time != score_Mode_2[score_Mode_2_Sequence[n]].time) {
									SortScore_Score_Custom(2, time_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Score_Custom(2, time_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					else if (sort_type == 1) {
						SortScore_Score_Custom(2, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							score = score_Mode_2[score_Mode_2_Sequence[m]].score;
							score_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (score != score_Mode_2[score_Mode_2_Sequence[n]].score) {
									SortScore_Time_Custom(2, score_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Time_Custom(2, score_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					i = j;
					break;
				}
			}
		}
	}

	else if (mode == 4) {
		for (i = 0; i < score_Mode_4_Size; i++) {
			score_Mode_4_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_4_Size / 2; i++) {
			min = max = score_Mode_4[score_Mode_4_Sequence[score_Mode_4_Size / 2]].score;
			min_pos = max_pos = score_Mode_4_Size / 2;
			for (j = i; j < score_Mode_4_Size - i; j++) {
				if (score_Mode_4[score_Mode_4_Sequence[j]].score < min) {
					min = score_Mode_4[score_Mode_4_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_4[score_Mode_4_Sequence[j]].score > max) {
					max = score_Mode_4[score_Mode_4_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == i) {
				if (max_pos == score_Mode_4_Size - 1 - i) {
					temp = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_4_Sequence[score_Mode_4_Size - 1 - i];
					score_Mode_4_Sequence[score_Mode_4_Size - 1 - i] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;

					temp = score_Mode_4_Sequence[i];
					score_Mode_4_Sequence[i] = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_4_Sequence[i];
				score_Mode_4_Sequence[i] = score_Mode_4_Sequence[max_pos];
				score_Mode_4_Sequence[max_pos] = temp;

				temp = score_Mode_4_Sequence[score_Mode_4_Size - 1 - i];
				score_Mode_4_Sequence[score_Mode_4_Size - 1 - i] = score_Mode_4_Sequence[min_pos];
				score_Mode_4_Sequence[min_pos] = temp;
			}
		}


		for (i = 0; i < score_Mode_4_Size - 1; i++) {
			stage = score_Mode_4[score_Mode_4_Sequence[i]].stage;
			stage_pos = i;
			for (j = i + 1; j < score_Mode_4_Size; j++) {
				if (stage != score_Mode_4[score_Mode_4_Sequence[j]].stage) {
					min_pos = stage_pos;
					max_pos = j - 1;
					if (sort_type == 0) {
						SortScore_Time_Custom(4, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							time = score_Mode_4[score_Mode_4_Sequence[m]].time;
							time_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (time != score_Mode_4[score_Mode_4_Sequence[n]].time) {
									SortScore_Score_Custom(4, time_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Score_Custom(4, time_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					else if (sort_type == 1) {
						SortScore_Score_Custom(4, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							score = score_Mode_4[score_Mode_4_Sequence[m]].score;
							score_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (score != score_Mode_4[score_Mode_4_Sequence[n]].score) {
									SortScore_Time_Custom(4, score_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Time_Custom(4, score_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					i = j - 1;
					break;
				}
				else if (j == score_Mode_4_Size - 1) {
					min_pos = stage_pos;
					max_pos = j;
					if (sort_type == 0) {
						SortScore_Time_Custom(4, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							time = score_Mode_4[score_Mode_4_Sequence[m]].time;
							time_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (time != score_Mode_4[score_Mode_4_Sequence[n]].time) {
									SortScore_Score_Custom(4, time_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Score_Custom(4, time_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					else if (sort_type == 1) {
						SortScore_Score_Custom(4, min_pos, max_pos);
						for (m = min_pos; m < max_pos; m++) {
							score = score_Mode_4[score_Mode_4_Sequence[m]].score;
							score_pos = m;
							for (n = m + 1; n < max_pos + 1; n++) {
								if (score != score_Mode_4[score_Mode_4_Sequence[n]].score) {
									SortScore_Time_Custom(4, score_pos, n - 1);
									m = n - 1;
									break;
								}
								else if (n == max_pos) {
									SortScore_Time_Custom(4, score_pos, n);
									m = n;
									break;
								}
							}
						}
					}
					i = j;
					break;
				}
			}
		}
	}
}

void SortScore_Score(int mode) {
	int min, max;
	int min_pos, max_pos;
	int temp;
	int i, j;


	if (mode == 2 && score_Mode_2_Size > 0) {
		for (i = 0; i < score_Mode_2_Size; i++) {
			score_Mode_2_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_2_Size / 2; i++) {
			min = max = score_Mode_2[score_Mode_2_Sequence[score_Mode_2_Size / 2]].score;
			min_pos = max_pos = score_Mode_2_Size / 2;
			for (j = i; j < score_Mode_2_Size - i; j++) {
				if (score_Mode_2[score_Mode_2_Sequence[j]].score < min) {
					min = score_Mode_2[score_Mode_2_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_2[score_Mode_2_Sequence[j]].score > max) {
					max = score_Mode_2[score_Mode_2_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == i) {
				if (max_pos == score_Mode_2_Size - 1 - i) {
					temp = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_2_Sequence[score_Mode_2_Size - 1 - i];
					score_Mode_2_Sequence[score_Mode_2_Size - 1 - i] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;

					temp = score_Mode_2_Sequence[i];
					score_Mode_2_Sequence[i] = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_2_Sequence[i];
				score_Mode_2_Sequence[i] = score_Mode_2_Sequence[max_pos];
				score_Mode_2_Sequence[max_pos] = temp;

				temp = score_Mode_2_Sequence[score_Mode_2_Size - 1 - i];
				score_Mode_2_Sequence[score_Mode_2_Size - 1 - i] = score_Mode_2_Sequence[min_pos];
				score_Mode_2_Sequence[min_pos] = temp;
			}
		}
	}

	else if (mode == 3 && score_Mode_3_Size > 0) {
		for (i = 0; i < score_Mode_3_Size; i++) {
			score_Mode_3_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_3_Size / 2; i++) {
			min = max = score_Mode_3[score_Mode_3_Sequence[score_Mode_3_Size / 2]].score;
			min_pos = max_pos = score_Mode_3_Size / 2;
			for (j = i; j < score_Mode_3_Size - i; j++) {
				if (score_Mode_3[score_Mode_3_Sequence[j]].score < min) {
					min = score_Mode_3[score_Mode_3_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_3[score_Mode_3_Sequence[j]].score > max) {
					max = score_Mode_3[score_Mode_3_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == i) {
				if (max_pos == score_Mode_3_Size - 1 - i) {
					temp = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_3_Sequence[score_Mode_3_Size - 1 - i];
					score_Mode_3_Sequence[score_Mode_3_Size - 1 - i] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;

					temp = score_Mode_3_Sequence[i];
					score_Mode_3_Sequence[i] = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_3_Sequence[i];
				score_Mode_3_Sequence[i] = score_Mode_3_Sequence[max_pos];
				score_Mode_3_Sequence[max_pos] = temp;

				temp = score_Mode_3_Sequence[score_Mode_3_Size - 1 - i];
				score_Mode_3_Sequence[score_Mode_3_Size - 1 - i] = score_Mode_3_Sequence[min_pos];
				score_Mode_3_Sequence[min_pos] = temp;
			}
		}


		for (i = 0; i < score_Mode_3_Size - 1; i++) {
			min = score_Mode_3[score_Mode_3_Sequence[i]].score;
			min_pos = i;
			for (j = i + 1; j < score_Mode_3_Size; j++) {
				if (min != score_Mode_3[score_Mode_3_Sequence[j]].score) {
					SortScore_Time_Custom(3, min_pos, j - 1);
					i = j - 1;
					break;
				}
				else if (j == score_Mode_3_Size - 1) {
					SortScore_Time_Custom(3, min_pos, j);
					i = j;
					break;
				}
			}
		}
	}

	else if (mode == 4 && score_Mode_4_Size > 0) {
		for (i = 0; i < score_Mode_4_Size; i++) {
			score_Mode_4_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_4_Size / 2; i++) {
			min = max = score_Mode_4[score_Mode_4_Sequence[score_Mode_4_Size / 2]].score;
			min_pos = max_pos = score_Mode_4_Size / 2;
			for (j = i; j < score_Mode_4_Size - i; j++) {
				if (score_Mode_4[score_Mode_4_Sequence[j]].score < min) {
					min = score_Mode_4[score_Mode_4_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_4[score_Mode_4_Sequence[j]].score > max) {
					max = score_Mode_4[score_Mode_4_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == i) {
				if (max_pos == score_Mode_4_Size - 1 - i) {
					temp = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_4_Sequence[score_Mode_4_Size - 1 - i];
					score_Mode_4_Sequence[score_Mode_4_Size - 1 - i] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;

					temp = score_Mode_4_Sequence[i];
					score_Mode_4_Sequence[i] = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_4_Sequence[i];
				score_Mode_4_Sequence[i] = score_Mode_4_Sequence[max_pos];
				score_Mode_4_Sequence[max_pos] = temp;

				temp = score_Mode_4_Sequence[score_Mode_4_Size - 1 - i];
				score_Mode_4_Sequence[score_Mode_4_Size - 1 - i] = score_Mode_4_Sequence[min_pos];
				score_Mode_4_Sequence[min_pos] = temp;
			}
		}
	}
}

void SortScore_Score_Custom(int mode, int from, int to) {
	int min, max;
	int min_pos, max_pos;
	int temp;
	int i, j;
	int sortSize = to - from;


	if (mode == 2 && sortSize > 0) {
		for (i = 0; i < (sortSize + 1) / 2; i++) {
			min = max = score_Mode_2[score_Mode_2_Sequence[(from + to) / 2]].score;
			min_pos = max_pos = (from + to) / 2;
			for (j = from + i; j < to + 1 - i; j++) {
				if (score_Mode_2[score_Mode_2_Sequence[j]].score < min) {
					min = score_Mode_2[score_Mode_2_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_2[score_Mode_2_Sequence[j]].score > max) {
					max = score_Mode_2[score_Mode_2_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == from + i) {
				if (max_pos == to - i) {
					temp = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_2_Sequence[to - i];
					score_Mode_2_Sequence[to - i] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;

					temp = score_Mode_2_Sequence[from + i];
					score_Mode_2_Sequence[from + i] = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_2_Sequence[from + i];
				score_Mode_2_Sequence[from + i] = score_Mode_2_Sequence[max_pos];
				score_Mode_2_Sequence[max_pos] = temp;

				temp = score_Mode_2_Sequence[to - i];
				score_Mode_2_Sequence[to - i] = score_Mode_2_Sequence[min_pos];
				score_Mode_2_Sequence[min_pos] = temp;
			}
		}
	}

	else if (mode == 3 && sortSize > 0) {
		for (i = 0; i < (sortSize + 1) / 2; i++) {
			min = max = score_Mode_3[score_Mode_3_Sequence[(from + to) / 2]].score;
			min_pos = max_pos = (from + to) / 2;
			for (j = from + i; j < to + 1 - i; j++) {
				if (score_Mode_3[score_Mode_3_Sequence[j]].score < min) {
					min = score_Mode_3[score_Mode_3_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_3[score_Mode_2_Sequence[j]].score > max) {
					max = score_Mode_3[score_Mode_2_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == from + i) {
				if (max_pos == to - i) {
					temp = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_3_Sequence[to - i];
					score_Mode_3_Sequence[to - i] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;

					temp = score_Mode_3_Sequence[from + i];
					score_Mode_3_Sequence[from + i] = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_3_Sequence[from + i];
				score_Mode_3_Sequence[from + i] = score_Mode_3_Sequence[max_pos];
				score_Mode_3_Sequence[max_pos] = temp;

				temp = score_Mode_3_Sequence[to - i];
				score_Mode_3_Sequence[to - i] = score_Mode_3_Sequence[min_pos];
				score_Mode_3_Sequence[min_pos] = temp;
			}
		}
	}

	else if (mode == 4 && sortSize > 0) {
		for (i = 0; i < (sortSize + 1) / 2; i++) {
			min = max = score_Mode_4[score_Mode_4_Sequence[(from + to) / 2]].score;
			min_pos = max_pos = (from + to) / 2;
			for (j = from + i; j < to + 1 - i; j++) {
				if (score_Mode_4[score_Mode_4_Sequence[j]].score < min) {
					min = score_Mode_4[score_Mode_4_Sequence[j]].score;
					min_pos = j;
				}
				else if (score_Mode_4[score_Mode_4_Sequence[j]].score > max) {
					max = score_Mode_4[score_Mode_4_Sequence[j]].score;
					max_pos = j;
				}
			}
			if (min_pos == from + i) {
				if (max_pos == to - i) {
					temp = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_4_Sequence[to - i];
					score_Mode_4_Sequence[to - i] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;

					temp = score_Mode_4_Sequence[from + i];
					score_Mode_4_Sequence[from + i] = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_4_Sequence[from + i];
				score_Mode_4_Sequence[from + i] = score_Mode_4_Sequence[max_pos];
				score_Mode_4_Sequence[max_pos] = temp;

				temp = score_Mode_4_Sequence[to - i];
				score_Mode_4_Sequence[to - i] = score_Mode_4_Sequence[min_pos];
				score_Mode_4_Sequence[min_pos] = temp;
			}
		}
	}
}

void SortScore_Time(int mode) {
	float min, max;
	int min_pos, max_pos;
	int temp;
	int i, j;


	if (mode == 2 && score_Mode_2_Size > 0) {
		for (i = 0; i < score_Mode_2_Size; i++) {
			score_Mode_2_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_2_Size / 2; i++) {
			min = max = score_Mode_2[score_Mode_2_Sequence[score_Mode_2_Size / 2]].time;
			min_pos = max_pos = score_Mode_2_Size / 2;
			for (j = i; j < score_Mode_2_Size - i; j++) {
				if (score_Mode_2[score_Mode_2_Sequence[j]].time < min) {
					min = score_Mode_2[score_Mode_2_Sequence[j]].time;
					min_pos = j;
				}
				else if (score_Mode_2[score_Mode_2_Sequence[j]].time > max) {
					max = score_Mode_2[score_Mode_2_Sequence[j]].time;
					max_pos = j;
				}
			}
			if (max_pos == i) {
				if (min_pos == score_Mode_2_Size - 1 - i) {
					temp = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = temp;
				}
				else {
					temp = score_Mode_2_Sequence[score_Mode_2_Size - 1 - i];
					score_Mode_2_Sequence[score_Mode_2_Size - 1 - i] = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = temp;

					temp = score_Mode_2_Sequence[i];
					score_Mode_2_Sequence[i] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;
				}
			}
			else {
				temp = score_Mode_2_Sequence[i];
				score_Mode_2_Sequence[i] = score_Mode_2_Sequence[min_pos];
				score_Mode_2_Sequence[min_pos] = temp;

				temp = score_Mode_2_Sequence[score_Mode_2_Size - 1 - i];
				score_Mode_2_Sequence[score_Mode_2_Size - 1 - i] = score_Mode_2_Sequence[max_pos];
				score_Mode_2_Sequence[max_pos] = temp;
			}
		}
	}

	else if (mode == 3 && score_Mode_3_Size > 0) {
		for (i = 0; i < score_Mode_3_Size; i++) {
			score_Mode_3_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_3_Size / 2; i++) {
			min = max = score_Mode_3[score_Mode_3_Sequence[score_Mode_3_Size / 2]].time;
			min_pos = max_pos = score_Mode_3_Size / 2;
			for (j = i; j < score_Mode_3_Size - i; j++) {
				if (score_Mode_3[score_Mode_3_Sequence[j]].time < min) {
					min = score_Mode_3[score_Mode_3_Sequence[j]].time;
					min_pos = j;
				}
				else if (score_Mode_3[score_Mode_3_Sequence[j]].time > max) {
					max = score_Mode_3[score_Mode_3_Sequence[j]].time;
					max_pos = j;
				}
			}
			if (min_pos == i) {
				if (max_pos == score_Mode_3_Size - 1 - i) {
					temp = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_3_Sequence[score_Mode_3_Size - 1 - i];
					score_Mode_3_Sequence[score_Mode_3_Size - 1 - i] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;

					temp = score_Mode_3_Sequence[i];
					score_Mode_3_Sequence[i] = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_3_Sequence[i];
				score_Mode_3_Sequence[i] = score_Mode_3_Sequence[max_pos];
				score_Mode_3_Sequence[max_pos] = temp;

				temp = score_Mode_3_Sequence[score_Mode_3_Size - 1 - i];
				score_Mode_3_Sequence[score_Mode_3_Size - 1 - i] = score_Mode_3_Sequence[min_pos];
				score_Mode_3_Sequence[min_pos] = temp;
			}
		}

		for (i = 0; i < score_Mode_3_Size - 1; i++) {
			min = score_Mode_3[score_Mode_3_Sequence[i]].time;
			min_pos = i;
			for (j = i + 1; j < score_Mode_3_Size; j++) {
				if (min != score_Mode_3[score_Mode_3_Sequence[j]].time) {
					SortScore_Score_Custom(3, min_pos, j - 1);
					i = j - 1;
					break;
				}
				else if (j == score_Mode_3_Size - 1) {
					SortScore_Score_Custom(3, min_pos, j);
					i = j;
					break;
				}
			}
		}
	}

	else if (mode == 4 && score_Mode_4_Size > 0) {
		for (i = 0; i < score_Mode_4_Size; i++) {
			score_Mode_4_Sequence[i] = i;
		}

		for (i = 0; i < score_Mode_4_Size / 2; i++) {
			min = max = score_Mode_4[score_Mode_4_Sequence[score_Mode_4_Size / 2]].time;
			min_pos = max_pos = score_Mode_4_Size / 2;
			for (j = i; j < score_Mode_4_Size - i; j++) {
				if (score_Mode_4[score_Mode_4_Sequence[j]].time < min) {
					min = score_Mode_4[score_Mode_4_Sequence[j]].time;
					min_pos = j;
				}
				else if (score_Mode_4[score_Mode_4_Sequence[j]].time > max) {
					max = score_Mode_4[score_Mode_4_Sequence[j]].time;
					max_pos = j;
				}
			}
			if (min_pos == i) {
				if (max_pos == score_Mode_4_Size - 1 - i) {
					temp = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_4_Sequence[score_Mode_4_Size - 1 - i];
					score_Mode_4_Sequence[score_Mode_4_Size - 1 - i] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;

					temp = score_Mode_4_Sequence[i];
					score_Mode_4_Sequence[i] = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_4_Sequence[i];
				score_Mode_4_Sequence[i] = score_Mode_4_Sequence[max_pos];
				score_Mode_4_Sequence[max_pos] = temp;

				temp = score_Mode_4_Sequence[score_Mode_4_Size - 1 - i];
				score_Mode_4_Sequence[score_Mode_4_Size - 1 - i] = score_Mode_4_Sequence[min_pos];
				score_Mode_4_Sequence[min_pos] = temp;
			}
		}
	}
}

void SortScore_Time_Custom(int mode, int from, int to) {
	float min, max;
	int min_pos, max_pos;
	int temp;
	int i, j;
	int sortSize = to - from;


	if (mode == 2 && sortSize > 0) {
		for (i = 0; i < (sortSize + 1) / 2; i++) {
			min = max = score_Mode_2[score_Mode_2_Sequence[(from + to) / 2]].time;
			min_pos = max_pos = (from + to) / 2;
			for (j = from + i; j < to + 1 - i; j++) {
				if (score_Mode_2[score_Mode_2_Sequence[j]].time < min) {
					min = score_Mode_2[score_Mode_2_Sequence[j]].time;
					min_pos = j;
				}
				else if (score_Mode_2[score_Mode_2_Sequence[j]].time > max) {
					max = score_Mode_2[score_Mode_2_Sequence[j]].time;
					max_pos = j;
				}
			}
			if (max_pos == from + i) {
				if (min_pos == to - i) {
					temp = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = temp;
				}
				else {
					temp = score_Mode_2_Sequence[to - i];
					score_Mode_2_Sequence[to - i] = score_Mode_2_Sequence[max_pos];
					score_Mode_2_Sequence[max_pos] = temp;

					temp = score_Mode_2_Sequence[from + i];
					score_Mode_2_Sequence[from + i] = score_Mode_2_Sequence[min_pos];
					score_Mode_2_Sequence[min_pos] = temp;
				}
			}
			else {
				temp = score_Mode_2_Sequence[from + i];
				score_Mode_2_Sequence[from + i] = score_Mode_2_Sequence[min_pos];
				score_Mode_2_Sequence[min_pos] = temp;

				temp = score_Mode_2_Sequence[to - i];
				score_Mode_2_Sequence[to - i] = score_Mode_2_Sequence[max_pos];
				score_Mode_2_Sequence[max_pos] = temp;
			}
		}
	}

	else if (mode == 3 && sortSize > 0) {
		for (i = 0; i < (sortSize + 1) / 2; i++) {
			min = max = score_Mode_3[score_Mode_3_Sequence[(from + to) / 2]].time;
			min_pos = max_pos = (from + to) / 2;
			for (j = from + i; j < to + 1 - i; j++) {
				if (score_Mode_3[score_Mode_3_Sequence[j]].time < min) {
					min = score_Mode_3[score_Mode_3_Sequence[j]].time;
					min_pos = j;
				}
				else if (score_Mode_3[score_Mode_3_Sequence[j]].time > max) {
					max = score_Mode_3[score_Mode_3_Sequence[j]].time;
					max_pos = j;
				}
			}
			if (min_pos == from + i) {
				if (max_pos == to - i) {
					temp = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_3_Sequence[to - i];
					score_Mode_3_Sequence[to - i] = score_Mode_3_Sequence[min_pos];
					score_Mode_3_Sequence[min_pos] = temp;

					temp = score_Mode_3_Sequence[from + i];
					score_Mode_3_Sequence[from + i] = score_Mode_3_Sequence[max_pos];
					score_Mode_3_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_3_Sequence[from + i];
				score_Mode_3_Sequence[from + i] = score_Mode_3_Sequence[max_pos];
				score_Mode_3_Sequence[max_pos] = temp;

				temp = score_Mode_3_Sequence[to - i];
				score_Mode_3_Sequence[to - i] = score_Mode_3_Sequence[min_pos];
				score_Mode_3_Sequence[min_pos] = temp;
			}
		}
	}

	else if (mode == 4 && sortSize > 0) {
		for (i = 0; i < (sortSize + 1) / 2; i++) {
			min = max = score_Mode_4[score_Mode_4_Sequence[(from + to) / 2]].time;
			min_pos = max_pos = (from + to) / 2;
			for (j = from + i; j < to + 1 - i; j++) {
				if (score_Mode_4[score_Mode_4_Sequence[j]].time < min) {
					min = score_Mode_4[score_Mode_4_Sequence[j]].time;
					min_pos = j;
				}
				else if (score_Mode_4[score_Mode_4_Sequence[j]].time > max) {
					max = score_Mode_4[score_Mode_4_Sequence[j]].time;
					max_pos = j;
				}
			}
			if (min_pos == from + i) {
				if (max_pos == to - i) {
					temp = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;
				}
				else {
					temp = score_Mode_4_Sequence[to - i];
					score_Mode_4_Sequence[to - i] = score_Mode_4_Sequence[min_pos];
					score_Mode_4_Sequence[min_pos] = temp;

					temp = score_Mode_4_Sequence[from + i];
					score_Mode_4_Sequence[from + i] = score_Mode_4_Sequence[max_pos];
					score_Mode_4_Sequence[max_pos] = temp;
				}
			}
			else {
				temp = score_Mode_4_Sequence[from + i];
				score_Mode_4_Sequence[from + i] = score_Mode_4_Sequence[max_pos];
				score_Mode_4_Sequence[max_pos] = temp;

				temp = score_Mode_4_Sequence[to - i];
				score_Mode_4_Sequence[to - i] = score_Mode_4_Sequence[min_pos];
				score_Mode_4_Sequence[min_pos] = temp;
			}
		}
	}
}

//

// Ranking

int SetCutSize() {
	return (WIN_HEIGHT - 200) / 40;
}

//

// Image

void InitImage() {
	image_background_menu = createBitmap("image/background/background_main.png");
	image_background_game = createBitmap("image/background/background_game.png");

	image_title = createBitmap("image/background/title.png");
	image_menu_start = createBitmap("image/button/button_start.png");
	image_menu_ranking = createBitmap("image/button/button_ranking.png");
	image_menu_option = createBitmap("image/button/button_option.png");
	image_menu_exit = createBitmap("image/button/button_exit.png");

	//image_select_mode_1 = createBitmap("image/button/button_story.png");
	image_select_mode_2 = createBitmap("image/button/button_allkill.png");
	image_select_mode_3 = createBitmap("image/button/button_infinite.png");
	image_select_mode_4 = createBitmap("image/button/button_boss.png");
	image_select_mode_4_select = createBitmap("image/button/button_boss_select.png");
	image_select_exit = createBitmap("image/button/button_back.png");

	image_ranking_allkill = createBitmap("image/ranking/allkill.png");
	image_ranking_infinite = createBitmap("image/ranking/infinite.png");
	image_ranking_boss = createBitmap("image/ranking/boss.png");
	image_ranking_rank = createBitmap("image/ranking/rank.png");
	image_ranking_date = createBitmap("image/ranking/date.png");
	image_ranking_stage = createBitmap("image/ranking/stage.png");
	image_ranking_score = createBitmap("image/ranking/score.png");
	image_ranking_time = createBitmap("image/ranking/time.png");

	image_minimapType_1 = createBitmap("image/button/minimap_balanced.png");
	image_minimapType_2 = createBitmap("image/button/minimap_small.png");
	image_minimapType_3 = createBitmap("image/button/minimap_default.png");
	image_minimapType_4 = createBitmap("image/button/minimap_big.png");
	image_explanation = createBitmap("image/button/button_explanation.png");
	image_explanation_image = createBitmap("image/background/image_explanation.png");
	image_option_exit = createBitmap("image/button/button_back.png");

	image_0 = createBitmap("image/number/0.png");
	image_1 = createBitmap("image/number/1.png");
	image_2= createBitmap("image/number/2.png");
	image_3 = createBitmap("image/number/3.png");
	image_4 = createBitmap("image/number/4.png");
	image_5 = createBitmap("image/number/5.png");
	image_6 = createBitmap("image/number/6.png");
	image_7 = createBitmap("image/number/7.png");
	image_8 = createBitmap("image/number/8.png");
	image_9 = createBitmap("image/number/9.png");

	image_mode_2_stage_clear = createBitmap("image/background/stage_clear.png");
	image_game_over = createBitmap("image/background/game_over.png");
	image_press_enter = createBitmap("image/background/press_enter.png");

	image_player1 = createBitmap("image/mob/hero_right.png");
	image_player2 = createBitmap("image/mob/hero_left.png");
	image_pet = createBitmap("image/particle/particle2.png");
	image_enemy1 = createBitmap("image/mob/slime_normal.png");
	image_enemy2 = createBitmap("image/mob/slime_health.png");
	image_enemy3 = createBitmap("image/mob/slime_fast.png");
	image_enemy4 = createBitmap("image/mob/slime_boom.png");
	image_enemy5 = createBitmap("image/mob/slime_invisible.png");
	image_enemy6 = createBitmap("image/mob/slime_invincible.png");
	image_boss = createBitmap("image/mob/boss.png");
	image_playerBullet = createBitmap("image/bullet/bullet_hero.png");
	image_petBullet = createBitmap("image/particle/particle3.png");
	image_particle = createBitmap("image/particle/particle.png");
}

void DestroyImage() {
	destroyBitmap(image_background_menu);
	destroyBitmap(image_background_game);

	destroyBitmap(image_title);
	destroyBitmap(image_menu_start);
	destroyBitmap(image_menu_ranking);
	destroyBitmap(image_menu_option);
	destroyBitmap(image_menu_exit);

	//destroyBitmap(image_select_mode_1);
	destroyBitmap(image_select_mode_2);
	destroyBitmap(image_select_mode_3);
	destroyBitmap(image_select_mode_4);
	destroyBitmap(image_select_mode_4_select);
	destroyBitmap(image_select_exit);

	destroyBitmap(image_ranking_allkill);
	destroyBitmap(image_ranking_infinite);
	destroyBitmap(image_ranking_boss);
	destroyBitmap(image_ranking_rank);
	destroyBitmap(image_ranking_date);
	destroyBitmap(image_ranking_stage);
	destroyBitmap(image_ranking_score);
	destroyBitmap(image_ranking_time);

	destroyBitmap(image_minimapType_1);
	destroyBitmap(image_minimapType_2);
	destroyBitmap(image_minimapType_3);
	destroyBitmap(image_minimapType_4);
	destroyBitmap(image_explanation);
	destroyBitmap(image_explanation_image);
	destroyBitmap(image_option_exit);

	destroyBitmap(image_0);
	destroyBitmap(image_1);
	destroyBitmap(image_2);
	destroyBitmap(image_3);
	destroyBitmap(image_4);
	destroyBitmap(image_5);
	destroyBitmap(image_6);
	destroyBitmap(image_7);
	destroyBitmap(image_8);
	destroyBitmap(image_9);

	destroyBitmap(image_mode_2_stage_clear);
	destroyBitmap(image_game_over);
	destroyBitmap(image_press_enter);

	destroyBitmap(image_player1);
	destroyBitmap(image_player2);
	destroyBitmap(image_pet);
	destroyBitmap(image_enemy1);
	destroyBitmap(image_enemy2);
	destroyBitmap(image_enemy3);
	destroyBitmap(image_enemy4);
	destroyBitmap(image_enemy5);
	destroyBitmap(image_enemy6);
	destroyBitmap(image_boss);
	destroyBitmap(image_playerBullet);
	destroyBitmap(image_petBullet);
	destroyBitmap(image_particle);

	image_background_menu = NULL;
	image_background_game = NULL;

	image_title = NULL;
	image_menu_start = NULL;
	image_menu_ranking = NULL;
	image_menu_option = NULL;
	image_menu_exit = NULL;

	//image_select_mode_1 = NULL;
	image_select_mode_2 = NULL;
	image_select_mode_3 = NULL;
	image_select_mode_4 = NULL;
	image_select_mode_4_select = NULL;
	image_select_exit = NULL;

	image_ranking_allkill = NULL;
	image_ranking_infinite = NULL;
	image_ranking_boss = NULL;
	image_ranking_rank = NULL;
	image_ranking_date = NULL;
	image_ranking_stage = NULL;
	image_ranking_score = NULL;
	image_ranking_time = NULL;

	image_minimapType_1 = NULL;
	image_minimapType_2 = NULL;
	image_minimapType_3 = NULL;
	image_minimapType_4 = NULL;
	image_explanation = NULL;
	image_explanation_image = NULL;
	image_option_exit = NULL;

	image_0 = NULL;
	image_1 = NULL;
	image_2 = NULL;
	image_3 = NULL;
	image_4 = NULL;
	image_5 = NULL;
	image_6 = NULL;
	image_7 = NULL;
	image_8 = NULL;
	image_9 = NULL;

	image_mode_2_stage_clear = NULL;
	image_game_over = NULL;
	image_press_enter = NULL;

	image_player1 = NULL;
	image_player2 = NULL;
	image_pet = NULL;
	image_enemy1 = NULL;
	image_enemy2 = NULL;
	image_enemy3 = NULL;
	image_enemy4 = NULL;
	image_enemy5 = NULL;
	image_enemy6 = NULL;
	image_boss = NULL;
	image_playerBullet = NULL;
	image_petBullet = NULL;
	image_particle = NULL;
}

//
