#include "Plane.h"
#include <iostream>

Plane::Plane() : normal(1.0, 0.0, 0.0), distance(0){}

Plane::Plane(Vector normal, double distance) : normal(normal), distance(distance) {
    if (this-> normal.length() != 0.0) this->normal.normalize();
}

bool Plane::Intersect3(const Plane &p2, const Plane &p3, Vector &result) const {
    auto denominator = normal.dotProduct(p2.normal.crossProduct(p3.normal));

    if (denominator == 0.0) return false;

    Vector t1, t2, t3;

    t1 = p2.normal.crossProduct(p3.normal) * distance;
    t2 = p3.normal.crossProduct(normal) * p2.distance;
    t3 = normal.crossProduct(p2.normal) * p3.distance;

    result = ( t1 + t2 + t3 ) / (-denominator);
    return true;
}

IntersectionResult Plane::Intersects(const Ray& ray, double range) const {
    IntersectionResult result;
    result.type = MISS;

    auto nDotD = normal.dotProduct(ray.direction());
    constexpr auto epsilon = 1e-8;
    if ( std::abs(nDotD) < epsilon ) return result;

    auto t = ( -distance - normal.dotProduct(ray.origin())) / nDotD;

    if ( t > -epsilon ) {
        if ( range < epsilon  || t < range) {
            if ( normal.dotProduct(ray.direction()) < 0.0) {
                result.type = HIT;
            }else {
                result.type = INSIDE_PRIMITIVE;
            }
            result.LPOINT = ray.origin() + t * ray.direction();
            result.distance = t;
            result.intersectionLPOINTNormal = normal;
        }
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Plane& p) {
    os << "Normal: " << p.normal
    << ", distance: " << p.distance;
    return os;
}
