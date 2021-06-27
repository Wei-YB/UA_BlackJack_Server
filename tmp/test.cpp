#include "log.h"

int main()
{
    set_logger_name("async_logger");
    set_log_path("./tmp.log");
    auto logger = spdlog::basic_logger_mt<spdlog::async_factory>(g_logger_name, g_log_path);

    logger_ptr->info("hello spdlog.");
    return 0;
}
