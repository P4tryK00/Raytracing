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
    void normalize();

    [[nodiscard]] double length() const;
    [[nodiscard]] Vector normalized() const;


    [[nodiscard]] double dotProduct(const Vector& v) const;
    [[nodiscard]] Vector crossProduct(const Vector& v) const;
};

//wynik ten sam wektor * m ale zapis m * wektor, po to to
inline Vector operator*(double m, const Vector& v) {
    return v * m;
}

std::ostream& operator<<(std::ostream& os, const Vector& v);



#endif //VECTOR_VECTOR_H
