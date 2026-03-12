#include <iostream>
#include <SDL2/SDL_mouse.h>

#include "user_interface.hpp"
#include "window.hpp"
#include "scene.hpp"
#include "camera.hpp"

#define TARGET_FPS 24
#define FRAME_DELAY 1000 / TARGET_FPS

int main(int argc, char const *argv[])
{
    std::cout << "Starting simulation window\n";
    
    BlackHole sagittariusA(8.54 * 1e36);
    Scene2 scene(sagittariusA);
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    Window::createWindow(window, renderer, 600, 400, "Black hole simulation 2D");

    UserInterface userInterface(renderer, &scene);

    SDL_Event event;
    bool running = true;
    while (running)
    {
        Uint32 frameStart = SDL_GetTicks();
        
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }

            userInterface.updateEvents(&event);
        }

        userInterface.updateCamera();
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        scene.drawScene(renderer);

        userInterface.updateUI();

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY)
            SDL_Delay(FRAME_DELAY - frameTime);

        Window::updateRenderer(renderer);
    }

    Window::quit(renderer, window);
}
