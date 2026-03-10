#ifndef RENDERER_MATH_IMAGE_
#define RENDERER_MATH_IMAGE_

#include "vector3.h"

#include <iostream>
#include <optional>

using Color = Vector3;

class Image 
{
private:
    int width, height;
    Color *data;

    bool validPixel(int x, int y) const
    {
        auto pixelIndex = x + width * y;
        return ((x >= 0) && (y >= 0) && (pixelIndex < (width * height))); 
    }

public:
    Image(int width, int height) : width(width), height(height) 
    {
        data = (Color *) malloc(sizeof(Color) * width * height);
        for (int i = 0; i < width * height; i++) 
        {
            data[i] = Color(1,1,1);
        }
    }

    ~Image()
    {
        free(data);
    }

    int getWidth() const { return this->width; }
    int getHeight() const { return this->height; }

    /**
     * @brief Sets a pixel in the image.
     * @return true if pixel was set succesfully, else false.
     */
    bool setPixel(int x, int y, Color color)
    {
        //std::clog << "(x,y) = (" << x << " " << y << ")";
        int index = x + width * y;
        //std::clog << "index = " << index << "Validpixel: " << validPixel(x, y) << "\n";
        if (validPixel(x, y))
        {
            data[index] = color;
            return true;
        }
        return false;
    }
    
    /**
     * @brief Gets the color of a pixel
     * @return the color if pixel is valid else nullopt
     */
    std::optional<Color> getPixel(int x, int y) const
    {
        int index = x + width * y;
        if (validPixel(x, y))
        {
            return data[index];
        }
        
        return std::nullopt;
    }

    void example()
    {
        for (int j = 0; j < this->height; j++) {
            std::clog << "\rScanlines remaining: " << (this->height - j) << ' ' << std::flush;
            for (int i = 0; i < this->width; i++) {
                bool res = setPixel(i, j, Color(double(i)/(this->width-1), double(j)/(this->height-1), 0));
                std::clog << "Set pixel success: " << res << "\n";
            }
        }
    }
};

#endif