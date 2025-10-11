#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <sys/time.h>
#include <random>
#include <vector>
using namespace std;
typedef vector<int> vi;
typedef vector<vi> vvi;
const int n=500;
void slowmult(vvi& a, vvi& b, vvi&c){
	for(int i=0;i<n;++i){
		for(int j=0;j<n;++j){
			int sum=0;
			for(int k=0;k<n;++k){
				sum+=a[i][k]*b[k][j];
			}
			c[i][j]=sum;
		}
	}
}
void mult(vvi& a, vvi& b, vvi&c, int rs, int rf,int cs, int cf){// [s,f) are ind. of rows
	for(int i=rs;i<rf;++i){
		for(int j=cs;j<cf;++j){
			int sum=0;
			for(int k=0;k<n;++k){
				sum+=a[i][k]*b[k][j];
			}
			c[i][j]=sum;
		}
	}
}
void solve(int row_blocks, int col_blocks){
	cout<<"r: "<<row_blocks<<" c: "<<col_blocks<<endl;
	int row_block_size=n/row_blocks;
	int col_block_size=n/col_blocks;
	vvi a(n,vi(n,2));
	vvi b(n,vi(n,3));
	vvi c(n,vi(n,0));
	vector<thread> th;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < row_blocks; i+=row_block_size) {
        int row_start = i * row_block_size;
        int row_end = (i == row_blocks - 1)?n : row_start + row_block_size;
        for (int j = 0; j < col_blocks; j+=col_block_size){
            int col_start = j * col_block_size;
            int col_end = (j == col_blocks - 1)?n : col_start + col_block_size;

            th.emplace_back(mult,ref(a),ref(b),ref(c),
                    row_start, row_end, col_start, col_end);
        }
    }
	for(auto&t:th){
		t.join();	
	}
	auto end = std::chrono::high_resolution_clock::now();
    auto d= std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    cout<<"Execution time: "<< d.count()<<"\n\n";
}
int main(){
	solve(32,32);
	return 0;
}
