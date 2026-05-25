#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include "Vector.h"
#include "Color.h"

class Light {
public:
    virtual ~Light() = default;

    // Zwraca znormalizowany wektor kierunkowy od oświetlanego punktu DO źródła światła (L)
    virtual Vector getDirectionFrom(const Vector& point) const = 0;

    // Zwraca natężenie/kolor światła docierającego do danego punktu.
    // Uwzględnia zanikanie światła wraz z odległością (tzw. attenuation).
    virtual Color getIntensityAt(const Vector& point) const = 0;

    // Zwraca odległość od punktu na scenie do źródła światła.
    // Niezbędne do algorytmu rzucania cieni (Shadow Rays).
    virtual double getDistanceFrom(const Vector& point) const = 0;
};

#endif //RAYTRACING_LIGHT_H