CC = gcc
CFLAGS  = -g -Wall

all:
	$(CC) $(CFLAGS) common.c receiver.c -o receiver
	$(CC) $(CFLAGS) common.c sender.c -o sender

clean:
	rm -f sender receiver
