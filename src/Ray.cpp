#include "Ray.h"
#include <iostream>

Ray::Ray() : Distance_ (0.0) {
    Direction_ = Vector(1.0, 0.0, 0.0);
}

Ray::Ray(Vector origin, Vector direction) {
    Origin_ = origin;
    Distance_ = direction.length();
    if (Distance_ != 0.0) {
        Direction_ = direction.normalized();
    }else {
        Direction_ = Vector(1.0, 0.0, 0.0);
    }

    Destination_ = Origin_ + (Direction_ * Distance_);
}

Ray::Ray(Vector origin, double distance) {
    Origin_ = origin;
    Distance_ = distance;
    Direction_ = Vector(1.0, 0.0, 0.0);
    Destination_ = Origin_ + (Direction_ * Distance_);
}

Ray::Ray(Vector origin, Vector direction, double distance) {
    Origin_ = origin;
    Distance_ = distance;

    if ( direction.length() != 0.0 ) {
        Direction_ = direction.normalized();
    }else {
        Direction_ = Vector(1.0, 0.0, 0.0);
    }

    Destination_ = Origin_ + (Direction_ * distance);
}

std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << "Origin: " << ray.Origin_
    << ", Dir: " << ray.Direction_
    << ", Distance: " << ray.Distance_
    << ", Dest: " << ray.Destination_;
    return os;
}