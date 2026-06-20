#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <substring>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    const char *needle = argv[2];
    size_t needle_len = strlen(needle);
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    struct stat st;
    if (fstat(fd, &st) == -1) {
        perror("fstat");
        close(fd);
        return 1;
    }
    off_t size = st.st_size;
    if (size == 0) {
        close(fd);
        return 0;
    }
    char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    if (needle_len == 0) {
        munmap(data, size);
        close(fd);
        return 0;
    }
    char *pos = data;
    while ((pos = memmem(pos, size - (pos - data), needle, needle_len)) != NULL) {
        printf("%ld\n", pos - data);  
        pos += needle_len;
    }
    munmap(data, size);
    close(fd);
    return 0;
}
