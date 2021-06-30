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
其中config_path是代理服务的配置文件路径，文件主要包含了监听客户端连接和请求的ip和端口、输出日志路径、代理对后端模块开放的gRPC服务地址，以及后端模块提供的gRPC服务的地址。config文件夹中提供了两个配置例子，proxy.config是我们调试整个后端时的一个配置，test.config是压测时的配置。最后一个参数除了可以填start外，还可以填stop，后者用于关闭该服务。

此外在test目录下包含了本模块的单元测试源码client.cpp，它实现了简单的压力测试：在单线程中模拟n个（n可以自己在命令行参数中指定）客户端并发地往proxy发送一系列的请求，并记录每个请求收到响应所需的时间，最后计算出代理的QPS和平均响应时间。该测试中没有包含gRPC服务性能的测试，并且是在没有后端模块参与的情况下测试的，也就是说当代理收到用户请求、发给gRPC Client线程之后，gRPC Client线程并不会真发给后端服务模块，而是本地简单处理后直接发回给客户端。在测试代理模块之前，需要将include文件夹下的common.h文件中的`DEBUG_MODE`宏定义从0修改为1，然后重新编译代理模块。测试用例的编译和运行步骤如下：

```bash
cd build
make -j
./proxy config_path start
cd ..
mkdir -p test/build
cd test/build
cmake ..
make -j
./client host port samples_path client_number log_path
```
其中log_path参数均为输出运行日志路径，host和port是代理监听的ip和端口，samples_path是samples.txt文件的路径，test文件夹下有一个预先写好的，client_number是要模拟的客户端数量。在测试时候需要用到config文件夹下的test.config配置。

## 代理与后端模块的通信
代理模块与后端之间的通信通过gRPC来实现，并约定所有与代理交互的后端模块都暴露一个名为Notify的接口，代理模块也向后端暴露一个名为Notify的接口。接口入参和出参为protobuf message类型`Request`和`Response`，它的具体定义在protos文件夹下的UA_BlackJack.proto文件中。

## 代理与客户端的通信
客户端和代理通过tcp通信，两者间的通信协议遵循如下形式：`| message type (4 bytes) | message length (4 bytes) | message (variable) |`。其中`message type`可能取值为1（REQUEST）和2（RESPONSE），当前版本中其它取值均非法，会被代理丢弃。`message length`为消息内容`message`的字节长度。`message type`和`message length`均为网络字节序。`message`是protobuf消息体`Request`或`Response`序列化后的内容，这里两个消息体的定义和UA_BlackJack.proto文件中的定义相同。
