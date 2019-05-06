#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <string.h>
#include <stdbool.h>
/*** Skeleton for Lab 1 ***/

/***** Globals ******/
float **a; /* The coefficients */
float *x;  /* The unknowns */
float *y;  /* The unknowns */
float *b;  /* The constants */
float err; /* The absolute relative error */
int num = 0;  /* number of unknowns */
int comm_size;
int my_rank;
int work_per_process;
int non_divisible_work;
int rank_zero_work;


/****** Function declarations */
// void check_matrix(); /* Check whether the matrix will converge */
void get_input();  /* Read input from file */

/********************************/

/* Function definitions: functions are ordered alphabetically ****/
/*****************************************************************/

/* 
   Conditions for convergence (diagonal dominance):
   1. diagonal element >= sum of all other elements of the row
   2. At least one diagonal element > sum of all other elements of the row
 */
// void check_matrix()
// {
//   int bigger = 0; /* Set to 1 if at least one diag element > sum  */
//   int i, j;
//   float sum = 0;
//   float aii = 0;
  
//   for(i = 0; i < num; i++)
//   {
//     sum = 0;
//     aii = fabs(a[i][i]);
	
//     for(j = 0; j < num; j++)
//        if( j != i)
//    sum += fabs(a[i][j]);
	   
//     if( aii < sum)
//     {
//       printf("The matrix will not converge.\n");
//       exit(1);
//     }
	
//     if(aii > sum)
//       bigger++;
	
//   }
  
//   if( !bigger )
//   {
//      printf("The matrix will not converge\n");
//      exit(1);
//   }
// }


/******************************************************/
/* Read input from file */
/* After this function returns:
 * a[][] will be filled with coefficients and you can access them using a[i][j] for element (i,j)
 * x[] will contain the initial values of x
 * b[] will contain the constants (i.e. the right-hand-side of the equations
 * num will have number of variables
 * err will have the absolute error that you need to reach
 */

void get_input(char filename[])
{
	FILE * fp;
	int i,j;  
 
	fp = fopen(filename, "r");
  
	if(!fp)
	{
		printf("Cannot open file %s\n", filename);
		exit(1);
	}

	fscanf(fp,"%d ",&num);
	fscanf(fp,"%f ",&err);

	work_per_process = num/comm_size;
	non_divisible_work = num - comm_size*work_per_process;
	rank_zero_work = non_divisible_work + work_per_process;

	/* Now, time to allocate the matrices and vectors */
	if (my_rank ==0){
		a = (float**)malloc(rank_zero_work * sizeof(float*));
		if(!a)
		{
			printf("Cannot allocate a!\n");
			exit(1);
		}

		for(i = 0; i < rank_zero_work; i++) 
		{
			a[i] = (float *)malloc(num * sizeof(float)); 
			if( !a[i])
			{
				printf("Cannot allocate a[%d]!\n",i);
				exit(1);
			}
		}
	}else{
		a = (float**)malloc(work_per_process * sizeof(float*));
		if(!a)
		{
			printf("Cannot allocate a!\n");
			exit(1);
		}
		for(i = 0; i < work_per_process; i++) 
		{
			a[i] = (float *)malloc(num * sizeof(float)); 
			if( !a[i])
			{
				printf("Cannot allocate a[%d]!\n",i);
				exit(1);
			}
		}
	}
	x = (float *) malloc(num * sizeof(float));
	y = (float *) malloc(num * sizeof(float));
	if( !x)
	{
		printf("Cannot allocate x!\n");
		exit(1);
	}
  
	for(i = 0; i < num; i++)
	{
		fscanf(fp,"%f ", &x[i]);
	}

	if (my_rank > 0){
		char line[400000];
		int km = 0;
		while (km < (my_rank -1)*work_per_process + rank_zero_work) {
			fgets(line, sizeof(line), fp); 
			km++;
		}
	}

  // fseek(fp, start_process, SEEK_SET);
	if (my_rank ==0){
		b = (float *) malloc(rank_zero_work * sizeof(float));
	}else{
		b = (float *) malloc(work_per_process * sizeof(float));
	}
	if(!b)
	{
		printf("Cannot allocate b!\n");
		exit(1);
	}

	if (my_rank == 0){
		for(i = 0; i < rank_zero_work; i++)
		{
			for(j = 0; j < num; j++)
			{
				fscanf(fp,"%f ",&a[i][j]);
			}
	   		/* reading the b element */
	   		fscanf(fp,"%f ",&b[i]);
		}
	}else{
		for(i = 0; i < work_per_process; i++)
		{
			for(j = 0; j < num; j++)
			{
				fscanf(fp,"%f ",&a[i][j]);
			}
			/* reading the b element */
			fscanf(fp,"%f ",&b[i]);
		}
  	}
 	fclose(fp); 
}

void solve_equation(int starting, int ending){
  for (int i = starting; i < ending; i++){
	float temp_number = b[i - starting];
	for (int j = 0; j < num; j++){
		if ( i != j){
		  temp_number = temp_number - x[j]*a[i - starting][j];
		}
	}
	x[i] = temp_number/a[i - starting][i];
  }
}

/************************************************************/
int main(int argc, char *argv[])
{
	int nit = 0; /* number of iterations */
	FILE * fp;
	char output[100] ="";
  
	if( argc != 2)
	{
		printf("Usage: ./gsref filename\n");
		exit(1);
	}

	MPI_Init(NULL , NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	get_input(argv[1]);

	int *sendcounts = malloc(sizeof(int)*comm_size);
	int *displs = malloc(sizeof(int)*comm_size);
	int sum = 0;
   	for (int i = 0; i < comm_size; i++) {
	  	if (i ==0){
			sendcounts[i] = rank_zero_work;
	  	}else{
			sendcounts[i] = work_per_process;
	  	}
	  	displs[i] = sum;
	  	sum += sendcounts[i];
  	}

  	while (1 == 1)
  	{
		for ( int k = 0; k < num; k++)
		{
	  		y[k] = x[k];
		}

		if (my_rank == 0){
		  	solve_equation(0, rank_zero_work);
		}else{
		  	int starting_non_zero  = rank_zero_work + work_per_process*(my_rank - 1);
		  	solve_equation(rank_zero_work + work_per_process*(my_rank - 1), starting_non_zero + work_per_process);
		}

		MPI_Allgatherv(MPI_IN_PLACE, 0, MPI_FLOAT, x, sendcounts, displs, MPI_FLOAT, MPI_COMM_WORLD);
		nit++;

		bool iteration_again = false;
		for (int l = 0; l < num; l++)
		{
			float error = abs(((x[l] - y[l])*100)/x[l]);
			if (error >= err*100){
			  	iteration_again = true;
			  	break;
			}
		}
		if (iteration_again){
		  	continue;
		}
		break;
	}

 	/* Only process with rank zero is writing results to file */
	if (my_rank == 0){
  		sprintf(output,"%d.sol_txt",num);
  		fp = fopen(output,"w");
  		if(!fp)
  		{
			printf("Cannot create the file %s\n", output);
			exit(1);
  		}
	  
		for( int i = 0; i < num; i++)
		{
	 		fprintf(fp,"%f\n",x[i]);
		}
  		printf("total number of iterations: %d\n", nit); 
  		fclose(fp);
 	}
 	MPI_Finalize();
 	// my code ends here
 	exit(0);
}
