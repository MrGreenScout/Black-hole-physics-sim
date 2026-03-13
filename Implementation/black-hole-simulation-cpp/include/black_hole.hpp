#pragma once

/** Represent a black hole with a mass and a schwartzchild radius */
class BlackHole
{
public:
    /** Constants */
    constexpr static const double G_SI   = 6.674e-11;
    constexpr static const double C_SI   = 3.0e8;

    /** The mass of the black hole */
    double mass;

    /** The Schwartzchild radius */
    double rs;

    /** Geometric Mass unitless constant */
    long double massGeo;

    BlackHole(double mass) : mass(mass), massGeo(G_SI * mass / (C_SI * C_SI)) { rs = 2; }
};