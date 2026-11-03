// stupid_dd.cpp
#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include "dd.h"

using namespace std;
using vdd = vector<DD>;

vdd gauss(vdd &a, vdd &b, int N) {
    for (int i = 0; i < N; ++i) {
        DD aii = a[i * N + i], bi = b[i];

        for (int j = i + 1; j < N; ++j) {
            DD c = -a[j * N + i] / aii;
            b[j] += c * bi;

            for (int k = i; k < N; ++k)
                a[j * N + k] += c * a[i * N + k];
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

vdd mult(const vdd& a, const vdd& b, int N) {
    vdd ans(N);
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

    b = mult(a, x_true, N);
    vdd A_orig = a, B_orig = b, x;
    
    auto start = chrono::steady_clock::now();
    x = gauss(a, b, N);
    auto end = chrono::steady_clock::now();
    double time_ms = chrono::duration<double, milli>(end - start).count();

    vdd Ax = mult(A_orig, x, N);
    DD res_sq = 0.0;
    for (int i = 0; i < N; ++i) {
        DD diff = Ax[i] - B_orig[i];
        res_sq += diff * diff;
    }
    double residual = sqrt(res_sq).hi;

    DD err_sq = 0.0, true_sq = 0.0;
    for (int i = 0; i < N; ++i) {
        DD diff = x_true[i] - x[i];
        err_sq += diff * diff;
        true_sq += x_true[i] * x_true[i];
    }
    double rel_error = sqrt(err_sq).hi / sqrt(true_sq).hi;

    cout << N << ',' << M << ',';
    cout << fixed << setprecision(3) << time_ms << ',';
    cout << scientific << setprecision(6) << residual << ',';
    cout << scientific << setprecision(6) << rel_error << ',';

    for (int i = 0; i < 5; ++i) {
        cout << fixed << setprecision(15) << x[i].hi;
        if (i < 4) cout << ',';
    }
    cout << '\n';

    return 0;
}