#include <iostream>
#include <chrono>
#include <sys/time.h>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;
void mulBlock(const vvi& A, const vvi& B, vvi& C,
                   int blockRow, int blockCol, int blockSize, int blocksPerDim,
                   mutex& mtx) {
    int N = blockSize;
    vvi tempBlock(N, vi(N, 0.0));

    for (int k = 0; k < blocksPerDim; ++k) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                for (int x = 0; x < N; ++x) {
                    tempBlock[i][j]+=A[blockRow*N+i][k*N+x]*B[k*N+x][blockCol*N+j];
                }
            }
        }
    }

    lock_guard<mutex> lock(mtx);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[blockRow * N + i][blockCol * N + j] = tempBlock[i][j];
        }
    }
}

void solve(const vvi& A, const vvi& B, vvi& C,
    int blockSize, int blocksPerDim, mutex& mtx,
    int startBlock, int endBlock) 
{
    for (int blockIndex = startBlock; blockIndex < endBlock; ++blockIndex) {
        int blockRow = blockIndex / blocksPerDim;
        int blockCol = blockIndex % blocksPerDim;
        mulBlock(A, B, C, blockRow, blockCol, blockSize, blocksPerDim, mtx);
    }
}

void res(int N,int blockSize) {
    const int blocksPerDim = N / blockSize;
    vvi A(N, vi(N, 1)), B(N, vi(N, 1)), C(N, vi(N, 0));
    // cout<<"n "<<N<<endl;
    // cout<<"blsize "<<blockSize<<endl;
    mutex mtx;

    int totalBlocks = blocksPerDim * blocksPerDim;
    // cout<<"totBl "<<totalBlocks<<endl;
    int numThreads = thread::hardware_concurrency();
    // cout<<"th "<<numThreads<<endl;
    if (numThreads == 0) numThreads = 4;
    int blocksPerThread = totalBlocks / numThreads;

	auto start = std::chrono::high_resolution_clock::now();

    vector<thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        int startBlock = i * blocksPerThread;
        int endBlock = (i == numThreads-1) ? totalBlocks : startBlock + blocksPerThread;

        threads.emplace_back(solve, cref(A), cref(B), ref(C),
                             blockSize, blocksPerDim, ref(mtx),
                             startBlock, endBlock);
    }

    for (auto& t : threads)
        t.join();

	auto end = std::chrono::high_resolution_clock::now();
    auto d= std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout<<"time: "<< d.count()<<"\n\n";

}

int main(){
    //for(int i=16;i<=512;i*=2)
    res(1000,800);
    return 0;
}