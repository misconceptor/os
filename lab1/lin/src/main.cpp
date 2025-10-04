#include <iostream>
#include "Number.h"
#include "Vector.h"
using namespace std;
int main(){
    Number a=Number::Zero().add(1.732050808);
    Number b = Number::One().mul(4);
    cout<<a.getVal()<<' '<<b.getVal()<<endl;
    cout<<"number operations:\n";
    cout<<"a + b = "<<a.add(b).getVal()<<endl;
    cout<<"a - b = "<<a.sub(b).getVal()<<endl;
    cout<<"a * b = "<<a.mul(b).getVal()<<endl;
    cout<<"a / b = "<<a.div(b).getVal()<<endl;

    cout<<"vector operations:\n";
    Vector m(b,a);
    Vector n(VOne());
    cout<<"m = ";
    m.print();
    cout<<"n = ";
    n.print();

    Number t=Number::Zero().add(1.732050808);
    Number s = Number::One();
    Vector v(t,s);
    cout<<"angle = ";
    cout<<v.angle().getVal()<<endl;
    cout<<"radius = ";
    cout<<v.radius().getVal()<<endl;
    return 0;
}