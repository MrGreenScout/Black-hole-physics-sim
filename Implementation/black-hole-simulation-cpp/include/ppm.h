#ifndef RENDERER_MATH_PPM_
#define RENDERER_MATH_PPM_

#include <iostream>

#include "vector3.h"
#include "image.h"

/** Writes out color in a ppm format */
void write_color(std::ostream &out, const Color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

void exportPPM(std::ostream &out, const Image &image)
{
    out << "P3\n" << image.getWidth() << ' ' << image.getHeight() << "\n255\n";

    for (int j = 0; j < image.getHeight(); j++) {
        std::clog << "\rScanlines remaining: " << (image.getHeight() - j) << ' ' << std::flush;
        for (int i = 0; i < image.getWidth(); i++) {
            auto color { image.getPixel(i, j) };
            if (color)
            {
                write_color(out, *color);
            }
            else 
            {
                write_color(out, Color(0,0,0));
            }
        }
    }

    std::clog << "\rDone.                 \n";
}

#endif