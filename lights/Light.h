#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include "Vector.h"
#include "Color.h"


class Light {
public:
    virtual ~Light() = default;

    // Oblicza i zwraca znormalizowany wektor kierunkowy 'L' biegnący od punktu na obiekcie DO źródła światła.
    virtual Vector getDirectionFrom(const Vector& point) const = 0;

    // Wyznacza ostateczną barwę i energię światła docierającego do analizowanego punktu przestrzeni.
    virtual Color getIntensityAt(const Vector& point) const = 0;

    // Oblicza odległość euklidesową od punktu intersekcji geometrii do centrum źródła światła.
    virtual double getDistanceFrom(const Vector& point) const = 0;
};

#endif //RAYTRACING_LIGHT_H