#ifndef RAYTRACING_SPOTLIGHT_H
#define RAYTRACING_SPOTLIGHT_H

#include "Light.h"

// implementacja źródła światła typu Spot Light
class SpotLight : public Light {
public:
    SpotLight(const Vector& position, const Vector& direction, const Color& intensity, double cutoffAngleDegrees,
        double outerCutoffAngleDegrees, double constantAtten = 1.0, double linearAtten = 0.0, double quadAtten = 0.0);


    // Zwraca znormalizowany wektor celujący od badanego punktu DO centrum reflektora.
    Vector getDirectionFrom(const Vector &point) const override;

    // Zwraca finalne natężenie światła dla wybranego punktu w przestrzeni.
    // Metoda ta nakłada na bazową energię dwie maski:
    // 1. Tłumienie odległościowe 
    // 2. Tłumienie kątowe (wygaszanie światła, jeśli punkt znajduje się na krawędzi lub poza stożkiem).
    Color getIntensityAt(const Vector &point) const override;

    // Zwraca odległość  od punktu do centrum reflektora.
    // Stanowi rygorystyczny limit (t_max) przy generowaniu promieni cienia 
    double getDistanceFrom(const Vector &point) const override;

private:
    // P: Punkt w globalnym układzie współrzędnych będący początkiem emisji.
    Vector position_;

    // D: Znormalizowany wektor określający główną oś (kierunek celowania) stożka świetlnego.
    Vector direction_;

    // Bazowa moc/kolor emitowanego promieniowania.
    Color intensity_;


    double cutoffAngleCos_;
    double outerCutoffAngleCos_;

    // Współczynniki tłumienia odległościowego 
    double constantAtten_;
    double linearAtten_;
    double quadAtten_;

};

#endif //RAYTRACING_SPOTLIGHT_H