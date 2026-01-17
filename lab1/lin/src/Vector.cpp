#include "Vector.h"
#include <iostream>
#include <cmath>

Vector::Vector() : x(Number::Zero()), y(Number::Zero()) {}
Vector::Vector(const Number& _x, const Number& _y) : x(_x), y(_y) {}

Number Vector::getX() const { return x; }
Number Vector::getY() const { return y; }

Vector Vector::add(const Vector& other) const {
    return Vector(x.add(other.x), y.add(other.y));
}

Number Vector::angle() const {
    return createNumber(std::atan2(y.getVal(), x.getVal()));
}

Number Vector::radius() const {
    Number xSquared = x.mul(x);
    Number ySquared = y.mul(y);
    return createNumber(std::sqrt(xSquared.add(ySquared).getVal()));
}

void Vector::print() const {
    std::cout << "(" << x.getVal() << ", " << y.getVal() << ")" << std::endl;
}

const Vector VectorZero(Number::Zero(), Number::Zero());
const Vector VectorOne(Number::One(), Number::One());