#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include <iosfwd>
#include "Vector.h"
#include "Ray.h"
#include "IntersectionResult.h"



class Plane {
public:
    Plane();
    Plane(Vector normal, double distance);
    Plane(Vector normal, double distance, Color color);

    [[nodiscard]] Vector Normal() const {return normal_;}
    void Normal(const Vector& n) { normal_ = n;}

    [[nodiscard]] double Distance() const { return distance_;}
    void Distance(double d) { distance_ = d; }

    bool Intersect3(const Plane &p2, const Plane &p3, Vector &result) const;
    [[nodiscard]] IntersectionResult intersect(const Ray& ray, double t_min, double t_max) const;

    friend std::ostream& operator<<(std::ostream& os, const Plane& p);

private:
    Vector normal_;
    double distance_;
    Material material_;
};


#endif //RAYTRACING_PLANE_H
