#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

void spawn(const char* cmd, int in_fd, int out_fd) {
    pid_t pid = fork();
    if (pid == 0) {
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        execl(cmd, cmd, nullptr);
        perror("execl failed");
        exit(1);
    }
}

int main() {
    int p1[2], p2[2], p3[2];
    
    if (pipe(p1) == -1 || pipe(p2) == -1 || pipe(p3) == -1) {
        perror("Pipe failed");
        return 1;
    }
    spawn("./m", STDIN_FILENO, p1[1]);
    close(p1[1]);

    spawn("./a", p1[0], p2[1]);
    close(p1[0]);
    close(p2[1]);

    spawn("./p", p2[0], p3[1]);
    close(p2[0]);
    close(p3[1]);

    spawn("./s", p3[0], STDOUT_FILENO);
    close(p3[0]);

    for(int i = 0; i < 4; ++i) wait(nullptr);
    return 0;
}