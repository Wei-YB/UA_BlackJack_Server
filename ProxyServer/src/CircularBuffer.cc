#include <string>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "CircularBuffer.h"

using namespace Net;

CircularBuffer::CircularBuffer(int bufferSize) : m_bufferSize(bufferSize)
{
    m_buffer = new char[bufferSize];
    if (!m_buffer)
    {
        throw "CircularBuffer: fail to allocate buffer.\n";
    }
    m_head = 0;
    m_tail = -1;
}

CircularBuffer::CircularBuffer(const CircularBuffer &circBuffer) : m_bufferSize(circBuffer.m_bufferSize)
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

int Net::put(CircularBuffer &buffer, const char *src, size_t n)
{
    int head = buffer.m_head, tail = buffer.m_tail;
    int freeRoom = buffer.capacity() - buffer.size();
    int tailRoom = tail >= head ? buffer.capacity() - tail - 1 : freeRoom;
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

int Net::get(CircularBuffer &buffer, char *dst, size_t n)
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

int Net::read(int fd, CircularBuffer &buffer)
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
        tail = (tail + ret) % capacity;
        buffer.m_tail = tail;
    } while (tail == capacity - 1 && head != 0);

    return buffer.size() - oldSize;
}

int Net::write(int fd, CircularBuffer &buffer, bool freeAfterWrite)
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
        writeSize = tail >= writeIdx ? tail - writeIdx + 1 : capacity - writeIdx;
        ret = ::write(fd, rawBuffer + writeIdx, writeSize);
        if (ret <= 0)
        {
            if (ret == 0 || errno == EWOULDBLOCK || errno == EAGAIN || errno == EINTR)
            {
                if (freeAfterWrite)
                    buffer.free(cnt);
                return cnt;
            }
            buffer.free(cnt);
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

void Net::circularBufferToString(const CircularBuffer &buffer, size_t length, ::std::string &str)
{
    int tailRoom;
    if (buffer.empty())
        return;
    if (length > buffer.size())
        length = buffer.size();
    tailRoom = buffer.m_tail >= buffer.m_head ?\
                                buffer.m_tail - buffer.m_head + 1 \
                                : buffer.m_bufferSize - buffer.m_head;
    if (tailRoom >= length)
    {
        str.append(buffer.m_buffer + buffer.m_head, length);
    }
    else
    {
        str.append(buffer.m_buffer + buffer.m_head, tailRoom);
        str.append(buffer.m_buffer, length - tailRoom);
    }
}

std::string Net::circularBufferToString(const CircularBuffer &buffer, size_t length)
{
    if (buffer.empty()) return "";
    length = length > buffer.size() ? buffer.size() : length;

    int tailRoom = buffer.m_tail >= buffer.m_head ?\
                                buffer.m_tail - buffer.m_head + 1 \
                                : buffer.m_bufferSize - buffer.m_head;
    if (tailRoom >= length)
    {
        return std::move(std::string(buffer.m_buffer + buffer.m_head, length));
    }
    else
    {
        std::string res(buffer.m_buffer + buffer.m_head, tailRoom);
        return std::move(res.append(buffer.m_buffer, length - tailRoom));
    }
}


