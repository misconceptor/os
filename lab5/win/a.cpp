#include <iostream>
#include <sstream>
#include <string>
using namespace std;
int main(){
    string line;
    getline(cin,line);
    istringstream iss(line);
    long long a;
    while(iss>>a){
        cout<<a+7<<' ';
    }
    cout<<endl;
    return 0;
}