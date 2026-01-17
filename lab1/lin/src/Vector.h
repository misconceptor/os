#ifndef VECTOR_H
#define VECTOR_H
#include "Number.h"

class Vector {
private:
    Number x, y;
public:
    Vector();
    Vector(const Number& _x, const Number& _y);
    
    Number getX() const;
    Number getY() const;
    
    Vector add(const Vector& other) const;
    
    Number angle() const;
    Number radius() const;
    
    void print() const;
};
extern const Vector VectorZero;
extern const Vector VectorOne;
#endif