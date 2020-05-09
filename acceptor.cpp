#include <assert.h>
#include "acceptor.h"

void acceptor::acceptor_init(int port) {
   
    listen_port = 8888;
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // fcntl(listen_fd, F_SETFL, O_NONBLOCK);
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int on = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int rt1 = bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rt1 < 0) {
        error(1, errno, "bind failed ");
    }

    int rt2 = listen(listen_fd, LISTENQ);
    if (rt2 < 0) {
        error(1, errno, "listen failed ");
    }

//    signal(SIGPIPE, SIG_IGN);
    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

  //  if ((connfd = accept(listen_fd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
    //    error(1, errno, "bind failed ");
   // }
}

