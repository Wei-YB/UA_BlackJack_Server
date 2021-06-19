#ifndef _CIRC_BUF_H_
#define _CIRC_BUF_H_
#include <string.h>
#include <unistd.h>

#define MAX(a, b)   a > b ? a : b
#define MIN(a, b)   a > b ? b : a

namespace tcp
{
    
struct CircularBuffer
{
    CircularBuffer(int bufferSize) : m_bufferSize(bufferSize)
    {
        m_buffer = new char[bufferSize];
        if (!m_buffer)
        {
            throw "CircularBuffer: fail to allocate buffer.\n";
        }
        m_head = 0;
        m_tail = -1;
    }

    CircularBuffer(const CircularBuffer &circBuffer) : m_bufferSize(circBuffer.m_bufferSize)
    {
        m_buffer = new char[m_bufferSize];
        if (!m_buffer)
        {
            throw "CircularBuffer: fail to allocate buffer.\n";
        }
        memcpy(m_buffer, circBuffer.m_buffer, m_bufferSize);
        m_head = circBuffer.m_head;
        m_tail = circBuffer.m_tail;
    }

    CircularBuffer &operator=(const CircularBuffer &circBuffer) = delete;

    ~CircularBuffer() {delete [] m_buffer;}

    bool empty() const {return m_tail == -1;}

    int capacity() const {return m_bufferSize;}

    int size() const 
    {
        if (m_tail == -1)
            return 0;
        return m_tail >= m_head ? m_tail - m_head + 1 : m_bufferSize - m_head + m_tail + 1;
    }

    void free(int nbytes) 
    {
        if (size() <= nbytes)
        {
            m_head = 0;
            m_tail = -1;
        }
        else
        {
            m_head = m_bufferSize - m_head > nbytes ? m_head + nbytes : nbytes - m_bufferSize + m_head;
        }
    }

    char *m_buffer;
    const int m_bufferSize;
    int m_head;
    int m_tail;
};

int put(CircularBuffer &buffer, const char *src, size_t n)
{
    int head = buffer.m_head, tail = buffer.m_tail;
    int freeRoom = buffer.capacity() - buffer.size();
    int tailRoom = tail >= head ? buffer.capacity() - head - 1 : freeRoom;
    char *rawBuffer = buffer.m_buffer;
    n = MIN(n, freeRoom);
    
    if ((tail >= head && tailRoom >= n) || buffer.m_tail < buffer.m_head)
    {
        memcpy(rawBuffer + tail + 1, src, n);
        tail += n;
    }
    else
    {
        memcpy(rawBuffer + tail + 1, src, tailRoom);
        memcpy(rawBuffer, src + tailRoom, n - tailRoom);
        tail = n - tailRoom - 1;
    }
    
    buffer.m_tail = tail;
    return n;
}

int get(CircularBuffer &buffer, char *dst, size_t n)
{
    int head = buffer.m_head, tail = buffer.m_tail;
    int capacity = buffer.capacity();
    char *rawBuffer = buffer.m_buffer;
    n = MIN(n, buffer.size());
    
    if (tail >= head || n <= capacity - head)
    {
        memcpy(dst, rawBuffer + head, n);
    }
    else
    {
        memcpy(dst, rawBuffer + head, capacity - head);
        memcpy(dst + capacity - head, rawBuffer, n - capacity + head);
    }
    
    return n;
}

int read(int fd, CircularBuffer &buffer)
{
    int ret = 0;
    int tail = buffer.m_tail, head = buffer.m_head;
    int capacity = buffer.capacity();
    int oldSize = buffer.size();
    char *rawBuffer = buffer.m_buffer;
    // if the buffer is full, don't read in
    if (oldSize == capacity)
    {
        return ret;
    }
    
    do {
        int readIdx = (tail + 1) % capacity;
        int readSize = head > readIdx ? head - readIdx : capacity - readIdx;
        ret = ::read(fd, rawBuffer + readIdx, readSize);
        if (ret <= 0)
        {
            if (ret < 0 && (errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR))
            {
                return buffer.size() - oldSize;
            }
            return -1;
        } 
        tail += ret;
        buffer.m_tail = tail;
    } while (tail == capacity - 1);

    return buffer.size() - oldSize;
}

int write(int fd, CircularBuffer &buffer, bool freeAfterWrite = true)
{
    int ret = 0;
    int tail = buffer.m_tail, head = buffer.m_head;
    int capacity = buffer.capacity();
    int oldSize = buffer.size();
    char *rawBuffer = buffer.m_buffer;
    // if the buffer is empty, don't write out
    if (oldSize == 0)
    {
        return ret;
    }
    
    int writeIdx = head;
    int writeSize, cnt = 0;
    do {
        writeSize = tail >= writeIdx ? tail - writeIdx + 1 : capacity - writeIdx + 1;
        ret = ::write(fd, rawBuffer + writeIdx, writeSize);
        if (ret <= 0)
        {
            if (cnt)
            {
                buffer.free(cnt);
            }
            if (ret == 0 || errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
            {
                return cnt;
            }
            return -1;
        } 
        cnt += ret;
        writeIdx = (writeIdx + ret) % capacity;
    } while (writeIdx == 0 && tail != capacity - 1);

    if (freeAfterWrite)
    {
        buffer.free(cnt);
    }
    return cnt;
}

};
#endif