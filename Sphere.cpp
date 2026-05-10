#include "Sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere() : Center_(), Radius_( 0.0 ) {}

Sphere::Sphere( double r ) : Center_(), Radius_(r) {}

Sphere::Sphere( Vector v ) : Center_(v), Radius_( 0.0 ) {}

Sphere::Sphere( Vector v, double r) : Center_(v), Radius_(r) {}

IntersectionResult Sphere::Hit(Ray ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;

    Vector oc = ray.origin() - Center_;

    double a = ray.direction().dotProduct(ray.direction());
    double b = oc.dotProduct(ray.direction());
    double c = oc.dotProduct(oc) - (Radius_ * Radius_);

    auto discriminant = b * b - a * c;

    if (discriminant >= 0) {
        auto temp = (-b - std::sqrt(discriminant)) /  a;

        if (temp < t_max && temp > t_min) {
            result.type = HIT;
            result.distance = temp;
            result.LPOINT = ray.origin() + temp * ray.direction();
            result.intersectionLPOINTNormal = (result.LPOINT - Center_).normalized();
            return result;
        }

        temp = (-b + std::sqrt(discriminant)) /  a;

        if (temp < t_max && temp > t_min) {
            result.type = HIT;
            result.distance = temp;
            result.LPOINT = ray.origin() + temp * ray.direction();
            result.intersectionLPOINTNormal = (result.LPOINT - Center_).normalized();
            return result;
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Sphere& sph) {
    os << "Center: " << sph.Center_
    << ", Radius: " << sph.Radius_;
    return os;
}


