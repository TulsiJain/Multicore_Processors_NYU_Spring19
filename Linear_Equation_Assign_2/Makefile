# n represents the nunber of processes
# e represents the nunber of unknowns
n=1
e=1000

default: cc

.ONESHELL:
compile cc:
	mpicc -std=c99 -o solve gs.c -lm
	time mpirun -n $(n) solve input/$(e).txt
