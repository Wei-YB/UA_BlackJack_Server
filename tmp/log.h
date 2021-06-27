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

void create_logger();

#define logger_ptr    spdlog::get(g_logger_name)

#endif