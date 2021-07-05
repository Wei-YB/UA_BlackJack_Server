#include "spdlog/spdlog.h"
#include <string>

using namespace spdlog;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        info("incorrect number of arguments.");
        return 0;
    }
    std::string logLevel(argv[1]);
    if (logLevel == "debug")
        set_level(level::debug);
    else if (logLevel == "info")
        set_level(level::info);
    else if (logLevel == "trace")
        set_level(level::trace);
    else if (logLevel == "warn")
        set_level(level::warn);
    else if (logLevel == "error")
        set_level(level::err);
    else if (logLevel == "critical")
        set_level(level::critical);
    else
        {info("unknown level.");return -1;}
    debug("debug.");
    info("info");
    warn("warn");
    trace("trace");
    critical("critical");
    error("error");
    return 0;
}