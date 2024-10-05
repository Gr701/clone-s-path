#include <iostream>
#include <SDL2/SDL.h>
#include "headers/log.h"
#include "headers/enemy.h"

void Enemy::render(SDL_Renderer *i_renderer, SDL_Texture *i_texture)
{
    renderer = i_renderer;
    //surface = SDL_LoadBMP("assets/enemy.bmp");
    //texture = SDL_CreateTextureFromSurface(renderer, surface);
    texture = i_texture;
}

void Enemy::setSettings(int i_x, int i_xVel, bool i_isExist, int i_hp)
{
    if (i_xVel < 0) {xVel = -1;} else {xVel = 1;}
    x = i_x;
    isExist = i_isExist;
    hp = i_hp;
}

void Enemy::setPos(int i_y, int i_leftPointX, int i_rightPointX)
{
    rightPointX = i_rightPointX;
    leftPointX = i_leftPointX;
    y = i_y;
}

void Enemy::move() 
{
    if (x < leftPointX)
    {
        xVel = 1;
    }
    else if (x > rightPointX)
    {
        xVel = -1;
    }
    x += xVel;
    y += yVel;
    collisionRect = {x, y, width-6, height};
    visionCircle = {x + width/2, y + height/2, 350};
}

void Enemy::draw()
{
    dstRect = {x-3, y, width + 6, height};
    if (xVel > 0) {flip = SDL_FLIP_NONE;} else {flip = SDL_FLIP_HORIZONTAL;}
    if (isMoving)
    {
        if (animCounter >= 40) {animCounter = 0;}  
        srcRect= {animX[animCounter/5], 192, 17, 30};
        animCounter += 1;
    }
    else 
    {
        srcRect = {26, 222, 26, 30};
    }
    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0, NULL, flip);
}

void Enemy::drawWgun()
{
    fPoint.y = y + 33;
    if (xVel > 0) {fPoint.x = x + 75;}
    else {fPoint.x = x - 42;}
    if (animCounter >= 40) {animCounter = 0;}
    if (xVel > 0) {flip = SDL_FLIP_NONE; dstRect = {x-6, y, 78, 90};} else {flip = SDL_FLIP_HORIZONTAL; dstRect = {x-33, y, 78, 90};}
    if (isMoving)
    {
        srcRect= {animXgun[animCounter/5], 222, 26, 30};
        animCounter += 1;
    }
    else
    {
        srcRect = {26, 222, 26, 30};
    }
    
    SDL_RenderCopyEx(renderer, texture, &srcRect, &dstRect, 0, NULL, flip);
    //health
    if(hp < 5)
    {
        SDL_SetRenderDrawColor(renderer, 140, 70, 70, 255);
        dstRect = {x - 12, y - 6, hp * 12, 3};
        SDL_RenderFillRect(renderer, &dstRect);
        SDL_SetRenderDrawColor(renderer, 115, 60, 60, 255);
        dstRect = {x - 12, y - 6, 60, 3};
        SDL_RenderDrawRect(renderer, &dstRect);
    }
    //drawCircle(renderer, x + width/2, y + height/2, 350);//
}

