#pragma once

class Game
{
	private: 
		
		Vector2 smallRectPoints[4], 
				bigRectPoints[2],
				rect1Points[2],
				rect2Points[2],
				rectPoints[4]; 
		SDL_Event event;
		
		SDL_Window * window;
		SDL_Renderer * renderer;

		SDL_Surface* surface;
		SDL_Texture* menuTexture;
		SDL_Texture* sprites;
		

SDL_Rect srcMenuRect, srcDotRect, dstDotRect1, dstDotRect2,
		healthSrcRect = {244, 221, 7, 6},
		eSourceRect = {103, 147, 11, 11},
		healthAnimSrc[5] = {
			{244, 221, 7, 6},
			{243, 220, 9, 7},
			{242, 219, 11, 9},
			{241, 218, 13, 11},
			{240, 217, 15, 13}},
		healthAnimDst[5] = {
			{93 * 5, 45 * 5, 14 * 5, 12 * 5},
			{91 * 5, 43 * 5, 18 * 5, 14 * 5},
			{89 * 5, 41 * 5, 22 * 5, 18 * 5},
			{87 * 5, 39 * 5, 26 * 5, 22 * 5},
			{85 * 5, 37 * 5, 30 * 5, 26 * 5}},
		gunAnimSrc[6] = {
			{244, 235, 21, 11}, 
			{243, 234, 23, 13},
			{242, 233, 25, 15},
			{241, 232, 27, 17}, 
			{240, 231, 29, 19},
			{239, 230, 31, 21}},
		gunAnimDst[6] = {
			{79 * 5, 39 * 5, 21 * 10, 11  * 10}, 
			{77 * 5, 37 * 5, 23 * 10, 13 * 10},
			{75 * 5, 35 * 5, 25 * 10, 15 * 10},
			{73 * 5, 33 * 5, 27 * 10, 17 * 10}, 
			{71 * 5, 31 * 5, 29 * 10, 19 * 10},
			{69 * 5, 29 * 5, 31 * 10, 21 * 10}};

Vector2 enterCollisionPoint = {727, 305}, newAreaCpoint = {700, 180}, 
		openCaseCpoints[2] = {{355, 215}, {770, 175}};
		
		int action = 0;
		//action meaning: 0 - nothing, 1 - enter the door; 2 - new area; 3 - open case;
		
		SDL_Rect testRect = {250, 250, 100, 50};

		int showCounter;

		Animation animations[2];
		Animation cutscene[6], cutscene2[2];

		int isCaseOpened[2] = {false, false};
		int isAreaOpened = false;
		
		Player player;
		Gun gun;
		bool isGunExist[5] = {false, false, false, false, false};
		Bullet bullets[15]; 
		Bullet eBullets[15];
		int bulletsNumber = 15, bulletSpawnNumber = 0, eBulletSpawnNumber = 0;

		int mapNow;

		int enemiesNumber;
		int enemiesKilled = 0;
		int enemiesHp[4][3] = {{5, 5, 5}, {5, 5, 5}, {5, 5, 5}, {5, 5, 5}};
		int enemiesExist[4][3] = {{true, true, true}, {true, true, true}, {true, true, true}, {true, true, true}};
		int enemiesX[4][3] = {{610, 450, 0}, {330, 0, 0}, {450, 0, 0}, {300, 750, 0}};
		int enemiesXvel[4][3] = {{1, -1, 1}, {1, -1, 1}, {1, -1, 1}, {-1, 1, 1}};
		Enemy enemies[3]; //= {Enemy(renderer, 100, 600, 700), Enemy(renderer, 300, 400, 500), Enemy(renderer, 200, 600, 700)};

		struct Texture {SDL_Rect srcRect; SDL_Rect dstRect; 
						int add = 0; bool isOnGround = false; int lowY = dstRect.y + dstRect.h + add;};
						//SDL_Rect collisionRect = {dstRect.x + 3, dstRect.y + dstRect.h - 6, dstRect.w - 6, 6};};
		Texture textures[12];
		int texturesNumber;
		int stonesNumber;
		SDL_Rect colliders[6];
		int collidersNumber;

		int mouseX, mouseY;
		
		const int frameDelay = 1000 / 60;
		Uint32 frameStart;
		Uint32 st = 0;
		int frameTime;

		int check;

		//audio

		Mix_Music *musicmp3;
		Mix_Chunk *shot, *getHp, *getGun, *openChest;

		void move();

		int collisionCheck();

		int rectsCollision(SDL_Rect& rect1, SDL_Rect& rect2);

		int rectPointCollision(SDL_Rect& rect, Vector2& point);

		int circlePointCollision(Circle& circle, Vector2& point);

		int rectCircleCollision (SDL_Rect& rect, Circle& circle);

		void draw();

		int playCutscene();

		int playCutscene2();

		int map1();

		int map2();

		int map3();

		int map4();

		int map5();

		int map6();

		void prepareMap();

		void destroyBullets();

		void restoreEnemiesInfo(int map);

		void saveEnemiesInfo(int map);

		void restart();

		int end();

		int quit();

		void fpsRestriction();


	public:

		Game();

		int start();
};