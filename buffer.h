#ifndef BUFFER_H
#define BUFFER_H

#define INIT_BUFFER_SIZE 65536
//数据缓冲区
class buffer {
public:
  buffer();

//void buffer_free(buffer *buffer);

int buffer_writeable_size();

int buffer_readable_size();

int buffer_front_spare_size();

//往buffer里写数据
int buffer_append(char *data, int size);

//往buffer里写数据
int buffer_append_char(char data);

//往buffer里写数据
int buffer_append_string(char * data);

//读socket数据，往buffer里写
int buffer_socket_read(int fd);

//读buffer数据
char buffer_read_char();

//查询buffer数据
//char * buffer_find_CRLF();

void make_room(int size);
//private:
    char *data;          //实际缓冲
    int readIndex;       //缓冲读取位置
    int writeIndex;      //缓冲写入位置
    int total_size;      //总大小
};




#endif