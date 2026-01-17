#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <csignal>
#include <dirent.h>
#include <unistd.h>
#include <sstream>

void killByName(const std::string& name) {
    DIR* dir = opendir("/proc");
    if (!dir) return;
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (isdigit(entry->d_name[0])) {
            std::string path = std::string("/proc/") + entry->d_name + "/comm";
            FILE* f = fopen(path.c_str(), "r");
            if (f) {
                char comm[256];
                if (fgets(comm, sizeof(comm), f)) {
                    comm[strcspn(comm, "\n")] = 0;
                    if (name == comm) {
                        kill(std::stoi(entry->d_name), SIGTERM);
                        std::cout << "  [Killer] Found and signaled: " << name << " (PID: " << entry->d_name << ")" << std::endl;
                    }
                }
                fclose(f);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--id" && i + 1 < argc) {
            kill(std::stoi(argv[++i]), SIGTERM);
        } else if (std::string(argv[i]) == "--name" && i + 1 < argc) {
            killByName(argv[++i]);
        }
    }

    char* env = getenv("PROC_TO_KILL");
    if (env) {
        std::stringstream ss(env);
        std::string name;
        while (std::getline(ss, name, ',')) {
            if (!name.empty()) killByName(name);
        }
    }
    return 0;
}