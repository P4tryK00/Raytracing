#include "Ray.h"

Ray::Ray() : Distance_ (0.0) {}

Ray::Ray(Vector origin, Vector direction) {
    Origin_ = origin;
    Direction_ = direction;
}

Ray::Ray(Vector origin, double distance) {
    Origin_ = origin;
    Distance_ = distance;
}