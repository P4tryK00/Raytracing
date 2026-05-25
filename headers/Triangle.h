#ifndef RAYTRACING_TRIANGLE_H
#define RAYTRACING_TRIANGLE_H

#include "Vector.h"
#include "Ray.h"
#include "IntersectionResult.h"

class Triangle {
public: 
    Triangle(Vector v0, Vector v1, Vector v2, Material mat);
    [[nodiscard]] IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;
    [[nodiscard]] Vector Normal() const { return normal_;}
    

private:
    Vector v0_, v1_, v2_;
    Vector normal_;
    Material material_;

};


#endif //RAYTRACING_TRIANGLE_H
