#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>

int to1D( int x, int y, int z ) {
    return (z * 256 * 256) + (y * 256) + x;
}


int main () {
	MPI_Init(NULL, NULL);
	//Init Variable
	int size=0,rank=0,arraySize=16777216,m=0,n=0,j=0,i=0;
	int* warna;
	int* par;
	
	/* Get the number of processes */
	MPI_Comm_size(MPI_COMM_WORLD, &size); 

	/* Get my rank among all the processes */
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	par = (int*)malloc((size*arraySize)*sizeof(int));
	memset(par,0,(size*arraySize)*sizeof(int));		
	
	FILE * fd = fopen("rgbdata-temp","r");
		
	fscanf(fd, "%d %d", &m, &n);
		
	warna = (int *) malloc ((m * n) * sizeof(int));
	memset(warna,0,sizeof(int)*(m*n));
		
	if(rank==0){
		
		for(i=0; i < ((m * n)/3); i++){
			fscanf(fd, "%d %d %d", &(warna[3 * i]), &(warna[3 * i + 1]), &(	warna[3 * i + 2] ));
			//printf("%d %d %d",(warna[3 * i]), (warna[3 * i + 1]), (	warna[3 * i + 2] ));
			
		}
		fclose(fd);
	}
	
	int* warnaLocal=(int*)malloc((m * n)/size * sizeof(int));
	int* parLocal=(int*)malloc(arraySize*sizeof(int));

	MPI_Scatter(par, arraySize, MPI_INT, parLocal, arraySize, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(warna, ((m*n))/size, MPI_INT, warnaLocal, ((m*n))/size, MPI_INT, 0, MPI_COMM_WORLD);

	for(i=0; i < (((m * n)/size)/3); i++){
			parLocal[to1D(warnaLocal[3 * i],warnaLocal[(3 * i) + 1],warnaLocal[(3 * i) + 2])]+=1;
			printf("2. Processor %d has data: %d %d %d => %d\n", rank, warnaLocal[3 * i],warnaLocal[3 * i + 1], warnaLocal[3 * i + 2], parLocal[to1D(warnaLocal[3 * i],warnaLocal[(3 * i) + 1],warnaLocal[(3 * i) + 2])]);
	}

	MPI_Gather(parLocal, arraySize, MPI_INT, par, arraySize, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank==0){
		for(j=1;j<size;j++){
			for(i=0; i< (m*n)/3;i++){
				par[to1D(warna[3 * i],warna[(3 * i) + 1],warna[(3 * i) + 2])]+=par[(arraySize*j)+to1D(warna[3 * i],warna[(3 * i) + 1],warna[(3 * i) + 2])];
				par[(arraySize*j)+to1D(warna[3 * i],warna[(3 * i) + 1],warna[(3 * i) + 2])]=0;
			}
		}
		for(i=0; i< (m*n)/3;i++){
			printf("4. Processor %d has data %d %d %d => %d \n",rank,warna[3 * i], warna[3 * i + 1], warna[3 * i + 2],par[to1D(warna[3 * i],warna[(3 * i) + 1],warna[(3 * i) + 2])]);
		}
		
		for(i=0; i< (m*n)/3;i++){
			if(par[to1D(warna[3 * i],warna[(3 * i) + 1],warna[(3 * i) + 2])]>0){
				printf("%d %d %d |",warna[3 * i], warna[3 * i + 1], warna[3 * i + 2]);
				for(j=0;j<par[to1D(warna[3 * i],warna[(3 * i) + 1],warna[(3 * i) + 2])];j++){
					printf("#");
				}
				par[to1D(warna[3 * i],warna[(3 * i) + 1],warna[(3 * i) + 2])]=0;
				printf("\n");
			}
		}
		
	}
	MPI_Finalize();
	return EXIT_SUCCESS;
}
