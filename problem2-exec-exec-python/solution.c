#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    char expr[4096];
    if (fgets(expr, sizeof(expr), stdin) == NULL) {
        return 0;   
    }
    size_t len = strlen(expr);
    if (len > 0 && expr[len - 1] == '\n') {
        expr[len - 1] = '\0';
    }
    char cmd[4096];
    snprintf(cmd, sizeof(cmd), "print(%s)", expr);
    execlp("python3", "python3", "-c", cmd, NULL);
    perror("execlp python3");
    return 1;
}
