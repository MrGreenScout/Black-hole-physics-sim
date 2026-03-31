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
    constexpr static const size_t MAX_PHOTONS = 1000;

    Camera2 camera;
    BlackHole blackHole;

    std::deque<Photon2> photons;

    Scene2(BlackHole blackHole, int width, int height) : blackHole(blackHole), camera(Camera2(Vector3(0,0,0), width, height)) 
    {
        //camera.pixelsPerUnit = Camera2::DEFAUL_PIXELS_PER_UNIT / blackHole.massGeo;
        //camera.translationSpeed /= blackHole.massGeo;
    }

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
        auto pos = this->camera.toScreenSpaceCoordinate(Vector3(0, 0, 0)); // Black hole position
        // Draws photon sphere
        Shapes::drawCircle(renderer, std::get<0>(pos), std::get<1>(pos),
                           blackHole.photonSphere * camera.pixelsPerUnit, BLUE);
        // Draws the black hole
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
                int alpha = int(255.999 * i / Photon2::MAX_HISTORY_SIZE);
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