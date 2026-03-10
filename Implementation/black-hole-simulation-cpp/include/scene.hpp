#pragma once

class BlackHole
{
public:
    /** The mass of the black hole */
    double mass;

    /** The Schwartzchild radius */
    double rs;

    BlackHole(double mass) : mass(mass) { rs = 2 * mass; }
};