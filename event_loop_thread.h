#ifndef EVENT_LOOP_THREAD_H
#define EVENT_LOOP_THREAD_H

#include    <pthread.h>
//#include"eventloop.h"
class eventloop;
class  event_loop_thread {
    public:
    
    //初始化已经分配内存的event_loop_thread
    int event_loop_thread_init(int);
    static void * event_loop_thread_run(void *arg);
    //由主线程调用，初始化一个子线程，并且让子线程开始运行event_loop
    eventloop *event_loop_thread_start();
     eventloop * loop;
    private:

   
    pthread_t thread_tid;        /* thread ID */
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    char * thread_name;
    long thread_count;    /* # connections handled */
};


#endif