CC = gcc
CFLAGS = -I include -Wall -Werror

src = $(wildcard src/*.c)
obj = $(patsubst src/%.c, build/%.o, $(src))
headers = $(wildcard include/*.h)

hip: $(obj)
	$(CC) $(CFLAGS) $(obj) -o hip

build/%.o: src/%.c ${headers}
	$(CC) $(CFLAGS) -c $< -o $@

all: hip install

install:
	mkdir -p bin && mv hip bin/

clean:
	rm -r build/*.o bin/
