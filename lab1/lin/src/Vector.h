#ifndef VECTOR_H
#define VECTOR_H
#include "Number.h"
#include <cmath>
class Vector {
private:
    Number x, y;
public:
    Vector();
    Vector(const Number& _x, const Number& _y);
    Vector(const Vector& rhs);
    Number getX() const;
    Number getY() const;
    Vector add(const Vector& other) const;
    Vector operator+(const Vector& other) const;
    Number angle() const;
    Number radius() const;
    void print() const;
};

const Vector& VOne();
const Vector& VZero();

#endif // VECTOR_H
