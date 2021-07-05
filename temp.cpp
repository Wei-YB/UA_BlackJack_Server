#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>

int main()
{
    const char *ip = "127.0.0.1";
    unsigned short port = 54321;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 < connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)))
    {
        std::cout << "successfully connect to host" << std::endl;
    }
    else
        std::cout << "fail to connect." << std::endl;
    close(sockfd);
    return 0;
}