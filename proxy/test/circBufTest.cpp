#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include "../include/net/circ_buf.h"

// test the function of write across and read across situations
// netByte indicates whether use the network byte order
void test1(bool netByte = false)
{
    // assign a 130 bytes buffer
    Net::CircularBuffer buffer(130);
    // write 32 int32_t variables to buffer
    for (int32_t i = 1; i <= 32; ++i)
    {
        Net::writeAs(buffer, i, netByte);
    }
    // read 32 integers from buffer
    for (int32_t i = 1; i < 32; ++i)
    {
        int32_t res;
        Net::readAs(buffer, 0, res, netByte);
        if (res != i)
        {
            if (netByte)
                std::cout << "test1 fail with netByte enabled, read result is not consistent with write result." << std::endl;
            else
                std::cout << "test1 fail, read result is not consistent with write result." << std::endl;
            exit(0);
        }
        buffer.free(sizeof(res));
    }
    // write 10 more integers to buffer
    for (int32_t i = 33; i < 32 + 10; ++i)
    {
        Net::writeAs(buffer, i, netByte);
    }
    // read the remaining integers from buffer
    for (int32_t i = 32; i < 32 + 10; ++i)
    {
        int32_t res;
        Net::readAs(buffer, 0, res, netByte);
        if (res != i)
        {
            if (netByte)
                std::cout << "test1 fail with netByte enabled, read result is not consistent with write result." << std::endl;
            else
                std::cout << "test1 fail, read result is not consistent with write result." << std::endl;
            exit(0);
        }
        buffer.free(sizeof(res));
    }
    if (!buffer.empty())
    {
        if (netByte)
            std::cout << "test1 fail with netByte enabled, buffer not empty after reading all elements." << std::endl;
        else
            std::cout << "test1 fail, buffer not empty after reading all elements." << std::endl;
        exit(0);
    }
    if (netByte)
        std::cout << "test1 with netByte enabled pass." << std::endl;
    else
        std::cout << "test1 pass." << std::endl;
    return;
}

// test the put and get methods
void test2()
{
    const std::string helloStr = "Hello World.";
    const std::string byeStr = "Goodbye World.";
    char readBuffer[1024];
    // assign a buffer not long enough to hold two strings
    Net::CircularBuffer buffer((helloStr.size() + byeStr.size()) * 3 / 2);
    // put the helloStr to buffer
    if (helloStr.size() != Net::put(buffer, helloStr.c_str(), helloStr.size()))
    {
        std::cout << "test2 failed, the size putted in buffer is inconsistent with the string size." << std::endl;
        exit(0);
    }
    // read the content we just put into the buffer, and see whether they are equal
    int byteGet;
    if ((byteGet = Net::get(buffer, readBuffer, sizeof(readBuffer))) != helloStr.size())
    {
        std::cout << "test2 failed, the size put in buffer is inconsistent with the get size." << std::endl;
        exit(0);
    }
    // compare the content
    if (strncmp(helloStr.c_str(), readBuffer, byteGet) != 0)
    {
        std::cout << "test2 failed, the content get from the buffer is inconsistent with the string." << std::endl;
        exit(0);
    }
    // write 1 byte to avoid reset of the head and tail of buffer
    Net::put(buffer, readBuffer, 1);
    buffer.free(byteGet);
    // put the helloStr to buffer
    if (byeStr.size() != Net::put(buffer, byeStr.c_str(), byeStr.size()))
    {
        std::cout << "test2 failed, fail to put content correctly across the buffer edge." << std::endl;
        exit(0);
    }
    buffer.free(1);
    // read the content we just put into the buffer, and see whether they are equal
    if ((byteGet = Net::get(buffer, readBuffer, sizeof(readBuffer))) != byeStr.size())
    {
        std::cout << "test2 failed, the size put across the buffer edge is inconsistent with the get size." << std::endl;
        exit(0);
    }
    // compare the content
    if (strncmp(byeStr.c_str(), readBuffer, byteGet) != 0)
    {
        std::cout << "test2 failed, the content across the buffer edge is inconsistent with the string." << std::endl;
        exit(0);
    }
    buffer.free(byteGet);
    if (!buffer.empty())
    {
        std::cout << "test2 failed, the buffer after free is not empty." << std::endl;
        exit(0);
    }
    std::cout << "test2 passed." << std::endl;
}

// test the circularBufferToString method
void test3()
{
    const char *helloStr = "Hello World.";
    size_t len = strlen(helloStr);
    // assign a buffer not long enough to hold two strings
    Net::CircularBuffer buffer(len * 3 / 2);
    // put helloStr to buffer
    Net::put(buffer, helloStr, len);
    // 
    std::string str;
    Net::circularBufferToString(buffer, buffer.size(), str);
    // test whether the translated string is consistent with the helloStr
    if (strncmp(str.c_str(), helloStr, str.length()) != 0)
    {
        std::cout << "test3 failed, the content putted in buffer is inconsistent with the string transformed from the buffer." << std::endl;
        std::cout << "str: " << str << std::endl;
        exit(0);
    }
    // test the across case
    Net::put(buffer, helloStr, 1);
    buffer.free(len);
    Net::put(buffer, helloStr, len);
    buffer.free(1);
    std::string str2;
    Net::circularBufferToString(buffer, buffer.size(), str2);
    // test whether the translated string is consistent with the helloStr
    if (strncmp(str2.c_str(), helloStr, str2.length()) != 0)
    {
        std::cout << "test3 failed, the content putted across the buffer edge is inconsistent with the string transformed from the buffer." << std::endl;
        std::cout << "str: " << str << std::endl;
        exit(0);
    }
    buffer.free(len);
    if (!buffer.empty())
    {
        std::cout << "test3 failed, the buffer after free is not empty." << std::endl;
        exit(0);
    }
    std::cout << "test3 passed." << std::endl;
}

static int setNonBlocking(int fd)
{
    int opt = fcntl(fd, F_GETFL);
    if (fcntl(fd, F_SETFL, opt | O_NONBLOCK) < 0)
    {
        return -1;
    }
    return 0;
}

// test the read and write methods
void test4()
{
    int pipefd[2];
    if (pipe2(pipefd, O_NONBLOCK) != 0)
    {
        std::cout << "test4: fail to create pipe." << std::endl;
        exit(0);
    }
    // test write 
    std::string helloStr = "Hello world."; 
    Net::CircularBuffer buffer(3 * helloStr.size() / 2);
    Net::put(buffer, helloStr.c_str(), helloStr.size());
    int byteWritten = Net::write(pipefd[1], buffer, false);
    if (byteWritten == -1)
    {
        std::cout << "test4: fatal error happens when writting." << std::endl;
        exit(0);
    }
    if (byteWritten != helloStr.size())
    {
        std::cout << "test4: bytes written is not the same as the original strlen." << std::endl;
        exit(0);
    }
    // test read
    Net::CircularBuffer buffer2(3 * helloStr.size() / 2);
    int byteRead = Net::read(pipefd[0], buffer2);
    if (byteRead == -1)
    {
        std::cout << "test4: fatal error happens when reading." << std::endl;
        exit(0);
    }
    if (byteRead != helloStr.size())
    {
        std::cout << "test4: bytes read is not the same as the original strlen." << std::endl;
        exit(0);
    }
    // compare them
    std::string str;
    Net::circularBufferToString(buffer2, byteRead, str);
    if (strncmp(str.c_str(), helloStr.c_str(), byteRead) != 0)
    {
        std::cout << "test4: read string is not the same as the original string." << std::endl;
        exit(0);
    }
    
    // test the across case
    Net::put(buffer, helloStr.c_str(), 1);
    buffer.free(helloStr.size());
    Net::put(buffer2, helloStr.c_str(), 1);
    buffer2.free(helloStr.size());
    Net::put(buffer, helloStr.c_str(), helloStr.size());
    buffer.free(1);
    byteWritten = Net::write(pipefd[1], buffer, false);
    if (byteWritten == -1)
    {
        std::cout << "test4: fatal error happens when writting across the edge." << std::endl;
        exit(0);
    }
    if (byteWritten != helloStr.size())
    {
        std::cout << "test4: bytes written across the edge is not the same as the original strlen." << std::endl;
        exit(0);
    }
    // test read
    byteRead = Net::read(pipefd[0], buffer2);
    if (byteRead == -1)
    {
        std::cout << "test4: fatal error happens when reading across the edge." << std::endl;
        exit(0);
    }
    if (byteRead != helloStr.size())
    {
        std::cout << "test4: bytes read across the edge is not the same as the original strlen." << std::endl;
        exit(0);
    }
    // compare them
    buffer2.free(1);
    std::string str2;
    Net::circularBufferToString(buffer2, byteRead, str2);
    if (strncmp(str2.c_str(), helloStr.c_str(), byteRead) != 0)
    {
        std::cout << "test4: read string across the edge is not the same as the original string." << std::endl;
        std::cout << "str2: " << str2 << std::endl;
        exit(0);
    }

    std::cout << "test4 passed." << std::endl;
}

int main()
{
    test1(false);
    test1(true);
    test2();
    test3();
    test4();
    return 0;
}