# spdlog

Very fast, header-only/compiled, C++ logging library.

## 安装

#### 静态库版本 

```console
$ git clone https://github.com/gabime/spdlog.git
$ cd spdlog && mkdir build && cd build
$ cmake .. && make -j
$ sudo cp libspdlog.a /usr/local/lib
$ cd ..
$ sudo cp include/spdlog /usr/local/include -r
```

完成后，在自己的源代码里include相应的头文件即可以使用。

## 平台

 * Linux, FreeBSD, OpenBSD, Solaris, AIX
 * Windows (msvc 2013+, cygwin)
 * macOS (clang 3.5+)
 * Android

## Features

* 速度快
* 只需要包含头文件和静态库文件
* 异步模式 (可选)
* 单线程或多线程的日志管理器（logger）.
* 多样的日志输出方式:
  * 可循环的日志输出.
  * 每天新建一个日志文件输出.
  * 控制台日志输出 (支持不同颜色).
  * 系统日志输出.
  * Windows 事件日志.
  * Windows debugger日志 
* - 日志过滤.日志的等级可以在运行时或者编译时进行修改.
* 日志的等级可由程序变量argv或者系统环境变量进行修改.

## 使用方法

#### 为灵活的切换控制台输出和文件输出，推荐以下使用方法

**1 控制台输出**

```c++
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

int main() 
{
    
    auto logger = spdlog::stdout_color_mt("stdout logger");
    spdlog::set_default_logger(logger);
    
    //在main函数里设置好默认的logger后，在各个class的成员函数里直接使用spdlog命名空间下的info error debug等方法就可以使用logger配置的模式
    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);
    
    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");
    
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..");    
    
    
    SPDLOG_TRACE("Some trace message with param {}", 42);
    SPDLOG_DEBUG("Some debug message");
}

```


---

#### 2 基本文件输出

```c++
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

int main() 
{   //若要更改日志输出方式，只需更改日志管理器logger的类型
    auto logger = spdlog::basic_logger_mt("fileoutput logger", "logs/log.txt");
    spdlog::set_default_logger(logger);
    
    
    //在main函数里设置好默认的logger后，在各个class的成员函数里直接使用spdlog命名空间下的info error debug等方法就可以使用logger配置的模式
    ...
}

```

---

#### 3 循环日志文件输出（当文件达到指定大小后，新日志会从头开始覆盖已有的日志）

```c++
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"

int main() 
{
    // 创建一个可循环的日志文件管理器，限制文件大小为5Mb和最大3个文件输出。
    auto max_size = 1048576 * 5;
    auto max_files = 3;
    auto logger = spdlog::rotating_logger_mt("logger_name", "logs/rotating.txt", max_size, max_files);
    spdlog::set_default_logger(logger);
    
    
    //在main函数里设置好默认的logger后，在各个class的成员函数里直接使用spdlog命名空间下的info error debug等方法就可以使用logger配置的模式
    ...
}

```

---

#### 4 每日文件输出

```c++
#include "spdlog/sinks/daily_file_sink.h"
int main() 
{
    //创建一个daily logger - 每天2:30am都会创建一个新文件
    auto logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 2, 30);
    spdlog::set_default_logger(logger);
    
    
    //在main函数里设置好默认的logger后，在各个class的成员函数里直接使用spdlog命名空间下的info error debug等方法就可以使用logger配置的模式
    ...
}

```

---

#### 5 异步日志输出

1 所谓同步日志，日志输出语句与程序的业务逻辑语句将在同一个线程运行。即当输出日志时，必须等待日志输出语句执行完毕后，才能执行后面的业务逻辑语句。

2 而使用异步日志进行输出时，日志输出语句与业务逻辑语句并不是在同一个线程中运行，而是有专门的线程用于进行日志输出操作，处理业务逻辑的主线程不用等待即可执行后续业务逻辑。

```c++
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
int main() 
{
    //默认的线程池设置在创建async logger前是可以修改的
    // spdlog::init_thread_pool(8192, 1); // queue with 8k items and 1 backing thread.
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "logs/async_log.txt");
    // 也可以是
    // auto async_file = spdlog::create_async<spdlog::sinks::basic_file_sink_mt>("async_file_logger", "logs/async_log.txt");
    spdlog::set_default_logger(async_file);
    
    
    //在main函数里设置好默认的logger后，在各个class的成员函数里直接使用spdlog命名空间下的info error debug等方法就可以使用logger配置的模式
    ...
}

```

---

#### 6 设置日志级别

spdlog中的日志级别从低到高分别为 trace < debug < info < warn < error < critical < off. 只有比设置的日志级别高的日志才会输出。

如设置日志级别 spdlog::set_level(spdlog::level::info); 则只有级别为info  warn  error  critical的日志才会输出。

如关闭日志输出，需spdlog::set_level(spdlog::level::off);

```CC
int main (int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::info); //全局性设置
}
```

#### 7 从环境变量或者main函数的argv里更改日志级别

```c++
#include "spdlog/cfg/env.h"
int main (int argc, char *argv[])
{
    spdlog::cfg::load_env_levels();
    // or from command line:
    // ./example SPDLOG_LEVEL=info,mylogger=trace
    // #include "spdlog/cfg/argv.h" // for loading levels from argv
    // spdlog::cfg::load_argv_levels(argc, argv);
}
```

So then you can:

```console
$ export SPDLOG_LEVEL=info,mylogger=trace$ ./example
```

---

Documentation can be found in the [wiki](https://github.com/gabime/spdlog/wiki/1.-QuickStart) pages.

---

Thanks to [JetBrains](https://www.jetbrains.com/?from=spdlog) for donating product licenses to help develop **spdlog** <a href="https://www.jetbrains.com/?from=spdlog"><img src="logos/jetbrains-variant-4.svg" width="94" align="center" /></a>