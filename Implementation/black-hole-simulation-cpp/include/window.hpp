#pragma once
#include <SDL2/SDL.h>

#include "vector3.h"

namespace Window
{
    void createWindow(SDL_Window*& window, SDL_Renderer*& renderer, 
                      int width, int height, const std::string& title)
    {
        SDL_Init(SDL_INIT_VIDEO);
        
        window = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height, 0
        );
        
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    void setRenderDrawColor(SDL_Renderer*& renderer, Color color) 
    {
        int r = int(255.999 * color.x());
        int g = int(255.999 * color.y());
        int b = int(255.999 * color.z());
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    }

    void drawPixel(SDL_Renderer*& renderer, int x, int y, Color color)
    {
        setRenderDrawColor(renderer, color);
        SDL_RenderDrawPoint(renderer, x, y);
    }

    void updateRenderer(SDL_Renderer*& renderer)
    {
        SDL_RenderPresent(renderer);
    }

    void quit(SDL_Renderer*& renderer, SDL_Window*& window)
    {
        SDL_DestroyRenderer(renderer); renderer = nullptr;
        SDL_DestroyWindow(window); window = nullptr;
        SDL_Quit();
    }
}

namespace Shapes
{
    void drawCircle(SDL_Renderer*& renderer, 
                    int x, int y, int radius, Color color)
    {
        Window::setRenderDrawColor(renderer, color);
        
        int radiusSqr = radius * radius;
        for (int x0 = x - radius; x0 <= x + radius; x0++)
        {
            for (int y0 = y - radius; y0 <= y + radius; y0++)
            {
                int dx = x - x0, dy = y - y0;
                if (dx * dx + dy * dy <= radiusSqr) 
                    SDL_RenderDrawPoint(renderer, x0, y0);
            }
        }
    }
}