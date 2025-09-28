#pragma once

#ifdef VECTOR_EXPORTS
#define VECTOR_API __declspec(dllexport)
#else
#define VECTOR_API __declspec(dllimport)
#endif
#include <iostream>
#include "Number.h"
class VECTOR_API Vector{
public:
   Number x,y;
   Vector(){}
   Vector(const Number& _x,const Number& _y);
   Vector(const Vector& other);
   Vector& operator=(const Vector& other);
   Number radius()const;
   Number angle()const;
   Vector add(const Vector& rhs) const;
};
extern VECTOR_API const Vector V_ZERO;
extern VECTOR_API const Vector V_ONE;