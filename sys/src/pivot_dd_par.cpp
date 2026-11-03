// pivot_dd_par.cpp
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include <omp.h>
#include "dd.h"

using namespace std;
using vdd = vector<DD>;

vdd gauss_pivot_dd_par(vdd &a, vdd &b, int N) {
    #pragma omp parallel
    {
        for (int i = 0; i < N; ++i) {
            int idx = i;
            DD mx = 0.0;

            #pragma omp single
            {
                for (int j = i; j < N; ++j) {
                    DD q = abs(a[j * N + i]);
                    if (q > mx) {
                        mx = q;
                        idx = j;
                    }
                }

                swap(b[i], b[idx]);
                for (int j = i; j < N; ++j) 
                    swap(a[i * N + j], a[idx * N + j]);
            }

            DD aii = a[i * N + i];
            DD bi = b[i];

            #pragma omp for
            for (int j = i + 1; j < N; ++j) {
                DD c = -a[j * N + i] / aii;
                b[j] += c * bi;
                for (int k = i; k < N; ++k)
                    a[j * N + k] += c * a[i * N + k];
            }
        }
    }

    vdd x(N);
    for (int i = N - 1; i >= 0; --i) {
        for (int j = N - 1; j > i; --j) 
            b[i] -= a[i * N + j] * x[j];
        x[i] = b[i] / a[i * N + i];
    }
    return x;
}

vdd mult_dd_par(const vdd& a, const vdd& b, int N) {
    vdd ans(N);
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j)
            ans[i] += a[i * N + j] * b[j];
    return ans;
}

int main(int argc, char *argv[]) {
    int N = 2000, M = 18;
    if (argc == 3) {
        N = stoi(argv[1]);
        M = stoi(argv[2]);
    }

    vdd a(N * N), b(N), x_true(N);
    
    mt19937 gen(42);
    uniform_real_distribution<double> dist(-100.0, 100.0);
    
    for (auto &val : a) val = dist(gen);

    for (int i = 0; i < N; ++i) x_true[i] = DD(i + M);

    b = mult_dd_par(a, x_true, N);

    vdd A_orig = a, B_orig = b, x;
    
    auto start = chrono::steady_clock::now();
    x = gauss_pivot_dd_par(a, b, N);
    auto end = chrono::steady_clock::now();
    double time_ms = chrono::duration<double, milli>(end - start).count();
    
    vdd Ax = mult_dd_par(A_orig, x, N);
    DD res_sq = 0.0;
    for (int i = 0; i < N; ++i) {
        DD diff = Ax[i] - B_orig[i];
        res_sq += diff * diff;
    }
    double residual_norm = sqrt(res_sq).hi;

    DD err_sq = 0.0, true_sq = 0.0;
    for (int i = 0; i < N; ++i) {
        DD diff = x_true[i] - x[i];
        err_sq += diff * diff;
        true_sq += x_true[i] * x_true[i];
    }
    double relative_error = sqrt(err_sq).hi / sqrt(true_sq).hi;
    
    cout << N << ',' << M << ',';
    cout << fixed << setprecision(3) << time_ms << ",";
    cout << scientific << setprecision(6) << residual_norm << ",";
    cout << scientific << setprecision(6) << relative_error << ",";
    for (int i = 0; i < 5; ++i) {
        cout << fixed << setprecision(15) << x[i].hi; 
        if (i < 4) cout << ',';
    }
    cout << '\n';
    return 0;
}