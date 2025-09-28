#pragma once
#ifndef NUMBER_H
#define NUMBER_H
#include <stdexcept>
#include <iostream>
#include <cmath>

class Number{
public:
    double val;
    static const Number ZERO;
    static const Number ONE;
    Number();
    Number(double v);
    Number(const Number& rhs);
    bool operator==(const Number&r)const;
    double getVal()const;
    Number operator-(const Number&r)const;
    Number operator+(const Number&r)const;
    Number operator*(const Number&r)const;
    Number operator/(const Number&r)const;
    Number& operator=(const Number&r);
    friend std::istream& operator>>(std::istream& is, Number& num);
    friend std::ostream& operator<<(std::ostream&os,const Number&num);
    static Number mkNum(double v);
};
#endif // NUMBER_H