#include <sys/types.h>
#include <string>
#include <arpa/inet.h>
#include <string.h>
#include "common.h"
#include "CircularBuffer.h"
#include "ClientProxyProtocol.h"
#include "UA_BlackJack.pb.h"

using namespace Net;
using ua_blackjack::Request;
using ua_blackjack::Response;

int pack(int32_t type, const std::string &msg, CircularBuffer &buffer)
{
    int32_t msgLen = msg.length();
    if (buffer.capacity() - buffer.size() < msgLen + PACKAGE_HDR_LEN)
        return -1;
    writeAs(buffer, type, true);
    writeAs(buffer, msgLen, true);
    put(buffer, msg.c_str(), msgLen);
    return 0;
}

std::string unpack(CircularBuffer &buffer, int32_t *type)
{
    // the package must be bigger tha 8 bytes to be complete
    if (buffer.size() < PACKAGE_HDR_LEN)
    {
        return "";
    }
    int32_t msgLen;
    readAs(buffer, 0, *type, true);
    readAs(buffer, 4, msgLen, true);
    if (buffer.size() >= msgLen + PACKAGE_HDR_LEN)
    {
        buffer.free(PACKAGE_HDR_LEN);
        return std::move(circularBufferToString(buffer, msgLen));
    }
    return "";
}

StringPiece unpack_sp(Net::CircularBuffer &buffer, int *offset, int32_t *type)
{
    // the package must be bigger tha 8 bytes to be complete
    int off = *offset;
    if (buffer.size() - off < PACKAGE_HDR_LEN)
    {
        return StringPiece(NULL, 0, 0);
    }
    int32_t msgLen;
    readAs(buffer, off, *type, true);
    readAs(buffer, off + 4, msgLen, true);
    if (buffer.size() - off >= msgLen + PACKAGE_HDR_LEN)
    {
        // buffer.free(PACKAGE_HDR_LEN);
        *offset = (off + msgLen + PACKAGE_HDR_LEN) % buffer.capacity();
        return StringPiece(&buffer, (buffer.m_head + off + PACKAGE_HDR_LEN) % buffer.capacity(), msgLen);
    }
    return StringPiece(NULL, 0, 0);
}

void ParseFromStringPiece(Request &request, StringPiece stringPiece)
{
    if (stringPiece.continuous())
    {
        request.ParseFromArray((void*)stringPiece.head(), stringPiece.length());
    }
    else
    {
        std::string tmp(stringPiece.length(), '\0');
        get(*stringPiece.cirBuf_, (char*)tmp.c_str(), tmp.size());
        request.ParseFromString(tmp);
    }
    stringPiece.free();
}

void ParseFromStringPiece(Response &response, StringPiece stringPiece)
{
    if (stringPiece.continuous())
    {
        response.ParseFromArray((void*)stringPiece.head(), stringPiece.length());
    }
    else
    {
        std::string tmp(stringPiece.length(), '\0');
        get(*stringPiece.cirBuf_, (char*)tmp.c_str(), tmp.size());
        response.ParseFromString(tmp);
    }
    stringPiece.free();
}

