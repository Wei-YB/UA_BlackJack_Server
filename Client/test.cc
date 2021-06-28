#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#include <unistd.h>

#include <iostream>

auto logger = spdlog::basic_logger_mt("basic_logger", "./log-test.txt");

int main() {
    logger->info("test");
    return 0;
}