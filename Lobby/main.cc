#include "LobbyService.h"

using ua_blackjack::LobbyServer;

int main() {
    spdlog::set_level(spdlog::level::trace);
    LobbyServer server("9.134.76.179:50051", "127.0.0.1:50051");
    server.Run();
}