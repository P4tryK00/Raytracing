#include "Vector.h"
#include <iostream>
#include <cmath>
#include <stdexcept>


Vector::Vector() : x(0.0), y(0.0), z(0.0) {}

Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

Vector::Vector(const Vector &v1, const Vector &v2) {
    x = v2.x - v1.x;
    y = v2.y - v1.y;
    z = v2.z - v1.z;
}

void Vector::operator+= ( const Vector &v ) {
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector::operator-= ( const Vector &v ) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

Vector Vector::operator+( const Vector &v ) const {
    Vector result = *this;
    result += v;
    return result;
}

Vector Vector::operator-( const Vector &v ) const {
    Vector result = *this;
    result -= v;
    return result;
}

Vector Vector::operator* ( double m ) const {
    return {x * m, y * m, z * m};
}


Vector Vector::operator/ ( double d ) const {
    if (d != 0.0) {
        return {x / d, y / d, z / d};
    }else {
        throw std::invalid_argument("Dividing by zero");
    }
}

void Vector::divide(double d) {
    if (d != 0.0) {
        x /= d;
        y /= d;
        z /= d;
    }else {
        throw std::invalid_argument("Dividing by zero");
    }
}

void Vector::scalarMultiply(double m) {
    x *= m;
    y *= m;
    z *= m;
}

double Vector::length() const {
    return std::sqrt( x*x + y*y + z*z );
}

Vector Vector::normalized() const {
    constexpr auto epsilon = 1e-8;
    auto length = this->length();
    if (length > epsilon) {
        Vector result = *this;
        result.divide(length);
        return result;
    }else {
        throw std::invalid_argument("Dividing by zero");
    }
}

void Vector::normalize() {
    constexpr auto epsilon = 1e-8;
    auto length = this->length();
    if (length > epsilon) {
        this->divide(length);
    }else {
        throw std::invalid_argument("Dividing by zero");
    }
}

double Vector::dotProduct(const Vector& v) const {
    return ( x * v.x ) + ( y * v.y ) + ( z * v.z );
}

Vector Vector::crossProduct(const Vector& v) const {

    return { ( y * v.z ) - ( z * v.y ), ( z * v.x ) - ( x * v.z ), ( x * v.y ) - ( y * v.x ) };
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << v.x << " " << v.y << " " << v.z;
    return os;
}