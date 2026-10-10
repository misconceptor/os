/*
taskset -c 0 ./helper
*/
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int run_baseline(int N, int B) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        string n_str = to_string(N);
        string b_str = to_string(B);
        const char *args[] = {"./baseline_block", n_str.c_str(), b_str.c_str(), NULL};
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

int main() {
    cout << "N, B, time_us\n";

    vector<int> N = {2000};
    vector<int> B = {1, 5, 10, 30, 50, 300, 500};
    for (auto &i : N) {
        for (auto &j : B)
            if (run_baseline(i, j) != 0)
                return 1;
    }
    return 0;
}
