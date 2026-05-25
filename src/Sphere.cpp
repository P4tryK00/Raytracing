#include "Sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere() : center_(), radius_( 0.0 ), material_() {}

Sphere::Sphere( double r ) : center_(), radius_(r), material_() {}

Sphere::Sphere( Vector v ) : center_(v), radius_( 0.0 ), material_() {}

Sphere::Sphere( Vector v, double r) : center_(v), radius_(r), material_() {}

Sphere::Sphere(Vector v, double r, Color color) : center_(v), radius_(r) {
    material_.baseColor = color;
}

IntersectionResult Sphere::intersect(const Ray& ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;

    Vector oc = ray.origin() - center_; 

    double a = ray.direction().dotProduct(ray.direction());
    double b = oc.dotProduct(ray.direction());
    
    double c = oc.dotProduct(oc) - (radius_ * radius_); 

    auto discriminant = b * b - a * c;

    if (discriminant >= 0) {
        auto temp = (-b - std::sqrt(discriminant)) /  a;

        if (temp < t_max && temp > t_min) {
            result.type = HIT;
            result.distance = temp;
            result.LPOINT = ray.origin() + temp * ray.direction();
            result.intersectionLPOINTNormal = (result.LPOINT - center_).normalized(); 
            
            result.material = material_; 
            return result;
        }

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
    return result;
}

std::ostream& operator<<(std::ostream& os, const Sphere& sph) {
    os << "Center: " << sph.center_
       << ", Radius: " << sph.radius_;
    return os;
}