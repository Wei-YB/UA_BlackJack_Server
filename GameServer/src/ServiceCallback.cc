#include "ServiceCallback.h"
void nomalStartGameCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
    auto args = request.args();
    responce.set_status(0);
    UidList uids;
    BlackJackRoomID roomid = atoi(args[0].c_str());

    spdlog::info("Create Room Ask come roomid {0}", roomid);
    for (int i = 1; i < args.size(); i++)
    {
        uids.push_back(atoi(args[i].c_str()));
    }

    createstEnv_t(roomid, uids);
}
void nomalLeaveRoomCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
}
void nomalDoubleCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
}
void nomalSurrenderCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce)
{
}