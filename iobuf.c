#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "iobuf.h"

#ifdef DEBUG
#define DBG(fmt, args...) do { fprintf(stderr, fmt,  ##args); } while(0)
#else
#define DBG(fmt, args...)   do {  } while(0)
#endif

#define min(x, y) (((x) < (y)) ? (x) : (y))
#define max(x, y) (((x) > (y)) ? (x) : (y))

static void memzero(volatile unsigned char *buf, size_t len)
{
    if (buf != NULL) {
        while(len--)
            *buf++ = 0;
    }
}

int iobuf_resize(struct iobuf *io, size_t new_size)
{
    int ok = 1;
    if (new_size % IOBUF_CHUNK_SIZE)
        new_size = (new_size / IOBUF_CHUNK_SIZE + 1) * IOBUF_CHUNK_SIZE;
    if (new_size == 0) {
        memzero(io->buf, io->cap);
        free(io->buf);
        io->buf = NULL;
        io->len = io->cap = 0;
    } else if (new_size != io->cap) {
        DBG("memory realloc: %ld ---> %ld\n", io->cap, new_size);
        void *p = realloc(io->buf, new_size);
        if (p) {
            size_t len = min(new_size, io->len);
            if(len > 0)
                memmove(p, io->buf, len);
            io->buf = (unsigned char *)p;
            io->cap = new_size;
        } else {
            ok = 0;
        }
    }
    iobuf_dump(io, io->cap);
    return ok;
}

int iobuf_init(struct iobuf *io, size_t size)
{
    return iobuf_resize(io, size);
}

void iobuf_free(struct iobuf *io)
{
    iobuf_resize(io, 0);
}

void iobuf_dump(struct iobuf *io, size_t len)
{
    size_t i;

    if (len > io->cap)
        len = io->cap;

    for (i=0; i<len; i++) {
        DBG("%02x ", io->buf[i]);
        if (!((i+1) % 16))
            DBG("\n");
    }
    // printf("|");
    // for (i=0; i<len; i++)
    //     printf("%c ", io->buf[i]);

    DBG("memory status: len(%ld) cap(%ld)\n", io->len, io->cap);
}

size_t iobuf_add(struct iobuf *io, size_t offset, const void *buf,
        size_t len)
{
    size_t new_cap = max((io->len + len), (offset + len));
    if (new_cap > io->cap) {
        new_cap += IOBUF_CHUNK_SIZE;
        new_cap -= new_cap % IOBUF_CHUNK_SIZE;
        if (!iobuf_resize(io, new_cap))
            return 0;
    }
    if (offset < io->len)
        memmove(io->buf + offset + len, io->buf + offset, io->len - offset);
    if (buf != NULL)
        memmove(io->buf + offset, buf, len);
    if (offset > io->len)
        io->len += offset - io->len;
    io->len += len;
    return len;
}