#pragma once

#include<unordered_set>
#include<Players.h>

namespace ua_black_jack_server { namespace lobby{

class Room{
private:
    std::unordered_set<int64_t> players_ready;
    std::unordered_set<int64_t> players_unready;

public:
    using UID = int64_t;

public:
    void Match_end();

    void Join_room(UID uid);

    void Leave_room(UID uid);

    bool Ready(UID uid);

    bool Full();

    bool Empty();

    void Start();
};
}
}
