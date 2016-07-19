
AR ?= ar
CC ?= gcc
PREFIX ?= /usr/local

CFLAGS = -O3 -std=c99 -Wall -Wextra -Ideps

SRCS = src/dl_list.c \
		   src/dl_list_node.c \
		   src/dl_list_iterator.c

OBJS = $(SRCS:.c=.o)

all: build/liblist.a

install: all
	cp -f build/liblist.a $(PREFIX)/lib/liblist.a
	cp -f src/dl_list.h $(PREFIX)/include/dl_list.h

uninstall:
	rm -f $(PREFIX)/lib/liblist.a
	rm -f $(PREFIX)/include/dl_list.h

build/liblist.a: $(OBJS)
	@mkdir -p build
	$(AR) rcs $@ $^

bin/test: test.o $(OBJS)
	@mkdir -p bin
	$(CC) $^ -o $@

bin/benchmark: benchmark.o $(OBJS)
	@mkdir -p bin
	$(CC) $^ -o $@

%.o: %.c
	$(CC) $< $(CFLAGS) -c -o $@

clean:
	rm -fr bin build *.o src/*.o

test: bin/test
	@./$<

benchmark: bin/benchmark
	@./$<

.PHONY: test benchmark clean install uninstall
