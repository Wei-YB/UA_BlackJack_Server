#ifndef _CIRC_BUF_H_
#define _CIRC_BUF_H_
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define MIN(a, b)   ((a) > (b) ? (b) : (a))
#define QUEUE_CAPACITY  64

namespace Net
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
        tail = (tail + ret) % capacity;
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

template<typename T>
int readAs(const CircularBuffer &buffer, size_t off, T &out, bool netByteOrder = false)
{
    // check it is possible to read a T type object from buffer
    if (buffer.size() < sizeof(T) + off)
        return -1;
    int tailRoom = buffer.m_tail >= ((buffer.m_head + off) % buffer.m_bufferSize) ?\
                    buffer.m_tail - buffer.m_head - off + 1 \
                    : buffer.m_bufferSize - buffer.m_head - off;
    // case 1: the tail room is long enough to hold the read type 
    if (tailRoom >= sizeof(T))
    {
        out = *((T*)(buffer.m_buffer + buffer.m_head + off));
    }
    // case 2: discontinuous memory, need to read it byte by byte
    else
    {
        char *ptr = (char*)&out;
        for (int i = 0; i < sizeof(T); ++i)
        {
            *(ptr + i) = *(buffer.m_buffer + ((buffer.m_head + off + i) % buffer.m_bufferSize));
        }
    }
    // we only allow int32_t or short to translate from net byte order to host order
    if (netByteOrder)
    {
        if (sizeof(T) == 2)
        {
            uint16_t tmp = ntohs(*((uint16_t*)(&out)));
            out = *((T*)(&tmp));
        }
        else if (sizeof(T) == 4)
        {
            uint32_t tmp = ntohl(*((uint32_t*)(&out)));
            out = *((T*)(&tmp));
        }
        else
        {
            return -1;
        }
    }
    
    return 0;
}

template<typename T>
int writeAs(CircularBuffer &buffer, T &in, bool netByteOrder = false)
{
    T dataToWrite;
    // check it is possible to read a T type object from buffer
    if (buffer.capacity() - buffer.size() < sizeof(T))
        return -1;
    // we only allow int32_t or short to translate from net byte order to host order
    
    if (netByteOrder)
    {
        if (sizeof(T) == 2)
        {
            *((uint16_t*)(&dataToWrite)) = ntohs(*((uint16_t*)(&in)));
        }
        else if (sizeof(T) == 4)
        {
            *((uint32_t*)(&dataToWrite)) = ntohl(*((uint32_t*)(&in)));
        }
        else
        {
            return -1;
        }
    }
    else
    {
        dataToWrite = in;
    }

    int freeRoom = buffer.capacity() - buffer.size();
    int tailRoom = buffer.m_tail >= buffer.m_head ? buffer.capacity() - buffer.m_tail - 1 : freeRoom;
    // case 1: the tail room is long enough to hold the read type 
    if (tailRoom >= sizeof(T))
    {
        *((T*)(buffer.m_buffer + buffer.m_tail + 1)) = dataToWrite;
    }
    // case 2: discontinuous memory, need to read it byte by byte
    else
    {
        char *ptr = (char*)&dataToWrite;
        for (int i = 0; i < sizeof(T); ++i)
        {
            *(buffer.m_buffer + ((buffer.m_tail + 1 + i) % buffer.m_bufferSize)) = *(ptr + i);
        }
    }
    buffer.m_tail = (buffer.m_tail + sizeof(T)) % buffer.m_bufferSize;
    
    return 0;
}

void circularBufferToString(const CircularBuffer &buffer, size_t length, std::string &str)
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



template<typename T>
class CircularQueue {
public:
    CircularQueue(size_t capacity = QUEUE_CAPACITY) : m_capacity(capacity), m_head(0), m_tail(0)
    {
        // to realize lock-free operations, we need to assign one more idel item
        m_buffer = new T[capacity + 1];
        if (!m_buffer)
            throw "CircularQueue: fail to create CircularQueue.";
    }

    ~CircularQueue() {delete [] m_buffer;}

public:
    // this method can only be called by producer thread
    int push(const T &item)
    {
        int head = m_head;
        // the queue is full
        if (((m_tail + 1) % (m_capacity + 1)) == head)
        {
            return -1;
        }
        m_buffer[m_tail] = item;
        m_tail = (m_tail + 1) % (m_capacity + 1);
        return 0;
    }

    // this method can only be called by consumer thread
    int front(T &item)
    {
        int tail = m_tail;
        // the queue is empty
        if (tail == m_head)
        {
            return -1;
        }
        item = m_buffer[m_head];
        return 0;
    }

    // this method can only be called by consumer thread
    void pop()
    {
        int tail = m_tail;
        if (tail != m_head)
        {
            m_head = (m_head + 1) % (m_capacity + 1);
        }
    } 
      
private:
    const size_t m_capacity;
    T* m_buffer;
    int m_head;
    int m_tail;
};

}
#endif
