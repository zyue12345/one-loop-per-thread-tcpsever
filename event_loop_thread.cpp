#include <assert.h>
#include "event_loop_thread.h"
#include "eventloop.h"
#include"log.h"
void *event_loop_thread ::event_loop_thread_run(void *arg) {
    event_loop_thread *eventLoopThread = (event_loop_thread *) arg;

    pthread_mutex_lock(&eventLoopThread->mutex);

    // 初始化化event loop，之后通知主线程
    eventLoopThread->loop =new eventloop;
     eventLoopThread->loop-> event_loop_init_with_name( eventLoopThread->thread_name);
   // yolanda_msgx("event loop thread init and signal, %s", eventLoopThread->thread_name);
    pthread_cond_signal(&eventLoopThread->cond);

    pthread_mutex_unlock(&eventLoopThread->mutex);

    //子线程event loop run
    eventLoopThread-> loop->event_loop_run();
    return NULL;
}

//初始化已经分配内存的event_loop_thread
int event_loop_thread ::event_loop_thread_init(int i) {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    loop = NULL;
    thread_count = 0;
    thread_tid = 0;
    char *buf =(char *) malloc(16);
    sprintf(buf, "Thread-%d\0", i + 1);
    thread_name = buf;
    return 0;
}


 eventloop * event_loop_thread ::event_loop_thread_start( ) {
    pthread_create(&this->thread_tid, NULL, &event_loop_thread_run, this);

    assert(pthread_mutex_lock(&mutex) == 0);

    while (loop == NULL) {
        assert(pthread_cond_wait(&cond, &mutex) == 0);
    }
    assert(pthread_mutex_unlock(&mutex) == 0);

   yolanda_msgx("event loop thread started, %s", thread_name);
    return loop;
}