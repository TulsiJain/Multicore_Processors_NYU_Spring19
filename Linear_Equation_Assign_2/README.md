# Steps to run on

1. make e=number_of_unknowns n=number_of_prccesses

## Table 1
Table shown below is about time taken for the problem of size 10, 100, 1000 and 10000 unknowns with varied number of processes. Time mentioned is real time obtained by time command. *<small> averaged over (~5) times. <small>


*Time indicated in s

| Unknowns/Processes   |      1      |  2 |   10 |   20 | 40 |
|----------|:-------------:|------:|------:| ------:|------:|
| 10   | 0.3762 | 0.3826 | 0.4924 |    -   | 	-  |
| 100  | 0.3792 |  0.384 | 0.4734 | 0.5922 | 0.9522|
| 1000 | 0.7164	| 0.577  | 0.5594 |	0.6818 | 1.1468|
| 10000|37.3842	| 21.561 |	5.709 | 3.616  | 3.254 |


## Table 2
Table shown below is about speedup for the problem of size 10, 100, 1000 and 10000 unknowns with varied number of processes. Speedup is calculated over real time line obtained by time command. *<small> averaged over (~5) times. <small>


*Speedup

| Unknowns/Processes  |   1  |  2 |   10 |   20 | 40 |
|----------|:-------------:|------:|------:| ------:|------:|
| 10   | 1 | 0.983 | 0.764 |    -   | 	-  |
| 100  | 1 |  0.987 | 0.801 | 0.64 | 0.398|
| 1000 | 1	| 1.241  | 1.28 |	1.05 | 0.624|
| 10000| 1	| 1.733 |	6.548 | 10.338  | 11.488  |