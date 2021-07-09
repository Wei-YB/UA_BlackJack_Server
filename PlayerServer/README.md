### Player Server 模块说明

#### 1. 模块配置

可以通过编辑 `player_server.config` 来配置 `Player Server` 的部分信息：

```
DBServer_ip 9.134.69.87			# 需要连接的DBServer的ip
DBServer_port 50051				# DBServer的端口
listen_port 50051				# Player Server监听的端口
log /var/log/PlayerLog.log		# log文件
daemon true						# 是否开启守护进程模式
```

#### 2. 运行

```
./PlayerServer start config_path
```

start 选项表示启动服务器，config_path指定配置文件路径

```
./PlayerServer stop
```

stop 选项表示停止服务器

#### 3.  模块功能

Player 模块主要负责处理和转发与用户信息相关的请求，主要涉及与DBServer之间的通信。与后端DBServer模块的通信通过gRPC来实现，约定通过 Notify 接口来请求数据，同时自身也提供一个 Notify 接口由Proxy调用。

为客户端提供以下请求的封装：

- SignUP：用户注册
- RankMe：查询自己的信息，包括用户名、积分、排名
- RankTop：查询 Top K 的积分排名情况
- MatchList：查询比赛list
- MatchInfo：查询具体某一局比赛的信息，包括对战的各个用户，以及积分情况

Player 模块针对某一些高频且信息固定的请求进行了缓存，避免大量重复访问DBServer带来的延时。例如GetName, GetUid等请求。
