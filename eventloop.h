#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#include<map>
//#include <pthread.h>
//#include "channel.h"
#include"event_dispatcher.h"
#include"epoll_dispatcher.h"
//#include "event_dispatcher.h"
//#include "common.h"
using namespace std;
//extern const struct event_dispatcher poll_dispatcher;
//extern const struct event_dispatcher epoll_dispatcher;
class event_dispatcher;
struct channel_element {
	int type; //1: add  2: delete
	channel *chann;
	channel_element *next;
};
class channel;
class eventloop
{
public:
	//eventloop();

	//~eventloop();

	void event_loop_init();

	void event_loop_init_with_name(char * thread_name);

	int event_loop_run();

	void event_loop_wakeup();

	int handleWakeup( );

	int event_loop_do_channel_event(int fd, channel *channel1, int type);

	int event_loop_handle_pending_channel();

	void event_loop_channel_buffer_nolock(int fd, channel *channel1, int type);

	int event_loop_add_channel_event(int fd, channel *channel1);

	int event_loop_remove_channel_event(int fd, channel *channel1);

	int event_loop_update_channel_event(int fd, channel *channel1);

	int event_loop_handle_pending_add(int fd, channel *channel);

	int event_loop_handle_pending_remove(int fd, channel *channel);

	int event_loop_handle_pending_update(int fd, channel *channel);

	int channel_event_activate(int fd, int res);

	struct channel_element *pending_head;
	struct channel_element *pending_tail;
private:
	int quit;
	event_dispatcher *eventDispatcher;
	map<int, channel*>channelMap;
	// event_dispatcher *event_dispatcher_data;
	//  struct channel_map *channelMap;
	int is_handle_pending;
	//pthread_t owner_thread_id;
	//pthread_mutex_t mutex;
	//pthread_cond_t cond;
	int socketPair[2];
	char *thread_name;
};

#endif
