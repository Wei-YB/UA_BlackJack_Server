#ifndef BUFFER_H
#define BUFFER_H

#define BUFFSIZE 8192

#include <errno.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>

namespace ua_blackjack {
namespace robust_io {
class Rio {
public:
    Rio() : fd_(-1), cnt_(0), bufptr_(buff_) {}
    void RioReadInit(int fd);
    int RioBufferReadn(char* usrbuf, int n);
    int RioBufferReadLine(char* usrbuf, int maxlen);
    int RioReadn(char* usrbuf, int n);
    int RioWriten(char* usrbuf, int n);

private:
    int fd_;
    int cnt_;
    char* bufptr_;
    char buff_[BUFFSIZE];

    int RioBufferRead(char* usrbuf, int n);
};
}  // namespace robust_io
}  // namespace ua_blackjack

#endif