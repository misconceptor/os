#include <iostream>
int main() {
    double x, total = 0;
    while (std::cin >> x) { total += x; }
    std::cout << total << std::endl;
    return 0;
}