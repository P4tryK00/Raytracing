#include "Sphere.h"
#include <cmath>
#include <iostream>

// --- KONSTRUKTORY ---
// Inicjalizują sferę matematycznie opartą wyłącznie na punkcie centralnym i promieniu.
Sphere::Sphere() : center_(), radius_( 0.0 ), material_() {}
Sphere::Sphere( double r ) : center_(), radius_(r), material_() {}
Sphere::Sphere( Vector v ) : center_(v), radius_( 0.0 ), material_() {}
Sphere::Sphere( Vector v, double r) : center_(v), radius_(r), material_() {}
Sphere::Sphere(Vector v, double r, Material mat) : center_(v), radius_(r), material_(mat) {}

// Analityczny algorytm testu kolizji promienia ze sferą.
// Opiera się na algebraicznym podstawieniu równania promienia P(t) = O + t*D
// do równania sfery (P - C)^2 = R^2.
// Prowadzi to do rozwiązania równania kwadratowego postaci at^2 + 2bt + c = 0.
IntersectionResult Sphere::intersect(const Ray& ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;

    // Wektor przesunięcia od środka sfery (C) do punktu startu promienia (O).
    Vector oc = ray.origin() - center_;

    // --- Zoptymalizowane współczynniki równania kwadratowego ---

    // a = D * D (zazwyczaj wynosi dokładnie 1.0, jeśli promień jest prawidłowo znormalizowany)
    double a = ray.direction().dotProduct(ray.direction());

    // b = (O - C) * D
    // Zastosowano tu krytyczną optymalizację: usunięto standardowe mnożenie przez 2.
    // Dzięki temu używamy tzw. "zredukowanej delty" (half-b), co pozwala oszczędzić
    // cenne cykle procesora na unikaniu operacji mnożenia i dzielenia przez 2 w pierwiastkach.
    double b = oc.dotProduct(ray.direction());

    // c = (O - C)^2 - R^2
    double c = oc.dotProduct(oc) - (radius_ * radius_);

    // Wyznacznik równania (Zredukowana Delta).
    // Określa fizyczną naturę przecięcia:
    // < 0: Promień omija sferę w przestrzeni (brak rzeczywistych pierwiastków).
    // = 0: Promień muska sferę w jednym punkcie (styczna).
    // > 0: Promień wchodzi w sferę i z niej wychodzi (2 punkty przecięcia).
    auto discriminant = b * b - a * c;

    if (discriminant >= 0) {
        // 1. Sprawdzenie pierwszego pierwiastka (odjęcie delty daje mniejsze 't').
        // Reprezentuje on przednią ścianę sfery - miejsce wejścia promienia.
        auto temp = (-b - std::sqrt(discriminant)) /  a;

        // Test Z-Buffera: czy punkt trafienia mieści się przed kamerą i przed limitem rysowania.
        if (temp < t_max && temp > t_min) {
            result.type = HIT;
            result.distance = temp;
            result.LPOINT = ray.origin() + temp * ray.direction();

            // Wektor normalny sfery w dowolnym punkcie na jej powierzchni to zawsze
            // znormalizowany wektor wycelowany od środka sfery do tego punktu uderzenia.
            result.intersectionLPOINTNormal = (result.LPOINT - center_).normalized();

            result.material = material_;
            return result;
        }

        // 2. Sprawdzenie drugiego pierwiastka (dodanie delty daje większe 't').
        // Reprezentuje on tylną ścianę sfery - miejsce wyjścia promienia.
        // Silnik uderzy w ten punkt, jeśli np. kamera (lub światło) znajduje się WEWNĄTRZ szkła.
        temp = (-b + std::sqrt(discriminant)) /  a;

        if (temp < t_max && temp > t_min) {
            result.type = HIT; // (W systemach szkła czasem ustawia się tu flagę INSIDE_PRIMITIVE)
            result.distance = temp;
            result.LPOINT = ray.origin() + temp * ray.direction();

            result.intersectionLPOINTNormal = (result.LPOINT - center_).normalized();

            result.material = material_;
            return result;
        }
    }

    // Zwracane, gdy oba pierwiastki leżały za kamerą (t < 0) lub za płaszczyzną odcięcia (t > t_max).
    return result;
}

// Narzędzie diagnostyczne wyświetlające pozycję i rozmiar sfery w logach konsoli.
std::ostream& operator<<(std::ostream& os, const Sphere& sph) {
    os << "Center: " << sph.center_
       << ", Radius: " << sph.radius_;
    return os;
}