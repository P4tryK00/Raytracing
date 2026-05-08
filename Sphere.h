#ifndef VECTOR_SPHARE_H
#define VECTOR_SPHARE_H

#include "Vector.h"
#include "Ray.h"
class Sphere {
public:
    Sphere();
    Sphere(double r);
    Sphere(Vector v);
    Sphere(Vector v, double r);

    bool Hit(Ray ray, double t_min, double t_max) const;
    Vector Center() const { return Center_; }
    void Center(Vector o) { Center_ = o; }
    double Radius() const { return Radius_; }
    void Radius()(double r) { Radius_ = r; }

    friend std::ostream& operator<<(std::ostream& os, const Sphere& sph);
    private:
    Vector Center_;
    double Radius_;
};


#endif //VECTOR_SPHARE_H
