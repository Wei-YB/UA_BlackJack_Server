#ifndef GLOBAL_H
#define GLOBAL_H

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <chrono>

#include "Rio.h"

typedef std::chrono::milliseconds MilliSeconds;
typedef std::chrono::steady_clock SteadyClock;
typedef std::chrono::time_point<SteadyClock> TimePoint;

extern std::shared_ptr<spdlog::logger> logger;
extern TimePoint start;
extern ua_blackjack::robust_io::Rio rio;

#endif