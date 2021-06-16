#include <stdio.h>
#include <sys/types.h>
#include <string>
#include <vector>

struct Resquest {
    int32_t cmd;
    int32_t length;
    std::vector<std::string> args;

};

int main(int argc, char **argv)
{
    // 
}