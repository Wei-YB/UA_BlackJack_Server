#include <sys/types.h>
#include <string>
#include <arpa/inet.h>
#include <string.h>
#include "common.h"
#include "CircularBuffer.h"
#include "ClientProxyProtocol.h"

using namespace NS;

int NS::pack(int32_t type,  
        const std::string &msg, 
        char *buffer, size_t size)
{
    // check that the buffer is large enough
    if (size < 8 + msg.size())
    {
        return -1;
    }
    type = ::htonl(type);
    uint32_t length = ::htonl(msg.size());
    *((int32_t*)buffer) = type;
    *((int32_t*)(buffer + 4)) = length;
    memcpy(buffer + 8, msg.c_str(), msg.size());
    return 0;
}

int NS::pack(int32_t type,  
        const std::string &msg, 
        ::Net::CircularBuffer &buffer)
{
    int32_t msgLen = msg.length();
    if (buffer.capacity() - buffer.size() < msg.length() + 8)
        return -1;
    ::Net::writeAs(buffer, type, true);
    ::Net::writeAs(buffer, msgLen, true);
    ::Net::put(buffer, msg.c_str(), msg.length());
    return 0;
}

int NS::unpack(const char *buffer, size_t size, int32_t *type, std::string &msg)
{
    // the package must be bigger tha 8 bytes to be complete
    if (size < 9)
    {
        return -1;
    }
    *type = ntohl(*((int32_t*)buffer));
    int32_t length = ntohl(*((int32_t*)(buffer + 4)));
    msg = std::string(buffer + 8, length - 8);
    return 0;
}

int NS::unpack(::Net::CircularBuffer &buffer, int32_t *type, std::string &msg)
{
    // the package must be bigger tha 8 bytes to be complete
    if (buffer.size() < 9)
    {
        return -1;
    }
    int32_t msgLen;
    ::Net::readAs(buffer, 0, *type, true);
    ::Net::readAs(buffer, 4, msgLen, true);
    buffer.free(8);
    if (buffer.size() >= msgLen)
    {
        ::Net::circularBufferToString(buffer, msgLen, msg);
    }
    return 0;
}
