#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>

void rotating_example()
{
    // Create a file rotating logger with 5mb size max and 3 rotated files
    auto max_size = 1048576 * 5;
    auto max_files = 3;
    auto logger = spdlog::rotating_logger_mt("some_logger_name", "logs/rotating.txt", max_size, max_files);
}

void basic_logfile_example()
{
    try 
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}

int main() 
{
    std::vector<int> ret;
    std::unordered_set<int> availableRooms_{1, 2, 3, 4, 5};
    std::unordered_set<int> emptyRooms_{5, 6, 7, 8, 9};

    std::copy(availableRooms_.begin(), availableRooms_.end(), std::back_inserter(ret));
    std::copy(emptyRooms_.begin(), emptyRooms_.end(), std::back_inserter(ret));

    for(auto& rid:ret)
      std::cout<<rid <<' ';
    std::cout<<std::endl;
}

