#ifndef NUMBER_H
#define NUMBER_H
#include <stdexcept>
#include <iostream>
class Number{
public:
    double val;
    static const Number ZERO;
    static const Number ONE;
    Number():val(0){}
    Number(double v):val(v){}
    bool operator==(const Number&r)const;
    Number operator-(const Number&r)const;
    Number operator+(const Number&r)const;
    Number operator*(const Number&r)const;
    Number operator/(const Number&r)const;
    Number operator=(const Number&r);
    friend std::istream& operator>>(std::istream& is, Number& num);
    friend std::ostream& operator<<(std::ostream&os,const Number&num);
    static Number mkNum(double v);
};
#endif // NUMBER_H