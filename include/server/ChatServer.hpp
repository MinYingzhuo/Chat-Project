#ifndef CHATSERVER_H /* 在 CHATSERVER_H 宏的保护下，防止头文件被多次包含，可以避免编译时的重复定义错误。 */
#define CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>

using namespace muduo;
using namespace muduo::net;

//聊天服务器主类
class ChatServer
{
public:
  //初始化聊天服务器对象
  ChatServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg);

  //启动服务
  void start();

private:
  TcpServer _server; // 组合muduo库，实现服务器功能的类对象
  EventLoop *_loop;

  //上报链接相关信息的回调函数
  void onConnection(const TcpConnectionPtr &conn);

  // 上报读写事件相关信息的回调函数
  void onMessage(const TcpConnectionPtr &conn, // 连接
                   Buffer *buffer,               // 缓冲区
                   Timestamp time);               // 接收到数据的时间信息


};


#endif