#ifndef VECTOR_RAY_H
#define VECTOR_RAY_H

#include <iosfwd>

#include "Vector.h"

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

class Ray {
public:
    Ray();
    Ray(Vector origin, Vector direction);
    Ray(Vector origin, double distance);
    Ray(Vector origin, Vector direction, double distance);

    Vector origin() const { return Origin_; }
    void origin(Vector o) { Origin_ = o; }
    Vector destination() const { return Destination_; }
    void destination(Vector d) { Destination_ = d;}
    Vector direction() const { return Direction_; }
    void direction(Vector d) { Direction_ = d; }
    double distance() const { return Distance_; }
    void distance(double d) { Distance_ = d; }

    friend std::ostream& operator<<(std::ostream& os, const Ray& ray);

private:
    Vector Origin_;
    Vector Destination_;
    Vector Direction_;
    double Distance_;
    



};



#endif //VECTOR_RAY_H
