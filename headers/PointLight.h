#ifndef RAYTRACING_POINTLIGHT_H
#define RAYTRACING_POINTLIGHT_H

#include "Light.h"

class PointLight : public Light {
public:
    // Konstruktor przyjmujący pozycję, kolor i opcjonalnie współczynniki zanikania
    PointLight(const Vector& position, const Color& intensity, 
               double constantAtten = 1.0, double linearAtten = 0.0, double quadAtten = 0.0);

    Vector getDirectionFrom(const Vector& point) const override;
    Color getIntensityAt(const Vector& point) const override;
    double getDistanceFrom(const Vector& point) const override;

private:
    Vector position_;
    Color intensity_;
    
    // Współczynniki odpowiedzialne za fizyczne zanikanie światła wraz z dystansem
    double constantAtten_;
    double linearAtten_;
    double quadAtten_;
};

#endif //RAYTRACING_POINTLIGHT_H