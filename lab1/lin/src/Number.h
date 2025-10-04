#pragma once
#ifndef NUMBER_H
#define NUMBER_H

class Number {
private:
    double val;
public:
    Number(double d = 0.0);
    Number(const Number& rhs);
    double getVal() const;
    void assign(const Number& rhs);
    Number add(const Number& rhs) const;
    Number sub(const Number& rhs) const;
    Number mul(const Number& rhs) const;
    Number div(const Number& rhs) const;
    static const Number& Zero();  
    static const Number& One();
};
#endif // NUMBER_H
