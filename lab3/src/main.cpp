#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include "buffered_channel.h"

struct RowTask {
    int row_idx;
    std::vector<int> data;
};

void worker_multiply(int start, int end, int N, const std::vector<std::vector<int>>& A, 
                     const std::vector<std::vector<int>>& B, BufferedChannel<RowTask>& chan) {
    for (int i = start; i < end; ++i) {
        std::vector<int> row_res(N, 0);
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                row_res[j] += A[i][k] * B[k][j];
            }
        }
        chan.Send({i, std::move(row_res)});
    }
}

int main() {
    const int N = 400; 
    const int buffer_cap = 20;
    
    std::vector<std::vector<int>> A(N, std::vector<int>(N, 1));
    std::vector<std::vector<int>> B(N, std::vector<int>(N, 1));
    std::vector<std::vector<int>> C(N, std::vector<int>(N, 0));

    BufferedChannel<RowTask> channel(buffer_cap);
    int num_workers = std::thread::hardware_concurrency();

    std::cout << "Matrix Multiplication (N = " << N << ") with " << num_workers << " workers..." << std::endl;

    auto start_time = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> workers;
    int chunk = N / num_workers;
    for (int i = 0; i < num_workers; ++i) {
        int s = i * chunk;
        int e = (i == num_workers - 1) ? N : (i + 1) * chunk;
        workers.emplace_back(worker_multiply, s, e, N, std::ref(A), std::ref(B), std::ref(channel));
    }

    for (int i = 0; i < N; ++i) {
        auto result = channel.Recv();
        if (result.second) {
            C[result.first.row_idx] = std::move(result.first.data);
        }
    }

    for (auto& w : workers) w.join();
    channel.Close();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    std::cout << "Time: " << duration.count() << " ms" << std::endl;

    return 0;
}