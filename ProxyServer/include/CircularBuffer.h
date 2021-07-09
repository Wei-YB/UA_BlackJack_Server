#ifndef _CIRC_BUF_H_
#define _CIRC_BUF_H_
#include <string>
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
    CircularBuffer(int bufferSize);

    CircularBuffer(const CircularBuffer &circBuffer);

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

// this struct can greately reduce the number of memcpy or malloc for string
struct StringPiece
{
    CircularBuffer *cirBuf_;
    int length_;
    int head_;

    StringPiece(CircularBuffer *cirBuf, int start, int len) 
            : cirBuf_(cirBuf), head_(start), length_(len) {}
    
    char *head() const {return cirBuf_->m_buffer + head_;}
    int length() const {return length_;}
    bool continuous() const;
    void free();
};

int put(CircularBuffer &buffer, const char *src, size_t n);

int get(CircularBuffer &buffer, char *dst, size_t n);

int read(int fd, CircularBuffer &buffer);

int write(int fd, CircularBuffer &buffer, bool freeAfterWrite = true);

template<typename T>
int readAs(const CircularBuffer &buffer, size_t off, T &out, bool netByteOrder = false)
{
    // check it is possible to read a T type object from buffer
    if (buffer.size() < sizeof(T) + off)
        return -1;
    int tailRoom = buffer.m_tail >= ((buffer.m_head + off) % buffer.m_bufferSize) ?\
                    buffer.m_tail - ((buffer.m_head + off) % buffer.m_bufferSize) + 1 \
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

void circularBufferToString(const CircularBuffer &buffer, size_t length, std::string &str);

std::string circularBufferToString(const CircularBuffer &buffer, size_t length);


// for performance concern, T should be movable class
// this class is not thread safe, and I recommend 
// passing notifications instead of using lock for 
// synchronization.  
template<typename T>
class CircularQueue {
public:
    CircularQueue(size_t capacity) : capacity_(capacity), head_(0), tail_(0)
    {
        // to realize lock-free operations, we need to assign one more idel item
        buffer_ = new T[capacity + 1];
        if (!buffer_)
            throw "CircularQueue: fail to create CircularQueue.";
    }

    ~CircularQueue() {delete [] buffer_;}

public:
    size_t Capacity() const {return capacity_;}

    size_t Size() const 
    {
        return tail_ < head_ ? \
                tail_ + capacity_ + 1 - head_ \
                : tail_ - head_;
    }

    bool Push(T &&item)
    {
        if (Size() == capacity_) return false;
        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % (capacity_ + 1);
        return true;
    }

    // warning: should check Size() before Pop.
    T &&Pop()
    {
        if (Size() == 0) return std::move(T());
        int head = head_;
        head_ = (head_ + 1) % (capacity_ + 1);
        return std::move(buffer_[head]);
    }
      
private:
    const size_t capacity_;
    T* buffer_;
    int head_;
    int tail_;
};

}
#endif
