#ifndef VECTOR_SPERE_H
#define VECTOR_SPERE_H

#include "Vector.h"
#include "Ray.h"
#include <iosfwd>
#include "IntersectionResult.h"

// Reprezentacja matematycznej sfery w przestrzeni trójwymiarowej.
// Jest to najtańszy obliczeniowo i najpopularniejszy prymityw geometryczny w ray tracingu.
// Zawdzięcza to temu, że test intersekcji promienia z kulą sprowadza się
// do bardzo szybkiego rozwiązania prostego równania kwadratowego.
class Sphere {
public:
    // --- KONSTRUKTORY ---
    // Zestaw podstawowych konstruktorów (w tym domyślny, tworzący sferę w początku układu współrzędnych).
    Sphere();
    Sphere(double r);
    Sphere(Vector v);

    // Główne konstruktory geometryczne definiujące pozycję w świecie (środek) i fizyczną wielkość (promień).
    Sphere(Vector v, double r);

    // Pełny konstruktor przypisujący sferze konkretny profil właściwości optycznych (szkło, lustro, mat itp.).
    Sphere(Vector v, double r, Material mat);

    // Główna funkcja testująca uderzenie fotonu (promienia) w powierzchnię figury.
    // Metoda jest oznaczona jako [[nodiscard]], by wymusić na programiście obsłużenie wyniku uderzenia.
    // Parametry t_min i t_max definiują przedział ważności uderzenia (mechanizm Z-Buffera),
    // pozwalając odrzucać uderzenia leżące za kamerą (t < t_min) lub za maksymalnym dystansem rysowania (t > t_max).
    [[nodiscard]] IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;

    // --- AKCESORY (Gettery / Settery) ---
    [[nodiscard]] Vector Center() const { return center_; }
    void Center(Vector o) { center_ = o; }

    [[nodiscard]] double Radius() const { return radius_; }
    void Radius(double r) { radius_ = r; }

    // Przeciążony operator strumienia – narzędzie diagnostyczne pozwalające łatwo
    // wypisać pozycję i rozmiar sfery w konsoli podczas debugowania silnika.
    friend std::ostream& operator<<(std::ostream& os, const Sphere& sph);

private:
    // C: Punkt centralny sfery w globalnym układzie współrzędnych (World Space).
    Vector center_;

    // R: Skalar definiujący odległość powierzchni figury od jej środka.
    double radius_;

    // Struktura DTO (Data Transfer Object) przechowująca komplet parametrów dla silnika cieniującego.
    Material material_;
};

#endif //VECTOR_SPERE_H