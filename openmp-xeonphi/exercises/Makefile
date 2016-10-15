# DON'T FORGET TO: source /opt/intel/bin/compilervars.sh intel64

all: sum-seq-offload.exec matrix-mul.exec sum-seq-offload-implicit.exec graph.exec
	@echo "Compiled everything! Yes!"

sum-seq-offload.exec: sum-seq-offload.c
	icc -o sum-seq-offload.exec sum-seq-offload.c -no-vec -unroll0 -qopt-report2 -qopenmp

matrix-mul.exec: matrix-mul.c
	icc -o matrix-mul.exec matrix-mul.c -no-vec -unroll0 -qopt-report2 -qopenmp

sum-seq-offload-implicit.exec: sum-seq-offload-implicit.c
	icc -o sum-seq-offload-implicit.exec sum-seq-offload-implicit.c -no-vec -unroll0 -qopt-report2 -qopenmp

graph.exec: graph.c graph.h
	icc -o graph.exec graph.c -no-vec -unroll0 -qopenmp

clean:
	- rm *.exec *.optrpt
