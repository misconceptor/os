#include <iostream>
using namespace std;
int main() {
    double x;
    double total_sum = 0;
    while (cin >> x) {
        total_sum += x;
    }
    cout << "Answer: " << total_sum << endl;
    return 0;
}