# 整体压测逻辑
用程序模拟多个虚拟玩家从注册-登录-游戏-社交全流程，虚拟玩家首先执行一组和大厅相关的请求，随后执行一组和房间相关的请求，之后执行和social相关的一组查询请求，最后执行一组和player相关的社交请求。每一组请求分别测试了从客户端到代理，代理到相应后端服务的链路能承受的并发量。

## 测试流程
首先每位虚拟玩家先执行SIGNUP请求完成注册，其中注册名为`player_x`，`x`为该虚拟玩家在测试程序中分配到的序号，注册密码统一为`pass`。

在等待所有玩家都注册完成之后，正式开始对大厅服务的压测。每位虚拟玩家执行如下请求序列：
1. 执行`LOGIN player_x pass`
2. 执行`ROOM_LIST`，若返回房间列表非空，则随机选择一个房间x跳到第5步
3. 执行`QUICK_MATCH`，若成功则直接跳到第7步
4. 执行`CREATE_ROOM`，记录房间号为x
5. 执行`JOIN_ROOM` x，若失败，则直接跳到第8步
6. 执行`READY`
7. 执行`LEAVE_ROOM`
8. 执行`LOGOUT`

完成对大厅服务的压测后，开始对房间服务的压测。每位虚拟玩家执行如下请求序列：
1. 执行`LOGIN player_x pass`
2. 执行`QUICK_MATCH`，若成功则直接跳到第4步
3. 执行`ROOM_LIST`，逐一尝试`JOIN`每个房间号，直到成功，若没有一个能成功，那么直接执行`LOGOUT`
4. 等待room发bet request，收到request后执行`BET 100`
5. 尝试执行`DOUBLE`
6. 等待room发hit request，执行`HIT`
7. 等待room发hit request，执行`STAND`
8. 尝试执行`SURRENDER`
9. 若上述请求执行完后无法退出到大厅，则执行`LEAVE_ROOM`请求

在执行上述逻辑期间，忽略所有从房间服务的update包。

完成对房间服务的压测后，开始对social服务的压测。每位玩家初始处于大厅：
1. 执行`INFO`
2. 执行`RANK_ME`
3. 执行`RANK_TOP 5`
4. 执行`LIST_MATCH`, 若有记录，选出第一个mid作为x，执行第5步
5. 执行`GET_MATCH_INFO x`

在完成对social服务的压测之后，开始对player服务的压测。每位玩家初始处于大厅：
1. 执行`ADD_FRIEND player_y`，其中y = (x + 1) % playerno，也就是当前玩家序号加一后对整体玩家数量取模。
2. 执行`LIST_WAITTING`，记录列表中所有玩家p[:]
3. 对p[:]中的玩家执行`ACCEPT_FRIEND`
4. 执行`LIST_FIREND`，记录列表中所有玩家f[:]
5. 对f[:]中的玩家执行`DELETE_FRIEND`

在以上测试中，所有玩家只要遇到一个请求超时，就立即退出测试，并记录已发送请求的数量和已发送的请求的平均延迟。


