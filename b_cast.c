#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
//#define N 10000
main(int argc, char *argv[]){
	int num_procs;
	int my_proc;
	int init, size, rank,final;
	int i,l;
  int N[3]={10000,100000,250000};
  //int N[3]={3,3,3};
	float buf[250000];
  double start,end,run_time;
	MPI_Status recv_status;

	/* Initializing MPI */
	if ((init = MPI_Init(&argc, &argv)) != MPI_SUCCESS) {
		printf("Error in init\n");
		exit(-2); 
	}

	/* Determining the size of the communicator */
	if ((size = MPI_Comm_size(MPI_COMM_WORLD, &num_procs))!= MPI_SUCCESS){
		printf("Error in size\n");
		exit(2);
	}

	/* Determining process number */
	if ((rank =MPI_Comm_rank(MPI_COMM_WORLD, &my_proc))!= MPI_SUCCESS) {
		printf("Error in rank\n");
		exit(1);
	}
//looping for different vector length
  
  for( l = 0; l <3 ; l++ ){
  
    if (my_proc == 0){
	     for (i = 0; i < N[l]; i++)
	       	  buf[i] = i;
    }

	 start = MPI_Wtime();
   /*broadcasting*/
   MPI_Bcast(buf,N[l],MPI_FLOAT,0,MPI_COMM_WORLD);  
	  if (my_proc == 0){
     end = MPI_Wtime();
     run_time = end-start;
     printf("\nbroadcasting a vector of %d floating point entries over %d nodes Runtime= %.16g",N[l],num_procs,run_time);
  }     	
}
   
  
  
 

	/* Terminate MPI */
	if ((final = MPI_Finalize()) != MPI_SUCCESS) {
		printf("Error in finalizing \n");
		exit(1);
	}
 

}