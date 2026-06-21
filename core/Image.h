#ifndef RAYTRACING_IMAGE_H
#define RAYTRACING_IMAGE_H
#include "Color.h"
#include <string>
#include <fstream>
#include <vector>

// Reprezentacja bufora ramki (Framebuffer).
// Przechowuje wyrenderowany obraz w pamięci operacyjnej przed ostatecznym zapisem na dysk.
class Image {
public:
    // Alokuje pamięć na wszystkie piksele obrazu przy starcie silnika.
    // Rozmiar wektora to dokładna liczba pikseli (szerokość * wysokość).
    Image(int width, int height) : width(width), height(height), pixels(width * height) {}

    // Wstawia ostatecznie wyliczony kolor do odpowiedniego punktu na matrycy.
    // Wykorzystuje spłaszczenie współrzędnych 2D (x, y) do indeksu 1D (y * width + x).
    // Jest to kluczowa optymalizacja wydajności (tzw. cache locality) – ciągła bloku pamięci
    // odczytuje się w C++ drastycznie szybciej niż z zagnieżdżonych struktur (vector<vector>).
    void setPixel(int x, int y, const Color& color) {
        pixels[ y * width + x ] = color;
    }

    // Odczytuje zapisany piksel ze spłaszczonej matrycy 1D.
    Color getPixel(int x, int y) const {
        return pixels[ y * width + x ];
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Wyzwala proces zrzutu bufora do pliku obrazu.
    // To tutaj (w implementacji w .cpp) następuje nałożenie korekcji Gamma i
    // konwersja kolorów zmiennoprzecinkowych do 8-bitowego standardu sRGB.
    void savePPM(const std::string& filename) const;

private:
    int width;
    int height;

    // Surowa, jednowymiarowa struktura danych przechowująca kolory w przestrzeni liniowej.
    std::vector<Color> pixels;
};

#endif //RAYTRACING_IMAGE_H