#include <iostream>
#include <SDL2/SDL.h>
#include "headers/log.h"
#include "headers/animation.h"

void Animation::create(SDL_Renderer *i_renderer, SDL_Texture *i_texture, SDL_Rect *i_src, SDL_Rect *i_dst, int i_animFrames, int i_divider, int i_seconds)
{
    renderer = i_renderer;
    texture = i_texture;
    src = i_src;
    dst = i_dst; 
    animFrames = i_animFrames;
    divider = i_divider;
    seconds = i_seconds;
    showFrames = seconds * 60;

}

void Animation::draw()
{
    if (showFrames > 0) 
    {
        SDL_RenderCopy(renderer, texture, &src[animCounter/divider], &dst[animCounter/divider]);
        animCounter++;
        if (animCounter >= divider * animFrames) {animCounter = 0;}
        showFrames--;
    }
    else
    {
        showFrames = seconds * 60;
        isPlaying = 0;
    }
}
