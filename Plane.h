#ifndef RAYTRACING_PLANE_H
#define RAYTRACING_PLANE_H

#include <iosfwd>
#include "Vector.h"
#include "Ray.h"

enum IntersectionType {
    MISS,
    HIT,
    INSIDE_PRIMITIVE
};

struct IntersectionResult {
    IntersectionType type;
    Vector LPOINT;
    double distance;
    Vector intersectionLPOINTNormal;
};

class Plane {
public:
    Plane();
    Plane(Vector normal, double distance);

    [[nodiscard]] Vector Normal() const {return normal;}
    void Normal(const Vector& n) { normal = n;}

    [[nodiscard]] double Distance() const { return distance;}
    void Distance(double d) { distance = d; }

    bool Intersect3(const Plane &p2, const Plane &p3, Vector &result) const;
    [[nodiscard]] IntersectionResult Intersects(const Ray& ray, double range) const;

    friend std::ostream& operator<<(std::ostream& os, const Plane& p);

private:
    Vector normal;
    double distance;
};


#endif //RAYTRACING_PLANE_H
