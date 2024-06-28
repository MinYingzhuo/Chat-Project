#include "ChatServer.hpp"
#include "json.hpp"
#include "ChatService.hpp"

#include <iostream>
#include <functional>
#include <string>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

// 初始化聊天服务器对象
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{
  // 注册链接回调
  _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));
  _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

  _server.setThreadNum(4);
}

// 启动服务
void ChatServer::start()
{
  _server.start();
}

// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
  // 客户端断开连接
  if (!conn->connected())
  {
    ChatService::instance()->clientCloseException(conn);

    conn->shutdown();
  }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn, // 连接
                           Buffer *buffer,               // 缓冲区
                           Timestamp time)               // 接收到数据的时间信息
{
  string buf = buffer->retrieveAllAsString();

  // 数据的序列化
  json js = json::parse(buf);

  // 目的： 完全 解耦 网络模块 与 业务模块 的代码
  // 通过js["msgid"],一个id对应一个操作， 获取 =》 业务handler  =》 conn js time
  // 通过回调
  auto msgHandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
  //回调消息绑定好的事件处理器，执行相应的业务处理
  msgHandler(conn, js, time);
}
