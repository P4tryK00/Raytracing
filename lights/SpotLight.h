#ifndef RAYTRACING_SPOTLIGHT_H
#define RAYTRACING_SPOTLIGHT_H

#include "Light.h"

// Konkretna implementacja źródła światła typu reflektor (Spot Light).
// Emituje fotony w formie ograniczonego stożka (stożek główny + półcień).
// Obiekt idealny do symulacji latarek, reflektorów samochodowych czy kierunkowego oświetlenia scenicznego.
class SpotLight : public Light {
public:
    // Konstruktor inicjalizujący parametry fizyczne i geometryczne reflektora.
    // Wymaga zdefiniowania dwóch kątów stożka (wewnętrznego i zewnętrznego), co
    // pozwala na matematyczne wyliczenie gładkiego przejścia (soft edge / penumbra)
    // na brzegach plamy światła, unikając ostrego, nienaturalnego odcięcia.
    SpotLight(const Vector& position, const Vector& direction, const Color& intensity, double cutoffAngleDegrees,
        double outerCutoffAngleDegrees, double constantAtten = 1.0, double linearAtten = 0.0, double quadAtten = 0.0);

    // --- IMPLEMENTACJA INTERFEJSU OPTYCZNEGO ---

    // Zwraca znormalizowany wektor celujący od badanego punktu DO centrum reflektora.
    Vector getDirectionFrom(const Vector &point) const override;

    // Zwraca finalne natężenie światła dla wybranego punktu w przestrzeni.
    // Metoda ta nakłada na bazową energię dwie maski:
    // 1. Tłumienie odległościowe (Prawo Odwrotnych Kwadratów).
    // 2. Tłumienie kątowe (wygaszanie światła, jeśli punkt znajduje się na krawędzi lub poza stożkiem).
    Color getIntensityAt(const Vector &point) const override;

    // Zwraca odległość euklidesową od punktu do centrum reflektora.
    // Stanowi rygorystyczny limit (t_max) przy generowaniu promieni cienia (Shadow Rays).
    double getDistanceFrom(const Vector &point) const override;

private:
    // P: Punkt w globalnym układzie współrzędnych będący początkiem emisji.
    Vector position_;

    // D: Znormalizowany wektor określający główną oś (kierunek celowania) stożka świetlnego.
    Vector direction_;

    // Bazowa moc/kolor emitowanego promieniowania.
    Color intensity_;

    // --- ZBUFOROWANE ZMIENNE TRYGONOMETRYCZNE (Krytyczna Optymalizacja) ---
    // Zamiast trzymać w pamięci oryginalne kąty, przechowujemy ich pre-kalkulowane cosinusy.
    // Umożliwia to zastąpienie bardzo drogiej funkcji std::acos() błyskawicznym iloczynem skalarnym
    // (dot product) podczas iteracji po milionach pikseli w głównej pętli cieniującej.
    double cutoffAngleCos_;
    double outerCutoffAngleCos_;

    // Współczynniki tłumienia odległościowego (Kc, Kl, Kq - stały, liniowy, kwadratowy).
    double constantAtten_;
    double linearAtten_;
    double quadAtten_;

};

#endif //RAYTRACING_SPOTLIGHT_H