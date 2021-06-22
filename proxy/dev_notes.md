+ 防止重复登陆这个功能是否应该由Social模块来做
+ 
## proxy模块介绍
proxy在整个服务器中只是承担了转发和过滤客户端的请求和响应，以及后端服务向客户发送的消息的功能，基本不涉及到业务细节。因此在流过proxy的任何消息都要遵循proxy能够识别的约定。

### 消息体的结构
我们约定流过proxy的任何消息只有两种类型：REQUEST还有RESPONSE。例如客户端向服务器请求房间列表、请求加入房间、请求加好友等，后端模块也能（通过代理）向客户端发送请求。这些请求所包含的字段的意义和类型可能不相同，但我们规定他们具有一致的protobuf定义：
```
message Request 
{
	enum RequestType {
		// request type define …
	}
	optional RequestType requestType = 1;
	optional int64 uid = 2;
	optional int64 stamp = 3;
	optional repeat string rags = 4;
}
```
proxy可以根据requestType字段判断确定讲客户端的请求转发到哪，也可以根据当前用户的状态而决定是否把这个请求过滤掉。uid这个字段可以用来标记发送这个请求的客户端，也可以用于proxy判断后端请求转发到哪个客户端。stamp这个字段记录了请求发送时间，主要用于和请求匹配。args是真正存储请求参数的地方，至于如何对它进行解释则由最终接收方负责。
后端模块对客户端请求的响应则统一用RESPONSE来表示，它的消息体定义是这样的：



## proxy与客户端交互协议事宜
虽然我们已经约定客户端和proxy之间使用protobuf来对消息进行序列化，但是存在以下情况是protobuf本身无法解决的：
+ 在一个TCP传输通道里可能会存在多个message，而由protobuf序列化后的message并不是self-delimiting的，也就是说
存在拆包和粘包的问题。

+ 一个TCP传输通道会传输多种类型的message，在我们的项目中是两种：Request和Response。这两种类型序列化后的长度
并不一样，也就是说存在传输消息类型模糊的问题。

解决上面两个问题的办法是proxy和客户端商定一个协议，协议报文头部包含要传输的消息体的类型和长度，例如说：
```
| type (int32_t) | length (int32_t) | body (serialized message) |
```
如果我们使用如上的协议来传输序列化后的proto消息体，那么我们要注意type和length字段都需要转成网络字节序后才能放入buffer中。

另外还有一个问题，因为proxy不仅仅要forward客户端的request，还要forward客户端的response，但是原先协定的Response是
不包含command type的，因此proxy是不知道要forward到哪儿的。因此Response可能还需要引入一个ResponseType的类型。
(上面这个问题不用担心了，可以通过response中的stamp字段解决转发模糊性的问题)

## 理清proxy的逻辑
后端模块往proxy发数据：
1. 发request，proxyServer收到后端异步请求后，会将有序对(requestHandler, stamp)以pipeid * stamp为key放进相应clientHandler的requestsMap中，并将request中的stamp字段改为pipefd * stamp，然后将request push进requestWaittingQueue中。最后是注册clientfd的可写事件(这个留到epollfd因为pipefd有事件而通知client时再做)。requestsMap的作用是在proxy收到client对某个request的response之时，能根据response里面的stamp快速索引到相应的requestHandler，以将该response forward到相应的后端模块。注意在forward之前proxy需要将response中的stamp字段恢复成原stamp值（保存requestHandler一块保存在requestsMap中）。requestWaittingQueue的作用是缓冲。
2. 发response，proxy代理client调用了后端模块的异步rpc之后，会收到异步的response。这个response会被proxy的异步响应监听线程push进相应clientHandler的responseWaittingQueue中，然后注册clientfd的可写事件（同样，留给epollfd通知pipefd中的事件时做）。
这里存在一个问题：就是以pipefd * stamp为key仍然可能发送冲突。因为即便后端模块将请求时刻作为stamp的值，这也只能保证该模块发送的request不会被混淆，但是不同模块的时钟并不是严格同步的，因此我才想到乘上一个pipefd来避免这种冲突。一般情况下我觉得这是work的，因为除非等待队列中包含两个时间间隔相差太久的request或者reponse，否则乘上一个随机的pipefd很难发生冲突。

proxy往client发数据：
1. 发Request，这时候clientfd可写，将requestWaittingQueue中的request逐个写进clientfd中（当然，需要封个协议包），直到没有request或者不能写入一个整包为止。若requestWaittingQueue已经空了，那就把clientfd的可写事件给取消注册了。
2. 发Response，这时候clientfd可写，将responseWaittingQueue中的response逐个写进clientfd中（当然，需要封个协议包），直到没有response或者不能写入一个整包为止。若responseWaittingQueue已经空了，那就把clientfd的可写事件给取消注册了。
这里存在一个问题：当clientfd可写时，按什么顺序来发送两个等待队列中的元素？一种解决方案是平等看待每个request和response，交替地将它们往clientfd中写。但实际情况可能会更复杂，因为按公平地观点来看的话，我们也应该将最早塞进队列的元素写进clientfd。

client往proxy发数据：
1. 发Request，这时候clientfd可读，读到Request后相应的clientHandler根据requestType选择调用相应的异步rpc。
2. 发Response，这时候clientfd可读，读到Response之后，需要根据其中的stamp字段找到这个Response相应的requestHandler，并将这个Response作为这个RPC request的reply。

最后一个问题：由于response和request监听线程和主线程共享着clientHandler中的resquestMap和waittingQueue，因此不可避免需要用到同步原语来保护数据结构，我担心这会导致整个系统的响应速度变慢。
不过我认为对于棋牌类这种弱联网游戏，client和后端的交互并不频繁，因此同步带来的锁争用情况并不会严重。
加了同步还会导致另外一个问题：刚才说的交替发送两个waittingQueue的元素会带来很大开销，因为你要不断获取两个队列的锁。因此我的想法是某一次的可写事件只写其中一个waittingQueue的元素，下一次的可写事件只写另一个waittingQueue的元素。当然如果某一次可写事件将其中一个waittingQueue排空了也没有填满tcp缓冲，那么继续写另一个waittingQueue的元素，直到tcp缓冲满或者另一个队列也空了。

当clientfd可写时，我们会将requestWaittingQueue中的requets逐一序列化封包后发给客户端，但有一个问题是这个过程失败了怎么处理。一个想法是准备一个标志着失败的response传递给requestHandler，但这样的话我们首先需要知道原request中的stamp，因为当前待发送的request中的stamp是修改过的。然而这要求我们去读requestsMap中的原stamp。
另一个想法是我们向requestHandler传递一个失败的标志，requestHandler收到失败标志后自行制备失败response。

### 为每个clientHandler分配一个发送和接收缓冲的问题：
由于我们不能保证一次write/read就能发送/接收一个完整的request或者response，因此肯定需要缓冲。另外，缓冲的另外一个目的是batching，也就是当收到或发送大量小报文时可以节省系统调用的次数。然而设置缓冲又会涉及到缓冲的读写策略：
1. 关于写缓冲策略：我们分几种情况来讨论。当需要发送一个大报文时，以致于一个缓冲区都无法装下的时候怎么办？对于这种情况，第一种可行的策略是使用动态内存分配，第二种可行的策略是仍然使用固定大小的缓冲，但是利用状态机的方式完成报文的分段转发，但三种可行的情况是设定报文长度上限。由于第三种方法最容易，且考虑到开发周期短，暂时使用第三种，但后期会考虑拓展到更优的策略。当要发送多个小报文时，我们将waittingQueue中的msg逐一序列化封包并写入缓冲，直到缓冲剩余的空间不够放下一整个msg或者是两个waittingQueue都已经空了。因此，我们当前的策略是不往缓冲中写不完整的包，这可以减小clientHandler的工作量。但是对于client来说仍然存在拆包的问题，处理拆包的问题就要交给client端来解决。但是后面我们会看到，proxy读缓冲时候也要处理拆包的问题，因此两者的处理策略可以相互借鉴。
2. 关于读缓冲策略：我们先从缓冲是空的情况开始考虑。现在clientfd上有可读事件，我们尽可能多地读取数据到缓冲中，如果读到的数据量小于缓冲大小，那显然这次读取操作已经清空了clientfd的tcp接收缓冲。但是我们仍然无法保证读到缓冲中的数据都是完整的一个包。我们这时候逐一解码每个包并调用异步rpc，直到缓冲空了或者最后一个包不完整。这个包剩余的部分会在下一次可读事件中补上。
然后我们收到了下一次的可读事件，如果这次仍然读完了clientfd的tcp接收缓冲，那么显然可以递归地用上一次情况来分析。现在我们考虑的是没有读完clientfd的tcp接收缓冲的情况。我们仍然逐个包地处理接收缓冲，知道没有或剩下一个不完整的包。那这时候我们还要不要用空出来的缓冲空间来继续读没有读完的clientfd数据？直观上来看是要的，因为我们用的是边沿出发模式，如果这是用户最后的请求或者是相当长一段时间内都不发送下一个请求，那么这个包就会得不到及时处理。也许我们可以将这个clientfd切换到水平触发，那么下一次epoll_wait的时候仍然会接着读这个未读完的数据，只不过可写事件也会同时被带动起，我不知道这会有什么副作用。
所以读策略是这样的：
```c++
int onRecv()
{
	int byteRead, pkgProcessed = 0;
	bool needToRead = true;
	while (needToRead)
	{
		byteRead = read(clientfd, readBuffer);
		if (byteRead < 0)
		{	// fatal error, close the client connection
			return -1;
		}
		if (readBuffer.size() != readBuffer.capacity())
		{
			needToRead = false;
		}
		// handle packages in buffer one by one
		for (;;) {//…}
	}
	return pkgProcessed;
}
```

### 关于环形缓冲的设计
其实我之前已经实现过一个简单CircularBuffer类了，这个类本身很简单，只有empty、capacity、size、free四个成员函数，底层数据结构就是一段连续的内存，以及已用缓冲起始和结束位置的标记。

但是为了CircularBuffer能和read/write等系统调用兼容，我重载了read/write方法：允许从fd读数据并写入CircularBuffer里面，以及将CircularBuffer中的内容写入到fd中。另外提供了put和get两个负责将连续内存和CircularBuffer相互转换的方法。

但是现在处理协议包拆包的操作又需要引入更多的操作。例如我想将CircularBuffer头部的4个字节作为一个int32读出来，因为我想判断这个包的长度。那么我们现在需要提供一个新的方法，或者说一个新的模板函数：
```c++
template<typename T>
int readAs(const CircularBuffer &buffer, size_t off, T &out, bool netByteOrder = false);
```
其中off表示从buffer当前起始位置的偏移off个字节开始读，out用来存储读出来的数据，同时实现自动类型推导，netByteOrder表示在buffer内的数据是否以网络字节序存储的，若是的话则读出之前还需要经过一个htonl（因此当前只支持读出整形。。。）。
提供readAs方法是不希望对于小对象也调用memcpy，以及还需要额外提供一个连续buffer。
另外我们也希望能够在CircularBuffer任意一个位置写入某个类型的数据：
```c++
template<typename T>
int writeAs(const CircularBuffer &buffer, size_t off, const T &in, bool netByteOrder = false);
```

另外我们还需要一个将CircularBuffer转为std::string的方法：
```c++
int circularBufferToString(const CircularBuffer &buffer, std::string &str);
```

### ClientHandler生命周期的管理
我之前的想法是用一个HandlerManager来管理每个EventHandler的生命期，当EventHandler的HandleEvents发生fatal error时，向HandlerManager调用expire来使这个Hander不再参与任何事件的处理。并且EventManager向epoll注册定时器事件，来周期性地释放过期的Handler。此外，HandlerManager还负责提Handler索引的功能。
但我现在觉得太复杂了，索引功能为何不通过几个全局的哈希表来实现？反正只有epoll线线程会访问这个哈希表。另外当时没把Handler释放工作交给EventLoop实在是想太多了，操着做框架的心，却是个做业务的命。

### 超时事件
一个维持着长连接的应用层协议应该需要心跳机制来确定客户端/服务是否还在正常工作。当然客户端这边若是shutdown的话一般是会有一个EPOLLHUP之类的事件来提醒服务端的，在我们这个项目里面就是提醒proxy。但如果我们考虑特殊情况的话就是客户端意外关断/网络丢包，使得这个EPOLLHUP事件没法到达proxy。这样的话proxy就会白白的浪费一个Handler资源。那究竟用什么样的方式来判断一个客户端是否还存活呢？一个办法就是客户端定期发心跳信号给proxy，若proxy在3个心跳周期内都没有收到客户端的心跳信号，那么就释放掉这个Handler。那么每个Handler应该向epoll注册一个timerfd，并且自己也保存这个timerfd。每当proxy收到客户端的一个消息（任何消息，不管是心跳还是请求），都得重置这个timerfd。因此当收到timerfd的事件时，我们就知道应该释放该Handler了。


