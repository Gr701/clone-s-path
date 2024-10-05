#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "headers/log.h"
#include "headers/player.h"
#include "headers/enemy.h"
#include "headers/gun.h"
#include "headers/animation.h"
#include "headers/game.h"
#include <cmath>

Game::Game()
{
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {Log("Initialize error");}
	Mix_Init(MIX_INIT_MP3);
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("Clone's path", 100, 100, 1000, 500, SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = SDL_CreateRenderer(window, -1, 0);
	//surface = SDL_LoadBMP("assets/menu.bmp");
	surface = IMG_Load("assets/sprites/menu.png");
	menuTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	//surface = SDL_LoadBMP("assets/all.bmp");
	surface = IMG_Load("assets/sprites/all.png");
	sprites = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	player.render(renderer, sprites);
	gun.render(renderer, sprites, 151, 160);

	for (int i = 0; i < 3; i++)
	{
		enemies[i].render(renderer, sprites);
	}
	//audio
	Mix_OpenAudio(48000, AUDIO_S16SYS, 1, 2048);
	musicmp3 = Mix_LoadMUS("assets/sounds/swbf.mp3");
	shot = Mix_LoadWAV("assets/sounds/shot.wav");
	getHp = Mix_LoadWAV("assets/sounds/gethp.wav");
	getGun = Mix_LoadWAV("assets/sounds/getgun.wav");
	openChest = Mix_LoadWAV("assets/sounds/chest.wav");


	//animations
	animations[0].create(renderer, sprites, healthAnimSrc, healthAnimDst, 5, 3, 1); //hp get
	animations[1].create(renderer, sprites, gunAnimSrc, gunAnimDst, 6, 5, 1); //gun get
}

void Game::move()
{
	//SOUND---------------------------------------------------
	//if (SDL_GetQueuedAudioSize(auDev) < 500000) {SDL_QueueAudio(auDev, music.b, music.l);}
	//PLAYER---------------------------------------------------
	int a = player.move(event);
	if(a == 1 && !isGunExist[mapNow - 1])
	{ 
		gun.spawn(player.x, player.y, player.isRight);
		isGunExist[mapNow - 1] = true;
	}
	//SHOOTING--------------------------------------------------
	for (int i = 0; i < bulletsNumber; i++)
	{
		if (bullets[i].isExist)
		{
			bullets[i].move();
		}
	}
	player.shootCounter++;
	if (player.kState[SDL_SCANCODE_SPACE] && player.isArmed && player.shootCounter >= 20)
	{
		player.shootCounter = 0;
		if (player.isStay)
		{
			if (player.isRight)
			{
				bullets[bulletSpawnNumber].spawn(renderer, player.x + 51, player.y + 42, 18, true, 12, 3);
			}
			else
			{
				bullets[bulletSpawnNumber].spawn(renderer, player.x - 9 - 12, player.y + 42, 18, false, 12, 3);
			}
		}
		else
		{
			if (player.isRight)
			{
				bullets[bulletSpawnNumber].spawn(renderer, player.x + 75, player.y + 33, 18, true, 12, 3);
			}
			else
			{
				bullets[bulletSpawnNumber].spawn(renderer, player.x - 30 - 12, player.y + 33, 18, false, 12, 3);
			}
		}
		bulletSpawnNumber++;
		Mix_PlayChannel(-1, shot, 0);
		if (bulletSpawnNumber >= bulletsNumber)
		{
			bulletSpawnNumber = 0;
		}
	}
	//ENEMIES-------------------------------------------------
	for (int i = 0; i < enemiesNumber; i++) 
	{
		if (enemies[i].isExist)
		{
			if(enemies[i].isMoving) 
			{
				enemies[i].move();
			}
			else
			{
				enemies[i].shootCounter++;

				int xDis = player.cPoint.x - enemies[i].fPoint.x;
				int yDis = player.cPoint.y - enemies[i].fPoint.y;
				int times = sqrt((xDis)*(xDis) + (yDis)*(yDis)) / 18;

				if (times == 0) {times = 1;}

				if (enemies[i].shootCounter >= 30) 
				{
					enemies[i].shootCounter = 0;
					eBullets[eBulletSpawnNumber].eSpawn(renderer, enemies[i].fPoint.x, enemies[i].fPoint.y, xDis/times, yDis/times, 12, 3);
					eBulletSpawnNumber++;
					Mix_PlayChannel(-1, shot, 0);
					if (eBulletSpawnNumber >= 15)
					{
						eBulletSpawnNumber = 0;
					}
				}
			}
		}
	}
	for (int i = 0; i < 15; i++)
	{
		if (eBullets[i].isExist) {eBullets[i].eMove();}
	}
}

int Game::collisionCheck()
{
	//SDL_GetMouseState(&mouseX, &mouseY); Logii(mouseX / 5, mouseY / 5);
	
	//enemy collision-------------------------------------------------
	for (int i = 0; i < enemiesNumber; i++)
	{
		if (enemies[i].isExist) 
		{
			if(rectsCollision(player.collisionRect, enemies[i].collisionRect))
			{
				return end();
			}
			for (int j = 0; j < bulletsNumber; j++)
			{
				if (bullets[j].isExist)
				{
					if (rectPointCollision(enemies[i].collisionRect, bullets[j].cPoint))
					{
						bullets[j].isExist = false;
						enemies[i].hp--;
						if (player.x > enemies[i].x) {enemies[i].xVel = 4;} else {enemies[i].xVel = -4;}
						if (enemies[i].hp <= 0) {enemies[i].isExist = false; enemiesKilled++; if (enemiesKilled == 5) {return playCutscene2();}}
					}
				}
			}
			//enemy Vision 
			if(circlePointCollision(enemies[i].visionCircle, player.cPoint))
			{
				int xDis = player.cPoint.x - enemies[i].fPoint.x;
				int yDis = player.cPoint.y - enemies[i].fPoint.y;
				if (abs(xDis / yDis) >= 2)
				{
					if (enemies[i].xVel < 0 && xDis < 0 || enemies[i].xVel > 0 && xDis > 0)
					{
						//enemies[i].xVel /= 4;
						enemies[i].isMoving = false;
					}
					//if (player.x - enemies[i].x < 0) {enemies[i].xVel = -1;} else {enemies[i].xVel = 1;}
				}
				else 
				{
					enemies[i].isMoving = true;
					//enemies[i].shootCounter = 30;
				}		
			}
			else
			{
				enemies[i].isMoving = true;
			}
		}
	}
	//COLLIDERS----------------------------------------------------------
	for (int i = 0; i < collidersNumber; i++)
	{
		if (rectsCollision(player.legCollisionRect, colliders[i]))
		{
			if ((player.legCollisionRect.y + player.legCollisionRect.h - player.y + player.prevY < colliders[i].y) || 
					(player.legCollisionRect.y - player.y + player.prevY > colliders[i].y + colliders[i].h))
			{
				player.y = player.prevY;
			}
			else if ((player.legCollisionRect.x + player.legCollisionRect.w - player.x + player.prevX < colliders[i].x) || 
				(player.legCollisionRect.x - player.x + player.prevX > colliders[i].x + colliders[i].w))
			{
				player.x = player.prevX;
			}
		}
	}
	//gun collision------------------------------------------------------
	if (isGunExist[mapNow - 1])
	{
		if (rectsCollision(gun.collisionRect, player.collisionRect))
		{
			isGunExist[mapNow - 1] = false;
			player.changeGun();
		}
	}
	//BULLETS------------------------------------------------------------
	for (int i = 0; i < bulletsNumber; i++)
	{
		if (bullets[i].isExist)
		{
			if (bullets[i].x < -50 || bullets[i].x > 1050)
			{
				bullets[i].isExist = false;
			}
		}
		if (eBullets[i].isExist)
		{
			if (rectPointCollision(player.collisionRect, eBullets[i].cPoint))
			{
				eBullets[i].isExist = false;
				player.hp--;
				if (player.hp <= 0) {return end();}
			}		
			else if (eBullets[i].x < -50 || eBullets[i].x > 1050)
			{
				eBullets[i].isExist = false;
			}
		}
	}
	//actions--------------------------------------------------------------
	if (rectPointCollision(player.collisionRect, enterCollisionPoint)) {action = 1;} else {action = 0;}
	if (rectPointCollision(player.collisionRect, newAreaCpoint)) {action = 2; newAreaCpoint = {10, 10};} 
	for (int i = 0; i < 2; i++)
	{
		if (rectPointCollision(player.collisionRect, openCaseCpoints[i])) {action = 3;}
	}
	
	return 1;
}

int Game::rectsCollision(SDL_Rect& rect1, SDL_Rect& rect2)
{
	rect1Points[0] = {rect1.x, rect1.y};
	rect1Points[1] = {rect1.x + rect1.w, rect1.y + rect1.h};

	rect2Points[0] = {rect2.x, rect2.y + rect2.h};
	rect2Points[1] = {rect2.x + rect2.w, rect2.y};

	for (int i = 0; i < 2; i++)
	{
		if (rect1Points[i].x >= rect2Points[0].x && rect1Points[i].x <= rect2Points[1].x && rect1Points[i].y <= rect2Points[0].y && rect1Points[i].y >= rect2Points[1].y)
		{
			return 1;
		}
		if (rect2Points[i].x >= rect1Points[0].x && rect2Points[i].x <= rect1Points[1].x && rect2Points[i].y >= rect1Points[0].y && rect2Points[i].y <= rect1Points[1].y)
		{
			return 1;
		}
	}
	return 0;
}

int Game::rectPointCollision(SDL_Rect& rect, Vector2& point)
{
	if (point.x >= rect.x && point.x <= rect.x + rect.w && point.y <= rect.y + rect.h && point.y >= rect.y)
	{
		return 1;
	} 
	return 0;
}

int Game::circlePointCollision(Circle& circle, Vector2& point)
{
	if (sqrt((point.x - circle.x)*(point.x - circle.x) + 
	(point.y - circle.y)*(point.y - circle.y)) < circle.r)
	{
		return 1;
	}
	return 0;
}

int Game::rectCircleCollision (SDL_Rect& rect, Circle& circle)
{
	rectPoints[0] = {rect.x, rect.y};
	rectPoints[1] = {rect.x, rect.y + rect.h};
	rectPoints[2] = {rect.x + rect.w, rect.y};
	rectPoints[3] = {rect.x + rect.w, rect.y + rect.h};

	for (int i = 0; i < 4; i++)
	{
		if (sqrt((rectPoints[i].x - circle.x)*(rectPoints[i].x - circle.x) + 
		(rectPoints[i].y - circle.y)*(rectPoints[i].y - circle.y)) < circle.r)
		{
			return 1;
		}
	}
	return 0;
}

void Game::draw()
{
	//bg
	if (mapNow == 5)
	{
		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	}
	else
	{
		SDL_SetRenderDrawColor(renderer, 154, 126, 89, 255);
	}
	SDL_RenderClear(renderer);
	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//
	//VERY LOW TEXTURES---------------------------------------
	for (int i = 0; i < texturesNumber; i++)
	{
		if (textures[i].isOnGround)
		{
			SDL_RenderCopy(renderer, sprites, &textures[i].srcRect, &textures[i].dstRect);
			//SDL_RenderDrawRect(renderer, &textures[i].collisionRect);// 
		}
	}
	//GUN------------------------------------------------------
	if (isGunExist[mapNow - 1]) {gun.draw();}
	//low textures-----------------------------------------------
	for (int i = 0; i < texturesNumber; i++)
	{
		if (player.y + 90 >= textures[i].lowY && !textures[i].isOnGround) 
		{
			SDL_RenderCopy(renderer, sprites, &textures[i].srcRect, &textures[i].dstRect);
			//SDL_RenderDrawRect(renderer, &textures[i].collisionRect);// 
		}
	}
	//enemies-----------------------------------------------------
	for (int i = 0; i < enemiesNumber; i++) 
	{
		//enemies[i].draw();
		if (enemies[i].isExist) {enemies[i].drawWgun();}
		//SDL_RenderDrawRect(renderer, &enemies[i].collisionRect);//
	}
	//player------------------------------------------------
	//SDL_RenderDrawRect(renderer, &testRect);
	player.draw();
	//SDL_RenderDrawRect(renderer, &player.collisionRect);//
	//BULLETS-------------------------------------------------------
	for (int i = 0; i < bulletsNumber; i++)
	{
		if (bullets[i].isExist) {bullets[i].draw();}
		if (eBullets[i].isExist) {eBullets[i].draw();}
	}
	//high textures------------------------------------------------- 
	//Logi(texturesNumber);
	for(int i = 0; i < texturesNumber; i ++)
	{
		if (player.y + 90 < textures[i].lowY && !textures[i].isOnGround)
		{
			SDL_RenderCopy(renderer, sprites, &textures[i].srcRect, &textures[i].dstRect); 
			//SDL_RenderDrawRect(renderer, &textures[i].collisionRect);//
		}
	}
	//colliders-----------------------------------------------------
	//for (int i = 0; i < collidersNumber; i++)
	//{
	//	SDL_RenderDrawRect(renderer, &colliders[i]);
	//}
	//HEALTH-------------------------------------------------------
	for (int i = 0; i < player.hp; i++)
	{
		dstDotRect1 = {5 + 24*i, 5, 21, 18};
		SDL_RenderCopy(renderer, sprites, &healthSrcRect, &dstDotRect1);
	}
	//E KEY--------------------------------------------------------
	if (action == 1)
	{
		dstDotRect1 = {167, 167, 55, 55};
		SDL_RenderCopy(renderer, sprites, &eSourceRect, &dstDotRect1);
	}
	if (action == 3 && mapNow == 5) 
	{
		dstDotRect1 = {167, 167, 55, 55};
		SDL_RenderCopy(renderer, sprites, &eSourceRect, &dstDotRect1);
	}
	//ANIMATIONS---------------------------------------------------
	for (int i = 0; i < 2; i++)
	{
		if (animations[i].isPlaying) {animations[i].draw();}
	}
	//update 
	SDL_RenderPresent(renderer);
}

int Game::start()
{
	showCounter = 0;
	srcMenuRect = {0, 60, 100, 50};
	srcDotRect = {0, 53, 3, 3}; 
	dstDotRect1 = {320, 330, 30, 30};
	dstDotRect2 = {640, 330, 30, 30};
	
	while (true) 
	{
		frameStart = SDL_GetTicks();
		//Game update
		SDL_RenderCopy(renderer, menuTexture, &srcMenuRect, NULL);

		showCounter ++;		
		if (showCounter <= 40)
		{
			SDL_RenderCopy(renderer, menuTexture, &srcDotRect, &dstDotRect1);
			SDL_RenderCopy(renderer, menuTexture, &srcDotRect, &dstDotRect2);
		}
		else if (showCounter > 80) {showCounter = 0;}

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
			else if (event.type = SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					//SDL_QueueAudio(auDev, music.b, music.l);
					Mix_PlayMusic(musicmp3, -1);
					return playCutscene();
				}
			}
		}
		fpsRestriction();
	}
}

int Game::playCutscene()
{
	//surface = SDL_LoadBMP("assets/cutscene.bmp");
	surface = IMG_Load("assets/sprites/cutscene.png");
	SDL_Texture *cutsceneTexture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect planetFrameSrc[24], planetFrameDst[24];
	for (int i = 0; i < 24; i++)
	{
		planetFrameSrc[i] = {0 + i*4, 0 + i*2, 200 - i*8, 100 - i*4};
		planetFrameDst[i] = {0, 0, 1000, 500};
	}
	cutscene[0].create(renderer, cutsceneTexture, planetFrameSrc, planetFrameDst, 24, 5, 2);
	SDL_Rect venatorFrameSrc[24], venatorFrameDst[24];
	for (int i = 0; i < 24; i++)
	{
		venatorFrameSrc[i] = {200 + i*4, 0 + i*2, 200 - i*8, 100 - i*4};
		venatorFrameDst[i] = {0, 0, 1000, 500};
	}
	cutscene[1].create(renderer, cutsceneTexture, venatorFrameSrc, venatorFrameDst, 24, 5, 2);
	SDL_Rect ccFrameSrc[15], ccFrameDst[15];
	for (int i = 0; i < 15; i++)
	{
		ccFrameSrc[i] = {32, 100, 50, 25};
		ccFrameDst[i] = {0, 0, 1000, 500};
	}
	ccFrameSrc[7] = {82, 100, 50, 25};
	ccFrameSrc[8] = {132, 100, 50, 25};
	ccFrameSrc[9] = {50, 175, 50, 25};
	ccFrameSrc[10] = {50, 175, 50, 25};
	ccFrameSrc[11] = {50, 175, 50, 25};
	ccFrameSrc[12] = {50, 175, 50, 25};
	ccFrameSrc[13] = {50, 175, 50, 25};
	ccFrameSrc[14] = {50, 175, 50, 25};
	cutscene[2].create(renderer, cutsceneTexture, ccFrameSrc, ccFrameDst, 15, 12, 3);
	SDL_Rect playerFrameSrc[12], playerFrameDst[12]; 
	for (int i = 0; i < 4; i++)
	{
		playerFrameSrc[i] = {0, 100, 50, 25};
		playerFrameDst[i] = {0, 0, 1000, 500};
	}
	playerFrameSrc[4] = {0, 125, 50, 25};
	playerFrameSrc[5] = {0, 150, 50, 25};
	playerFrameSrc[6] = {0, 175, 50, 25};
	playerFrameSrc[7] = {50, 1125, 50, 25};
	playerFrameDst[4] = {0, 0, 1000, 500};
	playerFrameDst[5] = {0, 0, 1000, 500};
	playerFrameDst[6] = {0, 0, 1000, 500};
	playerFrameDst[7] = {0, 0, 1000, 500};
	for (int i = 8; i < 12; i++)
	{
		playerFrameSrc[i] = {50, 150, 50, 25};
		playerFrameDst[i] = {0, 0, 1000, 500};
	}
	cutscene[3].create(renderer, cutsceneTexture, playerFrameSrc, playerFrameDst, 12, 10, 2);
	SDL_Rect capsuleFrameSrc[24], capsuleFrameDst[24]; 
	for (int i = 0; i < 24; i++)
	{
		capsuleFrameSrc[i] = {300 - i*4, 150 - i*2, 100, 50};
		capsuleFrameDst[i] = {0, 0, 1000, 500};
	}
	cutscene[4].create(renderer, cutsceneTexture, capsuleFrameSrc, capsuleFrameDst, 24, 5, 2);

	int animationNow = 0;
	cutscene[0].isPlaying = 1;
	while (true) 
	{
		frameStart = SDL_GetTicks();
		//Game update
		cutscene[animationNow].draw();
		if (cutscene[animationNow].isPlaying == 0) 
		{
			animationNow++; 
			if (animationNow > 4) 
			{
				player.start();
				return map1();
			};
			cutscene[animationNow].isPlaying = 1;
		}
		SDL_RenderPresent(renderer);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
			{
				player.start();
				return map1();
			}
		}
		fpsRestriction();
	}
}

int Game::playCutscene2()
{
	SDL_SetRenderDrawColor(renderer, 154, 126, 89, 255);
	int animationNow = 0;
	cutscene2[0].isPlaying = 1;
	SDL_Rect atteFrameSrc[240], atteFrameDst[240];
	SDL_Rect cabinFrameSrc[240], cabinFrameDst[240];
	for (int i = 0; i < 240; i++)
	{
		atteFrameSrc[i] = {0, 28, 205, 104};
		atteFrameDst[i] = {i*10 - 1400, -14, 1435, 728};
		cabinFrameSrc[i] = {105, 195, 25, 25};
		cabinFrameDst[i] = {i*10 - 371, 140, 175, 175};
	}
	cutscene2[0].create(renderer, sprites, atteFrameSrc, atteFrameDst, 240, 1, 4);
	cutscene2[1].create(renderer, sprites, cabinFrameSrc, cabinFrameDst, 240, 1, 4);
	while (true) 
	{
		frameStart = SDL_GetTicks();
		//Game update
		SDL_RenderClear(renderer);
		cutscene2[animationNow].draw();
		cutscene2[animationNow+1].draw();

		if (cutscene2[animationNow].isPlaying == 0) 
		{
			animationNow++; 
			if (animationNow > 0) 
			{
				return end();
			};
			cutscene2[animationNow].isPlaying = 1;
		}
		SDL_RenderPresent(renderer);
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
		}
		fpsRestriction();
	}
}

void Game::prepareMap()
{
	destroyBullets();
	for (int i = 0; i < stonesNumber; i++)
	{
		colliders[i + collidersNumber - stonesNumber] = {textures[i].dstRect.x, textures[i].dstRect.y + textures[i].dstRect.h - 6, textures[i].dstRect.w, 6};
	}
	fpsRestriction();
}

int Game::map1() 
{
	mapNow = 1;
	collidersNumber = 3;
	texturesNumber = 6;

	enterCollisionPoint = {-10, -10};
	//stones
	stonesNumber = 3;
	textures[0] = {{28, 0, 21, 15}, {20, 400, 105, 75}};
	textures[1] = {{49, 0, 12, 12}, {850, 10, 60, 60}};
	textures[2] = {{0, 0, 28, 28}, {63 * 5, 60, 140, 140}};
	//at te
	textures[3] = {{0, 28, 105, 50}, {475, 250, 525, 250}, 100};
	//cracks
	textures[4] = {{186, 0, 34, 23}, {40 * 5, 70 * 5, 34 * 5, 23 * 5}, -500, true};
	textures[5] = {{169, 0, 17, 17}, {158 * 5, 50 * 5, 85, 85}, -500, true};

	enemiesNumber = 2;
	restoreEnemiesInfo(1);
	enemies[0].setPos(100, 600, 700);
	enemies[1].setPos(300, 400, 500);

	prepareMap();
	
	while (true) 
	{
		frameStart = SDL_GetTicks();
		//game update
		move();
		if (!collisionCheck())
		{
			return 0;
		}
		if (player.x < 5)
		{
			player.x = 5;
		}
		if (player.y < 5)
		{
			player.y = 5;
		}		
		draw();

		if (player.x > 1000)
		{
			player.x = 0;
			saveEnemiesInfo(1);
			return map2();
		}
		if (player.y > 500)
		{
			player.y = 0;
			saveEnemiesInfo(1);
			return map3();
		}

    	if (SDL_PollEvent(&event))
    	{
    		if (event.type == SDL_QUIT)
    		{
    		    return quit();
    		}
    	}
		fpsRestriction();
	}
}

int Game::map2()
{
	mapNow = 2;
	collidersNumber = 3;
	texturesNumber = 6;

	enterCollisionPoint = {-10, -10};
	//stones
	stonesNumber = 3;
	textures[0] = {{28, 0, 21, 15}, {150, 50, 105, 75}};
	textures[1] = {{90, 0, 29, 13}, {850, 220, 145, 65}};
	textures[2] = {{49, 0, 12, 12}, {500, 400, 60, 60}};
	//at te
	textures[3] = {{105, 28, 100, 50}, {0, 250, 500, 250}, 100};
	//cracks 
	textures[4] = {{205, 35, 32, 29}, {148 * 5, 68 * 5, 32 * 5, 29 * 5}, -500, true};
	textures[5] = {{119, 0, 18, 23}, {108 * 5, 25, 18 * 5, 23 * 5}, -500, true};

	enemiesNumber = 1;
	restoreEnemiesInfo(2);
	enemies[0].setPos(38 * 5, 300, 700);

	prepareMap();

	while (true)
	{
		frameStart = SDL_GetTicks();

		//game update
		move();
		if (!collisionCheck())
		{
			return 0;
		}
		if (player.y < 5)
		{
			player.y = 5;
		}
		if (player.x > 950)
		{
			player.x = 950;
		}
		draw();

		if (player.x < 0)
		{
			player.x = 1000;
			saveEnemiesInfo(2);
			return map1();
		}
		if (player.y > 500)
		{
			player.y = 0;
			saveEnemiesInfo(2);
			return map4();
		}
		//

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
		}
		fpsRestriction();
	}
}

int Game::map3()
{
	mapNow = 3;
	collidersNumber = 5;
	colliders[0] = {540, 260, 80, 10};
	colliders[1] = {845, 260, 120, 10};
	enterCollisionPoint = {-10, -10};
	texturesNumber = 7;
	//stones
	stonesNumber = 3;
	textures[0] = {{28, 0, 21, 15}, {400, 250, 105, 75}};
	textures[1] = {{90, 0, 29, 13}, {80, 100, 145, 65}};
	textures[2] = {{90, 0, 29, 13}, {700, 400, 149, 65}};
	//at te
	textures[3] = {{0, 78, 105, 54}, {475, 0, 525, 270}};
	//cracks
	textures[4] = {{205, 73, 39, 12}, {162 * 5, 52 * 5, 39 * 5, 60}, -500, true}; 
	textures[5] = {{205, 64, 27, 9}, {102 * 5, 52 * 5, 27 * 5, 45}, -500, true};
	textures[6] = {{137, 0, 32, 28}, {25 * 5, 55 * 5, 160, 140}, -500, true};
	
	enemiesNumber = 1;
	restoreEnemiesInfo(3);
	enemies[0].setPos(350, 300, 630);
	
	prepareMap();

	while (true)
	{
		frameStart = SDL_GetTicks();

		//game update

		move();
		if (!collisionCheck())
		{
			return 0;
		}
		if (player.y > 405)
		{
			player.y = 405;
		}
		if (player.x < 5)
		{
			player.x = 5;
		}
		draw();
		if (player.x > 1000)
		{
			player.x = 0;
			saveEnemiesInfo(3);
			return map4();
		}
		if (player.y < 0)
		{
			player.y = 500;
			saveEnemiesInfo(3);
			return map1();
		}

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
		}

		fpsRestriction();
	}
}

int Game::map4()

{
	mapNow = 4;
	collidersNumber = 5;
	colliders[0] = {190, 260, 80, 10}; //at-te
	colliders[1] = {645, 250, 350, 120};//build
	enterCollisionPoint = {727, 305};
	texturesNumber = 8;
	//stones
	stonesNumber = 3;
	textures[0] = {{61, 0, 29, 23}, {485, 250, 145, 115}};
	textures[1] = {{49, 0, 12, 12}, {750, 70, 60, 60}};
	textures[2] = {{49, 0, 12, 12}, {80, 405, 60, 60}};
	//at te 
	textures[3] = {{105, 78, 100, 54}, {0, 0, 500, 270}};
	//cracks
	textures[4] = {{205, 85, 27, 9}, {33 * 5, 52 * 5, 27 * 5, 45}, -500, true};
	textures[5] = {{186, 0, 34, 23}, {400, 150, 170, 115}, -500, true};
	textures[6] = {{169, 0, 17, 17}, {800, 400, 85, 85}, -500, true};
	//building 
	textures[7] = {{205, 94, 70, 47}, {645, 135, 350, 235}};


	enemiesNumber = 1;
	restoreEnemiesInfo(4);
	enemies[0].setPos(310, 50, 410);
	enemies[1].setPos(200, 700, 930);
	
	prepareMap();

	while (true)
	{
		frameStart = SDL_GetTicks();

		//game update
		move();
		if (!collisionCheck())
		{
			return 0;
		}
		if (player.y > 405)
		{
			player.y = 405;
		}
		if (player.x > 950)
		{
			player.x = 950;
		}
		draw();

		if (player.x < 0)
		{
			player.x = 1000;
			saveEnemiesInfo(4);
			return map3();
		}
		if (player.y < 0)
		{
			player.y = 500;
			saveEnemiesInfo(4);
			return map2();
		}
		//E KEY
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e && action == 1)
		{
			player.x = 380;
			player.y = 350;
			saveEnemiesInfo(4);
			return map5();
		}
		//

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
		}

		fpsRestriction();
	}
}

int Game::map5()
{
	mapNow = 5;

	enemiesNumber = 0;
	stonesNumber = 0;
	enterCollisionPoint = {360, 440};
	texturesNumber = 8;
	collidersNumber = 6;

	//bg
	textures[0] = {{157, 170, 82, 82}, {295, 45, 410, 410}, 0, true};
	if (isAreaOpened) 
	{
		textures[8] = {{239, 170, 30, 46}, {705, 45, 150, 230}, 0, true};
		if (isCaseOpened[1]) {textures[9] = {{184, 151, 22, 17}, {725, 125, 110, 85}};} else {textures[9] = {{162, 151, 22, 17}, {725, 125, 110, 85}};}
		texturesNumber = 10;
	} 
	else {newAreaCpoint = {700, 180};}
	//boxes
	textures[1] = {{151, 151, 10, 10}, {310, 285, 50, 50}};
	textures[2] = {{151, 151, 10, 10}, {355, 285, 50, 50}};
	textures[3] = {{151, 151, 10, 10}, {475, 145, 50, 50}};
	textures[4] = {{151, 151, 10, 10}, {520, 145, 50, 50}};
	textures[5] = {{151, 151, 10, 10}, {500, 100, 50, 50}};
	colliders[0] = {95 * 5, 34 * 5, 19 * 5, 5 * 5};
	colliders[1] = {62 * 5, 65 * 5, 19 * 5, 2 * 5};
	//cases
	if (isCaseOpened[0]) {textures[6] = {{172, 132, 22, 17}, {310, 165, 110, 85}};} else {textures[6] = {{150, 132, 22, 17}, {310, 165, 110, 85}};}
	colliders[2] = {62 * 5, 42 * 5, 22 * 5, 8 * 5};
	colliders[3] = {145 * 5, 34 * 5, 22 * 5, 8 * 5};
	//table
	textures[7] = {{121, 133, 28, 28}, {550, 300, 140, 140}, -70};
	colliders[4] = {590, 325, 60, 125};
	colliders[5] = {550, 340, 135, 90};

	prepareMap();

	while (true)
	{
		frameStart = SDL_GetTicks();

		move();
		if (!collisionCheck())
		{
			return 0;
		}
		if (player.x < 305) {player.x = 305;}
		if (player.x > 650 && player.y > 180) {player.x = 650;}
		if (player.x > 800) {player.x = 800;}

		if (player.y < 100) {player.y = 100;}
		if (player.x > 655 && player.y > 175) {player.y = 175;}
		if (player.y > 355) {player.y = 355;}

		draw();

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e && action == 1)
		{
			player.x = 775;
			player.y = 300;
			return map4();
		}
		if (action == 2) 
		{
			//bg
			textures[8] = {{239, 170, 30, 46}, {705, 45, 150, 230}, 0, true};
			isAreaOpened = true;
			//case
			if (isCaseOpened[1]) {textures[9] = {{184, 151, 22, 17}, {725, 125, 110, 85}};} else {textures[9] = {{162, 151, 22, 17}, {725, 125, 110, 85}};}

			texturesNumber = 10;
		}
		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_e && action == 3)
		{
			if (player.x < 500) 
			{
				textures[6].srcRect = {172, 132, 22, 17};
				openCaseCpoints[0] = {10, 10};
				isCaseOpened[0] = true;
				animations[1].isPlaying = 1;
				player.changeGun();
				Mix_PlayChannel(-1, getGun, 0);
			}
			else 
			{
				textures[9].srcRect = {184, 151, 22, 17};
				openCaseCpoints[1] = {10, 10};
				isCaseOpened[1] = true;
				animations[0].isPlaying = 1;
				player.hp++;
				Mix_PlayChannel(-1, getHp, 0);
			}
			Mix_PlayChannel(-1, openChest, 0);
		}

		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
		}
		fpsRestriction();
	}
}

void Game::destroyBullets()
{
	for (int i = 0; i < bulletsNumber; i++)
	{
		bullets[i].isExist = false;
		eBullets[i].isExist = false;
	}
}

void Game::restoreEnemiesInfo(int map)
{
	map--;
	for (int i = 0; i < enemiesNumber; i++)
		{
			enemies[i].setSettings(enemiesX[map][i], enemiesXvel[map][i], enemiesExist[map][i], enemiesHp[map][i]);
		}
}

void Game::saveEnemiesInfo(int map)
{
	map--;
	for (int i = 0; i < enemiesNumber; i++)
		{
			enemiesX[map][i] = enemies[i].x;
			enemiesXvel[map][i] = enemies[i].xVel;
			enemiesExist[map][i] = enemies[i].isExist;
			enemiesHp[map][i] = enemies[i].hp;
		}
}

int Game::end()
{
	bool isAgain = true;
	showCounter = 0;
	srcMenuRect = {0, 0, 100, 50};
	dstDotRect1 = {270, 330, 30, 30};
	dstDotRect2 = {710, 330, 30, 30};

	while (true) 
	{
		frameStart = SDL_GetTicks();

		SDL_RenderCopy(renderer, menuTexture, &srcMenuRect, NULL);

		showCounter ++;
		if (showCounter <= 40)
		{
			SDL_RenderCopy(renderer, menuTexture, &srcDotRect, &dstDotRect1);
			SDL_RenderCopy(renderer, menuTexture, &srcDotRect, &dstDotRect2);
		}
		else if (showCounter > 80) {showCounter = 0;}

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				return quit();
			}
			else if  (event.type == SDL_KEYDOWN && event.key.repeat == 0)
			{
				if (event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_s)
				{
					if (isAgain)
					{
						isAgain = false;
						dstDotRect1 = {370, 410, 30, 30};
						dstDotRect2 = {600, 410, 30, 30};
						showCounter = 0;
					}
					else
					{
						isAgain = true;
						dstDotRect1 = {270, 330, 30, 30};
						dstDotRect2 = {710, 330, 30, 30};
						showCounter = 0;
					}
				}
				else if (event.key.keysym.sym == SDLK_RETURN)
				{
					if (isAgain)
					{
						restart();
						return map1();
					}
					else
					{
						return quit();
					}
				}
			}
		}
		fpsRestriction();
	}
}

void Game::restart()
{	
	enemiesKilled = 0;
	player.start();
	isGunExist[mapNow - 1] = false;
	//isGunExist[0] = true;
	//gun.spawn(100, 100, true);
	destroyBullets();
	for (int i = 0; i < 4; i++) 
	{
		for (int j = 0; j < 3; j++)
		{
			enemiesExist[i][j] = true;
			enemiesHp[i][j] = 5;
		}
	}
	//map 5
	isAreaOpened = false;
	isCaseOpened[0] = false;
	isCaseOpened[1] = false;
	openCaseCpoints[0] = {355, 215};
	openCaseCpoints[1] = {770, 175};
}

int Game::quit() 
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(menuTexture);
	SDL_DestroyTexture(sprites);
	Mix_FreeMusic(musicmp3);
	Mix_Quit();
	SDL_Quit();
	return 0;
}

void Game::fpsRestriction()
{
	frameTime = SDL_GetTicks() - frameStart;

	if (frameTime < frameDelay)
	{
		SDL_Delay(frameDelay - frameTime);
	}
	else
	{
		Log("over delay");
	}
}

