#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#include<math.h>

main(int argc, char *argv[]){
	int num_procs;
	int my_proc;
	int init, size, rank, send, recv, final,send_cal,mylevel,depth;
	int i, j,k,l, other_proc, flag = 1;
  int N[3]={10000,100000,250000};
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

  for( l = 0; l < 3; l++ ){
  
    if (my_proc == 0){
	     for (i = 0; i < N[l]; i++)
	       	  buf[i] = i;
    }	
   
	 if (my_proc == 0) {
      	 start = MPI_Wtime();
     		 for(k = 0; k < (log(num_procs)/log(2)); k++){
               send_cal = pow(2,k); //calculating node number to send data
               //printf("\nprocess %d is sending to %d\n",my_proc,send_cal);
           		 if ((send = MPI_Send(buf, N[l], MPI_FLOAT, send_cal,99, MPI_COMM_WORLD))!= MPI_SUCCESS) {
			             exit(1); 
		          }           
      	}
   }
  else 
  {
     if ((recv =MPI_Recv(buf, N[l], MPI_FLOAT, MPI_ANY_SOURCE,99, MPI_COMM_WORLD, &recv_status)) != MPI_SUCCESS){
			      exit(1); 
		    }
            depth=(log(num_procs)/log(2)); // calculating depth of binary tree
            mylevel=log(my_proc)/log(2);   //calculating level of current node in binary tree
            
      	 for(k = mylevel+1; k < depth; k++){
               send_cal=my_proc+pow(2,k); //calculating node number to send data
               //printf("\nprocess %d is sending to %d\n",my_proc,send_cal);
           		 if ((send = MPI_Send(buf, N[l], MPI_FLOAT, send_cal,99, MPI_COMM_WORLD))!= MPI_SUCCESS) {
			             exit(1); 
		          }           
      	}      
            
  }
    
  
	MPI_Barrier(MPI_COMM_WORLD);//waiting for all to finish
  if (my_proc == 0){
     end = MPI_Wtime();
     run_time = end-start;
     printf("\nbroadcasting a vector of %d floating point entries over %d nodes Runtime= %.16g\n",N[l],num_procs,run_time);
  }
  
 }

	/* Terminate MPI */
	if ((final = MPI_Finalize()) != MPI_SUCCESS) {
		printf("Error in finalizing \n");
		exit(1);
	}
 

}