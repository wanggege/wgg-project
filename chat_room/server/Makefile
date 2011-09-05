src = $(wildcard *.c)
obj = $(patsubst %.c,%.o,$(src))
CC = gcc
CFLAGS = -Wall -g

server:$(obj)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) $(FLAGS) $< -c

.PHONY:clean

clean:
	rm -rf *.o
	rm -rf server
