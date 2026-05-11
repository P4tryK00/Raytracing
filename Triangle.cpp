#include "Triangle.h"

#include <cmath>

Triangle::Triangle(Vector v0, Vector v1, Vector v2) : v0_(v0), v1_(v1), v2_(v2) {
    Vector edge1 = v1 - v0;
    Vector edge2 = v2 - v0;
    normal_ = edge1.crossProduct(edge2).normalized();
}

IntersectionResult Triangle::intersect(const Ray& ray, double range) const {
    IntersectionResult result;
    result.type = MISS;
    
    Vector edge1 = v1_ - v0_;
    Vector edge2 = v2_ - v0_;
    
    Vector h = ray.direction().crossProduct(edge2);
    auto a = edge1.dotProduct(h);
    
    constexpr auto EPSILON = 1e-8;
    
    if (std::abs(a) < EPSILON) {
        
        double distToPlane = normal_.dotProduct(ray.origin() - v0_);
        
        if (std::abs(distToPlane) < EPSILON) {
            
            Vector vertices[3] = {v0_, v1_, v2_};
            double closest_t = INFINITY;
            bool hit_found = false;

            for (int i = 0; i < 3; ++i) {
                Vector vStart = vertices[i];
                Vector vEnd = vertices[(i + 1) % 3];
                Vector E = vEnd - vStart;      
                Vector w = vStart - ray.origin();

                Vector DxE = ray.direction().crossProduct(E);
                double denom = normal_.dotProduct(DxE);

                if (std::abs(denom) > EPSILON) {
                    double t = normal_.dotProduct(w.crossProduct(E)) / denom;
                    double s = normal_.dotProduct(w.crossProduct(ray.direction())) / denom;

                   
                    if (t >= 0.0 && (range == 0.0 || t <= range) && s >= 0.0 && s <= 1.0) {
                        if (t < closest_t) {
                            closest_t = t;
                            hit_found = true;
                        }
                    }
                }
            }

            if (hit_found) {
                result.type = HIT;
                result.distance = closest_t;
                result.LPOINT = ray.origin() + ray.direction() * closest_t;
                result.intersectionLPOINTNormal = normal_;
                return result;
            }
            return result; 
        }
    }
    
    
    auto f = 1.0 / a;
    Vector s = ray.origin() - v0_;
    auto u = f * s.dotProduct(h);
    
    if (u <= EPSILON || u >= 1.0 - EPSILON) {
        return result;
    }
    
    Vector q = s.crossProduct(edge1);
    auto v = f * ray.direction().dotProduct(q);
    
    if ( v <= EPSILON || v + u >- 1.0 - EPSILON ) {
        return result;
    }
    
    auto t = f * edge2.dotProduct(q);
    
    if (t > -EPSILON) {
        if (range > -EPSILON && t > range) {
            return result;
        }
        
        if ( normal_.dotProduct(ray.direction()) < -EPSILON) {
            result.type = HIT;
        }else {
            result.type = INSIDE_PRIMITIVE;
        }
    
        result.distance = t;
        result.LPOINT = ray.origin() + ray.direction() * t;
        result.intersectionLPOINTNormal = normal_;
        
    }
    return result;
}