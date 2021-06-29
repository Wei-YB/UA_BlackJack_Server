# 代理模块介绍
代理在整个BlackJack Server中承担了转发和过滤客户端请求和响应的功能，以及转发后端服务向客户发送消息的功能。

## 编译与运行
代理模块的编译需要用到protobuf、gRPC、spdlog三个库，通常只需要源码安装gRPC库，因为它包含了protobuf等库。gRPC库的安装可以参考<https://grpc.io/docs/languages/cpp/quickstart/>，spdlog库的安装可以参考<https://github.com/gabime/spdlog>。

安装完上述依赖后就可以编译本单元了（切换到本模块的目录）：
```bash
cd protos/build
cmake ..
make -j
cd ../../build
cmake ..
make -j
```
若上面两步编译成功，就可以启动代理服务进程了：
```bash
./proxy config_path start
```
其中config_path是代理服务的配置文件路径，文件主要包含了监听客户端连接和请求的ip和端口、输出日志路径、代理对后端模块开放的gRPC服务地址，以及后端模块提供的gRPC服务的地址。最后一个参数除了可以填start外，还可以填stop，后者用于关闭该服务。

此外在test目录下包含了本模块简单的单元测试源码client.cpp和simpleBackEnd.cpp，后者主要用来模拟后端服务。client.cpp将samples.txt文件中指定的请求序列顺序地发送给proxy，并打印出发出的请求和接收到的请求内容（只有当模拟的客户端数量为1时才会打印出来）。为实现压力测试，它模拟n个（n可以自己在命令行参数中指定）客户端并发地往proxy发送一系列的请求，并统计所有请求-响应时间差的平均值，以此来衡量代理服务随客户端数量的性能变化曲线。单元测试的编译和运行
```bash
mkdir -p test/build
cd test/build
cmake ..
make -j
./simpleBackEnd log_path
./client host port samples_path client_number log_path
```
其中log_path参数均为输出运行日志路径，host和port是代理监听的ip和端口，samples_path是samples.txt文件的路径，test文件夹下有一个预先写好的，client_number是要模拟的客户端数量。但在测试时候需要将代理的配置文件中各个后端服务模块的地址修改成
```
lobbyAddress localhost:50051
roomAddress localhost:50052
socialAddress localhost:50053
playerAddress localhost:50054
```
然后再重启代理服务。

## 代理与后端模块的通信
代理模块与后端之间的通信通过gRPC来实现，并约定所有与代理交互的后端模块都暴露一个名为Notify的接口，代理模块也向后端暴露一个名为Notify的接口。接口入参和出参为protobuf message类型`Request`和`Response`，它的具体定义在protos文件夹下的UA_BlackJack.proto文件中。

## 代理与客户端的通信
客户端和代理通过tcp通信，两者间的通信协议遵循如下形式：`| message type (4 bytes) | message length (4 bytes) | message (variable) |`。其中`message type`可能取值为1（REQUEST）和2（RESPONSE），当前版本中其它取值均非法，会被代理丢弃。`message length`为消息内容`message`的字节长度。`message type`和`message length`均为网络字节序。`message`是protobuf消息体`Request`或`Response`序列化后的内容，这里两个消息体的定义和UA_BlackJack.proto文件中的定义相同。
