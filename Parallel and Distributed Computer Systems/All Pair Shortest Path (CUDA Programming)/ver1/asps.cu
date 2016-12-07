#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <cuda.h>

#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define RESET "\033[0m"

#define maxThreadsPerBlock 64 //8*8

void makeAdjacency();
void floydWarshallMethod();
void methods(int f);
void test(float *A_dev);
void printArray(float *array);

__global__ void kernel1(int k, int n, float *A);
__global__ void kernel2(int k, int n, float *A);
__global__ void kernel3(int k, int n, float *A, int cellsPerThread);

int n, w;		
float p;
float *D; 		/* D:distance (for the Floyd Warshall algorithm) */
float *A; 		/* A:array (for methods 1, 2, 3 as mentioned in the exercise) */
float *A_result1, *A_result2, *A_result3;	/* arrays for saving results in methods 1, 2, 3 */

float seq_time;
struct timeval startwtime, endwtime;
	cudaEvent_t start, stop;

//--------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv)
{

	if (argc!=4){
		printf("Enter: ./<name> n p w\n");
		exit(3);
	}

	n = 1<<atoi(argv[1]);
	p = atof(argv[2]);
	w = atoi(argv[3]);
    
    time_t t;
    srand((unsigned)time(&t));

    D = (float *)malloc(n*n*sizeof(float)); 
    A = (float *)malloc(n*n*sizeof(float)); 

	printf("n = %d \np = %.2f \nw = %d\n", n, p, w);
	

	makeAdjacency(); 	/* initialize the graph */
	printArray(A);

	floydWarshallMethod();
	printf("Floyd Warshall time:\t%f\n", seq_time);
	printArray(D);

	methods(1);
	printf("CUDA Method 1 time:\t%f -> ", seq_time);
	test(A_result1);
	printArray(A_result1);
	free(A_result1);

	methods(2);
	printf("CUDA Method 2 time:\t%f -> ", seq_time);
    test(A_result2);
    printArray(A_result2);
    free(A_result2);

	methods(3);
	printf("CUDA Method 3 time:\t%f -> ", seq_time);
    test(A_result3);
    printArray(A_result3);
	free(A_result3);

	free(D);
	free(A);
    return 1;
}

//--------------------------------------------------------------------------------------------------------------

/* create the array that represents the graph */
void makeAdjacency()
{
	int i, j;
	float randValue;

	for (i=0; i<n; i++){
		for (j=0; j<n; j++){
			randValue = (((double)rand()+1)/((double)RAND_MAX+1)); /* Get a random float value from 0 to 1 */
			if (randValue>p)
				A[i*n+j] = INFINITY;
			else 
				A[i*n+j] = randValue * w;
		}
        A[i*n+i] = 0;
	}
}

//--------------------------------------------------------------------------------------------------------------

/* print array for personal check (only for small n) */
void printArray(float *array) {
    int i;
    
    if (n>3*3) return; 

    for (i=0; i<n*n; i++) {
    	if (i % n == 0) printf("\n"); /* change line */
    	printf("%f  ",array[i]);
    }

    printf("\n");	
}

//--------------------------------------------------------------------------------------------------------------

/* floyd Warshall Method based on this: http://www.programming-algorithms.net/article/45708/Floyd-Warshall-algorithm */
void floydWarshallMethod()
{
	int i, j, k;

	/* copy arrays D <- A */
	for (i=0; i<n; i++){
		for (j=0; j<n; j++){
			D[i*n+j] = A[i*n+j];
		}
		D[i*n+i] = 0;
	}

    gettimeofday (&startwtime, NULL);

	for (k=0; k<n; k++){
		for (i=0; i<n; i++){
			for (j=0; j<n; j++){
                if (D[i*n+j] > D[i*n+k] + D[k*n+j]){
                    D[i*n+j] = D[i*n+k] + D[k*n+j];
                }
			}
		}
	}
	gettimeofday (&endwtime, NULL);
    seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
}


//--------------------------------------------------------------------------------------------------------------

/* calling kernel functions for methods 1, 2, 3 and calculating the execution time */
/* where f = 1, 2, 3 for methods 1, 2 or 3 respectively */
void methods(int f)
{
	float *A_dev;															/* gpu array */
	int k;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaMalloc((void **)&A_dev, n*n*sizeof(float)); 						/* allocate the memory in GPU */ 		

    /****** 1st Method ******/
	if (f==1){
		A_result1 = (float *) malloc (n*n*sizeof(float));
		cudaEventRecord(start);												/* start time counter */
		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		

		if (n*n<=maxThreadsPerBlock){ 										/* 64 threads (max) per block */
	     	dim3 threadsPerBlock(n, n);
	    	for (k=0; k<n; k++)
				kernel1<<<1, threadsPerBlock>>>(k, n, A_dev);	
		}
		else{			/* if threads per block > 64 */
			dim3 threadsPerBlock(8,8);
			dim3 blocksPerGrid(n/threadsPerBlock.x, n/threadsPerBlock.y);
			for (k=0; k<n; k++)
				kernel1<<<blocksPerGrid, threadsPerBlock>>>(k, n, A_dev);
		}
	   
	   cudaMemcpy(A_result1, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost); /* get results back to CPU */
	}


	/****** 2nd Method ******/
	/* same as 1st method but this time we are calling different kernel function that uses shared mem */
	else if (f==2){
		A_result2 = (float *) malloc (n*n*sizeof(float));
		cudaEventRecord(start);												/* start time counter */
		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */


		if (n*n<=maxThreadsPerBlock){ 										/* 64 threads (max) per block */
	     	dim3 threadsPerBlock(n, n);
	    	for (k=0; k<n; k++)
	    		kernel2<<<1, threadsPerBlock>>>(k, n, A_dev);
		}
		else{
			dim3 threadsPerBlock(8,8);
			dim3 blocksPerGrid(n/threadsPerBlock.x, n/threadsPerBlock.y);
			for (k=0; k<n; k++)
				kernel2<<<blocksPerGrid, threadsPerBlock>>>(k, n, A_dev);
		}
	   
	   	cudaMemcpy(A_result2, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}


	/****** 3rd Method ******/
	/* same as 2nd method but based on 4 cells per thread */
	else if (f==3){
		A_result3 = (float *) malloc (n*n*sizeof(float));
		cudaEventRecord(start);												/* start time counter */
		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		int cellsPerThread = 4;												/* number of cells per thread */
		
		if (n*n<=maxThreadsPerBlock * cellsPerThread){ 
   	    	dim3 threadsPerBlock(n/cellsPerThread, n/cellsPerThread);
	    	for (k=0; k<n; k++)
	    		kernel3<<<1, threadsPerBlock>>>(n, k, A_dev, cellsPerThread);
		}
		else{
			dim3 threadsPerBlock(8,8);
    		dim3 blocksPerGrid(n/(threadsPerBlock.x * cellsPerThread), n/(threadsPerBlock.y * cellsPerThread));
			for (k=0; k<n; k++)
				kernel3<<<blocksPerGrid, threadsPerBlock>>>(k, n, A_dev, cellsPerThread);
		}
	    
	    cudaMemcpy(A_result3, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}


    cudaEventRecord(stop);								/* stop time countdown*/
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&seq_time, start, stop);
    seq_time=seq_time/1000; 							/* convert milliseconds to seconds */

    cudaEventDestroy(start);							
    cudaEventDestroy(stop);
    cudaFree(A_dev);									/* release gpu array */

}	


//--------------------------------------------------------------------------------------------------------------

/* one cell per thread, without using shared memory */
__global__ void kernel1(int k, int n, float *A) 
{

	/* same as Floyd Warshal Method but with the use of gpu */
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;

	if (A[i*n+j] > A[i*n+k] + A[k*n+j]){
		A[i*n+j] = A[i*n+k] + A[k*n+j];
	}
}

//--------------------------------------------------------------------------------------------------------------

/* one cell per thread, using shared memory */
__global__ void kernel2(int k, int n, float *A) 
{

	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;
	
	__shared__ float shA[1];

	shA[threadIdx.x]=A[i*n+k];	/* transfer A-array cell to shared mem cell */

	/* do the check using the shared memory cell */
	if (A[i*n+j] > shA[threadIdx.x] + A[k*n+j]) {
		A[i*n+j] = shA[threadIdx.x] + A[k*n+j];
	}

}

//--------------------------------------------------------------------------------------------------------------

/* multiple cellsPerThread (4) per thread, using shared memory */
__global__ void kernel3(int k, int n, float *A, int cellsPerThread) 
{

	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;
	int c, l, m;

	 __shared__ float shA[4];
	 
	/* transfering 4 A-array cells to shared mem cells */ 
	c = 0; 
	for (l=i*cellsPerThread; l<(i*cellsPerThread) + cellsPerThread; l++) {
		shA[threadIdx.x*cellsPerThread + c] = A[l*n + k];
		c++;
	}
	c = 0;
	
	__syncthreads();
	
	/* do the check using the shared memory cells */
	for (l=i*cellsPerThread; l<(i*cellsPerThread) + cellsPerThread; l++) {
		for (m=j*cellsPerThread; m<(j*cellsPerThread) + cellsPerThread; m++) {
			if (A[l*n + m] > shA[threadIdx.x*cellsPerThread + c] + A[k*n + m]) {
				A[l*n + m] = shA[threadIdx.x*cellsPerThread + c] + A[k*n + m];
			}
		}
		c++;
	}
}

//--------------------------------------------------------------------------------------------------------------

/* test cuda results with serial Floyd Warshall method */ 
void test(float *A) {
	int i;

	for (i=0; i<n*n; i++) {
		if (D[i] != A[i]) {
			printf("%sFailed!\n%s", KRED, RESET);
			return;
		}
	}
	
	printf("%sPassed!\n%s",KGRN ,RESET);
}


//--------------------------------------------------------------------------------------------------------------
