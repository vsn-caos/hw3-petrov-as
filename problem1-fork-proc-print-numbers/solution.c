#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void print_chain(int num, int N) {
    if (num > N) return;
    if (num == N) {
        printf("%d\n", num);
        fflush(stdout);
        return;
    }
    printf("%d ", num);
    fflush(stdout);
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        print_chain(num + 1, N);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s N\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "N must be > 0\n");
        return 1;
    }
    print_chain(1, N);
    return 0;
}
