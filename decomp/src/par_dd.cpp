// stupid_dd_par.cpp
// A = LDL^T decomposition, double-double, parallel

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include "dd.h"
#include "omp.h"

using namespace std;

using vdd = vector<DD>;

void decomp_par(vdd &a, int N) {
    #pragma omp parallel 
    {
        for (int i = 0; i < N; ++i) {
            DD aii = a[i * N + i];

            #pragma omp for
            for (int j = i + 1; j < N; ++j) 
                a[j * N + i] /= aii;

            #pragma omp for
            for (int j = i + 1; j < N; ++j) {
                DD aji = a[j * N + i] * aii;

                for (int k = i + 1; k <= j; ++k) 
                    a[j * N + k] -= aji * a[k * N + i];
            }
        } 
    }
}

vdd mult(const vdd& a, const vdd &b, int N) {
    vdd ans(N);
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j)
            ans[i] += a[i * N + j] * b[j];
    return ans;
}

vdd solve_par(vdd &a, vdd &b, int N) {
    vdd x(N), y(N), z(N);
    y = b;

    decomp_par(a, N);

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < i; ++j)
            y[i] -= y[j] * a[i * N + j];

    for (int i = 0; i < N; ++i)
        z[i] = y[i] / a[i * N + i];

    x = z;
    for (int i = N - 1; i >= 0; --i)
        for (int j = i + 1; j < N; ++j)
            x[i] -= x[j] * a[j * N + i];

    return x;
}

int main(int argc, char *argv[]) {
    int N = 2000, M = 18, K = 4;
    if (argc == 4) {
        N = stoi(argv[1]);
        M = stoi(argv[2]);
        K = stoi(argv[3]);
    }

    vdd A(N * N), x_true(N), b(N);
    mt19937 gen(42);
    uniform_real_distribution<double> dist(-100.0, 0.0);

    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < i; ++j) 
            A[i * N + j] = A[j * N + i] = DD(dist(gen));
        
    DD p = DD(pow(10, 2 - K));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < i; ++j)
            A[i * N + i] -= A[i * N + j];

        A[i * N + i] *= 2.0;
        A[i * N + i] += (i == 0 ? p : DD(0.001));
    }

    for (int i = 0; i < N; ++i) x_true[i] = DD(i + M);
    
    b = mult(A, x_true, N);
    vdd A_copy = A;

    vdd x(N);
    auto start = chrono::steady_clock::now();
    x = solve_par(A, b, N);
    auto end = chrono::steady_clock::now();
    double time_ms = chrono::duration<double, milli>(end - start).count();

    vdd Ax = mult(A_copy, x, N);
    DD res_sq = 0.0;
    for (int i = 0; i < N; ++i) {
        DD diff = Ax[i] - b[i];
        res_sq += diff * diff;
    }
    double res_norm = sqrt(res_sq).hi;

    DD err_sq = 0.0, true_sq = 0.0;
    for (int i = 0; i < N; ++i) {
        DD diff = x_true[i] - x[i];
        err_sq += diff * diff;
        true_sq += x_true[i] * x_true[i];
    }
    double rel_error = sqrt(err_sq).hi / sqrt(true_sq).hi;

    cout << N << ',' << M << ',' << K << ',';
    cout << fixed << setprecision(3) << time_ms << ',';
    cout << scientific << setprecision(6) << res_norm << ',';
    cout << scientific << setprecision(6) << rel_error << ',';

    for (int i = 0; i < 5; ++i) {
        cout << fixed << setprecision(15) << x[i].hi;
        if (i < 4) cout << ',';
    }
    cout << '\n';

    return 0;
}