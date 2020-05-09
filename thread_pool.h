#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include "eventloop.h"
#include "event_loop_thread.h"
#include<vector>
class thread_pool {
    //创建thread_pool的主线程
    public:
    void thread_pool_new(eventloop *mainLoop, int threadNumber);

    void thread_pool_start();

    eventloop *thread_pool_get_loop();
    private:

    eventloop *mainLoop;
    //是否已经启动
    int started;
    //线程数目
    int thread_number;
    //数组指针，指向创建的event_loop_thread数组
    event_loop_thread *eventLoopThreads;

    //表示在数组里的位置，用来决定选择哪个event_loop_thread服务
    int position;

};



#endif