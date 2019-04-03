# p represents the nunber of cities
p=6

#t represents the number of threads
t=1

default: cc

.ONESHELL:
cc compile:
	gcc -std=c99 -fopenmp -o ptsm ptsm.c
	time ./ptsm $(p) $(t) cities$(p).txt