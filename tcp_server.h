#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include<functional>
#include"tcp_connection.h"
//typedef int (*connection_completed_call_back)(struct tcp_connection *tcpConnection);

//typedef int (*message_call_back)(struct buffer *buffer, struct tcp_connection *tcpConnection);

//typedef int (*write_completed_call_back)(struct tcp_connection *tcpConnection);

//typedef int (*connection_closed_call_back)(struct tcp_connection *tcpConnection);


#include "acceptor.h"
#include "eventloop.h"
#include "thread_pool.h"
//#include "thread_pool.h"
class tcp_connection;
class buffer;
 typedef function<int ( tcp_connection *)> connection_completed_call_back;
 typedef function<int (tcp_connection *)>  connection_closed_call_back;
typedef function<int (tcp_connection *)> write_completed_call_back;
 
 // the data has been read to (buf, len)
 typedef function<void (buffer*,tcp_connection *)> message_call_back;

class tcp_server {
public:
    //准备监听套接字
    void tcp_server_init(eventloop *loop, acceptor *acceptor,
                connection_completed_call_back connectionCallBack,
                message_call_back messageCallBack,
                write_completed_call_back writeCompletedCallBack,
                connection_closed_call_back connectionClosedCallBack,
                int threadNum);
    //开启监听
    void tcp_server_start();

    //设置callback数据
    void tcp_server_set_data();

    int handle_connection_established();

    void make_nonblocking(int fd) {
    fcntl(fd, F_SETFL, O_NONBLOCK);
   }
private:
    int port;
    eventloop *loop;
    acceptor *Tcpacceptor;
    connection_completed_call_back connectionCompletedCallBack;
    message_call_back messageCallBack;
    write_completed_call_back writeCompletedCallBack;
    connection_closed_call_back connectionClosedCallBack;
    int threadNum;
    thread_pool *threadPool;
    void * data; //for callback use: http_server
};


#endif

