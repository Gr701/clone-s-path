#include <iostream>
#include <SDL2/SDL.h>
#include "headers/log.h"
#include "headers/player.h"

void Player::render(SDL_Renderer *i_renderer, SDL_Texture *i_texture)
{
	renderer = i_renderer;
    //surface = SDL_LoadBMP("assets/player.bmp");
    //texture = SDL_CreateTextureFromSurface(renderer, surface);
	texture = i_texture;
}

void Player::start()
{
	x = 50;
	y = 50;
	hp = 3;
	isRight = true;
	dstRect = {x, y, d_width, height};
	collisionRect = {x, y, width, height};

	isArmed = true;
	changeGun();
}

int Player::move(SDL_Event &event)
{
	prevX = x;
	prevY = y;

	SDL_PumpEvents();
	kState = SDL_GetKeyboardState(NULL);

	if (kState[SDL_SCANCODE_W])
	{
		y -= speed;
	}
	if (kState[SDL_SCANCODE_A])
	{
		x -= speed;
	}
	if (kState[SDL_SCANCODE_S])
	{
		y += speed;
	}
	if (kState[SDL_SCANCODE_D])
	{
		x += speed;
	}
	if (prevX == x && prevY == y) {isStay = true;} 
	else if (prevX > x) {isRight = false; isStay = false;} else if (prevX < x) {isRight = true; isStay = false;} else {isStay = false;}
	collisionRect = {x, y, width, height};
	legCollisionRect = {x + 6, y + height - 6, width - 12, 6};
	cPoint.x = x + width/2;
	cPoint.y = y + height/2;
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
		if (event.key.keysym.sym == SDLK_q && isArmed)
		{
			changeGun();
			return 1;
		}
		//else if (event.key.keysym.sym == SDLK_f && isArmed)
		//{
		//	return 2;
		//}
	}	
	return 0;
}

void Player::draw() 
{
	if (!isArmed || isRight)
	{
		dstRect = {x-3, y, d_width, height};
	}
	else
	{
		dstRect = {x-30, y, d_width, height};
	}

	if (animCounter >= 40) {animCounter = 0;}
	
	if (isStay)  
	{
		srcRect = srcStandRect;
	}
	else{
		srcRect = {animX[animCounter/5], srcY, d_width/3, 30};	
		animCounter += 1; 
		if (isRight)
		{
			flip = SDL_FLIP_NONE;
		}
		else
		{
			flip = SDL_FLIP_HORIZONTAL;
		}
	}
	//if (SDL_GetTicks() - st > 17) {Log("> 17");}
	//st = SDL_GetTicks();
	SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0, NULL, flip);
}

void Player::changeGun()
{
	if (isArmed)
	{
		isArmed = false; 
		animX[0] = 17; animX[1] = 34; animX[2] = 51; animX[3] = 34; 
		animX[4] = 17; animX[5] = 68; animX[6] = 85; animX[7] = 68; 
		d_width = 51;
		height = 90;
		srcY = 132;
		srcStandRect = {0, 132, 17, 30};
	}
	else
	{
		isArmed = true; 
		animX[0] = 26; animX[1] = 52; animX[2] = 78; animX[3] = 52; 
		animX[4] = 26; animX[5] = 104; animX[6] = 130; animX[7] = 104;
		d_width = 78;
		height = 90;
		srcY = 162;
		srcStandRect = {0, 162, 26, 30};
	}	
	 
}

