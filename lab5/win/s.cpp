#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int main(){
    string line;
    getline(cin,line);
    istringstream iss(line);
    long long a,sum=0;
    while(iss>>a){
        sum+=a;
    }
    cout<<sum<<endl;
    return 0;
}