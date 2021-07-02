
#include <signal.h>

#include "Client.h"
#include "global.h"

using namespace ua_blackjack::client;
std::shared_ptr<Client> client = nullptr;

std::shared_ptr<spdlog::logger> logger = nullptr;
TimePoint start;

void handler(int sig) {
    std::cout << ":( timeout" << std::endl << std::endl;
    std::cout << "21Game "
              << "(" << client->GetState() << ")"
              << " > " << std::flush;
}

void init(const char* log_file) {
    logger = spdlog::basic_logger_mt("basic_logger", log_file);
    logger->flush_on(spdlog::level::trace);

    struct sigaction sa;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sa.sa_handler = handler;
    if (sigaction(SIGALRM, &sa, nullptr) == -1) {
        logger->error("sigaction error");
        exit(EXIT_FAILURE);
    }

    start = SteadyClock::now();
}

int main(int argc, char* argv[]) {
    assert(argc == 4);

    init(argv[3]);

    client.reset(new Client);
    client->Run(argv[1], argv[2]);

    return 0;
}