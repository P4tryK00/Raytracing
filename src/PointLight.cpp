#include "../headers/PointLight.h"
#include <cmath>

PointLight::PointLight(const Vector& position, const Color& intensity, 
                       double constantAtten, double linearAtten, double quadAtten)
    : position_(position), intensity_(intensity), 
      constantAtten_(constantAtten), linearAtten_(linearAtten), quadAtten_(quadAtten) {}

// Zwraca znormalizowany wektor kierunkowy (L) celujący ze sceny w stronę żarówki
Vector PointLight::getDirectionFrom(const Vector& point) const {
    return (position_ - point).normalized();
}

// Oblicza natężenie światła uwzględniając wzór z Równania 3 z instrukcji
Color PointLight::getIntensityAt(const Vector& point) const {
    double distance = getDistanceFrom(point);
    
    // Obliczanie współczynnika tłumienia (zanikania światła)
    double attenuation = constantAtten_ + (linearAtten_ * distance) + (quadAtten_ * distance * distance);
    
    // Zabezpieczenie przed dzieleniem przez zero (choć dla domyślnego constant=1.0 się nie zdarzy)
    if (attenuation <= 0.0) return intensity_;
    
    return intensity_ / attenuation;
}

// Zwraca długość wektora między punktem na obiekcie a źródłem światła
double PointLight::getDistanceFrom(const Vector& point) const {
    return (position_ - point).length();
}