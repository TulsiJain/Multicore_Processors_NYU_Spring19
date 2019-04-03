#include<limits.h> 
#include <stdlib.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>

void swap( int* first, int* second) { 
	int temp =  *first;
	*first = *second;
	*second = temp;
}

void check_cost( int *permute_array, int city_count, int* min_distance, int* min_path, int distance_matrix[city_count][city_count], int cost, int starting_point)  { 
	int costTotal = cost;
  	for ( int i = 0 ; i < city_count - 2; i++ ){
		if ( i == 0 ){
			costTotal = costTotal + distance_matrix[starting_point][*(permute_array)];
		} else{
			costTotal = costTotal + distance_matrix[*(permute_array+i-1)][*(permute_array+i)];
		}
		if (costTotal >= *min_distance){
			return;
		}
  	}
  	# pragma omp critical
  	{
	  	*min_distance =  costTotal;
	  	*(min_path+0) = starting_point;
	  	for ( int i =0; i < city_count - 2; i++){
	  		*(min_path+i+1) = *(permute_array+i);
	  	}
	}
}


void permutation(int *permute_array, int length, int starting_point, int* min_distance, int* min_path, int city_count,  int distance_matrix[city_count][city_count] ) {
	int n = length;
	// Weight index control array initialized by 0
	int p[n];
	for (int i = 0; i < n; i++)
	{
		p[i] = 0;
	}

	// i, j represents upper and lower bound index respectively for swapping
	int i = 1, j = 0;

	check_cost(permute_array, city_count, min_distance, min_path, distance_matrix, distance_matrix[0][starting_point], starting_point);
	// printf("\n");
	while (i < n)
	{
		// printf("hello %d %d\n", p[i], i);
		if (p[i] < i)
		{
			// if i is odd then j = p[i], otherwise j = 0
			j = (i % 2) * p[i];

			swap(permute_array+j, permute_array+i);
			check_cost(permute_array, city_count, min_distance, min_path, distance_matrix, distance_matrix[0][starting_point], starting_point);

			p[i]++; // increase index "weight" for i by one
			i = 1;  // reset index i to 1
		}
		// otherwise p[i] == i
		else
		{
			// reset p[i] to zero
			p[i] = 0;

			// set new index value for i (increase by one)
			i++;
		}
	}

}

int main(int argc, char *args[]){
	int city_count = atoi(args[1]);
	int min_path[city_count -1];
	int thread_count = atoi(args[2]);
	int distance_matrix[city_count][city_count];
	int min_distance = INT_MAX; 

	FILE *file;
	file = fopen(args[3], "r");
	if (file) {
		int i =0;
		int k =0;
		while (!feof (file)){
			fscanf (file, "%d", &k); 
			distance_matrix[i/city_count][i%city_count] = k;
			i++;
		}
		fclose(file);
	}else{
		printf("%s\n", "Error occured, can not open file");
	}

	omp_set_dynamic(1);
	#pragma omp parallel for num_threads(thread_count) 
	for (int j = 0; j < city_count - 1; j++) { 
		int permute_array[city_count -1];
		for(int i = 0; i < city_count - 1; i++) {
			permute_array[i] = i+1;
		}
		swap(permute_array, permute_array+j);
		permutation(permute_array+1, city_count - 2, j+1, &min_distance, min_path, city_count, distance_matrix);
	}

	#pragma omp barrier
	{
		printf("Best path: %d ", 0);
		for(int i = 0; i < city_count - 1; i++) {
	 	 	printf("%d ", *(min_path+i));
		}
		printf("\n");
		printf("Distance: %d\n", min_distance);
		return 0;
	}
	
}