#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H

#include <iosfwd>

class Vector {
    public:
    double x,y,z;


    Vector();

    Vector(double x, double y, double z);

    Vector(const Vector& v1, const Vector& v2);

    void operator+=(const Vector& v);
    void operator-=(const Vector& v);
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;

    Vector operator*(double m) const;
    Vector operator/(double d) const;

    void divide(double d);
    void scalarMultiply(double m);

    // Skraca wektor do długości dokładnie 1.0 zachowując jego zwrot.
    // wazne dla wektorów normalnych (N) i kierunków światła (L),
    // by iloczyn skalarny w modelu Phonga zwracał precyzyjny cosinus kąta padania.
    void normalize();


    // Zwraca  długość wektora (np. wyliczanie fizycznego dystansu w przestrzeni).
    [[nodiscard]] double length() const;

    // Zwraca znormalizowaną kopię wektora, nie modyfikując obiektu bazowego.
    [[nodiscard]] Vector normalized() const;


    // Iloczyn skalarny (Dot Product).
    // Zwraca skalar proporcjonalny do cosinusa kąta między znormalizowanymi wektorami. 
    // Służy do cieniowania matowego (Prawo Lamberta), obliczania rozbłysków (Phong)
    [[nodiscard]] double dotProduct(const Vector& v) const;

    // Iloczyn wektorowy (Cross Product).
    // Generuje nowy wektor idealnie prostopadły do obu wektorów wejściowych.
    // Niezbędny do wyznaczania wektorów normalnych dla trójkątów i budowania
    [[nodiscard]] Vector crossProduct(const Vector& v) const;
};

// Globalny operator zapewniający przemienność mnożenia (skalar * wektor).
inline Vector operator*(double m, const Vector& v) {
    return v * m;
}

std::ostream& operator<<(std::ostream& os, const Vector& v);

#endif //VECTOR_VECTOR_H