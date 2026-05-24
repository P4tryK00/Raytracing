#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H

#include "Vector.h"
#include "Ray.h"

class Triangle {
public: 
    Triangle(Vector v0, Vector v1, Vector v2);
    [[nodiscard]] IntersectionResult intersect(const Ray& ray, double range = 0) const;
    [[nodiscard]] Vector Normal() const { return normal_;}
    

private:
    Vector v0_, v1_, v2_;
    Vector normal_;

};


#endif //RAYTRACING_TRIANGLE_H
