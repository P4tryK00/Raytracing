//
// Created by patry on 22.05.2026.
//

#include "Image.h"
#include "Color.h"

void Image::savePPM(const std::string &filename) const {
    std::ofstream file(filename);
    file << "P3\n" << width << " " << height << "\n255\n";

    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {
            const Color& c = pixels[ y * width + x];
            file << Color::toByte(Color::gammaCorrect(c.red())) << " "
                 << Color::toByte(Color::gammaCorrect(c.green())) << " "
                 << Color::toByte(Color::gammaCorrect(c.blue())) << "\n";
        }
    }
}
