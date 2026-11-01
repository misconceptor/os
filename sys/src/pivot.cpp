#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <climits>
#include <iomanip>
#define pv(v) for(auto& el : v) cerr << el << ' '; cerr << '\n'
using namespace std;

using vd = vector<double>;
using ll = long long;

void sh() { cout << endl; }
 
template<typename T, typename... U>
void sh(T a, U... b) {
    cout << a << ' ';
    sh(b...);
}

void pm(vd &a, vd &b, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << a[i * N + j] << ' ';
        sh(b[i]);
    }
    sh();
}
vd gauss_pivot(vd &a, vd &b, int N) {

    for (int i = 0; i < N; ++i) {

        int idx = i;
        double mx = 0;

        for (int j = i; j < N; ++j) {

            double q = abs(a[j * N + i]);

            if (q > mx) {
                mx = q;
                idx = j;
            }
        }

        swap(b[i], b[idx]);

        for (int j = i; j < N; ++j) swap(a[i * N + j], a[idx * N + j]);

        double aii = a[i * N + i], bi = b[i];

        for (int j = i + 1; j < N; ++j) {

            double c = -a[j * N + i] / aii;
            b[j] += c * bi;

            for (int k = i; k < N; ++k)
                a[j * N + k] += c * a[i * N + k];

            // pm(a, b, N);
        }
    }

    vd x(N);

    for (int i = N - 1; i >= 0; --i) {
        for (int j = N - 1; j > i; --j) 
            b[i] -= a[i * N + j] * x[j];
        
        x[i] = b[i] / a[i * N + i];
    }

    return x;
}

vd mult(vd& a, vd &b, int N) {
    vd ans(N);
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j)
            ans[i] += a[i * N + j] * b[j];
    return ans;
}

int main() {
    const int N = 760;
    const int M = 18;
    vd a(N * N), b(N);
    // a = {1,2,3,4,5,6 ,6,7,9};
    // b = {1,2,7};
    for (auto &i : a) i = rand() % 71;
    for (int i = 0; i < N; ++i) b[i] = i + M;
    a[0] = 0;
    vd A = a, B = b;

    // pm(a, b, N);
    // sh();
    vd x = gauss_pivot(a, b, N);
    // cout << "answer:\n";
    // pv(x);
    // cout << "A * x:\n";
    vd s = mult(A,x, N);
    // pv(s);
    // cout << "B:\n";
    // pv(B);

    cout << setprecision(9) << fixed;
    for (int i = 0; i < N; ++i) {
        if (abs(s[i] - B[i]) > 1e-9) {
            // cout << "NO\n";
            cout << "no " << s[i] << ' ' << B[i] << endl;
            return 1;
        }
    }
    cout << "correct\n";
    return 0;
}