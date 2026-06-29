#include "Vector.h"
#include <iostream>
#include <cmath>
#include <stdexcept>


// Domyślny wektor zerowy. 
Vector::Vector() : x(0.0), y(0.0), z(0.0) {}

// Konstruktor punktu/wektora w przestrzeni trójwymiarowej.
Vector::Vector(double x, double y, double z) : x(x), y(y), z(z) {}

// Konstruktor wektora kierunkowego (od punktu v1 do punktu v2).
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

// Dodawanie wektorów (np. przesunięcie punktu o wektor kierunkowy O + t*D w równaniu promienia).
Vector Vector::operator+( const Vector &v ) const {
    Vector result = *this;
    result += v;
    return result;
}

// Odejmowanie wektorów (wyznaczanie wektora między dwoma punktami w przestrzeni).
Vector Vector::operator-( const Vector &v ) const {
    Vector result = *this;
    result -= v;
    return result;
}

// Skalowanie wektora. Zmienia jego długość, zachowując kierunek
// (np. wydłużenie promienia o wyliczony dystans trafienia w obiekt).
Vector Vector::operator* ( double m ) const {
    return {x * m, y * m, z * m};
}

// Dzielenie przez skalar ze sprzętowym zabezpieczeniem przed powstawaniem wartości
// nieokreślonych (NaN - Not a Number), które mogłyby zepsuć cały wygenerowany obraz.
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

// Zwraca długość wektora.
// Używane do sprawdzania rzeczywistej odległości, np. limitu dystansu dla promieni cienia (t_max).
double Vector::length() const {
    return std::sqrt( x*x + y*y + z*z );
}

// Zwraca nową kopię znormalizowanego wektora (o długości dokładnie 1.0).
// Używamy Epsilona do wyłapania wektorów mikroskopijnych, których normalizacja
// doprowadziłaby do błędu zmiennoprzecinkowego i wyrzucenia wyjątku.
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

// Modyfikuje oryginalny wektor, sprowadzając jego długość do 1.0.
// Normalizacja jest absolutnie krytyczna dla wektorów normalnych (N) i kierunkowych (D).
// Bez niej wszystkie iloczyny skalarne w modelu Phonga zwracałyby błędne jasności.
void Vector::normalize() {
    constexpr auto epsilon = 1e-8;
    auto length = this->length();
    if (length > epsilon) {
        this->divide(length);
    }else {
        throw std::invalid_argument("Dividing by zero");
    }
}

// --- KLUCZOWE OPERACJE ALGEBRY LINIOWEJ ---

// Iloczyn skalarny (Dot Product).
double Vector::dotProduct(const Vector& v) const {
    return ( x * v.x ) + ( y * v.y ) + ( z * v.z );
}

// Iloczyn wektorowy (Cross Product).
// Zwraca wektor idealnie prostopadły do dwóch wektorów wejściowych.
Vector Vector::crossProduct(const Vector& v) const {
    return { ( y * v.z ) - ( z * v.y ), ( z * v.x ) - ( x * v.z ), ( x * v.y ) - ( y * v.x ) };
}

std::ostream& operator<<(std::ostream& os, const Vector& v) {
    os << v.x << " " << v.y << " " << v.z;
    return os;
}