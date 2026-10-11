/* I K J Block
no avx
no unrolling
no loops rearrangement
o2
*/
#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <climits>
using namespace std;

using vi = vector<int>;
using ll = long long;

void block(const vi &a, const vi &b, vi &ans, const int N, const int B) {
    for (int ii = 0; ii < N; ii += B)
        for (int jj = 0; jj < N; jj += B)
            for (int kk = 0; kk < N; kk += B) {
                int I = min(N, ii + B);
                int J = min(N, jj + B);
                int K = min(N, kk + B);
                for (int i = ii; i < I; ++i)
                    for (int k = kk; k < K; ++k) {
                        int aik = a[i * N + k];
                        for (int j = jj; j < J; ++j)
                            ans[i * N + j] += aik * b[k * N + j];
                    }
            }
}

ll measure_block(const vi &a, const vi &b, vi &ans, int N, int B) {
    fill(ans.begin(), ans.end(), 0);
    auto start = chrono::steady_clock::now();
    block(a, b, ans, N, B);
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

    measure_block(a, b, ans, N, B);
    ll time = measure_block(a, b, ans, N, B);

    //microseconds
    cout << N << ", " << B << ", " << time << "\n";
    return 0;
}