#include <iostream>
#include <string>
#include "../include/net/circ_buf.h"

// test the function of writeAs and readAs
void test1()
{
    // assign a 128 bytes buffer
    Net::CircularBuffer buffer(128);
    // normal read and write
    int32_t num = 23478;
    if (0 > Net::writeAs(buffer, num))
    {
        std::cout << 
    }
    Net::
}


int main()
{
    
}