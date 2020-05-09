#include <assert.h>
#include "eventloop.h"
//#include "common.h"
#include "log.h"
//#include "event_dispatcher.h"
#include "channel.h"
#include <sys/syscall.h>
//#include "utils.h"
#define EPOLL_ENABLE
// in the i/o thread
#include <iostream>
using namespace std;

//int  handle(void *data) {
	////struct event_loop *eventLoop = (struct event_loop *) data;
	//char one;
	//ssize_t n = read(eventLoop->socketPair[1], &one, sizeof one);
	//if (n != sizeof one) {
	//	LOG_ERR("handleWakeup  failed");
	//}
	//yolanda_msgx("wakeup, %s", eventLoop->thread_name);
	//return 0;
//}
int eventloop:: event_loop_handle_pending_channel() {
	//get the lock
	//pthread_mutex_lock(&mutex);
	is_handle_pending = 1;

	struct channel_element *channelElement = pending_head;
	while (channelElement != NULL) {

		//save into event_map
		channel *channel = channelElement->chann;
		int fd = channel->fd;
		if (channelElement->type == 1) {
			event_loop_handle_pending_add(fd, channel);
		}
		else if (channelElement->type == 2) {
			event_loop_handle_pending_remove(fd, channel);
			cout << "successful!" << endl;
		}
		else if (channelElement->type == 3) {
			event_loop_handle_pending_update(fd, channel);
		}
		channelElement = channelElement->next;
	}

	pending_head = pending_tail = NULL;
	is_handle_pending = 0;

	//release the lock
	//pthread_mutex_unlock(&mutex);

	return 0;
}

void eventloop::event_loop_channel_buffer_nolock(int fd, channel *channel1, int type) {
	//add channel into the pending list
	struct channel_element *channelElement = new channel_element;
	channelElement->chann = channel1;
	channelElement->type = type;
	channelElement->next = NULL;
	if (pending_head == NULL) {
		pending_head = pending_tail = channelElement;
	}
	else {
		pending_tail->next = channelElement;
		pending_tail = channelElement;
	}
}

int eventloop::event_loop_do_channel_event(int fd, channel *channel1, int type) {
	//get the lock
	//pthread_mutex_lock(mutex);
	assert(is_handle_pending == 0);
	event_loop_channel_buffer_nolock(fd, channel1, type);
	//release the lock
	//pthread_mutex_unlock(&mutex);
	// if (!isInSameThread(eventLoop)) {
	//	event_loop_wakeup();
	//}
	//else {
	 event_loop_handle_pending_channel();
	//}

	return 0;

}

int eventloop::event_loop_add_channel_event(int fd, channel *channel1) {
	return event_loop_do_channel_event(fd, channel1, 1);
}

int eventloop::event_loop_remove_channel_event(int fd, channel *channel1) {
	return event_loop_do_channel_event(fd, channel1, 2);
}

int eventloop::event_loop_update_channel_event(int fd, channel *channel1) {
	return event_loop_do_channel_event(fd, channel1, 3);
}

// in the i/o thread
int eventloop::event_loop_handle_pending_add(int fd, channel *channel) {
	yolanda_msgx("add channel fd == %d, %s", fd, thread_name);
	//cout<<gettid()<<endl;
	//struct channel_map *map = eventLoop->channelMap;

	if (fd < 0)
	 return 0;

	//if (fd >= map->nentries) {
	//	if (map_make_space(map, fd, sizeof(struct channel *)) == -1)
	//		return (-1);
	//}

	//if ((map)->entries[fd] == NULL) {
	//	map->entries[fd] = channel;
	//	//add channel
	  channelMap[fd] = channel;
	//	struct event_dispatcher *eventDispatcher = eventLoop->eventDispatcher;
	  eventDispatcher->add(channel);
	//	return 1;
	//}//

	return 0;
}

// in the i/o thread
int eventloop::event_loop_handle_pending_remove(int fd, channel *channel1) {
	//struct channel_map *map = eventLoop->channelMap;
	assert(fd == channel1->fd);

	if (fd < 0)
		return 0;

	// if (fd >= map->nentries)
	//     return (-1);
	channelMap.erase(fd);
	// struct channel *channel2 = map->entries[fd];

	//update dispatcher(multi-thread)here
	// struct event_dispatcher *eventDispatcher = eventLoop->eventDispatcher;
	int retval = 0;
	/*if (eventDispatcher->del(eventLoop, channel2) == -1) {
		retval = -1;
	}
	else {
		retval = 1;
	}

	channelMap[fd] = NULL;*/
	return retval;
}

// in the i/o thread
int eventloop::event_loop_handle_pending_update(int fd, channel *channel) {
	// yolanda_msgx("update channel fd == %d, %s", fd, eventLoop->thread_name);
	// struct channel_map *map = eventLoop->channelMap;

	if (fd < 0)
		return 0;

	// if ((map)->entries[fd] == NULL) {
	//    return (-1);
	// }
	channelMap[fd] = channel;
	//update channel
	//struct event_dispatcher *eventDispatcher = eventLoop->eventDispatcher;
	//eventDispatcher->update(eventLoop, channel);
}

int eventloop::channel_event_activate(int fd, int revents) {
	// struct channel_map *map = eventLoop->channelMap;
	yolanda_msgx("activate channel fd == %d, revents=%d, %s", fd, revents, thread_name);
    cout<< pthread_self()<<endl;
	if (fd < 0)
		return 0;

	//if (fd >= map->nentries)return (-1);
   //cout<<"channel_event_activate"<<endl;
	channel *chann= channelMap[fd];
	assert(fd == chann->fd);

	if (revents & (EVENT_READ)) {
		if (chann->eventReadCallback) chann->eventReadCallback();
	}
	if (revents & (EVENT_WRITE)) {
		if (chann->eventWriteCallback) chann->eventWriteCallback();
	}

	return 0;

}

void eventloop::event_loop_wakeup() {
	char one = 'a';
	ssize_t n = write(socketPair[0], &one, sizeof one);
	cout<<"event_loop_wakeup()"<<endl;
	//if (n != sizeof one) {
	//	LOG_ERR("wakeup event loop thread failed");
	//}
}

int eventloop::handleWakeup() {
	////struct event_loop *eventLoop = (struct event_loop *) data;
	char one;
	ssize_t n = read(socketPair[1], &one, sizeof one);
	//if (n != sizeof one) {
	//	LOG_ERR("handleWakeup  failed");
	//}
	//yolanda_msgx("wakeup, %s", eventLoop->thread_name);
	return 0;
}

void  eventloop::event_loop_init() {
	return event_loop_init_with_name(NULL);
}
void  eventloop::event_loop_init_with_name(char *thread_name) {
	//pthread_mutex_init(&mutex, NULL);
	//pthread_cond_init(&cond, NULL);

	if (thread_name != NULL) {
		this->thread_name = thread_name;
	}
	else {
		this->thread_name = "main thread";
	}

	quit = 0;

#ifdef EPOLL_ENABLE
	//yolanda_msgx("set epoll as dispatcher, %s", thread_name);
	eventDispatcher = new epoll_dispatcher(this);
#else
	yolanda_msgx("set poll as dispatcher, %s", eventLoop->thread_name);
	eventLoop->eventDispatcher = &poll_dispatcher;
#endif
     eventDispatcher->init();
	//add the socketfd to event
	//owner_thread_id = pthread_self();
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) < 0) {
	//	LOG_ERR("socketpair set fialed");
	cout<<"socketpair set fialed"<<endl;
	}
	is_handle_pending = 0;
	pending_head = NULL;
	pending_tail = NULL;

	channel *chann= new channel(this);
	chann->channel_new(socketPair[1], EVENT_READ, bind(&eventloop::handleWakeup,this), NULL);
	event_loop_add_channel_event(socketPair[1], chann);

}


int eventloop::event_loop_run() {
	//assert(eventLoop != NULL);

	// event_dispatcher *dispatcher = eventDispatcher;

	/*if (owner_thread_id != pthread_self()) {
		exit(1);
	}*/

	//yolanda_msgx("event loop run, %s", thread_name);
	struct timeval timeval;
	timeval.tv_sec = 1;

	while (!quit) {
		//block here to wait I/O event, and get active channels
	    eventDispatcher->dispatch(&timeval);
		//handle the pending channel
		event_loop_handle_pending_channel();
	}

	//yolanda_msgx("event loop end, %s", thread_name);
	return 0;
}