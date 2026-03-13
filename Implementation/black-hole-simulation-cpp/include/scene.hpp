#pragma once

#include <vector>
#include <memory>

#include <SDL2/SDL_render.h>

#include "photon.hpp"
#include "window.hpp"
#include "black_hole.hpp"
#include "camera.hpp"

class Scene2
{
public:
    constexpr static const size_t MAX_PHOTONS = 20;

    Camera2 camera;
    BlackHole blackHole;

    std::deque<Photon2> photons;

    Scene2(BlackHole blackHole, int width, int height) : blackHole(blackHole), camera(Camera2(Vector3(0,0,0), width, height)) {}

    void addPhoton(Vector3 position, Vector3 direction)
    {
        photons.emplace_back(Photon2(position, direction, blackHole));

        if (photons.size() > MAX_PHOTONS) 
        {
            photons.pop_front();
        }
    }

    void drawScene(SDL_Renderer *renderer)
    {
        // Draws the black hole
        auto pos = this->camera.toScreenSpaceCoordinate(Vector3(0, 0, 0));
        Shapes::drawCircle(renderer, std::get<0>(pos), std::get<1>(pos), 
                           blackHole.rs * camera.pixelsPerUnit, RED);

        // Draws all photon paths
        for (auto& photon : photons)
        {
            photon.stepForward();
            size_t historyc = photon.path.size();
            
            if (historyc < 2) continue;

            for (size_t i = historyc - 1; i > 0; i--)
            {
                // Create fading color
                int alpha = int(255.999 * i / Photon2::maxHistorySize);
                SDL_SetRenderDrawColor(renderer, alpha, alpha, alpha, 255);

                auto a = this->camera.toScreenSpaceCoordinate(photon.path[i]),
                     b = this->camera.toScreenSpaceCoordinate(photon.path[i - 1]);
                
                SDL_RenderDrawLine(renderer, 
                                   std::get<0>(a), std::get<1>(a), 
                                   std::get<0>(b), std::get<1>(b));
            }
        }
    }
};