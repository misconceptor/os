#include <iostream>
#include "Number.h" 
#include <math.h>
//g++ -c Number.cpp -o Number.o
//ar rcs libnumber.a Number.o
//g++ main.cpp -L. -lnumber -o main
using namespace std;
int main(){
    return 0;
    Number k=Number::mkNum(11);
    cout<<"static: "<<k<<endl;
    Number n;
    cout<<"default ";
    cout<<n<<endl;
    Number a(5),b(12);
    cout<<a+b<<' ';
    cout<<a-b<<' ';
    cout<<a*b<<' ';
    cout<<a/b<<'\n';
    cout<<"enter: ";
    Number e;cin>>e;
    cout<<"e="<<e<<endl;
    return 0;
}