#pragma once

class Animation
{
    private:

        SDL_Texture *texture;
        SDL_Renderer *renderer;
        SDL_Rect *src, *dst;
        int animCounter = 0, animFrames, divider, seconds, showFrames;

    public:

        int isPlaying = 0; 

        void create(SDL_Renderer *i_renderer, SDL_Texture *i_texture, SDL_Rect *i_src, SDL_Rect *i_dst, int i_frames, int i_divider, int i_seconds);

        void draw();

};