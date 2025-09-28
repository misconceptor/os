#include <iostream>
#include "Number.h"
#include "Vector.h"
#include <cmath>
#include <iomanip>
using namespace std;
int main() {
	Number n(4.4), m = Number::ONE+n;
	cout << "n,m= " << n << ' ' << m << endl;
	cout << (n + m).getVal() << endl;
	cout << (n - m).getVal() << endl;
	cout << (n * m).getVal() << endl;
	cout << (n / m).getVal() << endl;

	Number ppi = Number(acos(-1));
	cout << setprecision(10) << fixed;
	cout << "pi=" << ppi << endl;
	cout << "compare with real pi: ";
	cout << (ppi == 3.1415926536) << endl<<endl;

	cout << setprecision(5) << fixed;
	Vector v(sqrt(3), 1);
	cout << "vector: " << v.x << ' ' << v.y << endl;
	cout << "radius and angle: " << v.radius() << ' ' << v.angle() << endl << endl;

	cout << "addition: v+one ";
	Vector u = v.add(V_ONE);
	cout << u.x <<' ' << u.y << endl;
	cout << "radius and angle: " << u.radius() << ' ' << u.angle() << endl << endl;
	return 0;
}