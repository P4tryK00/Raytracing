#include "Vector.h"
#include <iostream>
#include <cmath>
#include <stdexcept>


Vector::Vector() : x(0.0), y(0.0), z(0.0) {}

Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

Vector::Vector(const Vector &v1, const Vector &v2) {
    this->x = v2.x - v1.x;
    this->y = v2.y - v1.y;
    this->z = v2.z - v1.z;
}

void Vector::operator+= ( const Vector &v ) {
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
}

void Vector::operator-= ( const Vector &v ) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vector::divide(double d) {
    if (d != 0.0) {
        this->x /= d;
        this->y /= d;
        this->z /= d;
    }else {
        throw std::invalid_argument("Dividing by zero");
    }
}

void Vector::scalarMultiply(double m) {
    this->x *= m;
    this->y *= m;
    this->z *= m;
}

double Vector::length() const {
    return std::sqrt( x*x + y*y + z*z );
}

void Vector::normalize() {
    auto length = this->length();
    if (length != 0.0) {
        this->divide(length);
    }else {
        throw std::invalid_argument("Dividing by zero");
    }
}

double Vector::dotProduct(const Vector& v) const {
    return ( this->x * v.x ) + ( this->y * v.y ) + ( this->z * v.z );
}

Vector Vector::crossProduct(const Vector& v) const {
    Vector result;

    result.x = this->y * v.z - this->z * v.y;
    result.y = this->z * v.x - this->x * v.z;
    result.z = this->x * v.y - this->y * v.x;

    return result;
}
