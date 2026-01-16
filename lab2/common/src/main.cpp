#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <iomanip>

using namespace std;
using vi = vector<int>;
using vvi = vector<vi>;

struct BlockTask {
    const vvi* A;
    const vvi* B;
    vvi* C;
    int row, col, blockSize, blocksPerDim;
    pthread_mutex_t* mtx;
};

void standardMul(const vvi& A, const vvi& B, vvi& C, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}
void* multiplyBlockThread(void* args) {
    BlockTask* task = (BlockTask*)args;
    int k = task->blockSize;
    int rowStart = task->row * k;
    int colStart = task->col * k;
    vvi tempBlock(k, vi(k, 0));

    for (int m = 0; m < task->blocksPerDim; ++m) {
        int innerStart = m * k;
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                for (int x = 0; x < k; ++x) {
                    tempBlock[i][j] += (*task->A)[rowStart + i][innerStart + x] * (*task->B)[innerStart + x][colStart + j];
                }
            }
        }
    }

    pthread_mutex_lock(task->mtx);
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < k; ++j) {
            (*task->C)[rowStart + i][colStart + j] = tempBlock[i][j];
        }
    }
    pthread_mutex_unlock(task->mtx);
    return nullptr;
}
void run(int N, int k) {
    if (N % k != 0) return;
    int blocksPerDim = N / k;
    int totalThreads = blocksPerDim * blocksPerDim;
    
    vvi A(N, vi(N, 1)), B(N, vi(N, 1)), C(N, vi(N, 0));
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, nullptr);

    vector<pthread_t> threads(totalThreads);
    vector<BlockTask> tasks(totalThreads);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < blocksPerDim; ++i) {
        for (int j = 0; j < blocksPerDim; ++j) {
            int idx = i * blocksPerDim + j;
            tasks[idx] = {&A, &B, &C, i, j, k, blocksPerDim, &mtx};
            pthread_create(&threads[idx], nullptr, multiplyBlockThread, &tasks[idx]);
        }
    }

    for (int i = 0; i < totalThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout<<setw(12)<<k<<setw(20)<<totalThreads<<setw(20)<<duration.count()<<" ms\n";
    pthread_mutex_destroy(&mtx);
}

int main() {
    int N = 600;
    
    vvi A(N, vi(N, 1)), B(N, vi(N, 1)), C_std(N, vi(N, 0));

    cout<<"standard algorithm: ";

    auto s_start = chrono::high_resolution_clock::now();
    standardMul(A, B, C_std, N);
    auto s_end = chrono::high_resolution_clock::now();

    cout<<chrono::duration_cast<chrono::milliseconds>(s_end - s_start).count()<<" ms\n";
    cout<<setw(12)<<"Block Size"<<setw(20)<<"Threads"<<setw(20)<<"Time\n";

    vi k_values = {600, 300, 150, 120, 100, 75, 60, 50, 30};
    for (int k : k_values) {
        run(N, k);
    }

    return 0;
}