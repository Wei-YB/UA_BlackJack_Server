#ifndef _CLIENTPROXYPROTOCOL_H_
#define _CLIENTPROXYPROTOCOL_H_

#include <sys/types.h>
#include <string>
#include "CircularBuffer.h"

using namespace Net;
 
#define PACKAGE_HDR_LEN 8

static const int32_t INVAL = 0;
static const int32_t REQUEST = 1;
static const int32_t RESPONSE = 2;

int pack(int32_t type, const std::string &msg, CircularBuffer &buffer);

std::string unpack(Net::CircularBuffer &buffer, int32_t *type);

StringPiece unpack_sp(Net::CircularBuffer &buffer, int32_t *type);

namespace ua_blackjack
{
    class Request;
    class Response;
}
using ua_blackjack::Request;
using ua_blackjack::Response;

void ParseFromStringPiece(Request &request, StringPiece stringPiece);

void ParseFromStringPiece(Request &response, StringPiece stringPiece);

#endif