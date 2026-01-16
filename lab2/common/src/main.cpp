#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

#if defined(_WIN32)
    #include <windows.h>
    #define USE_WINDOWS_API
#else
    #include <thread>
    #include <mutex>
#endif

using namespace std;
using vi = vector<int>;
using vvi = vector<vi>;

struct ThreadData {
    const vvi *A, *B;
    vvi *C;
    int row, col, k, blocksPerDim;
};

#ifdef USE_WINDOWS_API
    CRITICAL_SECTION cs;
#else
    mutex mtx;
#endif

void multiplyBlock(const vvi* A, const vvi* B, vvi* C, int row, int col, int k, int blocksPerDim) {
    int rS = row * k, cS = col * k;
    vvi temp(k, vi(k, 0));

    for (int m = 0; m < blocksPerDim; ++m) {
        int innerStart = m * k;
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < k; ++j) {
                for (int x = 0; x < k; ++x) {
                    temp[i][j] += (*A)[rS + i][innerStart + x] * (*B)[innerStart + x][cS + j];
                }
            }
        }
    }

#ifdef USE_WINDOWS_API
    EnterCriticalSection(&cs);
#else
    mtx.lock();
#endif
    for (int i = 0; i < k; ++i)
        for (int j = 0; j < k; ++j)
            (*C)[rS + i][cS + j] = temp[i][j];
#ifdef USE_WINDOWS_API
    LeaveCriticalSection(&cs);
#else
    mtx.unlock();
#endif
}

#ifdef USE_WINDOWS_API
DWORD WINAPI WinThreadFunc(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    multiplyBlock(data->A, data->B, data->C, data->row, data->col, data->k, data->blocksPerDim);
    return 0;
}
#endif

void run_experiment(int N, int k, const vvi& A, const vvi& B, vvi& C) {
    if (N % k != 0) return;
    int blocksPerDim = N / k;
    int totalThreads = blocksPerDim * blocksPerDim;
    
    for(auto& row : C) fill(row.begin(), row.end(), 0);

    vector<ThreadData> taskList(totalThreads);
    auto start = chrono::high_resolution_clock::now();

#ifdef USE_WINDOWS_API
    vector<HANDLE> threads(totalThreads);
    for (int i = 0; i < blocksPerDim; ++i) {
        for (int j = 0; j < blocksPerDim; ++j) {
            int idx = i * blocksPerDim + j;
            taskList[idx] = {&A, &B, &C, i, j, k, blocksPerDim};
            threads[idx] = CreateThread(NULL, 65536, WinThreadFunc, &taskList[idx], STACK_SIZE_PARAM_IS_A_RESERVATION, NULL);
        }
    }
    WaitForMultipleObjects(totalThreads, threads.data(), TRUE, INFINITE);
    for (HANDLE h : threads) CloseHandle(h);
#else
    vector<thread> threads;
    for (int i = 0; i < blocksPerDim; ++i) {
        for (int j = 0; j < blocksPerDim; ++j) {
            threads.emplace_back(multiplyBlock, &A, &B, &C, i, j, k, blocksPerDim);
        }
    }
    for (auto& t : threads) t.join();
#endif

    auto end = chrono::high_resolution_clock::now();
    cout << setw(10) << k << setw(15) << totalThreads 
         << setw(15) << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
}

int main() {
    int N = 600;
#ifdef USE_WINDOWS_API
    InitializeCriticalSection(&cs);
#endif

    vvi A(N, vi(N, 1)), B(N, vi(N, 1)), C(N, vi(N, 0));

    cout << "Matrix Block Multiplication (N=" << N << ")" << endl;
    cout << setw(10) << "Block k" << setw(15) << "Threads" << setw(15) << "Time" << endl;
    cout<<endl;
    vi k_values = {600, 300, 200, 150, 120, 100, 75};
    for (int k : k_values) {
        run_experiment(N, k, A, B, C);
    }

#ifdef USE_WINDOWS_API
    DeleteCriticalSection(&cs);
#endif
    return 0;
}