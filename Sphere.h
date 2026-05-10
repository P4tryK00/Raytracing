#ifndef VECTOR_SPERE_H
#define VECTOR_SPERE_H

#include "Vector.h"
#include "Ray.h"
#include <iosfwd>

class Sphere {
public:
    Sphere();
    Sphere(double r);
    Sphere(Vector v);
    Sphere(Vector v, double r);

    [[nodiscard]] bool Hit(Ray ray, double t_min, double t_max) const;
    [[nodiscard]] Vector Center() const { return Center_; }
    void Center(Vector o) { Center_ = o; }
    [[nodiscard]] double Radius() const { return Radius_; }
    void Radius(double r) { Radius_ = r; }

    friend std::ostream& operator<<(std::ostream& os, const Sphere& sph);
    private:
    Vector Center_;
    double Radius_;
};


#endif //VECTOR_SPERE_H
