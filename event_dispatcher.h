#ifndef EVENT_DISPATCHER_DATA_H
#define EVENT_DISPATCHER_DATA_H

#include  <sys/epoll.h>
#include    <errno.h>
#include"common.h"
#include"channel.h"
//#include"eventloop.h"
//class eventloop;
//class  event_dispatcher;
//class channel;
class event_dispatcher
{
 public:
    /**  对应实现 */
    //const char *name;

   // epoll_dispatcher(eventloop *loop);

    //virtual ~dispatcher();
    /**  初始化函数 */
    virtual void init()=0;

    /** 通知dispatcher新增一个channel事件*/
    virtual  int add(channel * channel)=0;

    /** 通知dispatcher删除一个channel事件*/
    virtual int  del(channel * channel)=0;

    /** 通知dispatcher更新channel对应的事件*/
    virtual int  update(channel * channel)=0;

    /** 实现事件分发，然后调用event_loop的event_activate方法执行callback*/
   virtual  int  dispatch(struct timeval *)=0;

    /** 清除数据 */
   //virtual  void clear();

    
};

#endif