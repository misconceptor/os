#include <iostream>
#include <random>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <unistd.h>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

struct ThreadArgs {
    const vvi* A;
    const vvi* B;
    vvi* C;
    int blockSize;
    int blocksPerDim;
    int startBlock;
    int endBlock;
    pthread_mutex_t* mtx;
};

void mulBlock(const vvi& A, const vvi& B, vvi& C,
              int blockRow, int blockCol, int blockSize, int blocksPerDim,
              pthread_mutex_t* mtx) {
    int N = blockSize;
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

    pthread_mutex_lock(mtx);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[blockRow * N + i][blockCol * N + j] = tempBlock[i][j];
        }
    }
    pthread_mutex_unlock(mtx);
}

void* solve(void* args) {
    ThreadArgs* targs = (ThreadArgs*)args;
    for (int blockIndex = targs->startBlock; blockIndex < targs->endBlock; ++blockIndex) {
        int blockRow = blockIndex / targs->blocksPerDim;
        int blockCol = blockIndex % targs->blocksPerDim;
        mulBlock(*targs->A, *targs->B, *targs->C,
                 blockRow, blockCol, targs->blockSize, targs->blocksPerDim,
                 targs->mtx);
    }
    return nullptr;
}

void inp_matrix(vvi& M){
	int n=M.size();
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			cin>>M[i][j];	
		}	
	}	
}
void out_matrix(vvi M){
	int n=M.size();
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			M[i][j]=rand()%10+1;
		}	
		cout<<endl;
	}	
}
void res(int N, int blockSize) {
    const int blocksPerDim = N / blockSize;
    vvi A(N, vi(N,1)), B(N, vi(N,1)), C(N, vi(N, 0));
	// inp_matrix(A); i prefer to fill matrices beforehands due to greater amount of time needed to fill them manually
	// inp_matrix(B);
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, nullptr);

    int totalBlocks = blocksPerDim * blocksPerDim;
    int numThreads = sysconf(_SC_NPROCESSORS_ONLN);
    if (numThreads <= 0) numThreads = 4;

    int blocksPerThread = totalBlocks / numThreads;

    vector<pthread_t> threads(numThreads);
    vector<ThreadArgs> threadArgs(numThreads);

    auto start = chrono::high_resolution_clock::now();

    for (int i = 0; i < numThreads; ++i) {
        threadArgs[i] = {&A,&B,&C, blockSize,blocksPerDim,
                         i * blocksPerThread,
                         (i == numThreads - 1) ? totalBlocks : (i + 1) * blocksPerThread,
                         &mtx};
        pthread_create(&threads[i], nullptr, solve, &threadArgs[i]);
    }

    for (auto& t : threads) {
        pthread_join(t, nullptr);
    }

    auto end = chrono::high_resolution_clock::now();
    auto d = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "time: " << d.count() << " ms\n\n";

    pthread_mutex_destroy(&mtx);
}

int main() {
	int N=1000;
	for(int bs=100;bs<=N;bs+=100){
		cout<<"bs="<<bs<<' ';
		res(N, bs);
	}
    return 0;
}