#include "MycondintonCo.h"
int myConditionWait(int conditionID, int ms)
{
    pollfd pfd;
    pfd.fd = conditionID;
    pfd.events = POLLIN;
    int ret = poll(&pfd, 1, 1000); //监控一个信号量,把自己挂起
    int64_t buffer;
    read(conditionID, reinterpret_cast<void *>(&buffer), sizeof buffer);
    return ret; //0为超时
}
int createCondition(int __flag)
{
    int ret = eventfd(0, __flag);
    fcntl(ret, F_SETFL, O_NONBLOCK);
    return ret;
}
void myConditionSignal(int conditionID)
{
    int64_t val = 1;
    write(conditionID, reinterpret_cast<void *>(&val), sizeof val);
}