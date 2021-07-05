#include "DBServerAsynImpl.h"
#include <fstream>
#include <sstream>
#include <signal.h>

const auto pid_file_path = "/var/run/dbserver.pid";
std::string config_file_path = "./database.config";
void handler(int signum){
    if(signum != SIGINT){
        std::cerr <<"fatal error: wrong signal" << std::endl;
        return;
    }
    
}

ServerAsynImpl* server = nullptr;

struct Config{
    Config() = default;
    
    std::string redis_host = "127.0.0.1:6379";
    std::string grpc_host = "0.0.0.0:50051";
    std::string log_path = "./";
    int     init_score = 2000;
    bool daemonize = false;

    const std::string defalut_config = "./database.config";

    void GetConfig(const std::string& path = ""){
        std::ifstream config_file{};
        if(path.empty()){
            std::cout <<"use default config" << std::endl;
            config_file.open(defalut_config);
        }else{
            config_file.open(path);
        }
        if(config_file.bad()){
            std::cerr<<"bad config file path" << std::endl;
            exit(0);
        }
        std::cout <<"----reading config-----" << std::endl;
        std::string line;
        std::string key, value;
        while(getline(config_file, line)){
            if(line[0] == '#')
                continue;
            std::istringstream is(line);
            is >> key >> value;
            if(key == "redis_server")
                redis_host = value;
            else if(key == "grpc_host")
                grpc_host = value;
            else if(key == "init_score"){
                try{
                    init_score = std::stoi(key);
                }catch(std::exception e){
                    init_score = 2000;
                }
            }
            else if(key == "daemonize"){
                daemonize = (value == "yes");
            }
            else{
                std::cerr <<"invalid config key: " <<key << std::endl; 
            }
        }
    }
private:
};

void StartServer() {
    

    Config config;
    config.GetConfig(config_file_path);

    if(config.daemonize){
        std::cout <<"daemonize enable" << std::endl;
        daemon(1,0);
        std::ofstream pid_file(pid_file_path);
        pid_file << getpid();
        pid_file.close();
    }
   
    auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>
    ("async_file_logger", "logs/async_log.txt");
    spdlog::set_default_logger(async_file);
    spdlog::flush_on(spdlog::level::trace);
    spdlog::set_level(spdlog::level::trace);

    SPDLOG_TRACE("start database server");

    server = new ServerAsynImpl(config.grpc_host,config.redis_host,config.init_score);

    std::thread t1([](){server->Run();});

    t1.join();
    SPDLOG_INFO("server success stop");
}

void StopServer(){
    std::ifstream file(pid_file_path);
    if(file.bad()){
        std::cout <<"no DB Server running" << std::endl;
        return;
    }
    int pid = -1;
    file >> pid;
    file.close();
    if(pid != -1){
        kill(pid, SIGINT);
        std::cout <<"Send SIGINT to program: "<< pid << std::endl;
    }
    else{
        std::cerr <<"invalid pid in \"/var/run/dbserver.pid\"" << std::endl;
        return;
    }
}

// TODO reboot feature
// void Reboot();

int main(int argc, char** argv) {
    int ch;
    while((ch = getopt(argc, argv,"s:c:")) != -1)
    {
        switch (ch)
        {
        case 's':
            StopServer();
            return 0;
            break;
        case 'c':
            config_file_path = optarg;
        default:
            break;
        }
    }
    StartServer();
    
}
