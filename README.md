# Black Jack Back-end Server
An internship practicing project to design and build a back-end server for online Black Jack game.

## Build Project
Make sure you are in the root directory of the repository, then executes
```bash
mkdir -p build
cd build
cmake ..
make -j
```
, after which the executables are located in build/module_name/. If you only want to build one service module in this project, add `-D{MODULE_NAME}=ON` (e.g. `-DPROXY=ON`) to sccomplish this. 

As for how to configure and deploy each service, go to their subdirectories and check the README.md for instructions.

## Server Framework
![image](https://git.woa.com/ruiqizhang/UA_BlackJack_Server/raw/master/images/ServerFramework.png)

## Team members and division of work
The server is divided into six modules and each of them is assigned to certain members of the team. 

+ Front-end (Client): kristtong@tencent.com
+ Proxy: ruiqizhang@tencent.com
+ Lobby: owenybwei@tencent.com and stevejing@tencent.com
+ Room: kristtong@tencent.com
+ Social: aaronlv@kristtong@tencent.com
+ DB (database): owenybwei@tencent.com

