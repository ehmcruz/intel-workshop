CC = icc
CFLAGS = -no-vec -unroll0 -qopt-report2 -qopenmp -mmic

# DON'T FORGET TO: source /opt/intel/bin/compilervars.sh intel64
# export OFFLOAD_REPORT=2

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
