#ifndef RAYTRACING_IMAGE_H
#define RAYTRACING_IMAGE_H
#include "Color.h"
#include <string>
#include <fstream>
#include <vector>

class Image {
public:
    Image(int width, int height) : width(width), height(height), pixels(width * height) {}

    void setPixel(int x, int y, const Color& color) {
        pixels[ y * width + x ] = color;
    }

    Color getPixel(int x, int y) const {
        return pixels[ y * width + x ];
    }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void savePPM(const std::string& filename) const;

private:
    int width;
    int height;
    std::vector<Color> pixels;
};


#endif //RAYTRACING_IMAGE_H
