CC = gcc
CFLAGS  = -g -Wall
all:
	$(CC) $(CFLAGS) nodeA.c common.c -o nodeA
	$(CC) $(CFLAGS) nodeB.c common.c -o nodeB
	$(CC) $(CFLAGS) nodeC.c common.c -o nodeC

clean:
	rm -f nodeA nodeB nodeC
