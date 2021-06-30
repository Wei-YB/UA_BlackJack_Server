#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>

#include "Rio.h"
#include "UA_BlackJack.pb.h"

int inetPassiveSocket(const char* service, int type, socklen_t* addrlen, bool doListen, int backlog) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, optval, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = nullptr;
    hints.ai_addr = nullptr;
    hints.ai_next = nullptr;
    hints.ai_socktype = type;
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(nullptr, service, &hints, &result);
    if (s != 0) {
        return -1;
    }

    optval = 1;
    for (rp = result; rp != nullptr; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }

        close(sfd);
    }

    if (rp != nullptr && doListen) {
        if (listen(sfd, backlog) == -1) {
            free(result);
            return -1;
        }
    }

    if (rp != nullptr && addrlen != nullptr) {
        *addrlen = rp->ai_addrlen;
    }

    freeaddrinfo(result);

    return (rp == nullptr) ? -1 : sfd;
}

int inetListen(const char* service, int backlog, socklen_t* addrlen) {
    return inetPassiveSocket(service, SOCK_STREAM, addrlen, true, backlog);
}

int inetBind(const char* service, int type, socklen_t* addrlen) {
    return inetPassiveSocket(service, type, addrlen, false, 0);
}

unsigned char buffer_out_[8492];

int SerializeRoomResponse(ua_blackjack::Response& response) {
    auto data = response.SerializeAsString();

    uint32_t data_type = 2;
    uint32_t data_length = data.size();

    memset(buffer_out_, '\0', sizeof(buffer_out_));
    unsigned char* bufptr = buffer_out_;

    // construct data type
    uint32_t data_type_netorder = htonl(data_type);
    unsigned char* p = (unsigned char*)&data_type_netorder;
    for (int i = 0; i < sizeof(data_type_netorder); ++i) {
        *(bufptr++) = *(p + i);
    }

    // construct data length
    uint32_t data_length_netorder = htonl(data_length);
    p = (unsigned char*)&data_length_netorder;
    for (int i = 0; i < sizeof(data_length_netorder); ++i) {
        *(bufptr++) = *(p + i);
    }

    // construct data
    snprintf((char*)bufptr, sizeof(buffer_out_) - sizeof(data_type) - sizeof(data_length), data.c_str());

    return sizeof(data_type) + sizeof(data_length) + data.size();
}
int main() {
    int lfd = inetListen("50051", 10, nullptr);
    if (lfd == -1) {
        std::cout << "listen error" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Listening on 50051" << std::endl;

    int cfd = accept(lfd, nullptr, nullptr);
    if (cfd == -1) {
        std::cout << "accept error" << std::endl;
    }

    Rio rio(cfd);
    rio.RioReadInit(cfd);

    while (true) {
        // read data type
        uint32_t data_type;
        int len = rio.RioReadn((char*)&data_type, sizeof(data_type));
        if (len == 0) {
            close(cfd);
            break;
        }
        data_type = ntohl(data_type);

        std::cout << "data type: " << data_type << std::endl;

        // read data length
        uint32_t data_length;
        rio.RioReadn((char*)&data_length, sizeof(data_length));
        data_length = ntohl(data_length);

        std::cout << "data length: " << data_length << std::endl;

        assert(data_length > 0);

        // read data
        std::string data;
        data.resize(data_length);
        char* p = &data[0];
        rio.RioReadn(p, data_length);

        if (data_type == 1) {
            ua_blackjack::Request request;
            request.ParseFromString(data);
            std::cout << "request: ";
            std::cout << request.requesttype() << " " << request.uid() << " " << request.stamp() << " ";
            for (auto& arg : request.args()) std::cout << arg << " ";
            std::cout << std::endl;

            ua_blackjack::Response response;
            response.set_uid(request.uid());
            response.set_stamp(request.stamp());
            response.set_status(1);
            response.add_args("Roger that, well done");

            int res_len = SerializeRoomResponse(response);

            // write response to proxy
            rio.RioWriten((char*)buffer_out_, res_len);

        } else if (data_type == 2) {
            ua_blackjack::Response response;
            response.ParseFromString(data);
            std::cout << "Response: ";
            std::cout << response.status() << " " << response.uid() << " " << response.stamp() << " ";
            for (auto& arg : response.args()) std::cout << arg << " ";
            std::cout << std::endl;
        } else {
            std::cout << "ListenProxy: Wrong data type" << std::endl;
        }
    }

    return 0;
}