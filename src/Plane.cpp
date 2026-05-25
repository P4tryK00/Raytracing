#include "Plane.h"
#include <iostream>
#include <cmath>

Plane::Plane() : normal_(1.0, 0.0, 0.0), distance_(0), material_() {}

Plane::Plane(Vector normal, double distance) : normal_(normal), distance_(distance), material_() {
    this->normal_.normalize();
}

Plane::Plane(Vector normal, double distance, Material mat) : normal_(normal), distance_(distance), material_(mat) {
    this->normal_.normalize();
}

bool Plane::Intersect3(const Plane &p2, const Plane &p3, Vector &result) const {
    auto denominator = normal_.dotProduct(p2.normal_.crossProduct(p3.normal_));

    if (denominator == 0.0) return false;

    

    auto t1 = p2.normal_.crossProduct(p3.normal_) * distance_;
    auto t2 = p3.normal_.crossProduct(normal_) * p2.distance_;
    auto t3 = normal_.crossProduct(p2.normal_) * p3.distance_;

    result = ( t1 + t2 + t3 ) / (-denominator);
    return true;
}

IntersectionResult Plane::intersect(const Ray& ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;

    auto nDotD = normal_.dotProduct(ray.direction());
    constexpr auto epsilon = 1e-8;
    
    if ( std::abs(nDotD) < epsilon ) return result;

    auto t = ( -distance_ - normal_.dotProduct(ray.origin())) / nDotD;

    if ( t > t_min && t < t_max ) {
        
        if ( normal_.dotProduct(ray.direction()) < -epsilon) {
            result.type = HIT;
        } else {
            result.type = INSIDE_PRIMITIVE;
        }
        
        result.distance = t;
        result.LPOINT = ray.origin() + t * ray.direction();
        result.intersectionLPOINTNormal = normal_;
        
        result.material = material_;
    }
    
    return result;
}

std::ostream& operator<<(std::ostream& os, const Plane& p) {
    os << "Normal: " << p.normal_
       << ", distance: " << p.distance_;
    return os;
}