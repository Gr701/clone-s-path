#pragma once

class Player
{		
	private: 

		SDL_Renderer *renderer; 
		SDL_Texture *texture;

		int 
		width = 45, height = 90,
		d_width = 51,
		animCounter = 0,
		srcY = 132;		

		SDL_Rect 
		srcRect = {0, 0, 17, 30}, 
		dstRect = {x, y, width, height}, 
		srcStandRect = {0, 132, 17, 30};

		int animX[8] = {17, 34, 51, 34, 17, 68, 85, 68};
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		//Uint32 st = 0;
 
	public: 

		bool isRight = true, isArmed = false, isStay = true;

		int 
		speed = 3,
		prevX, prevY,
		x, y,
		hp, shootCounter = 10;
		const Uint8 *kState;

		Vector2 cPoint; //center

		SDL_Rect collisionRect, legCollisionRect;
		
		void render(SDL_Renderer *i_renderer, SDL_Texture *i_texture);

		void start();

		int move(SDL_Event& event);

		void draw();

		void changeGun();
};