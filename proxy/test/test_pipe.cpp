#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

// I want to test:
    // 1. when I set the O_DIRECT flag when calling pipe2, if the
    // free room of the pipe buffer is not enough for a whole package, 
    // what is the behavior. write half of it ? or just suspend.
    // ans: it will suspend, the size of the buffer seems to be larger than 2^15 bytes

    // 2. when there are multiple packages in the pipe buffer, what is 
    // the behavior when I read the pipe. would I get one package? or 
    // multiple packages.
    // ans: it only reads one package

    // 3. add O_NONBLOCK flags, and do the previous tests

void test1(bool nonBlock = false)
{
    int pipefd[2];
    if (pipe2(pipefd, nonBlock ? O_DIRECT | O_NONBLOCK : O_DIRECT) < 0)
    {
        printf("test1: fail to create pipe.\n");
        return;
    }

    char buffer[PIPE_BUF];
    // write the first package, should succeed
    int ret;
    for (int cnt = 0; true; cnt++)
    {
        printf("test1: write the %dth package to pipe.\n", cnt);
        if ((ret = write(pipefd[1], buffer, sizeof(buffer))) <= 0)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            printf("test1: fail to write the %dth package.\n", cnt);
            return;
        }
        printf("test1: write %d bytes.\n", ret);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
}

void test2()
{
    int pipefd[2];
    if (pipe2(pipefd, O_DIRECT) < 0)
    {
        printf("test2: fail to create pipe.\n");
        return;
    }

    char buffer[PIPE_BUF];
    // write the 10 packages to pipe, should succeed
    printf("test2: write 10 packages to pipe.\n");
    int ret;
    for (int cnt = 0; cnt < 10; cnt++)
    {
        
        if ((ret = write(pipefd[1], buffer, sizeof(buffer) / 10)) <= 0)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            printf("test2: fail to write the %dth package.\n", cnt + 1);
            return;
        }
    }
    
    // read a packages from pipe
    printf("test2: read a package from pipe.\n");
    if ((ret = read(pipefd[0], buffer, sizeof(buffer))) <= 0)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        printf("test2: fail to read package.\n");
        return;
    }
    printf("test2: read %d bytes.\n", ret);

    close(pipefd[0]);
    close(pipefd[1]);
}

int main()
{
    test1(true);
    test2();
    exit(0);
}