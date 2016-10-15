# DON'T FORGET TO: source /opt/intel/bin/compilervars.sh intel64

all: sum-seq-offload.exec
	@echo "Compiled everything! Yes!"

sum-seq-offload.exec: sum-seq-offload.c
	icc -o sum-seq-offload.exec sum-seq-offload.c -no-vec -unroll0 -qopt-report2 -qopenmp

clean:
	- rm *.exec *.optrpt
