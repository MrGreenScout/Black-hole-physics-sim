#pragma once

#include <vector>
#include <functional>

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include "vector3.h"
#include "camera.hpp"
#include "scene.hpp"

class UserInterface
{
public:
    bool isDraging = false;
    std::pair<int, int> dragStart;  
    std::pair<int, int> dragEnd;

    SDL_Renderer *renderer;
    Scene2 *scene;

    UserInterface(SDL_Renderer *renderer, Scene2 *scene) : renderer(renderer), scene(scene) {}

    void updateCamera()
    {
        int x = 0, 
            y = 0;

        if (SDL_GetRelativeMouseState(&x, &y) & SDL_BUTTON_LMASK)
            scene->camera.translate(Vector3(x, y, 0));

        if (isDraging)
        {
            SDL_GetMouseState(&x, &y);  
            dragEnd = std::make_pair(x, y);
        }
    }

    void updateUI()
    {
        if (isDraging) 
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer,
                               std::get<0>(dragStart), std::get<1>(dragStart),
                               std::get<0>(dragEnd),   std::get<1>(dragEnd));
        }
    }

    void updateEvents(SDL_Event *event) 
    {
        if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_RIGHT)
        {
            int x = 0, 
                y = 0;

            if (SDL_GetMouseState(&x, &y) & SDL_BUTTON_RMASK && !isDraging)
            {
                dragStart = std::make_pair(x,y);
                isDraging = true;
            }  
        }
        else if (event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_RIGHT)
        {
            isDraging = false;

            Vector3 dragStartW = scene->camera.toWorldSpaceCoordinate(dragStart),
                    dragEndW = scene->camera.toWorldSpaceCoordinate(dragEnd);
            scene->addPhoton(dragStartW, dragEndW - dragStartW);
        }
        else if (event->type == SDL_MOUSEWHEEL)
        {
            scene->camera.zoom(event->wheel.y);
        }
    }
};