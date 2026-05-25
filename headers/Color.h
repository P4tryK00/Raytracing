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

    Color operator+(const Color &c) const {
        return Color(r + c.r, g + c.g, b + c.b);
    }
    Color operator-(const Color &c) const {
        return Color(r - c.r, g - c.g, b - c.b);
    }
    Color operator*(const Color &c) const{
        return Color(r * c.r, g * c.g, b * c.b);
    }
    Color operator*(const double &s) const{
        return Color(r * s, g * s, b * s);
    }
    Color operator/(const double &s) const{
        return Color(r / s, g / s, b / s);
    }
    Color operator+=(const Color& c) {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }
   


    static double clamp01(double x) {
        return std::max(0.0,std::min(x,1.0));
    }

    static int toByte(double x) {
        return static_cast<int>(255.0 * clamp01(x));
    }

    static double gammaCorrect(double x) {
        return std::pow(clamp01(x), 1.0 / 2.2);
    }
private:
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

};

inline Color operator *(double s , const Color & c )
{
    return c * s ;
}

#endif //RAYTRACING_COLOR_H
