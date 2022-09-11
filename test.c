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
    iobuf_dump(&io, 32);
    ASSERT(io.buf != NULL && io.cap == max(32, IOBUF_CHUNK_SIZE) && io.len == 0);
    ASSERT(memcmp(io.buf, "\x00", 1) == 0);
    
    iobuf_add(&io, 3, "hello", strlen("hello"));
    iobuf_dump(&io, 64);
    ASSERT(io.buf != NULL && io.cap == max(32, IOBUF_CHUNK_SIZE) && io.len == 8);
    ASSERT(memcmp(io.buf, "\x00\x00\x00hello", 8) == 0);

    iobuf_resize(&io, 64);
    iobuf_dump(&io, 64);
    ASSERT(io.buf != NULL && io.cap == max(64, IOBUF_CHUNK_SIZE) && io.len == 8);
    ASSERT(memcmp(io.buf, "\x00\x00\x00hello", 8) == 0);

    iobuf_add(&io, 16, "bye", strlen("bye"));
    iobuf_dump(&io, 64);
    ASSERT(io.buf != NULL && io.cap == max(64, IOBUF_CHUNK_SIZE) && io.len == 19);
    ASSERT(memcmp(io.buf, "\x00\x00\x00hello\x00\x00\x00\x00\x00\x00\x00\x00\x62\x79\x65", 19) == 0);

    iobuf_del(&io, 0, 3);
    iobuf_dump(&io, 64);
    ASSERT(io.buf != NULL && io.cap == max(64, IOBUF_CHUNK_SIZE) && io.len == 16);
    ASSERT(memcmp(io.buf, "hello\x00\x00\x00\x00\x00\x00\x00\x00\x62\x79\x65", 16) == 0);

    iobuf_del(&io, 10, 100);
    iobuf_dump(&io, 64);
    ASSERT(io.buf != NULL && io.cap == max(64, IOBUF_CHUNK_SIZE) && io.len == 10);
    ASSERT(memcmp(io.buf, "hello\x00\x00\x00\x00\x00", 10) == 0);

    iobuf_del(&io, 0, 100);
    iobuf_dump(&io, 64);
    ASSERT(io.buf != NULL && io.cap == max(64, IOBUF_CHUNK_SIZE) && io.len == 0);
    ASSERT(memcmp(io.buf, "\x00", 1) == 0);

    iobuf_free(&io);
    ASSERT(io.buf == NULL && io.cap == 0 && io.len == 0);

    printf("PASS\n");
}