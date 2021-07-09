#ifndef _LOG_H_
#define _LOG_H_
#include <string>
#include "spdlog/spdlog.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

extern std::string g_logger_name;
extern std::string g_log_path;

inline void set_logger_name(const std::string &logger_name)
{
    g_logger_name = logger_name;
}

inline void set_log_path(const std::string &log_path)
{
    g_log_path = log_path;
}

inline void set_log_level(const std::string &log_level)
{
    if (log_level == "debug")
        spdlog::set_level(spdlog::level::debug);
    else if (log_level == "info")
        spdlog::set_level(spdlog::level::info);
    else if (log_level == "trace")
        spdlog::set_level(spdlog::level::trace);
    else if (log_level == "warn")
        spdlog::set_level(spdlog::level::warn);
    else if (log_level == "error")
        spdlog::set_level(spdlog::level::err);
    else if (log_level == "critical")
        spdlog::set_level(spdlog::level::critical);
}

#define logger_flush_on() spdlog::flush_on(spdlog::level::trace)

#define create_logger() spdlog::basic_logger_mt<spdlog::async_factory>(g_logger_name, g_log_path)


#define logger_ptr    spdlog::get(g_logger_name)


#endif