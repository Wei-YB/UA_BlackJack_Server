### Database Server 模块说明
#### 1 模块使用
##### 1.1 配置文件
可通过编辑`database.config`来配置服务器的部分信息，目前支持的选项如下
```
redis_server 127.0.0.1:6379     # redis服务的地址
grpc_host 0.0.0.0:50051         # 当前数据库服务器监听的端口
init_score 2000                 # 新用户的初始积分
daemonize yes                   # 是否启用守护进程模式
```
##### 1.2 启动
- 运行`./DatabaseService`即以默认方式启动本模块
    - 以非守护模式运行于`0.0.0.0:50051`端口上
    - 用户初始积分为2000分
    - 以`127.0.0.1:6379`为redis服务器。
- 通过`-c`选项，指定config文件的路径，此时程序将根据config文件的配置启动，如`./DatabaseService -c ./database.config`。
##### 1.3 停止
- 通过`-s stop`选项停止服务器，如`./DatabaseService -s stop`
##### 1.4 日志
- 日志文件存放在程序目录下的`./logs`文件夹中。
#### 2 模块设计
- 本模块采用单进程单线程方式实现，同步接受RPC请求并查询/写入redis数据库
- 引入第三方redis client作为访问redis的通道，具体可见:[acl redis](https://github.com/acl-dev/acl/blob/master/lib_acl_cpp/samples/redis/README.md)
- unfinished
#### 3 数据库结构
##### 3.1 用户信息
用户信息包括昵称，积分，密码等信息，均通过uid进行关联。在redis中使用`[uid]:[item]`的形式作为key进行存储。例如
- `2333:name`的key保存了uid为2333用户的昵称
- `2021:pass`的key保存了uid为2021用户的密码
使用此形式保存的项有：
- `[uid]:pass` -> String：密码
- `[uid]:name` -> String: 昵称
- `[uid]:score`-> String: 积分
- `[uid]:matches`-> List: 历史对局记录
- `[uid]:friend`-> List: 好友列表
- `[uid]:waitFriend` -> List: 待接收好友列表
最后，为了在登陆时根据昵称寻找对应的uid，也存储了
- `[nickname]:uid` -> String:uid 的键值对
##### 3.2 对局信息
对局信息通过对局ID建立与具体信息的映射关系，用户信息中的对局历史也保存的是对局ID，具体为
- `[对局ID]:match` -> Hash: 对局信息:{"time": 时间戳, [uid1]: 得分情况, [uid2]:得分情况, ...}
##### 3.3 排名
用户排名存储在`RANK`对应的有序集合中，集合的排序依据为用户的积分。因redis将各项按分值从小至大排序，因此逆向遍历排名表以获取正确的排名信息
##### 3.4 其他全局信息
- `UID`：此key中保存了当前最大的uid，可通过此key获取一个未使用的uid
- `MID`: 此key中保存了当前最大对局ID的值，可通过此key获取一个未使用的对局ID