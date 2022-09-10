#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iobuf.h"

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

#define ASSERT(expr)    \
    do {    \
        if (!(expr)) {    \
            printf("FAILURE %s:%d: %s\n", __FILE__, __LINE__, #expr); \
            exit(EXIT_FAILURE);  \
        }   \
    } while(0)  \

int main()
{
    struct iobuf io = {NULL, 0, 0};
    iobuf_init(&io, 32);
    ASSERT(io.buf != NULL && io.cap == max(32, IOBUF_CHUNK_SIZE) && io.len == 0);
    iobuf_dump(&io, io.cap);
    ASSERT(memcmp(io.buf, "\x00", 1) == 0);
    ASSERT(memcmp(io.buf, "\x01", 1) != 0);
    iobuf_add(&io, 3, "hello", strlen("hello"));
    iobuf_dump(&io, io.cap);
    ASSERT(io.buf != NULL && io.cap == max(32, IOBUF_CHUNK_SIZE) && io.len == 8);
    iobuf_resize(&io, 64);
    iobuf_dump(&io, io.cap);
    ASSERT(io.buf != NULL && io.cap == max(64, IOBUF_CHUNK_SIZE) && io.len == 8);
    iobuf_add(&io, 20, "bye", strlen("bye"));
    iobuf_dump(&io, io.cap);
    ASSERT(io.buf != NULL && io.cap == max(64, IOBUF_CHUNK_SIZE) && io.len == 23);
}