#include"epoll_dispatcher.h"
#include <errno.h>
#include"channel.h"
#include"iostream"
using namespace std;
// epoll_dispatcher::epoll_dispatcher( )
// {
  
// }
#define MAXEVENTS 100
 epoll_dispatcher:: epoll_dispatcher(eventloop *loop)
 {
     this->loop=loop;
 }
void epoll_dispatcher:: init() {
    //epoll_dispatcher_data *epollDispatcherData = malloc(sizeof(epoll_dispatcher_data));

    event_count = 0;
    nfds = 0;
    realloc_copy = 0;
    efd = 0;

    efd = epoll_create1(0);
    if (efd == -1) {
        error(1, errno, "epoll create failed");
    }
    this->loop=loop;
    events = (struct epoll_event *)calloc(MAXEVENTS, sizeof(struct epoll_event));
}


int epoll_dispatcher::add(channel *channel1) {
    //epoll_dispatcher_data *pollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;
    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | EPOLLIN;
    }
    if (channel1->events & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
     event.events = events | EPOLLET;
    if (epoll_ctl(efd, EPOLL_CTL_ADD, fd, &event) == -1) {
       :: error(1, errno, "epoll_ctl add  fd failed");
    }

    return 0;
}

int epoll_dispatcher::del(channel *channel1) {
    //epoll_dispatcher_data *pollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;

    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | EPOLLIN;
    }

    if (channel1->events & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
//    event.events = events | EPOLLET;
    if (epoll_ctl(efd, EPOLL_CTL_DEL, fd, &event) == -1) {
        error(1, errno, "epoll_ctl delete fd failed");
    }

    return 0;
}

int epoll_dispatcher::update(channel *channel1) {
    //epoll_dispatcher_data *pollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

    int fd = channel1->fd;

    int events = 0;
    if (channel1->events & EVENT_READ) {
        events = events | EPOLLIN;
    }

    if (channel1->events & EVENT_WRITE) {
        events = events | EPOLLOUT;
    }

    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
//    event.events = events | EPOLLET;
    if (epoll_ctl(efd, EPOLL_CTL_MOD, fd, &event) == -1) {
        error(1, errno, "epoll_ctl modify fd failed");
    }

    return 0;
}

int epoll_dispatcher::dispatch(struct timeval *timeval) {
    //epoll_dispatcher_data *epollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;
    int i, n;

    n = epoll_wait(efd, events, MAXEVENTS, -1);
   // yolanda_msgx("epoll_wait wakeup, %s", thread_name);
    for (i = 0; i < n; i++) {
        if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
            fprintf(stderr, "epoll error\n");
            close(events[i].data.fd);
            continue;
        }

        if (events[i].events & EPOLLIN) {
          //  yolanda_msgx("get message channel fd==%d for read, %s", events[i].data.fd, thread_name);
            loop->channel_event_activate(events[i].data.fd, EVENT_READ);
            //cout<<"epoll"<<endl;
        }

        if (events[i].events & EPOLLOUT) {
            //yolanda_msgx("get message channel fd==%d for write, %s", events[i].data.fd,thread_name);
            loop->channel_event_activate(events[i].data.fd, EVENT_WRITE);
        }
    }

    return 0;
}

// void epoll_clear(struct event_loop *eventLoop) {
//     //epoll_dispatcher_data *epollDispatcherData = (epoll_dispatcher_data *) eventLoop->event_dispatcher_data;

//     free(epollDispatcherData->events);
//     close(epollDispatcherData->efd);
//     free(epollDispatcherData);
//     event_dispatcher_data = NULL;

//     return;
// }