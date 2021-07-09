
#include <signal.h>

#include <fstream>

#include "Client.h"
#include "global.h"

using namespace ua_blackjack::client;
using namespace ua_blackjack::robust_io;

std::string server_ip;
std::string server_port;
std::string log_file;

std::shared_ptr<Client> client = nullptr;

std::shared_ptr<spdlog::logger> logger = nullptr;
TimePoint start;
Rio rio;

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
    if (argc != 2) {
        std::cerr << "invalid args" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::ifstream config;
    config.open(argv[1]);

    if (config.bad()) {
        std::cerr << "bad config file path" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "---------reading config---------" << std::endl;

    std::string option;
    config >> option >> server_ip;
    config >> option >> server_port;
    config >> option >> log_file;

    init(log_file.c_str());

    client.reset(new Client);
    client->Run(server_ip.c_str(), server_port.c_str());

    return 0;
}