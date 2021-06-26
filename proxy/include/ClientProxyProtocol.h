#ifndef _CLIENTPROXYPROTOCOL_H_
#define _CLIENTPROXYPROTOCOL_H_

#include <sys/types.h>
#include <string>
#include "CircularBuffer.h"

NAMESPACE_BEGIN

#define PACKAGE_HDR_LEN 8

const int32_t REQUEST = 0;
const int32_t RESPONSE = 1;


int pack(int32_t type, const std::string &msg, char *buffer, size_t size);

int pack(int32_t type, const std::string &msg, ::Net::CircularBuffer &buffer);

int unpack(const char *buffer, size_t size, int32_t *type, std::string &msg);

int unpack(::Net::CircularBuffer &buffer, int32_t *type, std::string &msg);

NAMESPACE_END

#endif