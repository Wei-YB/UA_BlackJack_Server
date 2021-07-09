#ifndef _COMMON_H_
#define _COMMON_H_
#include <iostream>
#include <string>
#include <unordered_map>

namespace ua_blackjack
{
    class Response;
    class Request;
}

using ua_blackjack::Response;
using ua_blackjack::Request;

#define DEBUG_MODE  0

#define NS UA_BLACKJACK_SERVER
#define NAMESPACE_BEGIN namespace NS {

#define NAMESPACE_END   }
typedef int FileDesc;
typedef int64_t UserId;


void print(std::ostream &os, const Response &response);

void print(std::ostream &os, const Request &request);

void print(std::ostream &os, int events);

#endif
