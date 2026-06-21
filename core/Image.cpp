#include "Image.h"
#include "Color.h"

// Eksportuje wygenerowaną buforowaną tablicę pikseli do nieskompresowanego formatu PPM (wariant P3).
// Jest to ostateczny etap potoku renderującego (pipeline), w którym surowe dane fizyczne
// przygotowywane są do poprawnego wyświetlenia na ekranie monitora.
void Image::savePPM(const std::string &filename) const {
    std::ofstream file(filename);

    // Zapis standardowego nagłówka pliku PPM:
    // 'P3' - magiczna liczba oznaczająca zapis kolorów RGB w formacie tekstowym (ASCII).
    // width, height - wymiary rozdzielczości obrazu.
    // '255' - maksymalna wartość dla pojedynczego kanału koloru (reprezentacja 8-bitowa).
    file << "P3\n" << width << " " << height << "\n255\n";

    // Przejście przez całą matrycę obrazu wiersz po wierszu (od góry do dołu, od lewej do prawej).
    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {

            // Pobranie koloru piksela. Tablica 'pixels' jest jednowymiarowa dla lepszej spójności
            // w pamięci cache (cache locality), więc współrzędne 2D (x, y) są spłaszczane do indeksu 1D.
            const Color& c = pixels[y * width + x];

            // Krytyczny moment eksportu:
            // 1. gammaCorrect() - zakrzywia fizyczną, liniową przestrzeń barw silnika do standardu sRGB.
            // 2. toByte() - kwantyzuje wartość zmiennoprzecinkową [0.0, 1.0] do liczby całkowitej [0, 255].
            file << Color::toByte(Color::gammaCorrect(c.red())) << " "
                 << Color::toByte(Color::gammaCorrect(c.green())) << " "
                 << Color::toByte(Color::gammaCorrect(c.blue())) << "\n";
        }
    }
}