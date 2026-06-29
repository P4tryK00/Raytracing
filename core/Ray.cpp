#include "Ray.h"
#include <iostream>

// Domyślny konstruktor promienia.
Ray::Ray() : Distance_ (0.0) {
    Direction_ = Vector(1.0, 0.0, 0.0);
}

// Konstruktor promienia oparty na wektorze łączącym dwa punkty.
Ray::Ray(Vector origin, Vector direction) {
    Origin_ = origin;
    Distance_ = direction.length();

  
    if (Distance_ != 0.0) {
        Direction_ = direction.normalized();
    } else {
        Direction_ = Vector(1.0, 0.0, 0.0);
    }

    // Wyliczenie punktu docelowego
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

    // Normalizacja i zabezpieczenie przed podaniem zdegenerowanego wektora wejściowego.
    if ( direction.length() != 0.0 ) {
        Direction_ = direction.normalized();
    } else {
        Direction_ = Vector(1.0, 0.0, 0.0);
    }

    Destination_ = Origin_ + (Direction_ * distance);
}
//debug 
std::ostream& operator<<(std::ostream& os, const Ray& ray) {
    os << "Origin: " << ray.Origin_
    << ", Dir: " << ray.Direction_
    << ", Distance: " << ray.Distance_
    << ", Dest: " << ray.Destination_;
    return os;
}