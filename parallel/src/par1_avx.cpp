/* 
OpenMP
I K J Block
avx
no unrolling
no loops interchange 
o3 march native

OMP_NUM_THREADS=4 ./par1_avx 2000 50
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <climits>
#include <omp.h>
using namespace std;

using vi = vector<int>;
using ll = long long;

void block_par1_avx(const vi &a, const vi &b, vi &ans, const int N, const int B) {
    #pragma omp parallel for
    for (int ii = 0; ii < N; ii += B)
        for (int jj = 0; jj < N; jj += B)
            for (int kk = 0; kk < N; kk += B) {
                int I = min(N, ii + B);
                int J = min(N, jj + B);
                int K = min(N, kk + B);
                for (int i = ii; i < I; ++i) {
                    int iN = i * N;
                    for (int k = kk; k < K; ++k) {
                        int kN = k * N;
                        int aik = a[iN + k];
                        #pragma omp simd
                        for (int j = jj; j < J; ++j)
                            ans[iN + j] += aik * b[kN + j];
                    }
                }
            }
}

ll measure_block_par1_avx(const vi &a, const vi &b, vi &ans, int N, int B) {
    fill(ans.begin(), ans.end(), 0);
    auto start = chrono::steady_clock::now();
    block_par1_avx(a, b, ans, N, B);
    auto end = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "usage: " << argv[0] << " <N> <B>\n";
        return 1;
    }
    int N = stoi(argv[1]), B = stoi(argv[2]);
    if (N <= 0 || B <= 0) {
        cerr << "N and B must be positive\n";
        return 1;
    }

    mt19937 gen(42);
    uniform_int_distribution<int> dist(-100, 100);
    vi a(N * N), b(N * N), ans(N * N, 0);
    for (auto &x : a) x = dist(gen);
    for (auto &x : b) x = dist(gen);

    measure_block_par1_avx(a, b, ans, N, B);
    ll time = measure_block_par1_avx(a, b, ans, N, B);

    // microseconds
    cout << N << ", " << B << ", " << omp_get_max_threads() << ", " << time << "\n";
    return 0;
}