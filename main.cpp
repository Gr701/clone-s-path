#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include "headers/log.h"
#include "headers/player.h"
#include "headers/enemy.h"
#include "headers/gun.h"
#include "headers/animation.h"
#include "headers/game.h"

int main(int argc, char *argv[]) 
{
	Game game;
    int r = game.start();
    Logi(r);
    
    return r;    
}