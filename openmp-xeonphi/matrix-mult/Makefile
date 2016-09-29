CC = icc
CFLAGS = -no-vec -qopt-report2 -qopenmp -xAVX
#-xAVX
#-axAVX

OBJSUM := $(patsubst %.c,%.o,$(wildcard src/*.c))
OBJSUMF := $(patsubst %.c,%.exec,$(wildcard src/*.c))

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: main.o sum
	@echo "Everything compiled! Enjoy!"

%.exec: %.o
	$(CC) $(CFLAGS) -o $@ $< main.o
	objdump -D $@ > $@.txt

sum: $(OBJSUMF)
	@echo "sum sample compiled!"

clean:
	- rm -rf main.o $(OBJSUM) $(OBJSUMF)
