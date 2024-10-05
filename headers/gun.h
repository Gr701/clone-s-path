#pragma once

class Gun
{
    private:

        SDL_Renderer* renderer;
        SDL_Texture * texture; 

        int
        width = 51, 
        height = 21;

        SDL_Rect srcRect = {103, 132, 17, 7};
        
    public:

        SDL_Rect dstRect;
        SDL_Rect collisionRect;
        
        void render(SDL_Renderer* i_renderer, SDL_Texture* i_texture, int x, int y);

        void draw();

        void spawn(int x, int y, bool isRight);

};

class Bullet 
{
    public:
        int x, y, xVel, yVel, width, height;
        bool isExist = false;
        SDL_Rect dstRect; 
        Vector2 cPoint;
        SDL_Renderer *renderer;

        void spawn(SDL_Renderer *i_renderer, int i_x, int i_y, int speed, bool isRight, int i_width, int i_height);
        
        void move();

        void draw();

        void eSpawn(SDL_Renderer *i_renderer, int i_x, int i_y, int i_xVel, int i_yVel, int i_width, int i_height);

        void eMove();
};