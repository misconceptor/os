#include <iostream>
#include "vec/Vector.h"
#include "num/Number.h"
/*
g++ -fPIC -c num/Number.cpp -o Number.o
ar rcs num/libnumber.a Number.o
g++ -fPIC -shared vec/Vector.cpp num/libnumber.a -o vec/libvector.so
g++ main.cpp -Lnum -lnumber -Lvec -lvector -o main
export LD_LIBRARY_PATH=vec:$LD_LIBRARY_PATH
./main
*/
using namespace std;
int main(){
    Number n(5),m(3.333);
    cout<<"n="<<n<<' ';
    cout<<"m="<<m<<endl;
    n=n+Number::ONE;
    cout<<"inc: n="<<n<<endl;
    cout<<"n+m: "<<n+m<<endl;
    cout<<"n-m: "<<n-m<<endl;
    cout<<"n*m: "<<n*m<<endl;
    cout<<"n/m: "<<n/m<<endl;

    Number a(1.732050808),b(1);
    Vector v(a,b);
    cout<<"v: "<<v<<endl;
    cout<<"radius: ";
    cout<<v.radius()<<endl;
    cout<<"angle(rad): ";
    cout<<v.angle()<<endl;
    Vector u(Vector::ONE);
    cout<<"u: "<<u<<endl;
    cout<<"v+u:"<<v+u<<endl;
    return 0;
}