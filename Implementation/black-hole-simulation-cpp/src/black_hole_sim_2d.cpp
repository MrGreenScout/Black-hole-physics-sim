#include <iostream>
#include <SDL2/SDL_mouse.h>

#include "user_interface.hpp"
#include "window.hpp"
#include "scene.hpp"
#include "camera.hpp"

#define TARGET_FPS 60
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

int main(int argc, char const *argv[])
{
    std::cout << "Starting simulation window\n";
    
    BlackHole sagittariusA(8.54 * 1e36);
    Scene2 scene(sagittariusA, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    Window::createWindow(window, renderer, SCREEN_WIDTH, SCREEN_HEIGHT, "Black hole simulation 2D");

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

        int frameDelay = 1000 / TARGET_FPS;
        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay)
            SDL_Delay(frameDelay - frameTime);

        Window::updateRenderer(renderer);
    }

    Window::quit(renderer, window);
}
