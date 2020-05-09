#ifndef TCP_CONNECTION
#define TCP_CONNECTION

#include "eventloop.h"
#include "channel.h"
#include "buffer.h"
#include "tcp_server.h"
//#include"callback.h”
class buffer;
class channel;
class tcp_connection;
 typedef function<int ( tcp_connection *)> connection_completed_call_back;
 typedef function<int ( tcp_connection *)>  connection_closed_call_back;
typedef function<int ( tcp_connection *)> write_completed_call_back;
 
 // the data has been read to (buf, len)
 typedef function<void (buffer*,tcp_connection *)> message_call_back;

class tcp_connection {
public:
//tcp_connection::tcp_connection(eventloop *loop);
void tcp_connection_new(int fd, eventloop *eventLoop, connection_completed_call_back connectionCompletedCallBack,
                   connection_closed_call_back connectionClosedCallBack,
                   message_call_back messageCallBack, write_completed_call_back writeCompletedCallBack);

//应用层调用入口
int tcp_connection_send_data(char *data,int size);

//应用层调用入口
int tcp_connection_send_buffer(buffer *buffer);

void tcp_connection_shutdown(struct tcp_connection * tcpConnection);

int handle_connection_closed();

int handle_read();

int handle_write();
   
private:
    eventloop *loop;
 
    char *name;
    buffer *input_buffer;   //接收缓冲区
    buffer *output_buffer;  //发送缓冲区
    channel *Tcpchannel;

    connection_completed_call_back connectionCompletedCallBack;
    message_call_back messageCallBack;
    write_completed_call_back writeCompletedCallBack;
    connection_closed_call_back connectionClosedCallBack;

  //  void * data; //for callback use: http_server
   // void * request; // for callback use
   // void * response; // for callback use
};

//int tcp_connection_append_buffer(struct tcp_connection *tcpConnection);
#endif