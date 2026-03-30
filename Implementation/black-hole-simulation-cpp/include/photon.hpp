#pragma once

#include "vector3.h"
#include "scene.hpp"
#include "black_hole.hpp"
#include "math_utils.hpp"
#include <iostream>
#include <math.h>
#include <stdexcept>
#include <functional>
#include <optional>
#include <deque>

class Photon2
{
private:
    constexpr static const double H = 0.05;

    /** Polar coordinates */
    double phi, r;
    double dphi;//, dr;
    double drSign;

    /** The Schwartzchild radius */
    double rs;

    /** Conserved quantities */
    double L, E;

    /** boolean flags */
    bool absorbed = false;

    double rSgn(double dr0)
    {
        double sign = sgn(dr0);
        sign += sign == 0.0;
        
        return sign;
    }

public:
    constexpr static const int maxHistorySize = 200;

    /** The path of the photon */
    std::deque<Vector3> path;

    Photon2(Vector3 pos, Vector3 dir, BlackHole blackHole) 
    {
        double x = pos.x(), 
               y = pos.y();

        this->rs = blackHole.rs;

        this->r = pos.length();
        this->phi = atan2(y, x);

        /** Fix direction to be 2D */
        dir = normalize(Vector3(dir.x(),dir.y(),0));

        double dx = dir.x(), 
               dy = dir.y();

        Vector3 rNorm   = normalize(Vector3(x, y, 0));
        Vector3 phiNorm = normalize(Vector3(-y, x, 0));
        double dr   = dot(dir, rNorm);
        this->dphi = dot(dir, phiNorm) / r;

        this->drSign = rSgn(dot(dir, rNorm));
        //this->dr = abs(this->dr);

        L = r * r * dphi;
        E = sqrt((dr * dr) + ((1 - (rs / r)) * L * L / (r * r)));
    }

    double properRadius(double r)
    {
        if (r <= rs) return rs;

        double term1 = sqrt(r * (r - rs));
        double term2 = rs * log((sqrt(r) + sqrt(r - rs)) / sqrt(rs));

        return term1 + term2;
    }

    /** Gets the current carthesian position of the photon */
    Vector3 position()
    {
        double pr = r;//properRadius(this->r);

        return Vector3(
            pr * cos(this->phi),
            pr * sin(this->phi),
            0
        );
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
            d = 0;
            std::clog << "Warning: Clamped Unphysical state\n";
            //throw std::invalid_argument("Unphysical state");
        }

        double dr0 = sign * sqrt(d),
             dphi0 = L / (r0 * r0);
        return Vector3(dr0, dphi0, 0);
    }

    /** Finds the new position after a step h forward */
    Vector3 rk4step(double h)
    {
        double sign = drSign;

        Vector3 k1 = f(r, phi, sign),
                k2 = f(r + h / 2.0 * k1.x(), phi + h / 2.0 * k1.y(), rSgn(k1.x())),
                k3 = f(r + h / 2.0 * k2.x(), phi + h / 2.0 * k2.y(), rSgn(k2.x())),
                k4 = f(r + h * k3.x(), phi + h * k3.y(), rSgn(k3.x()));
        
        Vector3 result = Vector3(r, phi, 0) + (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4);

        return result;
    }

    /** Finds the turning point with a modified bisection method */
    Vector3 findTurningPoint()
    {
        Vector3 stateStart(r, phi, 0);
        Vector3 stateMid = stateStart;
        double h = H;
        for (size_t i = 0; i < 50; i++)
        {
            stateMid = rk4step(h / 2.0);

            if (delta(stateMid.x()) > 0) stateStart = stateMid;
            
            h /= 2;

            this->r = stateStart.x();
            this->phi = stateStart.y();
            Vector3 dstate = f(stateStart.x(), stateStart.y(), drSign);
            //this->dr   = abs(dstate.x());
            this->dphi = dstate.y();

            if (abs(stateStart.x() - stateMid.x()) < 1e-15) break;
        }
        
        return stateMid;
    }

    /** Steps the photon forward one step h in the affine parameter */
    std::optional<Vector3> stepForward()
    {
        if (this->absorbed == true) 
        {
            if (path.size() > 0) path.pop_front();
            return {};
        }

        Vector3 state = rk4step(H);
        if (delta(state.x()) < 0)
        {
            state = findTurningPoint();
            drSign = -drSign;
        }

        if (state.x() <= rs) // If absorbed into black hole
        {
            std::cout << "Absorbed\n";
            absorbed = true;
            return {};
        }

        // Update current position and velocity
        Vector3 dstate = f(state.x(), state.y(), drSign);

        this->r = state.x();
        this->phi = state.y();
        //this->dr = abs(dstate.x());
        this->dphi = dstate.y();

        // Register path history
        path.push_back(position());

        if (path.size() >= maxHistorySize) path.pop_front();

        return state;
    }
};

