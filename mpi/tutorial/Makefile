CC=mpicc

all: hello-world send-recv
	@echo "Compiled! Yes!"

hello-world: hello-world.c
	$(CC) -o hello-world hello-world.c

send-recv: send-recv.c
	$(CC) -o send-recv send-recv.c

clean:
	- rm -f hello-world send-recv
