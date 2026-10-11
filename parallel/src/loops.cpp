/* I K J
no blocks
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

void loops(const vi &a, const vi &b, vi &ans, const int N) {
        for (int i = 0; i < N; ++i)
            for (int k = 0; k < N; ++k) {
                int aik = a[i * N + k];
                for (int j = 0; j < N; ++j)
                    ans[i * N + j] += aik * b[k * N + j];
            }
}

ll measure_loops(const vi &a, const vi &b, vi &ans, int N) {
    fill(ans.begin(), ans.end(), 0);
    auto start = chrono::steady_clock::now();
    loops(a, b, ans, N);
    auto end = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "usage: " << argv[0] << " <N>\n";
        return 1;
    }
    int N = stoi(argv[1]);
    if (N <= 0) {
        cerr << "N must be positive\n";
        return 1;
    }

    mt19937 gen(42);
    uniform_int_distribution<int> dist(-100, 100);
    // const int N = 2000;
    vi a(N * N), b(N * N), ans(N * N, 0);
    for (auto &x : a) x = dist(gen);
    for (auto &x : b) x = dist(gen);

    measure_loops(a, b, ans, N);
    ll baseline_time = measure_loops(a, b, ans, N);
    // microseconds
    cout << N << ", " << baseline_time << "\n";
    return 0;
}