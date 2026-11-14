// stupid_par.cpp
// A = LDL^T decomposition, double, parallel

#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include "utils.h"
#include "omp.h"

using namespace std;
using vd = vector<double>;

void decomp_par(vd &a, int N) {
    #pragma omp parallel 
    {
        for (int i = 0; i < N; ++i) {

            double aii = a[i * N + i];

            #pragma omp for
            for (int j = i + 1; j < N; ++j) 
                a[j * N + i] /= aii;

            #pragma omp for
            for (int j = i + 1; j < N; ++j) {

                double aji = a[j * N + i] * aii;

                for (int k = i + 1; k <= j; ++k) 
                    a[j * N + k] -= aji * a[k * N + i];
            }
        } 
    }
}

vd mult(const vd& a, const vd &b, int N) {
    vd ans(N);
    #pragma omp parallel for
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < N; ++j)
            ans[i] += a[i * N + j] * b[j];
    return ans;
}

void sh(vd& a, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            cout << a[i * N + j] << ' ';
        cout << endl;
    }
}

vd solve_par(vd &a, vd &b, int N) {
    vd x(N), y(N), z(N);
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

    vd A(N * N, 0.0);
    vd x_true(N), b(N);
    mt19937 gen(42);
    uniform_real_distribution<double> dist(-100.0, 0.0);

    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < i; ++j) 
            A[i * N + j] = A[j * N + i] = dist(gen);
        
     
    double p = pow(10, 2 - K);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < i; ++j)
            A[i * N + i] -= A[i * N + j];

        A[i * N + i] *= 2;
        A[i * N + i] += (i == 0 ? p : 0.001);
    }


    for (int i = 0; i < N; ++i) x_true[i] = i + M;
    b = mult(A, x_true, N);

    vd A_copy = A;

    vd x(N);
    double time = measure_time([&](){
        x = solve_par(A, b, N);
    });

    vd Ax = mult(A_copy, x, N);
    double res_norm = residue(Ax, b);

    vd diff(N);
    for (int i = 0; i < N; ++i) 
        diff[i] = x_true[i] - x[i];
    
    double rel_error = l2_norm(diff) / l2_norm(x_true);

    cout << N << ',' << M << ',' << K << ',';
    cout << fixed << setprecision(3) << time << ',';
    cout << scientific << setprecision(6) << res_norm<< ',';
    cout << scientific << setprecision(6) << rel_error << ',';

    for (int i = 0; i < 5; ++i) {
        cout << fixed << setprecision(6) << x[i];
        if (i < 4) cout << ',';
    }
    cout << '\n';

    return 0;
}