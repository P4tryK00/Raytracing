#include "PointLight.h"
#include <cmath>


PointLight::PointLight(const Vector& position, const Color& intensity,
                       double constantAtten, double linearAtten, double quadAtten)
    : position_(position), intensity_(intensity),
      constantAtten_(constantAtten), linearAtten_(linearAtten), quadAtten_(quadAtten) {}

// Wyznacza wektor 'L' (Light Vector) używany w równaniu oświetlenia Phonga
// Wektor celuje od badanego punktu na powierzchni DO źródła światła.
Vector PointLight::getDirectionFrom(const Vector& point) const {
    return (position_ - point).normalized();
}

// Oblicza natężenie światła docierającego do danego punktu przestrzeni.
// Symuluje wygaszanie światła.
Color PointLight::getIntensityAt(const Vector& point) const {
    double distance = getDistanceFrom(point);

    // Model tłumienia 
    // - constantAtten_ (Kc): Zapobiega przekroczeniu jasności początkowej blisko źródła
    // - linearAtten_ (Kl):  człon do płynnego wygaszania światła.
    // - quadAtten_ (Kq): Człon fizyczny. Odpowiada za prawo odwrotnych kwadratów
    double attenuation = constantAtten_ + (linearAtten_ * distance) + (quadAtten_ * distance * distance);

    // Zabezpieczenie przed błędem dzielenia przez zero  oraz odwróceniem kolorów
    // w przypadku wprowadzenia ujemnych współczynników z poziomu konfiguracji sceny.
    if (attenuation <= 0.0) return intensity_;
    
    return intensity_ / attenuation;
}

// Wyznacza  odległość  między punktem uderzenia a żarówką.
double PointLight::getDistanceFrom(const Vector& point) const {
    return (position_ - point).length();
}