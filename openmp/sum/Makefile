CC = gcc
CFLAGS = -fopenmp

OBJSUM := $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJSUMF := $(patsubst %.c,%.exec,$(wildcard src/*.c))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: main.o sum
	@echo "Everything compiled! Enjoy!"

%.exec: %.o
	$(CC) $(CFLAGS) -o $@ $< main.o

sum: $(OBJSUMF)
	@echo "sum sample compiled!"

clean:
	- rm -rf main.o $(OBJSUM) $(OBJSUMF)
