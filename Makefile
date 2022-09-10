ver = release

ifeq ($(ver), release)
CFLAGS += -g -O2  -Wall -Werror -Wno-unused
else
CFLAGS += -g -O2  -Wall -Werror -Wno-unused -DDEBUG
endif

all: test

test: test.c iobuf.c iobuf.h
	gcc $^ -o $@ $(CFLAGS)

clean:
	rm -f ./test