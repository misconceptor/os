#include <iostream>
#include <vector>
#include <random>
// #include <iomanip>
#include "utils.h"

using namespace std;
using vd = vector<double>;

void decomp(vd &a, int N) {

    for (int i = 0; i < N; ++i) a[i * N + i] = 1;

    for (int i = 0; i < N; ++i) {

        double aii = a[i * N + i];

        for (int j = i + 1; j < N; ++j) {

            double c = a[j * N + i] / aii;
            a[i * N + j] = c;

            for (int k = i; k < N; ++k) {
                // a[j * N + k] -= c * a[i * N + k];
                a[j * N + k] -= c * a[i * N + k];
                // a[i * N + j] = c;
            }
        }
        // d[i] = a[i * N + i];
    } 
}

vd mult(const vd& a, const vd &b, int N) {
    vd ans(N);
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j)
            ans[i] += a[i * N + j] * b[j];
    return ans;
}

void sh(vd& a, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) cout << a[i * N + j] << ' ';
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    int N = 2000, M = 18, K = 4;
    if (argc == 3) {
        N = stoi(argv[1]);
        M = stoi(argv[2]);
    }

    vd A(N * N, 0.0);
    vd x_true(N), b(N);
    mt19937 gen(42);
    uniform_real_distribution<double> dist(-100.0, 0.0);

    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < i; ++j) 
            A[i * N + j] = A[j * N + i] = dist(gen);
        
     
    // diags = 0
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < i; ++i) A[i * N + i] -= A[i * N + j];
        A[i * N + i] += (i == 0 ? 0.01 : 0.001);
    }

    for (int i = 0; i < N; ++i) x_true[i] = i + M;
    b = mult(A, x_true, N);

    vd A_copy = A;



    return 0;
}