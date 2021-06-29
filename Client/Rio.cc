#include "Rio.h"

using namespace ua_blackjack::robust_io;

void Rio::RioReadInit(int fd) {
    fd_ = fd;
    cnt_ = 0;
    bufptr_ = buff_;
}

int Rio::RioBufferRead(char* usrbuf, int n) {
    int ret;

    while (cnt_ <= 0) {
        cnt_ = read(fd_, buff_, sizeof(buff_));
        if (cnt_ < 0) {
            if (errno != EINTR) return -1;
        } else if (cnt_ == 0) {
            return 0;
        } else {
            bufptr_ = buff_;
        }
    }

    ret = std::min(cnt_, n);
    memcpy(usrbuf, bufptr_, ret);
    bufptr_ += ret;
    cnt_ -= ret;
}

int Rio::RioBufferReadn(char* usrbuf, int n) {
    int nleft = n;
    int nread;
    char* bufp = usrbuf;
    while (nleft > 0) {
        if ((nread = RioBufferRead(bufp, nleft)) < 0) {
            return -1;
        } else if (nread == 0) {
            break;
        }
        nleft -= nread;
        bufp += nread;
    }

    return n - nleft;
}

int Rio::RioBufferReadLine(char* usrbuf, int maxlen) {
    int n, rc;
    char* bufp = usrbuf;
    char c;
    for (n = 1; n < maxlen; ++n) {
        if ((rc = RioBufferRead(&c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                ++n;
                break;
            }
        } else if (rc == 0) {
            if (n == 1) {
                return 0;
            } else {
                break;
            }
        } else {
            return -1;
        }
    }
    *bufp = 0;
    return n - 1;
}

int Rio::RioReadn(char* usrbuf, int n) {
    int nleft = n;
    int nread = 0;
    char* bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = read(fd_, bufp, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else {
                return -1;
            }
        } else if (nread == 0) {
            break;
        }

        nleft -= nread;
        bufp += nread;
    }

    return n - nleft;
}

int Rio::RioWriten(char* usrbuf, int n) {
    int nleft = n;
    int nwritten = 0;
    char* bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd_, bufp, nleft)) < 0) {
            if (errno == EINTR) {
                nwritten = 0;
            } else {
                return -1;
            }
        }

        nleft -= nwritten;
        bufp += nwritten;
    }

    return n - nleft;
}