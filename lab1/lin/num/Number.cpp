//#include <iostream>
#include "Number.h"
bool Number::operator==(const Number&r)const{
    return abs(val-r.val)<1e-9;
}
Number Number::operator -(const Number&r)const{
    return (Number)(val-r.val);
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
Number Number::operator =(const Number&r){
    val=r.val;
    return *this;
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
const Number Number::ZERO(Number(0.0));
const Number Number::ONE(Number(1.0));