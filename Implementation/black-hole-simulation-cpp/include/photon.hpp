#pragma once

#include "vector3.h"
#include "scene.hpp"
#include "math_utils.hpp"
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <functional>

class Photon2
{
private:
    static const double H = 0.01;

    /** Cartesian coordinates */
    double x, y;

    /** Polar coordinates */
    double phi, r;
    double dphi, dr;

    /** The Schwartzchild radius */
    double rs;

    /** Conserved quantities */
    double L, E;

public:


    Photon2(Vector3 pos, Vector3 dir, BlackHole blackHole) : x(pos.x()), y(pos.y()) 
    {
        this->rs = blackHole.rs;

        this->r = pos.length();
        this->phi = atan2(y, x);

        /** Fix direction to be 2D */
        dir = normalize(Vector3(dir.x(),dir.y(),0));

        double dx = dir.x(), 
               dy = dir.y();

        this->dr = project(dir, Vector3(x,y,0)).length();
        this->dphi = project(dir,Vector3(-y,x,0)).length();

        L = r * r * dphi;
        E = sqrt((dr * dr) + ((1 - (rs / r)) * L * L / (dr * dr)));
    }

    double delta(double r0)
    {
        return E * E - (1 - rs / r0) * (L * L) / (r0 * r0);
    }

    Vector3 f(double r0, double phi0, double sign)
    {
        
        double d = delta(r0);
        if (d < 0) 
        {
            throw std::invalid_argument("Unphysical state");
        }

        double dr0 = L / (r0 * r0),
             dphi0 = sign * (sqrt(d));
        return Vector3(dr0, dphi0, 0);
    }

    Vector3 rk4step()
    {
        double sign = sgn(dphi);
        sign += sign == 0.0;

        Vector3 k1 = Vector3(dr, dphi, 0),
                k2 = f(r + H / 2.0 * k1.x(), phi + H / 2.0 * k1.y(), sign),
                k3 = f(r + H / 2.0 * k2.x(), phi + H / 2.0 * k2.y(), sign),
                k4 = f(r + H * k1.x(), phi + H * k1.y(), sign);
        
        Vector3 result = Vector3(r, phi, 0) + (H / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);

        return result;
    }

    /**
     * Integrates the path of the photon using Runge-Kutta 4
     * @param steps is the amount of steps taken with Runge-Kutta
     * @return a vector of each point
     */
    std::vector<Vector3> path(int steps)
    {
        for (size_t i = 0; i < steps; i++)
        {
            double r1, phi1;
            try
            {
                Vector3 state = rk4step();
                r1 = state.x();
                phi1 = state.y();
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
                break;
            }
            
            double d = delta(r1);

            if (d < 0.0)
            {
                //TODO: Fix finding the turning point of the 
            }
        }
        
    }
};

