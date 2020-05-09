#include <assert.h>
//#include "utils.h"
#include "thread_pool.h"

void thread_pool ::thread_pool_new(eventloop *mainLoop, int threadNumber) {

    //struct thread_pool *threadPool = malloc(sizeof(struct thread_pool));
   this->mainLoop = mainLoop;
   this->position = 0;
   this->thread_number = threadNumber;
   this->started = 0;
   this->eventLoopThreads = NULL;
}

//一定是main thread发起
void thread_pool ::thread_pool_start() {
    assert(!started);
   // assertInSameThread(threadPool->mainLoop);

    started = 1;


    if (thread_number <= 0) {
        return;
    }

    eventLoopThreads = (event_loop_thread *)malloc(thread_number * sizeof(event_loop_thread));
    for (int i = 0; i < thread_number; ++i) {
        eventLoopThreads[i].event_loop_thread_init(i);
        eventLoopThreads[i].event_loop_thread_start();
    }
}

//一定是main thread中选择
 eventloop *thread_pool ::thread_pool_get_loop() {
    assert(started);
   // assertInSameThread(threadPool->mainLoop);

    //优先选择当前主线程
     eventloop *selected = mainLoop;

    //从线程池中按照顺序挑选出一个线程
    if (thread_number > 0) {
        selected = eventLoopThreads[position].loop;
        if (++position >= thread_number) {
            position = 0;
        }
    }

    return selected;
}
