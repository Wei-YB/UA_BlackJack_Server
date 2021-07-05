#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
// #include "ClientProxyProtocol.h"
#include "co_routine.h"

void *Foo(void *arg)
{
    co_enable_hook_sys();
    return 0;
}

int main()
{
    stCoRoutine_t *co;
    co_create(&co, NULL, Foo, NULL);
    co_resume(co);

    return 0;
}