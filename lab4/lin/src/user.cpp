#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
bool isProcessRunning(const char* name) {
    string cmd = string("pgrep -x ") + name + " > /dev/null";
    return (system(cmd.c_str()) == 0);
}
void runKiller(const char* arg1 = nullptr, const char* arg2 = nullptr) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        if (arg1 && arg2) {
            char* args[] = {(char*)"./killer", (char*)arg1, (char*)arg2, nullptr};
            execv(args[0], args);
        } else {
            char* args[] = {(char*)"./killer", nullptr};
            execv(args[0], args);
        }
        perror("execv");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            cout << "Killer exited with status " << WEXITSTATUS(status) << endl;
        } else {
            cout << "Killer did not exit normally" << endl;
        }
    }
}
int main() {
    //PLEASE LAUNCH NAUTILUS OR VIM MANUALLY BEFORE LAUNCHING MY PROGRAM
    setenv("PROC_TO_KILL", "nautilus,vim", 1);
    cout << "PROC_TO_KILL set to: " << getenv("PROC_TO_KILL") << endl;
    if (isProcessRunning("nautilus")) {
        cout << "Nautilus is running before Killer." << endl;
    } else {
        cout << "Nautilus is NOT running before Killer." << endl;
    }
    runKiller();
    if (!isProcessRunning("nautilus")) {
        cout << "Nautilus killed successfully by Killer." << endl;
    } else {
        cout << "Nautilus is STILL running after Killer." << endl;
    }
    runKiller("--id", "12345");
    runKiller("--name", "vim");
    unsetenv("PROC_TO_KILL");
    //cout << "PROC_TO_KILL environment variable removed." << endl;
    return 0;
}