#include "ProgramProcess.h"
void readConfigFile(std::string configFilePath)
{
    std::ifstream _file;
    _file.open(configFilePath);
    if (!_file)
    {
        std::cout << configFilePath << " not exist " << std::endl;
        exit(1);
    }

    std::cout << "read config file..." << std::endl;
    while (!_file.eof())
    {
        std::string buffer;
        _file >> buffer;
        if (buffer[0] == '#') //注释不读
        {
            continue;
        }
        if (buffer == "ProxyServiceAddr")
        {
            _file >> buffer;
            ProxyServiceAddr = buffer;
        }
        else if (buffer == "LobbyServiceAddr")
        {
            _file >> buffer;
            LobbyServiceAddr = buffer;
        }
        else if (buffer == "DatabaseServiceAddr")
        {
            _file >> buffer;
            DatabaseServiceAddr = buffer;
        }
        else if (buffer == "controlTcpPort")
        {
            _file >> buffer;
            controlTcpPort = buffer;
        }
        else if (buffer == "gameRpcAddr")
        {
            _file >> buffer;
            gameRpcAddr = buffer;
        }
        else
        {
            continue;
        }
    }
}