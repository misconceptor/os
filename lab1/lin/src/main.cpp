#include <iostream>
#include "Number.h"
#include "Vector.h"

int main() {
    Number n1 = createNumber(3.0);
    Number n2 = createNumber(4.0);
    std::cout<<"Number 1: "<<n1.getVal()<<"\nNumber 2: "<<n2.getVal()<<std::endl; 
    std::cout << "Number Addition: " << n1.add(n2).getVal() << std::endl;

    Vector v1(n1, n2);
    std::cout << "Vector v1: "; v1.print();
    std::cout << "v1 Radius: " << v1.radius().getVal() << std::endl;
    std::cout << "v1 Angle: " << v1.angle().getVal() << std::endl;

    Vector vSum = v1.add(VectorOne);
    std::cout << "v1 + VectorOne: "; vSum.print();

    return 0;
}