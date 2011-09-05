#client:my_client.o wrap.o analyse_message.o land_user.o printf_s.o
#	gcc -o client my_client.o wrap.o analyse_message.o land_user.o printf_s.c
#my_client.o:my_client.c wrap.h analyse_message.h
#	gcc -c my_client.c
#wrap.o:wrap.c
#	gcc -c wrap.c
#analyse_message.o: analyse_message.c analyse_message.h
#	gcc -c analyse_message.c
#land_user.o: land_user.c analyse_message.h
#	gcc -c land_user.c
#printf_s.o:printf_s.c
#	gcc -c printf_s.c
src = $(wildcard *.c) 
obj = $(patsubst %.c,%.o,$(src))
CC = gcc
CFLAGS = -Wall -g

client:$(obj)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) $(CFLAGS) $< -c

.PHONY:clean

clean:
	rm -rf *.o
	rm -rf client

