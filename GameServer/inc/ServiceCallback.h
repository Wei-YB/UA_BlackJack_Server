#pragma once
#include "ServerAsync.h"
void nomalStartGameCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce);
void nomalLeaveRoomCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce);
void nomalDoubleCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce);
void nomalSurrenderCallback(ua_blackjack::Request &request, ua_blackjack::Response &responce);