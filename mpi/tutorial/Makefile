CC=mpicc

all: hello-world send-recv scatter gather bcast
	@echo "Compiled! Yes!"

hello-world: hello-world.c
	$(CC) -o hello-world hello-world.c

send-recv: send-recv.c
	$(CC) -o send-recv send-recv.c

scatter: scatter.c
	$(CC) -o scatter scatter.c

gather: gather.c
	$(CC) -o gather gather.c

bcast: bcast.c
	$(CC) -o bcast bcast.c

clean:
	- rm -f hello-world send-recv scatter gather bcast
