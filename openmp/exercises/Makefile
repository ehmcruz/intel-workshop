all: selection-sort.exec task-test.exec fibonacci.exec vector-reduce-sum-split.exec
	@echo "Compiled everything! Yes!"

selection-sort.exec: selection-sort.c
	gcc -o selection-sort.exec selection-sort.c -fopenmp

task-test.exec: task-test.c
	gcc -o task-test.exec task-test.c -fopenmp

fibonacci.exec: fibonacci.c
	gcc -o fibonacci.exec fibonacci.c -O2 -fopenmp

vector-reduce-sum-split.exec: vector-reduce-sum-split.c
	gcc -o vector-reduce-sum-split.exec vector-reduce-sum-split.c -fopenmp

clean:
	- rm *.exec
