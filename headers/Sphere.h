#ifndef VECTOR_SPERE_H
#define VECTOR_SPERE_H

#include "Vector.h"
#include "Ray.h"
#include <iosfwd>
#include "IntersectionResult.h"


class Sphere {
public:
    Sphere();
    Sphere(double r);
    Sphere(Vector v);
    Sphere(Vector v, double r);
    Sphere(Vector v, double r, Material mat);


    [[nodiscard]] IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;
    [[nodiscard]] Vector Center() const { return center_; }
    void Center(Vector o) { center_ = o; }
    [[nodiscard]] double Radius() const { return radius_; }
    void Radius(double r) { radius_ = r; }

    friend std::ostream& operator<<(std::ostream& os, const Sphere& sph);
    private:
    Vector center_;
    double radius_;
    Material material_;
};


#endif //VECTOR_SPERE_H
