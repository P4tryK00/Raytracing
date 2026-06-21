#ifndef RAYTRACING_POINTLIGHT_H
#define RAYTRACING_POINTLIGHT_H

#include "Light.h"

// Konkretna implementacja wszechkierunkowego źródła światła (Point Light).
// Symuluje obiekt fizyczny (np. gołą żarówkę zawieszoną w przestrzeni), z którego
// fotony rozchodzą się równomiernie we wszystkich kierunkach.
// Dziedziczy po interfejsie Light, co pozwala na polimorficzne przechowywanie
// i przetwarzanie tego obiektu wewnątrz wektora świateł w klasie Scene.
class PointLight : public Light {
public:
    // Konstruktor inicjalizujący fizyczne parametry źródła.
    // Oprócz wektora pozycji i energii początkowej (intensity), przyjmuje opcjonalne
    // parametry tłumienia (Attenuation), które pozwalają na wyjście poza sztywne ramy
    // Prawa Odwrotnych Kwadratów i na artystyczne sterowanie zasięgiem światła.
    PointLight(const Vector& position, const Color& intensity,
               double constantAtten = 1.0, double linearAtten = 0.0, double quadAtten = 0.0);

    // --- IMPLEMENTACJA INTERFEJSU OPTYCZNEGO ---

    // Wylicza znormalizowany wektor 'L' celujący od punktu uderzenia w stronę żarówki.
    // W przeciwieństwie do świateł kierunkowych (np. Słońca), wektor ten jest unikalny
    // dla absolutnie każdego sprawdzanego punktu w przestrzeni.
    Vector getDirectionFrom(const Vector& point) const override;

    // Oblicza natężenie światła (energię) docierającego do danego punktu.
    // Aplikuje na bazowy kolor (intensity_) matematyczny model zanikania z odległością.
    Color getIntensityAt(const Vector& point) const override;

    // Oblicza dokładny promień rzuconego promienia cienia (odległość punkt -> źródło).
    double getDistanceFrom(const Vector& point) const override;

private:
    // P: Punkt w globalnym układzie współrzędnych definiujący centrum emisji fotonów.
    Vector position_;

    // Bazowa, maksymalna energia promieniowania u samego źródła (odległość d = 0).
    Color intensity_;

    // --- Parametry równania tłumienia (Attenuation Equation) ---
    // Mianownik równania ma postać: Kc + Kl*d + Kq*d^2

    // Kc (Constant): Współczynnik stały. Zapobiega eksplozji jasności blisko źródła (dzieleniu przez < 1.0).
    double constantAtten_;

    // Kl (Linear): Współczynnik liniowy. Używany zazwyczaj do sztucznego symulowania mgły lub smogu.
    double linearAtten_;

    // Kq (Quadratic): Współczynnik kwadratowy. Najważniejszy element fizyczny,
    // realizujący rozpraszanie energii na sferze o rosnącym promieniu (Inverse-Square Law).
    double quadAtten_;
};

#endif //RAYTRACING_POINTLIGHT_H