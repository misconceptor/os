#include "Vector.h"
#include <math.h>
Vector::Vector(const Number&_x,const Number&_y){
    x=_x;
    y=_y;
}
Number Vector::radius()const {
    Number r = static_cast<Number>(std::sqrt(x.val*x.val+y.val*y.val));
    return r;
}
Number Vector::angle()const{
    Number a=std::atan2(y.val,x.val);
    return a;
}
Vector Vector::operator+(const Vector&rhs)const{
    return Vector(x+rhs.x, y+rhs.y);
}
Vector Vector::operator=(const Vector&rhs){
    x=rhs.x;
    y=rhs.y;
    return *this;
}
std::ostream&operator<<(std::ostream&os,const Vector&v){
    os<<"("<<v.x<<", "<<v.y<<")";
    return os;
}
std::istream&operator>>(std::istream& is,Vector&v){
    is>>v.x>>v.y;
    return is;
}
//const Vector Vector::ZERO(Number::ZERO, Number::ZERO);
//const Vector Vector::ONE(Number::ONE, Number::ONE);
const Vector Vector::ZERO(Number(0), Number(0));
const Vector Vector::ONE(Number(1), Number(1));
