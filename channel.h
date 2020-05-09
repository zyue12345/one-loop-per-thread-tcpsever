#ifndef CHANNEL_H
#define CHANNEL_H

#define EVENT_TIMEOUT    0x01
/** Wait for a socket or FD to become readable */
#define EVENT_READ        0x02
/** Wait for a socket or FD to become writeable */
#define EVENT_WRITE    0x04
/** Wait for a POSIX signal to be raised*/
#define EVENT_SIGNAL    0x08

#include<functional>
//#include"eventloop.h"


//typedef int(*event_read_callback)(void *data);

//typedef int(*event_write_callback)(void *data);
using namespace std;
typedef function <int(void)> event_read_callback;
typedef function <int(void)> event_write_callback;
class eventloop;
class channel {
public:
	channel(eventloop *loop);
	void channel_new(int fd, int events, event_read_callback eventReadCallback, event_write_callback eventWriteCallback);
	int channel_write_event_is_enabled();

	int channel_write_event_enable();

	int channel_write_event_disable();
	int fd;
	int events;  
	event_read_callback   eventReadCallback;
	event_write_callback  eventWriteCallback;
	void *data; 
    eventloop *loop;
};

#endif
