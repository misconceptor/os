#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int run_program(const string& prog, int N, int M, int K, const string& algo_name) {
    cout << algo_name << "," << flush;

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        string n_str = to_string(N);
        string m_str = to_string(M);
        string k_str = to_string(K);
        
        const char *args[] = {prog.c_str(), n_str.c_str(), m_str.c_str(), k_str.c_str(), NULL};
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
    if (argc != 4) {
        cerr << "usage: " << argv[0] << " <N> <M> <K>\n";
        return 1;
    }
    int N = stoi(argv[1]);
    int M = stoi(argv[2]);
    int K = stoi(argv[3]);

    cout << "algo,N,M,K,time_ms,residual,rel_error,x0,x1,x2,x3,x4\n";

    vector<pair<string, string>> programs = {
        {"./stupid", "ldlt_decomp"},
        {"./par", "ldlt_par"},
        {"./dd", "ldlt_DD"},
        {"./par_dd", "ldlt_par_DD"}
    };

    for (const auto& p : programs) {
        if (run_program(p.first, N, M, K, p.second) != 0) {
            cerr << "Warning: Failed or skipped " << p.first << "\n";
        }
    }

    return 0;
}