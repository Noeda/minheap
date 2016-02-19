CC := gcc
CFLAGS := -Wall
RM := rm -f

.PHONY: release debug clean

release: CFLAGS += -O2
release: libheap.so

debug: CFLAGS += -Og -g
debug: libheap.so

libheap.so: heap.c heap.h
	$(CC) $(CFLAGS) -shared -fPIC heap.c -o libheap.so

clean:
	$(RM) libheap.so


