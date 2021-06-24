#pragma once
#include <fcntl.h>
#include <sys/eventfd.h>
#include <poll.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "co_routine.h"
int createCondition(int __flag);
int myConditionWait(int conditionID, int ms);
void myConditionSignal(int conditionID);