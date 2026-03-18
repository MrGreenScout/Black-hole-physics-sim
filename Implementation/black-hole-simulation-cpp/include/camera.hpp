#pragma once

#include <memory>

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_events.h>

#include "vector3.h"

class Camera2
{
public:
    constexpr static const double DEFAUL_PIXELS_PER_UNIT = 10;

    int width = 600, height = 400;
    Vector3 position = Vector3(0, 0, 0);
    double pixelsPerUnit = DEFAUL_PIXELS_PER_UNIT;
    double translationSpeed = DEFAUL_PIXELS_PER_UNIT * 1e-1;

    Camera2(Point3 position, int width, int height)  : position(position), width(width), height(height)
    {

    }

    void translate(Vector3 delta)
    {
        this->position += delta * Vector3(-1, 1, 0) * 0.1;
    }

    void zoom(int ticks)
    {
        pixelsPerUnit += translationSpeed * ticks;
    }

    Point3 toWorldSpaceCoordinate(std::pair<int, int> screenCoordinate)
    {   
        double sx = static_cast<double>(screenCoordinate.first),
               sy = static_cast<double>(screenCoordinate.second);

        double x = (sx - width * 0.5) / pixelsPerUnit + position.x(),
               y = (height * 0.5 - sy) / pixelsPerUnit + position.y();

        return Point3(x, y, 0);
    }

    std::pair<int, int> toScreenSpaceCoordinate(Point3 point)
    {
        double sx = (point.x() - position.x()) * pixelsPerUnit + width * 0.5;
        double sy = height * 0.5 - (point.y() - position.y()) * pixelsPerUnit;

        return {
            static_cast<int>(std::round(sx)),
            static_cast<int>(std::round(sy))
        };
    }
};
