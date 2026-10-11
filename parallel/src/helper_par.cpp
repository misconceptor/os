/*
do not use "taskset -c 0 ./helper" because it uses a single core
taskset -c 0-7 ./helper 2000 50
it allows 8 cores
*/
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int run_par(const char *prog, int N, int B, int p) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        string n_str = to_string(N);
        string b_str = to_string(B);
        string p_str = to_string(p);
        setenv("OMP_NUM_THREADS", p_str.c_str(), 1);
        const char *args[] = {prog, n_str.c_str(), b_str.c_str(), NULL};
        execvp(args[0], (char *const *)args);
        perror("execvp");
        _exit(127);
    }
    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "usage: " << argv[0] << " <N> <B>\n";
        return 1;
    }
    int N = stoi(argv[1]), B = stoi(argv[2]);

    cout << "N, B, threads, time_us\n";

    vector<int> P = {1, 2, 4, 6, 8, 16};
    for (int p : P)
        if (run_par("./par1", N, B, p) != 0)
        // if (run_par("./par1", N, B, p) != 0)
            return 1;

    return 0;
}