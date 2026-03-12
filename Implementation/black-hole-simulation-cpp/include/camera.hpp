#pragma once

#include <memory>

#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_events.h>

#include "vector3.h"

class Camera2
{
public:
    constexpr static const double DEFAUL_PIXELS_PER_UNIT = 10;

    Vector3 position = Vector3(0, 0, 0);
    double pixelsPerUnit = DEFAUL_PIXELS_PER_UNIT;

    Camera2(Point3 position)  : position(position)
    {

    }

    void translate(Vector3 delta)
    {
        this->position += delta;
    }

    void zoom(int ticks)
    {
        pixelsPerUnit += DEFAUL_PIXELS_PER_UNIT * ticks * 1e-1;
    }

    Point3 toWorldSpaceCoordinate(std::pair<int, int> screenCoordinate)
    {   
        double x = (static_cast<double>(screenCoordinate.first) - position.x()) / pixelsPerUnit,
               y = (position.y() - static_cast<double>(screenCoordinate.second)) / pixelsPerUnit;
               
        return Point3(x, y, 0);
    }

    std::pair<int, int> toScreenSpaceCoordinate(Point3 point)
    {
        int x = point.x() * pixelsPerUnit + position.x(),
            y = position.y() - point.y() * pixelsPerUnit;
        
        return std::make_pair(x,y);
    }
};
