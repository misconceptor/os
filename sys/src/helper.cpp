#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
using namespace std;

int run_program(const string& prog, int N, int M, const string& algo_name) {
    cout << algo_name << "," << flush;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        string n_str = to_string(N);
        string m_str = to_string(M);
        
        const char *args[] = {prog.c_str(), n_str.c_str(), m_str.c_str(), NULL};
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
        cerr << "usage: " << argv[0] << " <N> <M>\n";
        return 1;
    }
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);

    cout << "algo,N,M,time_ms,residual,rel_error,x0,x1,x2,x3,x4\n";

    if (run_program("./stupid", N, M, "stupid") != 0) return 1;
    if (run_program("./pivot", N, M, "pivot") != 0) return 1;

    if (run_program("./stupid_par", N, M, "stupid_par") != 0) return 1;
    if (run_program("./pivot_par", N, M, "pivot_par") != 0) return 1;

    return 0;
}