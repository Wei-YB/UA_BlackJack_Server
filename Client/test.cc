#include <unistd.h>

#include <iostream>

int main() {
    char c;
    while (read(STDIN_FILENO, &c, 1)) {
        std::cout << c;
    }
    return 0;
}