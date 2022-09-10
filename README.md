# iobuf

A simple io buffer.

# Build
```Bash
$ make              # build release version
$ make ver=debug    # build debug version
```

# Usage
```C
int iobuf_init(struct iobuf *io, size_t size);
int iobuf_resize(struct iobuf *io, size_t new_size);
void iobuf_dump(struct iobuf *io, size_t len);
size_t iobuf_add(struct iobuf *io, size_t offset, const void *buf, size_t len);
size_t iobuf_del(struct iobuf *io, size_t offset, size_t len);
void iobuf_free(struct iobuf *io);
```