#include <assert.h>
#include "common.h"
#include "tcp_server.h"
//#include "thread_pool.h"
//#include "utils.h"
#include "tcp_connection.h"

// int tcp_server(int port) {
//     int listenfd;
//     listenfd = socket(AF_INET, SOCK_STREAM, 0);

//     struct sockaddr_in server_addr;
//     bzero(&server_addr, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     server_addr.sin_port = htons(port);

//     int on = 1;
//     setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

//     int rt1 = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
//     if (rt1 < 0) {
//         error(1, errno, "bind failed ");
//     }

//     int rt2 = listen(listenfd, LISTENQ);
//     if (rt2 < 0) {
//         error(1, errno, "listen failed ");
//     }

//     signal(SIGPIPE, SIG_IGN);

//     int connfd;
//     struct sockaddr_in client_addr;
//     socklen_t client_len = sizeof(client_addr);

//     if ((connfd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
//         error(1, errno, "bind failed ");
//     }

//     return connfd;
// }


// int tcp_server_listen(int port) {
//     int listenfd;
//     listenfd = socket(AF_INET, SOCK_STREAM, 0);

//     struct sockaddr_in server_addr;
//     bzero(&server_addr, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     server_addr.sin_port = htons(port);

//     int on = 1;
//     setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

//     int rt1 = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
//     if (rt1 < 0) {
//         error(1, errno, "bind failed ");
//     }

//     int rt2 = listen(listenfd, LISTENQ);
//     if (rt2 < 0) {
//         error(1, errno, "listen failed ");
//     }

//     signal(SIGPIPE, SIG_IGN);

//     return listenfd;
// }


// int tcp_nonblocking_server_listen(int port) {
//     int listenfd;
//     listenfd = socket(AF_INET, SOCK_STREAM, 0);

//     make_nonblocking(listenfd);

//     struct sockaddr_in server_addr;
//     bzero(&server_addr, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     server_addr.sin_port = htons(port);

//     int on = 1;
//     setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

//     int rt1 = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
//     if (rt1 < 0) {
//         error(1, errno, "bind failed ");
//     }

//     int rt2 = listen(listenfd, LISTENQ);
//     if (rt2 < 0) {
//         error(1, errno, "listen failed ");
//     }

//     signal(SIGPIPE, SIG_IGN);

//     return listenfd;
// }





void tcp_server::tcp_server_init(eventloop *loop, acceptor *acceptor,
                connection_completed_call_back connectionCompletedCallBack,
                message_call_back messageCallBack,
                write_completed_call_back writeCompletedCallBack,
                connection_closed_call_back connectionClosedCallBack,
                int threadNum) {
  //  struct TCPserver *tcpServer = malloc(sizeof(struct TCPserver));
    this->loop = loop;
    Tcpacceptor = acceptor;
    this-> connectionCompletedCallBack = connectionCompletedCallBack;
    this->messageCallBack = messageCallBack;
    this-> writeCompletedCallBack = writeCompletedCallBack;
    this-> connectionClosedCallBack = connectionClosedCallBack;
    this->threadNum = threadNum;
  threadPool = new thread_pool;
  threadPool->thread_pool_new(loop, threadNum);
    data = NULL;
}


int tcp_server::handle_connection_established() {
   // struct TCPserver *tcpServer = (struct TCPserver *) data;
  //  struct acceptor *acceptor = tcpServer->acceptor;
    int listenfd = Tcpacceptor->listen_fd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int connected_fd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len);
    make_nonblocking(connected_fd);

 //   yolanda_msgx("new connection established, socket == %d", connected_fd);

    // choose event loop from the thread pool
    eventloop *eventLoop=threadPool->thread_pool_get_loop();

    // create a new tcp connection
    tcp_connection *tcpConnection = new tcp_connection;
    tcpConnection -> tcp_connection_new(connected_fd, eventLoop,
                                                              connectionCompletedCallBack,
                                                              connectionClosedCallBack,
                                                              messageCallBack,
                                                             writeCompletedCallBack);
    //for callback use
   // if (tcpServer->data != NULL) {
   //     tcpConnection->data = tcpServer->data;
   // }
    return 0;
}


//开启监听
void tcp_server::tcp_server_start( ) {
    //acceptor *acceptor = Tcpacceptor;
  //  eventloop *eventLoop = loop;

    //开启多个线程
   threadPool->thread_pool_start();

    //acceptor主线程， 同时把tcpServer作为参数传给channel对象
    channel *chann = new channel(loop);
    chann->channel_new(Tcpacceptor->listen_fd, EVENT_READ, bind(&tcp_server::handle_connection_established,this), NULL);
   loop->event_loop_add_channel_event(chann->fd, chann);
    return;
}

//设置callback数据
void tcp_server::tcp_server_set_data() {
    if (data != NULL) {
        this->data = data;
    }
}

