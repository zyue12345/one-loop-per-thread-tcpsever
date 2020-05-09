#include "tcp_connection.h"
#include<iostream>
//#include "utils.h"

//tcp_connection::tcp_connection(eventloop *loop)
//{
 //   this->loop=loop;
//}
int tcp_connection::handle_connection_closed() {
    //struct event_loop *eventLoop = tcpConnection->eventLoop;
   // struct channel *channel = tcpConnection->channel;
   loop->event_loop_remove_channel_event(Tcpchannel->fd, Tcpchannel);
    if (connectionClosedCallBack != NULL) {
        connectionClosedCallBack(this);
    }
}

int tcp_connection::handle_read( ) {
    //struct tcp_connection *tcpConnection = (struct tcp_connection *) data;
 //   struct buffer *input_buffer = tcpConnection->input_buffer;
   // struct channel *channel = tcpConnection->channel;

    if (input_buffer->buffer_socket_read(Tcpchannel->fd) > 0) {
        //应用程序真正读取Buffer里的数据
        if (messageCallBack != NULL) {
           messageCallBack(input_buffer, this);
        }
    } else {
         handle_connection_closed();
    }
}

//发送缓冲区可以往外写
//把channel对应的output_buffer不断往外发送
int tcp_connection::handle_write() {
   // struct tcp_connection *tcpConnection = (struct tcp_connection *) data;
    //struct event_loop *eventLoop = tcpConnection->eventLoop;
   // assertInSameThread(eventLoop);

   // struct buffer *output_buffer = tcpConnection->output_buffer;
   // struct channel *channel = tcpConnection->channel;

    ssize_t nwrited = write(Tcpchannel->fd, output_buffer->data + output_buffer->readIndex,
                            output_buffer->buffer_readable_size());
    if (nwrited > 0) {
        //已读nwrited字节
        output_buffer->readIndex += nwrited;
        //如果数据完全发送出去，就不需要继续了
        if (output_buffer->buffer_readable_size() == 0) {
           Tcpchannel->channel_write_event_disable();
        }
        //回调writeCompletedCallBack
        if (writeCompletedCallBack != NULL) {
            writeCompletedCallBack(this);
        }
    } else {
       // yolanda_msgx("handle_write for tcp connection %s", tcpConnection->name);
    }

}


void tcp_connection::tcp_connection_new(int connected_fd, eventloop *eventLoop,
                   connection_completed_call_back connectionCompletedCallBack,
                   connection_closed_call_back connectionClosedCallBack,
                   message_call_back messageCallBack, write_completed_call_back writeCompletedCallBack) {
    //struct tcp_connection *tcpConnection = malloc(sizeof(struct tcp_connection));
    this->writeCompletedCallBack = writeCompletedCallBack;
    this->messageCallBack = messageCallBack;
    this->connectionCompletedCallBack = connectionCompletedCallBack;
    this->connectionClosedCallBack = connectionClosedCallBack;
    this->loop = eventLoop;
    this->input_buffer  =new buffer;
    this->output_buffer = new buffer;


    char *buf = (char*)malloc(16);
    sprintf(buf, "connection-%d", connected_fd);
    name = buf;

    // add event read for the new connection
    Tcpchannel=new channel(loop);
    Tcpchannel->channel_new(connected_fd, EVENT_READ, bind(&tcp_connection::handle_read, this), bind(&tcp_connection::handle_write, this) /* ,tcpConnection */);
   

    //connectionCompletedCallBack callback
    if (connectionCompletedCallBack != NULL) {
         connectionCompletedCallBack(this);
    }

    loop->event_loop_add_channel_event(connected_fd, Tcpchannel);
}

//应用层调用入口
int tcp_connection::tcp_connection_send_data(char *data, int size) {
    size_t nwrited = 0;
    size_t nleft = size;
    int fault = 0;

    //channel *channel = tcpConnection->channel;
   // buffer *output_buffer = tcpConnection->output_buffer;
  // std::cout<<"tcp_connection_send_data"<<std::endl;
   //std::cout<<data<<std::endl;
   //std::cout<<size<<std::endl;
   //  nwrited = write(Tcpchannel->fd, data, size);
    //先往套接字尝试发送数据
    if (!Tcpchannel->channel_write_event_is_enabled() && output_buffer->buffer_readable_size() == 0) {
        nwrited = write(Tcpchannel->fd, data, size);
         //std::cout<<"qqqqq"<<std::endl;
        if (nwrited >= 0) {
            nleft = nleft - nwrited;
            if(nleft==0)
            {
                  if (writeCompletedCallBack != NULL) {
                  writeCompletedCallBack(this);
         }
            }
        } else {
            nwrited = 0;
            if (errno != EWOULDBLOCK) {
                if (errno == EPIPE || errno == ECONNRESET) {
                    fault = 1;
                }
            }
        }
    }

    if (!fault && nleft > 0) {
        //拷贝到Buffer中，Buffer的数据由框架接管
         //std::cout<<"ssss"<<std::endl;
        output_buffer->buffer_append(data + nwrited, nleft);
        if (!Tcpchannel->channel_write_event_is_enabled()) {
            Tcpchannel->channel_write_event_enable();
        }
    }

    return nwrited;
}

int tcp_connection::tcp_connection_send_buffer(buffer *buffer) {
    int size = buffer->buffer_readable_size();
    int result = tcp_connection_send_data( buffer->data + buffer->readIndex, size);
    buffer->readIndex += size;
    return result;
}

void tcp_connection::tcp_connection_shutdown(struct tcp_connection *tcpConnection) {
    if (shutdown(Tcpchannel->fd, SHUT_WR) < 0) {
        //yolanda_msgx("tcp_connection_shutdown failed, socket == %d", tcpConnection->channel->fd);
    }
}
