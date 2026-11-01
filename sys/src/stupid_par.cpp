// stupid_par.cpp
// bad version, no pivot, Parallel
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <omp.h>
#include "utils.h"

using namespace std;

using vd = vector<double>;

vd gauss(vd &a, vd &b, int N) {
    
    #pragma omp parallel
    {
        for (int i = 0; i < N; ++i) {
            double aii = a[i * N + i];
            double bi = b[i];

            #pragma omp for
            for (int j = i + 1; j < N; ++j) {
                double c = -a[j * N + i] / aii;
                b[j] += c * bi;
                for (int k = i; k < N; ++k)
                    a[j * N + k] += c * a[i * N + k];
            }
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

vd mult(const vd& a, const vd& b, int N) {
    vd ans(N);
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j)
            ans[i] += a[i * N + j] * b[j];
    return ans;
}

int main() {
    const int N = 2000, M = 18;
    vd a(N * N), b(N), x_true(N);
    
    mt19937 gen(42);
    uniform_real_distribution<double> dist(-100.0, 100.0);
    for (auto &val : a) val = dist(gen);

    for (int i = 0; i < N; ++i) x_true[i] = i + M;

    b = mult(a, x_true, N);

    vd A_orig = a, B_orig = b, x;
    
    double time_ms = measure_time([&](){
        x = gauss(a, b, N);
    });
    
    vd Ax = mult(A_orig, x, N);
    double residual_norm = residue(Ax, B_orig);

    vd diff(N);
    for (int i = 0; i < N; ++i) diff[i] = x_true[i] - x[i];
    double relative_error = l2_norm(diff) / l2_norm(x_true);
    
    cout << N << ',' << M << ',';
    cout << fixed << setprecision(3) << time_ms << ",";
    cout << scientific << setprecision(6) << residual_norm << ",";
    cout << scientific << setprecision(6) << relative_error << ",";
    
    for (int i = 0; i < 5; ++i) {
        cout << fixed << setprecision(6) << x[i];
        if (i < 4) cout << ',';
    }
    cout << '\n';

    return 0;
}