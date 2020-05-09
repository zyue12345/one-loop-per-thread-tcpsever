#include<iostream>
#include<pthread.h>
#include"acceptor.h"
//#include"channel.h"
#include"eventloop.h"
#include"tcp_server.h"
#include<functional>
using namespace std;
using namespace placeholders;
//typedef int(*event_read_callback)(void *data);
//
//typedef int(*event_write_callback)(void *data);
//
//
//struct channel {
//	int fd;
//	int events;   //��ʾevent����
//
//	event_read_callback eventReadCallback;
//	event_write_callback eventWriteCallback;
//	void *data; //callback data, ������event_loop��Ҳ������tcp_server����tcp_connection
//};
char rot13_char(char c) {

    if ((c >= 'a' && c <= 'm') || (c >= 'A' && c <= 'M'))

        return c + 13;

    else if ((c >= 'n' && c <= 'z') || (c >= 'N' && c <= 'Z'))

        return c - 13;

    else

        return c;

}
 void onMessage( buffer *input, tcp_connection *tcpConnection) {

    //printf("get message from tcp connection %s\n", tcpConnection->name);

    printf("%s", input->data);

   
    buffer *output=new buffer;

    int size = input->buffer_readable_size();
	// write(tcpConnection->Tcpchannel->fd, input->data, 0);
     //cout<<"input->buffer_readable_size()"<<"    "<<size<<endl;
    for (int i = 0; i < size; i++) {

        output->buffer_append_char(rot13_char(input->buffer_read_char()));
      

    }
    tcpConnection->tcp_connection_send_buffer(output);


}
 int ona(tcp_connection *tcpConnection) {

    //printf("get message from tcp connection %s\n", tcpConnection->name);
	cout<<"connection completed"<<endl;
	return 0;

}
 int onb(tcp_connection *tcpConnection) {

    //printf("get message from tcp connection %s\n", tcpConnection->name);
	cout<<"connection closed"<<endl;
	return 0;

}
 int onc(tcp_connection *tcpConnection) {

    //printf("get message from tcp connection %s\n", tcpConnection->name);
	cout<<"write completed"<<endl;
	return 0;

}
int main()
{
  //vi cout<< pthread_self()<<endl;
//	channel ca;
//	channel cb;
	eventloop loop;
	loop.event_loop_init();
	acceptor ac;
	ac.acceptor_init(8888);
	tcp_server server;
    server.tcp_server_init(&loop,&ac,
                 bind(ona,_1),
                bind(onMessage,_1,_2),
                NULL,
                bind(onb,_1),
               3);
//	loop.event_loop_channel_buffer_nolock(1, &ca, 2);
//	loop.event_loop_channel_buffer_nolock(1, &ca, 2);
	//cout<<"1"<<endl;
	//loop.event_loop_handle_pending_channel();
	//
       // loop. event_loop_init();
	 server.tcp_server_start( );
	 //loop.event_loop_wakeup();
	 loop.event_loop_run();
	
	//ca.data = (void *)100;
	//ca.eventReadCallback = eventReadCallback;
	//void *p = (void *)1;
	//ca.eventReadCallback(ca.data);
	//ca.channel_new(1, 2, eventReadCallback, eventReadCallback);
	//ca.eventReadCallback();
	//system("pause");
	cout<<"ssss"<<endl;
}
