#pragma once

static void Log(std::string msg)
{
    std::cout << msg << std::endl;
}

static void Logi(int msgi)
{
    std::cout << std::to_string(msgi) << std::endl;
}

static void Logii(int msgi, int msg)
{
    std::cout << std::to_string(msgi) + " " + std::to_string(msg)<< std::endl;
}

static void Logu(Uint32 msgi)
{
    std::cout << std::to_string(msgi) << std::endl;
}

static void drawCircle(SDL_Renderer *renderer, int x, int y, int r)
{
    SDL_RenderDrawPoint(renderer, x, y);
    SDL_RenderDrawPoint(renderer, x + r, y);
    SDL_RenderDrawPoint(renderer, x, y + r);
    SDL_RenderDrawPoint(renderer, x - r, y);
    SDL_RenderDrawPoint(renderer, x, y - r);
    SDL_RenderDrawPoint(renderer, x + (707 * r) / 1000, y + (0,707 * r) / 1000);
    SDL_RenderDrawPoint(renderer, x + (707 * r) / 1000, y - (0,707 * r) / 1000);
    SDL_RenderDrawPoint(renderer, x - (707 * r) / 1000, y + (0,707 * r) / 1000);
    SDL_RenderDrawPoint(renderer, x - (707 * r) / 1000, y - (0,707 * r) / 1000);
}

struct Vector2 {int x; int y;};
struct Circle {int x; int y; int r;};