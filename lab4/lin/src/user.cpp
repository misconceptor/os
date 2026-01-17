#include <iostream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

bool isProcessRunning(const std::string& name) {
    std::string cmd = "pgrep -x " + name + " > /dev/null 2>&1";
    return (system(cmd.c_str()) == 0);
}

void runKiller(const std::vector<std::string>& args) {
    pid_t pid = fork();
    if (pid == 0) {
        std::vector<char*> c_args;
        c_args.push_back((char*)"./killer");
        for (const auto& arg : args) {
            c_args.push_back((char*)arg.c_str());
        }
        c_args.push_back(nullptr);
        execvp(c_args[0], c_args.data());
        exit(1); 
    } else {
        waitpid(pid, nullptr, 0);
    }
}

int main() {
    std::string targets = "nautilus,Telegram,gnome-control-c";
    setenv("PROC_TO_KILL", targets.c_str(), 1);
    std::cout << "[USER] Environment variable PROC_TO_KILL set to: " << targets << std::endl;

    std::cout << "\n--- Checking processes before Killer ---" << std::endl;
    std::vector<std::string> procList = {"nautilus", "Telegram", "gnome-control-c"};
    for (const auto& p : procList) {
        std::cout << "Process " << p << (isProcessRunning(p) ? " is RUNNING." : " is NOT running.") << std::endl;
    }

    std::cout << "\n[USER] Running Killer (Environment Variable Mode)..." << std::endl;
    runKiller({}); 

    std::cout << "[USER] Running Killer (--name variation)..." << std::endl;
    runKiller({"--name", "nautilus"});

    std::cout << "\n--- Checking processes after Killer ---" << std::endl;
    for (const auto& p : procList) {
        std::cout << "Process " << p << (isProcessRunning(p) ? " is STILL RUNNING." : " is GONE.") << std::endl;
    }

    unsetenv("PROC_TO_KILL");
    if (getenv("PROC_TO_KILL") == nullptr) {
        std::cout << "\n[USER] Successfully deleted PROC_TO_KILL environment variable." << std::endl;
    }

    return 0;
}