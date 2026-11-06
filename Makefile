CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11
CPPFLAGS = $(shell pkg-config --cflags raylib)
LDLIBS = $(shell pkg-config --libs raylib) -lm

TARGET = breakout

.PHONY: all build clean

all: build

main.o: src/main.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/main.c -o main.o

file.o: src/file_operations.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/file_operations.c -o file.o

build: main.o file.o
	$(CC) main.o file.o -o $(TARGET) $(LDLIBS)

clean:
	rm -f *.o $(TARGET)
