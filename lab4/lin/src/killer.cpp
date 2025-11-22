#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;
void printHelp() {
    cout << "Usage:\n"
              << "  killer --id <pid>          Terminate process by PID\n"
              << "  killer --name <procname>   Terminate all processes with given name\n"
              << "  killer                     Terminate processes in PROC_TO_KILL env variable\n";
}

bool killProcessByPid(pid_t pid) {
    if (kill(pid, SIGTERM) == 0) {
        cout << "Terminated process PID " << pid << "\n";
        return true;
    } else {
        perror("Failed to kill process");
        return false;
    }
}

vector<pid_t> getPidsByName(const string& name) {
    vector<pid_t> pids;
    string cmd = "pgrep " + name;
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        cerr << "Failed to run pgrep\n";
        return pids;
    }
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe)) {
        pid_t pid = atoi(buffer);
        if (pid > 0) pids.push_back(pid);
    }
    pclose(pipe);
    return pids;
}

void killProcessesByName(const string& name) {
    vector<pid_t> pids = getPidsByName(name);
    if (pids.empty()) {
        cout << "No processes found with name: " << name << "\n";
        return;
    }
    for (pid_t pid : pids) {
        killProcessByPid(pid);
    }
}

void killProcessesFromEnv() {
    const char* env = getenv("PROC_TO_KILL");
    if (!env) {
        cout << "PROC_TO_KILL is not set.\n";
        return;
    }
    string procs(env);
    stringstream ss(procs);
    string procName;
    while (getline(ss, procName, ',')) {
        procName.erase(0, procName.find_first_not_of(" \t\n\r"));
        procName.erase(procName.find_last_not_of(" \t\n\r") + 1);
        cout << "Killing processes named: " << procName << "\n";
        killProcessesByName(procName);
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        killProcessesFromEnv();
        return 0;
    } else if (argc == 3) {
        string arg = argv[1];
        if (arg == "--id") {
            pid_t pid = stoi(argv[2]);
            killProcessByPid(pid);
            return 0;
        } else if (arg == "--name") {
            killProcessesByName(argv[2]);
            return 0;
        }
    }
    printHelp();
    return 1;
}