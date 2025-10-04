#include "Vector.h"
#include <iostream>

Vector::Vector() : x(Number::Zero()), y(Number::Zero()) {}

Vector::Vector(const Number& _x, const Number& _y)
    : x(_x), y(_y) {}

Vector::Vector(const Vector& rhs)
    : x(rhs.x), y(rhs.y) {}

Number Vector::getX() const {
    return x;
}

Number Vector::getY() const {
    return y;
}

void Vector::print() const {
    std::cout << '(' << x.getVal() << ' ' << y.getVal() << ")\n";
}
Vector Vector::add(const Vector& other) const {
    Number newX = x.add(other.x);
    Number newY = y.add(other.y);
    return Vector(newX, newY);
}
Vector Vector::operator+(const Vector& other) const {
    return this->add(other);
}
Number Vector::angle() const {
    double rad= std::atan2(y.getVal(),x.getVal());
    return Number(rad);
}
Number Vector::radius() const {
    double r = std::sqrt(x.getVal()*x.getVal()+
    y.getVal()*y.getVal());
    return Number(r);
}
const Vector& VOne() {
    static const Vector v(Number::One(), Number::One());
    return v;
}
const Vector& VZero() {
    static const Vector v(Number::Zero(), Number::Zero());
    return v;
}