#pragma once

#include <functional>


// Source - https://stackoverflow.com/a/4609795
// Posted by user79758, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-10, License - CC BY-SA 4.0

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}


double bisection(double a, double b, std::function<double (double)> func)
{   
    double c = a;
    for (size_t i = 0; i < 50; i++)
    {
        if (abs(b-a) < 1e-8) break;

        double c = (b-a) / 2;

        if (abs(func(c)) < 1e-8) break;

        if (func(c) * func(a) < 0)
        {
            b = c;
        }
        else
        {
            a = c;
        }
    }
    
    return c;
}