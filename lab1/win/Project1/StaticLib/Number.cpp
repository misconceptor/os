//#include <iostream>
#include "Number.h"

Number::Number() :val(0) {}
Number::Number(double v) :val(v) {}// { std::cout << "ctor num: " << val << std::endl; }
const Number Number::ZERO(0.0);
const Number Number::ONE(1.0);
Number::Number(const Number& rhs) {
    val = rhs.getVal();
    //std::cout << "copy num: " << val << std::endl;
}
Number& Number::operator =(const Number& r) {
    val = r.val;
    //std::cout << "operator= " << val << std::endl;
    return *this;
}
double Number::getVal() const {
    return val;
}
bool Number::operator==(const Number&r)const{
    return abs(val-r.val)<1e-9;
}
Number Number::operator -(const Number& r)const {
    return (Number)(val - r.val);
}
Number Number::operator +(const Number&r)const{
    return (Number)(val+r.val);
}
Number Number::operator *(const Number&r)const{
    return (Number)(val*r.val);
}
Number Number::operator /(const Number&r)const{
    if(r.val==0.0)throw std::runtime_error("div0");
    return (Number)(val/r.val);
}

std::istream& operator>>(std::istream&is,Number&num){
    is>>num.val;
    return is;
}
std::ostream& operator<<(std::ostream&os,const Number&num){
    os<<num.val;
    return os;
}
Number Number::mkNum(double v){
    return Number(v);
}
