#ifndef BUFFER_H
#define BUFFER_H

#define BUFFSIZE 8192

#include <errno.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>

class Rio {
public:
    Rio(int fd) : fd_(fd), cnt_(0), bufptr_(buff_) {}
    void RioReadInit(int fd);
    int RioBufferReadn(char* usrbuf, int n);
    int RioReadn(char* usrbuf, int n);
    int RioWriten(char* usrbuf, int n);

private:
    int fd_;
    int cnt_;
    char* bufptr_;
    char buff_[BUFFSIZE];

    int RioBufferRead(char* usrbuf, int n);
};

#endif