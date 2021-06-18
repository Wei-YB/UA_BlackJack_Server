#include <iostream>
#include "Poker.h"
#include "ShuffledPokers.h"
#include "Player.h"
#include "Room.h"
#include "co_routine.h"
const int roomSize = 500;
struct RoomRoutine
{
    BlackJackRoomID id;
    stCoRoutine_t *routine;
};
std::list<RoomRoutine> tasks;
void *createRoom(void *args)
{
    RoomRoutine *roomID = (RoomRoutine *)args;

    UidList playerId;
    playerId.emplace_back(roomID->id); //room i 中有 id为1的player
    std::shared_ptr<Room> room = std::make_shared<Room>(roomID->id, playerId);

    roomHashMap[room->getRoomId()] = room;
    while (true)
    {
        if (room->isExpired())
        {
            room.reset(); //析构room
        }
        co_yield_ct(); //出让使用权
    }
    return NULL;
}
void *destroyRoom(void *args)
{
    co_enable_hook_sys();
    for (auto &task : tasks)
    {
        poll(NULL, 0, 20); //每两秒删除一个房间
        std::cout << "delete one room" << std::endl;
        if (std::shared_ptr<Room> roomPtr = roomHashMap[task.id].lock())
        {
            roomPtr->setExpire();
            co_resume(task.routine); //恢复协程，让其自己释放资源
        }
        co_release(task.routine);
    }
    while (true)
    {
        co_yield_ct(); //出让使用权
    }
    return NULL;
}
void *printInfo(void *)
{
    co_enable_hook_sys();
    while (true)
    {
        poll(NULL, 0, 500); //每秒打印两次信息
        for (int i = 0; i < roomSize; i++)
        {
            if (roomHashMap[i].expired())
            {
                std::cout << "room " << i << " not existed" << std::endl;
            }
            else
            {
                std::cout << "room " << i << " existed" << std::endl;
            }
            if (playerHashMap[i].expired())
            {
                std::cout << "player " << i << " not existed" << std::endl;
            }
            else
            {
                std::cout << "player " << i << " existed" << std::endl;
            }
        }
        std::cout << std::endl;
    }
    return NULL;
}
int main(int agrc, char *argv[])
{
    for (int i = 0; i < roomSize; i++) //创建10个房间
    {
        RoomRoutine roomRoutine;
        roomRoutine.id = i;
        co_create(&roomRoutine.routine, NULL, createRoom, &roomRoutine);
        co_resume(roomRoutine.routine);
        tasks.emplace_back(roomRoutine);
    }

    //创建一个打印信息的任务
    stCoRoutine_t *printInfoCo;
    co_create(&printInfoCo, NULL, printInfo, NULL);
    co_resume(printInfoCo);

    //创建一个清除房间的任务
    stCoRoutine_t *destroyRoomCo;
    co_create(&destroyRoomCo, NULL, destroyRoom, NULL);
    co_resume(destroyRoomCo);

    co_eventloop(co_get_epoll_ct(), NULL, NULL);
    return 0;
}