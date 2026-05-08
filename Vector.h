#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H



class Vector {
    public:
    double x,y,z;
    Vector();
    Vector(double x, double y, double z);
    Vector(const Vector& v1, const Vector& v2);

    void operator+=(const Vector& v);
    void operator-=(const Vector& v);

    void divide(double d);
    void scalarMultiply(double m);

    double length() const;
    void normalize();


    double dotProduct(const Vector& v) const;

    Vector crossProduct(const Vector& v) const;

};



#endif //VECTOR_VECTOR_H
