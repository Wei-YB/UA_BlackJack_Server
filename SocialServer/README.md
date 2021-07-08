### Social Server 模块说明

#### 1. 模块配置

可以通过编辑 `social_server.config` 来配置 `Social Server` 的部分信息：

```
DBServer_ip 9.134.69.87			# 需要连接的DBServer的ip
DBServer_port 50051				# DBServer的端口
listen_port 50051				# Social Server监听的端口
log /var/log/SocialLog.log		# log文件
daemon true						# 是否开启守护进程模式
```

#### 2. 运行

```
./SocialServer start config_path
```

start 选项表示启动服务器，config_path指定配置文件路径

```
./SocialServer stop
```

stop 选项表示停止服务器

#### 3.  模块功能

Social 模块主要负责对用户的好友相关请求进行处理和转发，主要涉及与 DBServer之间的通信。与后端DBServer模块的通信通过gRPC来实现，约定通过 Notify 接口来请求数据，同时自身也提供一个 Notify 接口由Proxy调用。

数据库对每个用户都保存有两张表，分别为：

- Friend list: 好友列表
- Waiting friend list: 待接受的好友请求列表

同时为这两张表提供四个原始操作：

- Add friend: 添加好友， 操作对象为 Friend list
- Delete friend: 删除好友，操作对象为 Friend list
- Add wait friend: 添加待接受的好友请求，操作对象为 Waiting friend list
- Delete wait friend: 删除待接受的好友请求，操作对象为 Waiting friend list

Social Server 通过请求四个原始操作来操作以上的两张表，同时为客户端提供关于以下请求的封装：

- AddFriend
- AcceptFriend
- DeleteFriend
- DeleteWaitingFriend
- FriendList
- WaitingFriendList
