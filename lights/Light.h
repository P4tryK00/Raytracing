#ifndef RAYTRACING_LIGHT_H
#define RAYTRACING_LIGHT_H

#include "Vector.h"
#include "Color.h"

// Klasa abstrakcyjna (interfejs) reprezentująca polimorficzne źródło światła w scenie 3D.
// Stanowi fundament dla różnych typów oświetlenia (oświetlenie punktowe, kierunkowe, reflektory),
// umożliwiając jednorodne przetwarzanie strumienia światła w pętli cieniowania Renderera.
class Light {
public:
    virtual ~Light() = default;

    // Oblicza i zwraca znormalizowany wektor kierunkowy 'L' biegnący od punktu na obiekcie DO źródła światła.
    // Jest to kluczowy wektor wejściowy do wyznaczania kątów padania w modelu Phonga (składnik dyfuzyjny i spekularny).
    virtual Vector getDirectionFrom(const Vector& point) const = 0;

    // Wyznacza ostateczną barwę i energię światła docierającego do analizowanego punktu przestrzeni.
    // To tutaj implementowany jest fizyczny spadek natężenia promieniowania wraz z odległością (tzw. attenuation).
    virtual Color getIntensityAt(const Vector& point) const = 0;

    // Oblicza odległość euklidesową od punktu intersekcji geometrii do centrum źródła światła.
    // Wartość ta definiuje górną granicę ('t_max') dla promieni cienia (Shadow Rays),
    // co zapobiega wykrywaniu przeszkód leżących daleko za samym źródłem światła.
    virtual double getDistanceFrom(const Vector& point) const = 0;
};

#endif //RAYTRACING_LIGHT_H