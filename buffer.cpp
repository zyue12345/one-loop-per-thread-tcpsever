#include "buffer.h" 
#include "common.h"

const char *CRLF = "\r\n";

  buffer::buffer(){
   // struct buffer *buffer1 = malloc(sizeof(struct buffer));
   // if (!buffer1)
    //    return NULL;

    data =(char *) malloc(INIT_BUFFER_SIZE);
    total_size = INIT_BUFFER_SIZE;
    readIndex = 0;
    writeIndex = 0;
    //return buffer1;
}

//void buffer::buffer_free(struct buffer *buffer1) {
 //   free(buffer1->data);
//    free(buffer1);
//}

int buffer::buffer_writeable_size() {
    return total_size -writeIndex;
}

int buffer::buffer_readable_size() {
    return writeIndex - readIndex;
}

int buffer::buffer_front_spare_size() {
    return readIndex;
}

void buffer::make_room(int size) {
    if (buffer_writeable_size() >= size) {
        return;
    }
    //如果front_spare和writeable的大小加起来可以容纳数据，则把可读数据往前面拷贝
    if (buffer_front_spare_size() + buffer_writeable_size() >= size) {
        int readable = buffer_readable_size();
        int i;
        for (i = 0; i < readable; i++) {
            memcpy(data + i, data + readIndex + i, 1);
        }
        readIndex = 0;
        writeIndex = readable;
    } else {
        //扩大缓冲区
        char *tmp = (char *)realloc(data, total_size + size);
        if (tmp == NULL) {
            return;
        }
        data = tmp;
        total_size += size;
    }
}

int buffer::buffer_append(char*data, int size) {
    if (data != NULL) {
        make_room(size);
        //拷贝数据到可写空间中
        memcpy(data + writeIndex, data, size);
       writeIndex += size;
    }
}

int buffer::buffer_append_char(char data) {
    make_room( 1);
    //拷贝数据到可写空间中
    this->data[writeIndex++] = data;
}

int buffer::buffer_append_string(char *data) {
    if (data != NULL) {
        int size = strlen(data);
        buffer_append(data, size);
    }
}


int buffer::   buffer_socket_read(int fd) {
    char additional_buffer[INIT_BUFFER_SIZE];
    struct iovec vec[2];
    int max_writable = buffer_writeable_size();
    vec[0].iov_base =data + writeIndex;
    vec[0].iov_len = max_writable;
    vec[1].iov_base = additional_buffer;
    vec[1].iov_len = sizeof(additional_buffer);
    int result = readv(fd, vec, 2);
    if (result < 0) {
        return -1;
    } else if (result <= max_writable) {
       writeIndex += result;
    } else {
        writeIndex = total_size;
        buffer_append(additional_buffer, result - max_writable);
    }
    return result;
}

char buffer::  buffer_read_char() {
    char c = data[readIndex];
    readIndex++;
    return c;
}

//char *buffer_find_CRLF(struct buffer *buffer) {
 //   char *crlf = memmem(data + readIndex, buffer_readable_size(), CRLF, 2);
   // return crlf;
//}