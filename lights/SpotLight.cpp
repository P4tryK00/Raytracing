#include "SpotLight.h"
#include <cmath>
#include <numbers>

// Konstruktor reflektora (Spotlight). Emituje światło w określonym kierunku, ograniczone
// do bryły stożka. Pozwala na symulację latarek, reflektorów samochodowych czy scenicznych.
SpotLight::SpotLight(const Vector &position, const Vector &direction, const Color &intensity,
                    double cutoffAngleDegrees, double outerCutoffAngleDegrees, double constantAtten, double linearAtten,
                    double quadAtten) : position_(position), direction_(direction), intensity_(intensity),
                                        constantAtten_(constantAtten), linearAtten_(linearAtten), quadAtten_(quadAtten) {

    // Zabezpieczenie integralności wektora kierunkowego
    direction_ = direction.normalized();

    // --- KRYTYCZNA OPTYMALIZACJA (Prekalkulacja Cosinusów) ---
    // Zamiast przetrzymywać kąty w stopniach/radianach i w pętli renderującej
    // wyciągać arcus cosinus (acos) dla każdego promienia, obliczamy ich cosinusy już tutaj.
    // Dzięki temu w głównej pętli będziemy mogli użyć wyłącznie taniego iloczynu skalarnego.
    cutoffAngleCos_ = std::cos(cutoffAngleDegrees * std::numbers::pi / 180.0);
    outerCutoffAngleCos_ = std::cos(outerCutoffAngleDegrees * std::numbers::pi / 180.0);
}

// Wyznacza znormalizowany wektor 'L' od punktu na geometrii DO źródła światła (używane m.in. do cieni).
Vector SpotLight::getDirectionFrom(const Vector &point) const {
    return  (position_ - point).normalized();
}

// Zwraca fizyczny dystans między punktem a źródłem (limit dla promieni cienia - Shadow Rays).
double SpotLight::getDistanceFrom(const Vector &point) const {
    return (position_ - point).length();
}

// Oblicza natężenie światła uwzględniając spadek energii z dystansem (Attenuation)
// oraz tłumienie na krawędziach stożka (Soft Edge / Penumbra).
Color SpotLight::getIntensityAt(const Vector &point) const {
    auto distance = getDistanceFrom(point);

    // 1. Tłumienie odległościowe (Inverse-Square Law i parametry sztuczne)
    auto atteuntion = constantAtten_ + ( linearAtten_ * distance ) + ( quadAtten_ * distance * distance );
    if (atteuntion <= 0.0) atteuntion = 1.0; // Zabezpieczenie przed dzieleniem przez 0
    Color baseIntensity = intensity_ / atteuntion;

    // 2. Wektor kierunkowy światła biegnącego z żarówki do punktu uderzenia
    Vector lightToPoint = (point - position_).normalized();

    // 3. Iloczyn skalarny znormalizowanych wektorów daje precyzyjny cosinus kąta
    // między osią główną reflektora a kierunkiem do naszego punktu. To nasze 'theta'.
    auto theta = lightToPoint.dotProduct(direction_);

    // Obszar przejścia (półcień) między wewnętrznym (100% mocy) a zewnętrznym (0% mocy) stożkiem.
    auto epsilon = cutoffAngleCos_ - outerCutoffAngleCos_;
    auto intensityScale = 0.0;

    // 4. Liniowa interpolacja jasności na krawędzi stożka
    if (epsilon > 0.0) {
        // Normalizacja wartości 'theta' do przedziału [0.0, 1.0] w obrębie półcienia
        intensityScale = ( theta - outerCutoffAngleCos_) / epsilon;

        // Sztywne obcięcie (Clamp), aby światło nie osiągało wartości ujemnych poza stożkiem
        // ani nie przekraczało 100% w samym centrum.
        if (intensityScale < 0.0) intensityScale = 0.0;
        if (intensityScale > 1.0) intensityScale = 1.0;
    } else {
        // Twarda krawędź (Hard Edge): jeśli kąty wewnętrzny i zewnętrzny są równe (brak półcienia).
        intensityScale = ( theta >= cutoffAngleCos_) ? 1.0 : 0.0;
    }

    // Ostateczna aplikacja maski stożka na stłumioną energię świetlną
    return baseIntensity * intensityScale;
}