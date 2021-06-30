#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_adress port\n", basename(argv[0]));
        return -1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof address);
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);
    if (connect(sock, (struct sockaddr *)&address, sizeof address) < 0)
    {
        perror("connect");
    }
    else
    {
        while (true)
        {
            std::string sendStr;
            std::cin >> sendStr;
            send(sock, sendStr.c_str(), sendStr.size(), 0);
        }
    }
}