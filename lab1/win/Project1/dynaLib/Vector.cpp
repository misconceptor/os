#define VECTOR_EXPORTS
#include "Vector.h"
#include <math.h>
Vector::Vector(const Number& _x,const Number& _y){
    x=_x;
    y=_y;
    //std::cout << "ctor " << x.getVal() << ' ' << y.getVal() << std::endl;
}
Vector::Vector(const Vector& other) : x(other.x), y(other.y) {}
Number Vector::radius()const {
    double xx = x.getVal() * x.getVal();
    double yy = y.getVal() * y.getVal();
    return Number(std::sqrt(xx + yy));
}
Vector& Vector::operator=(const Vector& other) {
    //std::cout << "other: ";
    //std::cout << other.x.getVal() << ' ' << other.y.getVal() << std::endl;
    if (this != &other) {
        x = other.x;
        y = other.y;
    }
    return *this;
}
Number Vector::angle()const{
    return std::atan2(y.getVal(), x.getVal());
}
Vector Vector::add(const Vector& rhs) const {
    return Vector(x + rhs.x, y + rhs.y);
}
const Vector V_ZERO(Number(0.0), Number(0.0));
const Vector V_ONE(Number(1.0), Number(1.0));