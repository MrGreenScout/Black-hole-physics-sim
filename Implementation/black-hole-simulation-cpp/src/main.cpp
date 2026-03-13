#include <iostream>
#include <SDL2/SDL_mouse.h>

#include "user_interface.hpp"
#include "window.hpp"
#include "scene.hpp"
#include "camera.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

#define TARGET_FPS 60
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 400

struct ProgramState 
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    UserInterface *userInterface = nullptr;
    Scene2 *scene = nullptr;
    bool running = true;
};

static ProgramState state;

void mainLoop()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) 
    {
        if (event.type == SDL_QUIT) state.running = false;
        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        {
            state.running = false;
        }

        state.userInterface->updateEvents(&event);
    }
    #ifdef __EMSCRIPTEN__

    if (!state.running)
    {
        emscripten_cancel_main_loop();
        Window::quit(state.renderer, state.window);
        return;
    }

    #endif

    state.userInterface->updateCamera();
    
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
    SDL_RenderClear(state.renderer);

    state.scene->drawScene(state.renderer);

    state.userInterface->updateUI();
    Window::updateRenderer(state.renderer);
}

int main(int argc, char const *argv[])
{
    std::cout << "Welcome to the black hole physics sim!\n\n";
    std::cout << "Controls:\nMove around by holding LMB and draging\n";
    std::cout <<  "Spawn photon by holding RMB, draging and releasing in the direction you want to photon to travel\n";
    
    BlackHole sagittariusA(8.54 * 1e36);
    state.scene = new Scene2(sagittariusA, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Window::createWindow(state.window, state.renderer, SCREEN_WIDTH, SCREEN_HEIGHT, "Black hole simulation 2D");

    state.userInterface = new UserInterface(state.renderer, state.scene);

    #ifdef __EMSCRIPTEN__

    emscripten_set_main_loop(mainLoop, TARGET_FPS, 1);

    #endif

    #ifndef __EMSCRIPTEN__
    const int frameDelay = 1000 / TARGET_FPS;

    while (state.running)
    {
        Uint32 frameStart = SDL_GetTicks();
        
        mainLoop();

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        
        if (frameTime < frameDelay)
            SDL_Delay(frameDelay - frameTime);
    }

    Window::quit(state.renderer, state.window);
    #endif

    delete state.userInterface;
    delete state.scene;
    return 0;
}