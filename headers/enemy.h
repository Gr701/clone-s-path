#pragma once

class Enemy 
{
    private:
        
        SDL_Renderer* renderer;
        SDL_Texture* texture;    

        int 
        yVel = 0,
        width = 45, height = 90,
        rightPointX , leftPointX,
        animCounter = 0;

        SDL_Rect srcRect, dstRect;
        int animX[8] = {17, 34, 51, 34, 17, 68, 85, 68};
        int animXgun[8] = {26, 52, 78, 52, 26, 104, 130, 104};
        SDL_RendererFlip flip = SDL_FLIP_NONE;

    public:
 
        int y = 300, x = 500, xVel = 1, hp = 5, shootCounter = 30;

        bool isExist = true, isMoving = true;

        Vector2 fPoint;

        SDL_Rect collisionRect;

        Circle visionCircle;

        void render(SDL_Renderer *i_renderer, SDL_Texture *i_texture);

        void setSettings(int i_x, int i_xVel, bool i_isExist, int i_hp);

        void setPos(int n_y, int n_leftPointX, int n_rightPointX);

        void move();

        void draw();

        void drawWgun();
};