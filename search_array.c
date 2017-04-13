#include <stdio.h>
#include <stdlib.h>
#include <mpi.h> 
#define N 10000
main(int argc, char *argv[]){
	int num_procs;
	int my_proc;
	int init, size, rank, send,recv,final,read_data,gloc;
	int i, j,k,l, flag = 1;
  int sbuf[40000],rbuf[10000];
  MPI_Status recv_status;
  char line[80];
  FILE *fr;

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
  
  
    if (my_proc == 0){
	     //reading file content
           fr = fopen ("data.txt", "rt");
           l=0;
           while(fgets(line, 80, fr) != NULL) {
                    sscanf (line, "%d", &read_data);
                    sbuf[l]=read_data; 
                    l++; 
            }
            fclose(fr);            
    }	 
   
	 if (my_proc == 0) {
      
             for( i=0;i<10000;i++){
                   rbuf[i]=sbuf[i];
             }
               /*distributing one fourth to each of the other processors*/
             if ((send = MPI_Send(sbuf+10000, 10000, MPI_FLOAT, 1,99, MPI_COMM_WORLD))!= MPI_SUCCESS) {			        
			        exit(1); 
		         } 
             if ((send = MPI_Send(sbuf+20000, 10000, MPI_FLOAT, 2,99, MPI_COMM_WORLD))!= MPI_SUCCESS) {			        
			        exit(1); 
		         } 
             if ((send = MPI_Send(sbuf+30000, 10000, MPI_FLOAT, 3,99, MPI_COMM_WORLD))!= MPI_SUCCESS) {			      
			        exit(1); 
		         }           
      }
  
   
  else
  {
  if ((recv =MPI_Recv(rbuf, 10000, MPI_FLOAT, 0,99, MPI_COMM_WORLD, &recv_status)) != MPI_SUCCESS){
			exit(1); 
		}
   
  } 
  

  for( i=0;i<10000;i++){
    
    MPI_Iprobe(MPI_ANY_SOURCE, 99, MPI_COMM_WORLD, &flag,&recv_status);
    if(flag==1)
    {
       printf("\nProcess %d has stopped at %d\n",my_proc,i);
       break;
    }
    if(rbuf[i]==11)
    {
      gloc = my_proc*10000+i;
      for( k=0;k<num_procs;k++)
              if(k!=my_proc)
                            MPI_Send(&gloc, 1, MPI_FLOAT, k,99, MPI_COMM_WORLD);
     
     printf("\nFound on process %d : local index %d  global index = %d \n",my_proc, i, gloc);
     break;
    }
  }
  
if(i==10000)
{
 printf("\nProcess %d has stopped at %d\n",my_proc,i);
}

	/* Terminate MPI */
	if ((final = MPI_Finalize()) != MPI_SUCCESS) {
		printf("Error in finalizing \n");
		exit(1);
	}
 

}