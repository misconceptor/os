#include "buffered_channel.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <stdexcept>

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

struct BlockIndex {
    int row;
    int col;
};

void mulBlock(const vvi& A, const vvi& B, vvi& C,
              int blockRow, int blockCol, int N, int blocksPerDim) {
    vvi tempBlock(N, vi(N, 0));
    for (int k = 0; k < blocksPerDim; ++k) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int x = 0; x < N; ++x) {
                    tempBlock[i][j] += A[blockRow * N + i][k * N + x] * B[k * N + x][blockCol * N + j];
                }
            }
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[blockRow * N + i][blockCol * N + j] = tempBlock[i][j];
        }
    }
}

void solve_worker(const vvi& A, const vvi& B, vvi& C,
                  int blockSize, int blocksPerDim,
                  buffered_channel<BlockIndex>& channel) {
    while (true) {
        auto [block, ok] = channel.recv();
        if (!ok) break;
        mulBlock(A, B, C, block.row, block.col, blockSize, blocksPerDim);
    }
}

void res(int M, int blockSize) {
    if (M % blockSize != 0) {
        throw runtime_error("Matrix size M must be divisible by blockSize.");
    }
    const int blocksPerDim = M / blockSize;
    vvi A(M, vi(M, 1)), B(M, vi(M, 1)), C(M, vi(M, 0));
    
    const size_t channel_capacity = 1000; 
    buffered_channel<BlockIndex> block_channel(channel_capacity);

    int numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;
    
    cout << "N=: "<<M<<", block Size: " << blockSize << endl;
    cout << "blocks per dim: " << blocksPerDim << ", total blocks: " << blocksPerDim * blocksPerDim << endl;
    cout << "threads: " << numThreads << endl;

    auto start = std::chrono::high_resolution_clock::now();

    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(solve_worker, cref(A), cref(B), ref(C),
                             blockSize, blocksPerDim, ref(block_channel));
    }

    try {
        for (int row = 0; row < blocksPerDim; ++row) {
            for (int col = 0; col < blocksPerDim; ++col) {
                block_channel.send({row, col});
            }
        }
    } catch (const std::runtime_error& e) {
        cerr << "Error during task sending: " << e.what() << endl;
    }

    block_channel.close();

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto d = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Time: " << d.count() << " ms" << "\n\n";
    
}

int main() {
    for(int i=100;i<=500;i+=50){
        if(1000%i==0) res(1000, i); 
    }
    return 0;
}