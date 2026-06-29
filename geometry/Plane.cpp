#include "Plane.h"
#include <iostream>
#include <cmath>

Plane::Plane() : normal_(1.0, 0.0, 0.0), distance_(0), material_() {}

// Konstruktory oparte na równaniu płaszczyzny: N * P + d = 0
// Gdzie 'N' to wektor normalny, 'P' to dowolny punkt na płaszczyźnie, a 'd' to odległość od środka układu.
Plane::Plane(Vector normal, double distance) : normal_(normal), distance_(distance), material_() {
    // Normalizacja wektora płaszczyzny jest krytyczna. Jeśli N nie ma długości 1.0,
    // wyliczana odległość uderzenia promienia (t) ulegnie matematycznemu zniekształceniu.
    this->normal_.normalize();
}

Plane::Plane(Vector normal, double distance, Material mat) : normal_(normal), distance_(distance), material_(mat) {
    this->normal_.normalize();
}

// Algorytm analitycznego poszukiwania punktu przecięcia trzech płaszczyzn w przestrzeni 3D.
// Opiera się na rozwiązaniu układu 3 równań liniowych przy użyciu iloczynów wektorowych i mieszanych.
bool Plane::Intersect3(const Plane &p2, const Plane &p3, Vector &result) const {
    // Obliczenie wyznacznika (mianownika). Iloczyn mieszany (N1 dot (N2 cross N3))
    // jest równy objętości równoległościanu rozpiętego na tych 3 wektorach normalnych.
    auto denominator = normal_.dotProduct(p2.normal_.crossProduct(p3.normal_));

    // Jeśli mianownik to 0, płaszczyzny nie przecinają się w jednym punkcie (są np. równoległe).
    if (denominator == 0.0) return false;

    // Rozwiązanie z zastosowaniem reguły Cramera (Cramer's Rule) dla przecięcia przestrzennego.
    auto t1 = p2.normal_.crossProduct(p3.normal_) * distance_;
    auto t2 = p3.normal_.crossProduct(normal_) * p2.distance_;
    auto t3 = normal_.crossProduct(p2.normal_) * p3.distance_;

    result = ( t1 + t2 + t3 ) / (-denominator);
    return true;
}

// Test intersekcji pojedynczego promienia (Ray) z nieskończoną płaszczyzną.
IntersectionResult Plane::intersect(const Ray& ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;

    // Zmienna w mianowniku: Iloczyn skalarny wektora normalnego i wektora kierunku promienia.
    auto nDotD = normal_.dotProduct(ray.direction());
    constexpr auto epsilon = 1e-8;

    // Jeśli iloczyn skalarny jest bliski zeru, wektory są prostopadłe.
    // Oznacza to, że promień "ślizga się" równolegle do płaszczyzny i nigdy w nią nie uderzy.
    if ( std::abs(nDotD) < epsilon ) return result;

    // Równanie t = -(d + N * O) / (N * D) wywiedzione z wstawienia równania promienia
    // parametrycznego P = O + tD do równania płaszczyzny N * P + d = 0.
    auto t = ( -distance_ - normal_.dotProduct(ray.origin())) / nDotD;

    // Test Z-Buffera: sprawdzenie, czy uderzenie mieści się w akceptowalnym zakresie odległości.
    if ( t > t_min && t < t_max ) {

        // --- Sprawdzenie strony uderzenia (Backface / Frontface) ---
        // Jeśli promień leci "pod prąd" wektora normalnego (nDotD < 0), uderza w przód płaszczyzny.
        if ( normal_.dotProduct(ray.direction()) < -epsilon) {
            result.type = HIT;
        } else {
            // W przeciwnym razie uderzył od spodu / od tyłu.
            result.type = INSIDE_PRIMITIVE;
        }

        // Zapakowanie wyliczonych, krytycznych danych do struktury zwrotnej dla Renderera.
        result.distance = t;
        result.LPOINT = ray.origin() + t * ray.direction();
        result.intersectionLPOINTNormal = normal_; // Płaszczyzna ma stałą normalną w każdym punkcie

        result.material = material_;
    }

    return result;
}

// Przeciążony operator strumienia do zrzutu parametrów płaszczyzny w debugerze konsolowym.
std::ostream& operator<<(std::ostream& os, const Plane& p) {
    os << "Normal: " << p.normal_
       << ", distance: " << p.distance_;
    return os;
}