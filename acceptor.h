#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "common.h"

class acceptor{
public:
   void  acceptor_init(int port);
    int listen_port;
    int listen_fd;

} ;




#endif