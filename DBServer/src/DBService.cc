#include "DBServerAsynImpl.h"
#include <fstream>
#include <sstream>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>

const auto pid_file_path = "/var/run/dbserver.pid";
const auto config_file_path = "./database.config";

const auto default_config = "# this is database default config\n"
                            "redis_server 127.0.0.1:6379\n"
                            "grpc_host 0.0.0.0:50051\n"
                            "init_score 2000\n"
                            "daemonize yes\n"
                            "async_log yes\n";

ServerAsynImpl* server = nullptr;

void handler(int signum){
    if(signum != SIGINT){
        std::cerr <<"fatal error: wrong signal" << std::endl;
        return;
    }
    server->Shutdown();
    // delete server;
    SPDLOG_INFO("Server stop.");    
}


struct Config{
    Config() = default;
    
    std::string redis_host = "127.0.0.1:6379";
    std::string grpc_host = "0.0.0.0:50051";
    std::string log_path = "./";
    int     init_score = 2000;
    bool daemonize = false;
    bool asyncLog = false;
    void GetConfig(){

        struct stat buffer{};
        auto ret = ::stat(config_file_path, &buffer);
        if(ret < 0) // no config file
        {
            std::cout <<"config file not found, generate default config file"<< std::endl;
            std::ofstream config_file(config_file_path);
            config_file<<default_config;
            config_file.close();
            std::cout <<"please restart the server" << std::endl;
            exit(0);
        }
        std::ifstream config_file{};
        config_file.open(config_file_path);
        std::cout <<"reading config..." << std::endl;
        std::string line;
        std::string key, value;
        while(getline(config_file, line)){
            if(line[0] == '#')
                continue;
            std::istringstream is(line);
            is >> key >> value;
            if(key == "redis_server"){
                redis_host = value;
                std::cout <<"using "<<redis_host <<" as redis server"<< std::endl;
            }
            else if(key == "grpc_host"){
                grpc_host = value;
                std::cout <<"using "<< grpc_host<<" as grpc server"<< std::endl;
            }
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
            else if(key == "async_log"){
                if(value == "yes"){
                    std::cout <<"async log enable..."<< std::endl;
                    asyncLog = true;
                }
            }
            else{
                std::cerr <<"invalid config key: " <<key << std::endl; 
            }
        }
    }
private:
};


bool ServerRunning(){
    struct stat buffer{};
    auto ret = ::stat(pid_file_path, &buffer);
    if(ret < 0)
        return false;
    return true;
}

void StartServer() {
    // check pid file exists?
    
    signal(SIGINT, handler);
    
    Config config;
    config.GetConfig();
    
    
    
    if(config.daemonize){
        if(ServerRunning()){
            std::cerr<<"server already running....\nabort"<< std::endl;
            exit(0);
        }
        std::cout <<"daemonize enable..." << std::endl;
        daemon(1,0);
        std::ofstream pid_file(pid_file_path);
        pid_file << getpid();
        pid_file.close();
    }
    if(config.asyncLog){
        auto async_file = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "./logs/async_log.txt");
        spdlog::set_default_logger(async_file);
    }
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);
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
    remove(pid_file_path);
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
    while((ch = getopt(argc, argv,"s:")) != -1)
    {
        switch (ch)
        {
        case 's':
            StopServer();
            return 0;
            break;
        default:
            break;
        }
    }
    StartServer();
    
    delete server;
}
