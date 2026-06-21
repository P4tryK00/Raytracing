#include "PointLight.h"
#include <cmath>

// Inicjalizuje wszechkierunkowe, punktowe źródło światła (Point Light - odpowiednik fizycznej żarówki).
// Poza pozycją i kolorem, przyjmuje trzy współczynniki tłumienia (Attenuation),
// które pozwalają na precyzyjne, artystyczne lub fizyczne sterowanie zanikaniem światła.
PointLight::PointLight(const Vector& position, const Color& intensity,
                       double constantAtten, double linearAtten, double quadAtten)
    : position_(position), intensity_(intensity),
      constantAtten_(constantAtten), linearAtten_(linearAtten), quadAtten_(quadAtten) {}

// Wyznacza wektor 'L' (Light Vector) używany w równaniu oświetlenia Phonga/Blinna-Phonga.
// Wektor celuje od badanego punktu na powierzchni DO źródła światła.
Vector PointLight::getDirectionFrom(const Vector& point) const {
    // Normalizacja jest absolutnie konieczna, by późniejszy iloczyn skalarny (N dot L)
    // zwracał precyzyjny kosinus kąta padania, a nie wartość zniekształconą przez odległość.
    return (position_ - point).normalized();
}

// Oblicza natężenie światła docierającego do danego punktu przestrzeni.
// Symuluje rozpraszanie fotonów na rosnącej sferze promieniowania (wygaszanie światła).
Color PointLight::getIntensityAt(const Vector& point) const {
    double distance = getDistanceFrom(point);

    // Model tłumienia (wzór z Równania 3 z instrukcji).
    // Składa się z trzech członów w mianowniku:
    // - constantAtten_ (Kc): Zapobiega przekroczeniu jasności początkowej blisko źródła (zazwyczaj ustawiane na 1.0).
    // - linearAtten_ (Kl): Opcjonalny człon do sztucznego, płynnego wygaszania światła.
    // - quadAtten_ (Kq): Człon fizyczny. Odpowiada za prawo odwrotnych kwadratów (Inverse-Square Law).
    double attenuation = constantAtten_ + (linearAtten_ * distance) + (quadAtten_ * distance * distance);

    // Zabezpieczenie przed błędem dzielenia przez zero (NaN) oraz odwróceniem kolorów
    // w przypadku wprowadzenia ujemnych współczynników z poziomu konfiguracji sceny.
    if (attenuation <= 0.0) return intensity_;

    // Modulacja energii świetlnej. Składowe RGB są precyzyjnie skalowane przez wyliczony współczynnik zaniku.
    return intensity_ / attenuation;
}

// Wyznacza fizyczną odległość (promień t) między punktem uderzenia a żarówką.
// Wartość ta jest krytyczna przy rzucaniu promieni cienia (Shadow Rays) -
// staje się ona twardym limitem t_max. Dzięki temu silnik w Renderer::shade nie uznaje za zacieniające
// tych przeszkód, które leżą daleko za samym źródłem światła.
double PointLight::getDistanceFrom(const Vector& point) const {
    return (position_ - point).length();
}