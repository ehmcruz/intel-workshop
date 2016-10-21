# DON'T FORGET TO: source /opt/intel/bin/compilervars.sh intel64

all: vector-reduce-sum-mul-sequential.exec matrix-mul.exec
	@echo "Compiled everything! Yes!"

vector-reduce-sum-mul-sequential.exec: vector-reduce-sum-mul-sequential.c
	icc -o vector-reduce-sum-mul-sequential.exec vector-reduce-sum-mul-sequential.c -no-vec -unroll0 -qopt-report2 -qopenmp -xAVX

matrix-mul.exec: matrix-mul.c
	icc -o matrix-mul.exec matrix-mul.c -no-vec -unroll0 -qopt-report2 -qopenmp -xAVX

clean:
	- rm *.exec *.optrpt
