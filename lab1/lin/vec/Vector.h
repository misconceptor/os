#ifndef VECTOR_H
#define VECTOR_H
#include <iostream>
#include "../num/Number.h"
class Vector{
public:
   Number x,y;
   static const Vector ZERO; 
   static const Vector ONE; 
   Vector(){}
   Vector(const Number&_x,const Number&_y);
   Number radius()const;
   Number angle()const;
   Vector operator+(const Vector&rhs)const;
   Vector operator=(const Vector&rhs);
   friend std::ostream& operator<<(std::ostream&os, const Vector&v);
   friend std::istream& operator>>(std::istream&is, Vector&v);
};
#endif //VECTOR_H