#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
using namespace std;

int run_baseline(int N) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        string n_str = to_string(N);
        const char *args[] = {"./baseline", n_str.c_str(), NULL};
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
    for (int i = 7; i < 12; ++i) {
        int N = 1 << i;
        if (run_baseline(N - 1) != 0) return 1;
        if (run_baseline(N) != 0) return 1;
    }
    return 0;
}