#include "channel.h"
#include"eventloop.h"
#include<iostream>
     channel::channel(eventloop *loop)
	 {
		 this->loop=loop;
	 }
void channel::channel_new(int fd, int events, event_read_callback eventReadCallback, event_write_callback eventWriteCallback) {
	//  struct channel *chan = malloc(sizeof(struct channel));
	this->fd = fd;
	this->events = events;
	this->eventReadCallback = eventReadCallback;
	this->eventWriteCallback = eventWriteCallback;
	this->data = data;
}
int channel::channel_write_event_is_enabled() {
    //std::cout<<(events & EVENT_WRITE)<<std::endl;
	return  events & EVENT_WRITE;
	
}

int channel::channel_write_event_enable() {
    events = events | EVENT_WRITE;
    loop->event_loop_update_channel_event( fd, this);
}

int channel::channel_write_event_disable() {
    events = events & ~EVENT_WRITE;
    loop->event_loop_update_channel_event(fd, this);
}