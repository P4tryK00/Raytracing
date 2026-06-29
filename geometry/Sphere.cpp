#include "Sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere() : center_(), radius_( 0.0 ), material_() {}
Sphere::Sphere( double r ) : center_(), radius_(r), material_() {}
Sphere::Sphere( Vector v ) : center_(v), radius_( 0.0 ), material_() {}
Sphere::Sphere( Vector v, double r) : center_(v), radius_(r), material_() {}
Sphere::Sphere(Vector v, double r, Material mat) : center_(v), radius_(r), material_(mat) {}

//algorytm testu kolizji promienia ze sferą.
IntersectionResult Sphere::intersect(const Ray& ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;

    // Wektor przesunięcia od środka sfery (C) do punktu startu promienia (O).
    Vector oc = ray.origin() - center_;
    
    // a = D * D 
    double a = ray.direction().dotProduct(ray.direction());

    // b = (O - C) * D
    double b = oc.dotProduct(ray.direction());

    // c = (O - C)^2 - R^2
    double c = oc.dotProduct(oc) - (radius_ * radius_);

    // < 0: Promień omija sferę  
    // = 0: Promień muska sferę w jednym punkcie (styczna).
    // > 0: Promień wchodzi w sferę i z niej wychodzi (2 punkty przecięcia).
    auto discriminant = b * b - a * c;

    if (discriminant >= 0) {
        // 1. Sprawdzenie pierwszego pierwiastka (odjęcie delty daje mniejsze 't').
        // Reprezentuje on przednią ścianę sfery - miejsce wejścia promienia.
        auto temp = (-b - std::sqrt(discriminant)) /  a;

        // Test czy punkt trafienia mieści się przed kamerą i przed limitem rysowania.
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
        temp = (-b + std::sqrt(discriminant)) /  a;

        if (temp < t_max && temp > t_min) {
            result.type = HIT; 
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

std::ostream& operator<<(std::ostream& os, const Sphere& sph) {
    os << "Center: " << sph.center_
       << ", Radius: " << sph.radius_;
    return os;
}