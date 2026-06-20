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
    execlp("python3", "python3", "-c", expr, NULL);
    perror("execlp python3");
    return 1;
}
