#include <iostream>
#include <SDL2/SDL.h>
#include "headers/log.h"
#include "headers/gun.h"

void Gun::render(SDL_Renderer* i_renderer, SDL_Texture* i_texture, int x, int y)
{
    dstRect = {x, y, width, height};
    collisionRect = {x+6, y, width - 12, height};
    renderer = i_renderer; 
    texture = i_texture;
}

void Gun::draw()
{
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void Gun::spawn(int x, int y, bool isRight)
{
    if (isRight)
    {
        dstRect = {x + 51, y + 60, width, height};
        collisionRect = {x + 57, y + 60, width - 12, height};
    }
    else
    {
        dstRect = {x - 57, y + 60, width, height};
        collisionRect = {x - 51, y + 60, width - 12, height};
    }
}

void Bullet::spawn(SDL_Renderer *i_renderer, int i_x, int i_y, int speed, bool isRight, int i_width, int i_height)
{
    renderer = i_renderer;
    x = i_x;
    y = i_y;
    if (isRight)
    {
        xVel = speed;
    }
    else
    {
        xVel = -speed;
    }
    width = i_width;
    height = i_height;
    isExist = true;
    dstRect = {x, y, width, height};  
    cPoint.x = x + width/2;
    cPoint.y = y + height/2;
}

void Bullet::move()
{
    x += xVel;
    cPoint.x = x + width/2;
    cPoint.y = y + height/2;
    dstRect = {x, y, width, height};
}

void Bullet::eSpawn(SDL_Renderer *i_renderer, int i_x, int i_y, int i_xVel, int i_yVel, int i_width, int i_height)
{
    renderer = i_renderer;
    x = i_x;
    y = i_y;
    xVel = i_xVel;
    yVel = i_yVel;
    width = i_width;
    height = i_height;
    isExist = true;
    dstRect = {x, y, width, height};
}

void Bullet::eMove()
{
    x += xVel;
    y += yVel;
    cPoint.x = x + width/2;
    cPoint.y = y + height/2;
    dstRect = {x, y, width, height};
}

void Bullet::draw()
{
    SDL_SetRenderDrawColor(renderer, 230, 15, 15, 255);
    SDL_RenderFillRect(renderer, &dstRect);
}
