#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <endian.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IPv4-address> <port>\n", argv[0]);
        return 1;
    }
    const char *addr_str = argv[1];
    int port = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("socket");
        return 1;
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, addr_str, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        return 1;
    }
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        close(sockfd);
        return 1;
    }
    int num;
    uint32_t net_num;
    while (scanf("%d", &num) == 1) {
        net_num = htole32((uint32_t)num);
        if (send(sockfd, &net_num, sizeof(net_num), 0) != sizeof(net_num)) {
            perror("send");
            break;
        }
        uint32_t reply;
        ssize_t recvd = recv(sockfd, &reply, sizeof(reply), 0);
        if (recvd == -1) {
            perror("recv");
            break;
        } else if (recvd == 0) {
            close(sockfd);
            return 0;
        } else if (recvd != sizeof(reply)) {
            fprintf(stderr, "Partial recv\n");
            break;
        }
        uint32_t host_reply = le32toh(reply);
        printf("%d\n", (int)host_reply);
        fflush(stdout);
    }
    close(sockfd);
    return 0;
}
