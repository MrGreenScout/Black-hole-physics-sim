//#define EVALUATING

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

static int expectedEscaped = 0,
          expectedAbsorbed = 0; 

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

    #ifdef EVALUATING
    int escapedc = 0, absorbedc = 0, undecided = 0;

    bool complete = true;
    for (auto &photon : state.scene->photons)
    {
        if (photon.isEscaped() == false and photon.isAbsorbed() == false)
        {
            complete = false;
            undecided++;
        }

        if (photon.isEscaped()) escapedc++;
        if (photon.isAbsorbed()) absorbedc++;
    }

    if (complete || state.running == false) 
    {
        std::clog << "\nEvaluation Result: " << escapedc << " escaped, " << absorbedc << " absorbed\n";
        std::clog << "Expected   Result: " << expectedEscaped << " escaped, " << expectedAbsorbed << " absorbed\n";
        if (undecided != 0)
        {
            std::clog << "DNF: " << undecided << "\n\n";
        } 
        else 
        {
            std:: clog << "\n";
        }
    }
    #endif

    state.userInterface->updateUI();
    Window::updateRenderer(state.renderer);
}

int main(int argc, char const *argv[])
{
    std::cout << "Welcome to the black hole physics sim!\n\n";
    std::cout << "Controls:\nMove around by holding LMB and draging\n";
    std::cout << "Zoom in/out by scrolling\n";
    std::cout << "Spawn photon by holding RMB, draging and releasing in the direction you want to photon to travel\n";
    
    BlackHole sagittariusA(8.54 * 1e36);
    state.scene = new Scene2(sagittariusA, SCREEN_WIDTH, SCREEN_HEIGHT);
    
    Window::createWindow(state.window, state.renderer, SCREEN_WIDTH, SCREEN_HEIGHT, "Black hole simulation 2D");

    state.userInterface = new UserInterface(state.renderer, state.scene);

    #ifdef EVALUATING
    
    int x = -100, startY = 20;
    for (size_t i = 0; i < 1000; i++)
    {
        auto pos = Vector3(x, startY - 2.0 * startY / 999.0 * (double) i, 0);
        auto dir = Vector3(1, 0, 0);

        state.scene->addPhoton(pos, dir);
        
        double b = std::abs(pos.y());
        double bCrit = state.scene->blackHole.bCrit;

        std::clog << "b = " << b << ", abs(y) = " << std::abs(pos.y()) << "\n";

        if (b > bCrit) 
            expectedEscaped++;
        else
            expectedAbsorbed++;
    }

    std::clog << "Esc: " << expectedEscaped << " abso: " << expectedAbsorbed << "\n";
    
    #endif

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