#ifndef EPOLL_DISPATCHER_DATA_H
#define EPOLL_DISPATCHER_DATA_H
#include "eventloop.h"
#include  <sys/epoll.h>
#include    <errno.h>
#include"event_dispatcher.h"
#include"common.h"
//#include"channel.h"
class eventloop;
//class  event_dispatcher;
class channel;
class epoll_dispatcher:public event_dispatcher
{
 public:
    /**  对应实现 */
    //const char *name;
    epoll_dispatcher(eventloop *loop);
   // epoll_dispatcher(eventloop *loop);

    //virtual ~dispatcher();
    /**  初始化函数 */
    virtual void init();

    /** 通知dispatcher新增一个channel事件*/
    virtual  int add(channel * channel);

    /** 通知dispatcher删除一个channel事件*/
    virtual int  del(channel * channel);

    /** 通知dispatcher更新channel对应的事件*/
    virtual int  update(channel * channel);

    /** 实现事件分发，然后调用event_loop的event_activate方法执行callback*/
   virtual  int  dispatch(struct timeval *);

    /** 清除数据 */
   //virtual  void clear();
 private:
    int event_count;
    int nfds;
    int realloc_copy;
    int efd;
    struct epoll_event *events;
    eventloop *loop;
};

#endif