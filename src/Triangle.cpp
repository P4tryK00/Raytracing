#include "Triangle.h"

#include <cmath>

Triangle::Triangle(Vector v0, Vector v1, Vector v2, Material mat) : v0_(v0), v1_(v1), v2_(v2), material_(mat) {
    Vector edge1 = v1 - v0;
    Vector edge2 = v2 - v0;
    normal_ = edge1.crossProduct(edge2).normalized();
}

IntersectionResult Triangle::intersect(const Ray& ray, double t_min, double t_max) const {
    IntersectionResult result;
    result.type = MISS;
    
    Vector edge1 = v1_ - v0_;
    Vector edge2 = v2_ - v0_;
    
    Vector h = ray.direction().crossProduct(edge2);
    auto a = edge1.dotProduct(h);
    
    constexpr auto EPSILON = 1e-8;
    
    if (std::abs(a) < EPSILON) {
        return result;
    }
    
    
    auto f = 1.0 / a;
    Vector s = ray.origin() - v0_;
    auto u = f * s.dotProduct(h);
    
    if (u < -EPSILON || u > 1.0 + EPSILON ) {
        return result;
    }
    
    Vector q = s.crossProduct(edge1);
    auto v = f * ray.direction().dotProduct(q);
    
    if ( v < -EPSILON || v + u > 1.0 + EPSILON) {
        return result;
    }
    
    auto t = f * edge2.dotProduct(q);
    
    if (t > t_min && t < t_max) {
        if ( normal_.dotProduct(ray.direction()) < -EPSILON) {
            result.type = HIT;
        }else {
            result.type = INSIDE_PRIMITIVE;
        }
    
        result.distance = t;
        result.LPOINT = ray.origin() + ray.direction() * t;
        result.intersectionLPOINTNormal = normal_;
        
        result.material = material_;
        
    }
    return result;
}