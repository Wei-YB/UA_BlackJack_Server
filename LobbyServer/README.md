### 1 大厅模块简介

大厅模块在整个blackjack游戏中提供常见的游戏大厅服务，具体见下述接口说明。

Lobby服务模块分为三个线程。两个监听线程负责分别监听来自Proxy或Game的请求和来自Datebase或Game的响应，将请求和响应分别放在两个阻塞队列中，另一个工作线程负责从阻塞队列中取出请求或响应，并调用相应的解析函数处理请求发回对端。线程之间的通信采用eventfd事件通知。

### 2 编译与运行

#### 2.1 编译

若要编译整个BlackJack游戏，先切换到项目UA_BlackJack根目录下，运行以下指令。

```cmake
mkdir bulid
cd build
cmake ..
make -j8
```

若要单独编译该Lobby服务模块，先切换到项目UA_BlackJack根目录下，运行以下指令。

```cmake
mkdir bulid
cd build
cmake -DLOBBY=on ..
make -j8
```

#### 2.2 运行

若要单独运行该Lobby模块，先切换到项目UA_BlackJack根目录下，运行以下指令。

```
cd build/lobby
./LobbyServer </path/to/lobby.config> 
```

其中，</path/to/lobby.config> 为大厅模块运行时的配置文件路径，主要包含本机服务IP地址和端口myAddress、输出日志句柄名称logger_name、输出日志路径log_path、所请求的游戏对局服务的IP地址和端口号gameAddress、所请求的数据库服务地址和端口号databaseAddress等配置。运行示例：

```C++
./LobbyServer ../../LobbyServer/config/lobby.config 
```

#### 2.3 压力测试

运行压力测试模块，先切换到项目UA_BlackJack根目录下，运行以下指令。

```cmake
cd LobbyServer/stress_test
mkdir build
cd build
cmake ..
make -j8
```

编译成功后，运行以下指令

```
./proxy_client <running time> <threads nums> <client nums>
```

其中，running time为压测程序运行时间，thread nums为程序执行线程数量，client nums为每线程的TCP连接数量。

### 3 大厅模块与代理模块、数据库模块、游戏对局模块的通信

大厅模块与代理模块的通信、与后端数据库模块的通信、与游戏对局模块的通信均采用异步grpc实现，三者之间的相互通信均只采用一个RPC服务，名为Notify，根据发送方发送的request或者接收方回应的response里的不同的request_type解析并执行相应的调用，request和response的定义在UA_BlackJack.proto文件中。

### 4 大厅接口说明

#### 4.1 大厅提供给其他模块的服务

| 请求方       | 所请求的服务 | 入参               | 返回值        |
| ------------ | ------------ | ------------------ | ------------- |
| 代理模块     | Login        | nickname, password | uid           |
| 代理模块     | Logout       | uid                | void          |
| 代理模块     | JoinRoom     | uid, rid           | true or false |
| 代理模块     | CreateRoom   | uid                | true or false |
| 代理模块     | LeaveRoom    | uid                | true or false |
| 代理模块     | Ready        | uid                | true or false |
| 代理模块     | RoomList     | void               | rids          |
| 代理模块     | QuickMatch   | uid                | rid           |
| 游戏对局模块 | MatchEnd     | rid                | void          |

#### 4.2 大厅请求其他模块的服务

| 服务提供方   | 所提供的服务 | 入参               | 返回值   |
| ------------ | ------------ | ------------------ | -------- |
| 数据库模块   | GetUid       | nickname, password | uid      |
| 数据库模块   | GetPassword  | uid                | password |
| 游戏对局模块 | MatchStart   | rid,   uids        | void     |

