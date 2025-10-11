#include <iostream>
#include <chrono>
#include <vector>
#include <windows.h>

using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;

struct ThreadParams {
    const vvi* A;
    const vvi* B;
    vvi* C;
    int blockRow;
    int blockCol;
    int blockSize;
    int blocksPerDim;
    CRITICAL_SECTION* cs;
};

DWORD WINAPI mulBlockThread(LPVOID lpParam) {
    ThreadParams* params = reinterpret_cast<ThreadParams*>(lpParam);
    const vvi& A = *(params->A);
    const vvi& B = *(params->B);
    vvi& C = *(params->C);
    int blockRow = params->blockRow;
    int blockCol = params->blockCol;
    int blockSize = params->blockSize;
    int blocksPerDim = params->blocksPerDim;
    CRITICAL_SECTION* cs = params->cs;

    int N = blockSize;
    vvi tempBlock(N, vi(N, 0));

    for (int k = 0; k < blocksPerDim; ++k) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int sum = tempBlock[i][j];
                for (int x = 0; x < N; ++x) {
                    sum += A[blockRow * N + i][k * N + x] * B[k * N + x][blockCol * N + j];
                }
                tempBlock[i][j] = sum;
            }
        }
    }

    EnterCriticalSection(cs);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[blockRow * N + i][blockCol * N + j] = tempBlock[i][j];
        }
    }
    LeaveCriticalSection(cs);

    return 0;
}

int main() {
    int N = 1000;
    int blockSize = 500;
    int blocksPerDim = N / blockSize;
    vvi A(N, vi(N, 1));
    vvi B(N, vi(N, 1));
    vvi C(N, vi(N, 0));

    int totalBlocks = blocksPerDim * blocksPerDim;

    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    vector<HANDLE> threads;
    vector<ThreadParams> params(totalBlocks);
    
    auto start = std::chrono::high_resolution_clock::now();

    for (int blockIndex = 0; blockIndex < totalBlocks; ++blockIndex) {
        int blockRow = blockIndex / blocksPerDim;
        int blockCol = blockIndex % blocksPerDim;

        params[blockIndex] = {&A, &B, &C, blockRow, blockCol, blockSize, blocksPerDim, &cs};

        HANDLE hThread = CreateThread(NULL,0,mulBlockThread,&params[blockIndex],0,NULL);
        if (hThread == NULL) {
            cout << "Failed to create thread " << blockIndex << endl;
            DeleteCriticalSection(&cs);
            return 1;
        }
        threads.push_back(hThread);
    }
    WaitForMultipleObjects((DWORD)threads.size(), threads.data(), TRUE, INFINITE);
    for (HANDLE h : threads) {
        CloseHandle(h);
    }
    DeleteCriticalSection(&cs);

    auto end = std::chrono::high_resolution_clock::now();
    auto d= std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout<<"time: "<<d.count()<<endl;
    return 0;
}