#ifndef RAYTRACING_COLOR_H
#define RAYTRACING_COLOR_H

#include <algorithm>
#include <cmath>

class Color {
    public:
    Color() = default;
    Color(double red, double green, double blue) : r(red), g(green), b(blue) {}

    double red() const {return r;}
    double green() const {return g;}
    double blue() const {return b;}

    void setRed(double value) {r = value;}
    void setGreen(double value) {g = value;}
    void setBlue(double value) {b = value;}

    // Dodaje natężenia kolorów.
    // Używane do sumowania wkładu wielu źródeł światła na jednym obiekcie.
    Color operator+(const Color &c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    }

    // Odejmuje składowe koloru.
    // Zastosowanie pomocnicze przy operacjach na różnicach barw.
    Color operator-(const Color &c) const {
        return Color(r - c.r, g - c.g, b - c.b);
    }

    // Mnoży składowe dwóch kolorów (modulacja).
    // Symuluje absorpcję – np. białe światło padające na czerwoną ścianę.
    Color operator*(const Color &c) const{
        return Color(r * c.r, g * c.g, b * c.b);
    }

    // Skaluje natężenie koloru przez wartość liczbową.
    // Niezbędne do modyfikowania siły światła (np. spadek jasności z odległością).
    Color operator*(const double &s) const{
        return Color(r * s, g * s, b * s);
    }

    // Dzieli natężenie koloru przez wartość liczbową.
    // Wykorzystywane do uśredniania pikseli przy antyaliasingu (próbkowanie Jittered).
    Color operator/(const double &s) const{
        return Color(r / s, g / s, b / s);
    }

    // Akumuluje kolor w pętli.
    // Służy do dodawania światła dyfuzyjnego i lustrzanego do ostatecznego koloru piksela.
    Color operator+=(const Color& c) {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

    // Obcina wartość do fizycznego przedziału [0.0, 1.0].
    // Zapobiega błędom "przepalenia" (>1.0) przy eksporcie obrazu.
    static double clamp01(double x) {
        return std::max(0.0, std::min(x, 1.0));
    }

    // Konwertuje natężenie zmiennoprzecinkowe na 8-bitowy format piksela (0-255).
    // Konieczne do zapisu pliku obrazu (.ppm).
    static int toByte(double x) {
        return static_cast<int>(255.0 * clamp01(x));
    }

    // Aplikuje potęgową korekcję Gamma (1.0 / 2.2).
    // Tłumaczy fizyczną przestrzeń liniową (Linear) na standard wyświetlania monitora (sRGB).
    static double gammaCorrect(double x) {
        return std::pow(clamp01(x), 1.0 / 2.2);
    }

private:
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

};

// Umożliwia przemienne mnożenie przez skalar (skalar * kolor).
// Ułatwia zapis matematyczny równań w modelu oświetlenia.
inline Color operator *(double s , const Color & c )
{
    return c * s ;
}

#endif //RAYTRACING_COLOR_H