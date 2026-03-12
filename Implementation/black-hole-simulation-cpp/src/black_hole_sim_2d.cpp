#include <iostream>

#include "window.hpp"

int main(int argc, char const *argv[])
{
    std::cout << "Starting simulation window\n";
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    Window::createWindow(window, renderer, 600, 400, "Black hole simulation 2D");

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Shapes::drawCircle(renderer, 300, 200, 50, Color(1,0,0));
    Window::updateRenderer(renderer);

    SDL_Event event;
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }
    }

    Window::quit(renderer, window);
}
