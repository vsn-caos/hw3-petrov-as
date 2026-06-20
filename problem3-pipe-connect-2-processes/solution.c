#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s CMD1 CMD2\n", argv[0]);
        return 1;
    }
    char *cmd1 = argv[1];
    char *cmd2 = argv[2];
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("fork1");
        return 1;
    } else if (pid1 == 0) {
        close(pipefd[0]);        
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        close(pipefd[1]);
        execlp("/bin/sh", "sh", "-c", cmd1, NULL);
        perror("execlp cmd1");
        exit(1);
    }
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("fork2");
        return 1;
    } else if (pid2 == 0) {
        close(pipefd[1]);          
        if (dup2(pipefd[0], STDIN_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        close(pipefd[0]);
        execlp("/bin/sh", "sh", "-c", cmd2, NULL);
        perror("execlp cmd2");
        exit(1);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
