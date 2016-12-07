/* 		Hadjithoma Abdreas
 * 		Aristotle University of Thessaloniki		
 *		Sept 2016 		
 *		All Pair Shortest Path (APSP) Solution
 */



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
void methods(int f, int t);
void test(float *A_dev);
void printArray(float *array);
void transposeAdjacency();
void cwf3();

__global__ void kernel1(int k, int n, float *A);
__global__ void kernel2(int k, int n, float *A);
__global__ void kernel3(int k, int n, float *A, int cellsPerThread);

__global__ void kernel1_t(int k, int n, float *A, float *AT);
__global__ void kernel2_t(int k, int n, float *A, float *AT);
__global__ void kernel3_t(int k, int n, float *A, float *AT, int cellsPerThread);

__global__ void kernel1_t_v2(int k, int n, float *A, float *AT);
__global__ void kernel2_t_v2(int k, int n, float *A, float *AT);
__global__ void kernel3_t_v2(int k, int n, float *A, float *AT, int cellsPerThread);



int n, w;		
float p;
float *D; 		/* D: distance (for the Floyd Warshall algorithm) */
float *A; 		/* A: array (for methods 1, 2, 3 as mentioned in the exercise) */
float *AT; 		/* A_t: transposed adjancecy array */
float *A_result11, *A_result21, *A_result31;	/* arrays for saving results in methods 1, 2, 3 */
float *A_result12, *A_result22, *A_result32;	/* arrays for saving results in methods 1, 2, 3 */
float *A_result13, *A_result23, *A_result33;	/* arrays for saving results in methods 1, 2, 3 */

float seq_time;
struct timeval startwtime, endwtime;
//cudaEvent_t start, stop;\    



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
   	printf("n = %d \np = %.2f \nw = %d\n", n, p, w);


    time_t t;
    srand((unsigned)time(&t));

    D = (float *)malloc(n*n*sizeof(float)); 
    A = (float *)malloc(n*n*sizeof(float)); 
    AT = (float *)malloc(n*n*sizeof(float)); 


	makeAdjacency(); 		/* initialize the graph */
	transposeAdjacency();	/* transpose the Adjency*/

	floydWarshallMethod();
	printf("Floyd Warshall time:\t%f\n", seq_time);


	/* init the timers */
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
 	

	//without A-Transpose
	//1
	printf("\n");
 	cudaEventRecord(start);
	methods(1,1);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),
	printf("CUDA Method 1 time:\t%f\t -> ", seq_time/1000);
	test(A_result11);
	free(A_result11);


	//2
	cudaEventRecord(start);
	methods(2,1);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),
	printf("CUDA Method 2 time:\t%f\t -> ", seq_time/1000);
    test(A_result21);
    free(A_result21);

    //3
    cudaEventRecord(start);
    methods(3,1);
    cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),
	printf("CUDA Method 3 time:\t%f\t -> ", seq_time/1000);
    test(A_result31);
	free(A_result31);


	//with A transpose 
	//1
	printf("\n");
	cudaEventRecord(start);
	methods(1,2);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),	
  	printf("CUDA Method 1T time:\t%f\t -> ", seq_time/1000);
	test(A_result12);
	free(A_result12);

	//2
    cudaEventRecord(start);
    methods(2,2);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),	
  	printf("CUDA Method 2T time:\t%f\t -> ", seq_time/1000);
    test(A_result22);
    free(A_result22);	
	
	//3
    cudaEventRecord(start);
	methods(3,2);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),
  	printf("CUDA Method 3T time:\t%f\t -> ", seq_time/1000);
    test(A_result32);
	free(A_result32);

	//with A-transpose - version2
	//1
	printf("\n");
 	cudaEventRecord(start);
	methods(1,3);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),
	printf("CUDA Method 1T_v2 time:\t%f\t -> ", seq_time/1000);
	test(A_result13);
	free(A_result13);

	//2
 	cudaEventRecord(start);
	methods(2,3);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),
	printf("CUDA Method 2T_v2 time:\t%f\t -> ", seq_time/1000);
	test(A_result23);
	free(A_result23);

	//3
 	cudaEventRecord(start);
	methods(3,3);
	cudaEventRecord(stop);
  	cudaEventSynchronize(stop);
  	cudaEventElapsedTime(&seq_time, start, stop),
	printf("CUDA Method 3T_v2 time:\t%f\t -> ", seq_time/1000);
	test(A_result33);
	free(A_result33);

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

void transposeAdjacency()
{
	int i, j;
	for (i=0; i<n; i++){
		for (j=0; j<n; j++){
			AT[i*n+j] = A[j*n+i];
		}
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

/* Alling kernel functions for methods 1, 2, 3 and Alculating the execution time */
/* where f = 1, 2, 3 for methods 1, 2 or 3 respectively */
void methods(int f, int t)
{
	float *A_dev;
	float *AT_dev;												/* gpu array */
	int k;

	cudaMalloc((void **)&A_dev, n*n*sizeof(float)); 			/* alloATe the memory in GPU */ 		
	if (t) cudaMalloc((void **)&AT_dev, n*n*sizeof(float)); 	/* alloATe the memory in GPU */ 		

    /************************************************ 1st Method ************************************************/
	if (f==1 && t==1){
		A_result11 = (float *) malloc (n*n*sizeof(float));

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
	   
	   cudaMemcpy(A_result11, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost); /* get results back to CPU */
	}

    /******************************************* 1st Method using A^T *******************************************/
	else if (f==1 && t==2){
		A_result12 = (float *) malloc (n*n*sizeof(float));

		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		cudaMemcpy(AT_dev, AT, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table AΤ from CPU to GPU */


		if (n*n<=maxThreadsPerBlock){ 										/* 64 threads (max) per block */
	     	dim3 threadsPerBlock(n, n);
	    	for (k=0; k<n; k++)
	    		kernel1_t<<<1, threadsPerBlock>>>(k, n, A_dev, AT_dev);
		}
		else{			/* if threads per block > 64 */
			dim3 threadsPerBlock(8,8);
			dim3 blocksPerGrid(n/threadsPerBlock.x, n/threadsPerBlock.y);
			for (k=0; k<n; k++)
				kernel1_t<<<blocksPerGrid, threadsPerBlock>>>(k, n, A_dev, AT_dev);
		}
	   
	    cudaMemcpy(A_result12, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost); /* get results back to CPU */
	}

	/*************************************** 1st Method using A^T - Ver.2 ***************************************/
	else if (f==1 && t==3){
		A_result13 = (float *) malloc (n*n*sizeof(float));

		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		cudaMemcpy(AT_dev, AT, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table AT from CPU to GPU */

	    if (n*n<=maxThreadsPerBlock) {
	    	int threadsPerBlock =n*n; 
	    	dim3 blocksPerGrid((n+threadsPerBlock)/threadsPerBlock, n);
	    	
	    	for (int k=0; k<n; k++) {
				kernel1_t_v2<<<blocksPerGrid, threadsPerBlock>>>(k, n, A_dev, AT_dev);
	    	}
	    }
	    else {
	    	int threadsPerBlock = 128; 
	    	dim3 blocksPerGrid((n+threadsPerBlock)/threadsPerBlock, n);
	    	
	    	for (int k=0; k<n; k++) {
				kernel1_t_v2<<<blocksPerGrid, threadsPerBlock>>>(k, n, A_dev, AT_dev);
	    	}
		}	
	   	cudaMemcpy(A_result13, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}


    /************************************************ 2nd Method ************************************************/
	/* same as 1st method but this time we are Alling different kernel function that uses shared mem */
	else if (f==2 && t==1){

		A_result21 = (float *) malloc (n*n*sizeof(float));
		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */

		if (n*n<=maxThreadsPerBlock){ 										/* 64 threads (max) per block */
	     	dim3 threadsPerBlock(n, n);
    		size_t size = threadsPerBlock.x*sizeof(float);
	    	for (k=0; k<n; k++)
	    		kernel2<<<1, threadsPerBlock, size>>>(k, n, A_dev);
		}
		else{
			dim3 threadsPerBlock(8, 8);
			dim3 blocksPerGrid(n/threadsPerBlock.x, n/threadsPerBlock.y);
    		size_t size = threadsPerBlock.x*sizeof(float);

			for (k=0; k<n; k++)
				kernel2<<<blocksPerGrid, threadsPerBlock, size>>>(k, n, A_dev);
		}
	   
	   	cudaMemcpy(A_result21, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}

    /******************************************* 2nd Method using A^T *******************************************/
	else if (f==2 && t==2){
		A_result22 = (float *) malloc (n*n*sizeof(float));

		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		cudaMemcpy(AT_dev, AT, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table AT from CPU to GPU */

		if (n*n<=maxThreadsPerBlock){ 										/* 64 threads (max) per block */
	     	dim3 threadsPerBlock(n, n);
	    	for (k=0; k<n; k++)
	    		kernel2_t<<<1, threadsPerBlock>>>(k, n, A_dev, AT_dev);
		}
		else{
			dim3 threadsPerBlock(8,8);
			dim3 blocksPerGrid(n/threadsPerBlock.x, n/threadsPerBlock.y);
			for (k=0; k<n; k++)
				kernel2_t<<<blocksPerGrid, threadsPerBlock>>>(k, n, A_dev, AT_dev);
		}
	   
	   	cudaMemcpy(A_result22, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}

	/*************************************** 2nd Method using A^T - Ver.2 ***************************************/
	else if (f==2 && t==3){
		A_result23 = (float *) malloc (n*n*sizeof(float));

		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		cudaMemcpy(AT_dev, AT, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table AT from CPU to GPU */

	    if (n*n<=maxThreadsPerBlock) {
	    	int threadsPerBlock =n*n; 
	    	dim3 blocksPerGrid((n+threadsPerBlock)/threadsPerBlock, n);
	    	
	    	for (int k=0; k<n; k++) {
				kernel2_t_v2<<<blocksPerGrid,threadsPerBlock>>>(k, n, A_dev, AT_dev);
	    	}
	    }
	    else {
	    	int threadsPerBlock = 256; 
	    	dim3 blocksPerGrid((n+threadsPerBlock)/threadsPerBlock, n);
	    	
	    	for (int k=0; k<n; k++) {
				kernel2_t_v2<<<blocksPerGrid,threadsPerBlock>>>(k, n, A_dev, AT_dev);
	    	}
		}	
	   	cudaMemcpy(A_result23, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}


    /************************************************ 3rd Method ************************************************/
	/* same as 2nd method but based on 4 cells per thread */
	else if (f==3 && t==1){
		A_result31 = (float *) malloc (n*n*sizeof(float));
		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */

		int cellsPerThread = 4;												/* number of cells per thread */

		if (n*n<=maxThreadsPerBlock * cellsPerThread){ 
   	    	dim3 threadsPerBlock(n/cellsPerThread, n/cellsPerThread);
   	    	size_t size = (threadsPerBlock.x*cellsPerThread)*sizeof(float) + (threadsPerBlock.y*cellsPerThread)*sizeof(float);
			for (k=0; k<n; k++)
				kernel3<<<1, threadsPerBlock, size>>>(k, n, A_dev, cellsPerThread);
		}
		else{
			dim3 threadsPerBlock(8,8);
    		dim3 blocksPerGrid(n/(threadsPerBlock.x * cellsPerThread), n/(threadsPerBlock.y * cellsPerThread));
    		size_t size = (threadsPerBlock.x*cellsPerThread)*sizeof(float) + (threadsPerBlock.y*cellsPerThread)*sizeof(float);
			for (k=0; k<n; k++)
				kernel3<<<blocksPerGrid, threadsPerBlock, size>>>(k, n, A_dev, cellsPerThread);

		}
	    cudaMemcpy(A_result31, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}

    /******************************************* 3rd Method using A^T *******************************************/
	else if (f==3 && t==2){
		A_result32 = (float *) malloc (n*n*sizeof(float));
		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		cudaMemcpy(AT_dev, AT, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table AT from CPU to GPU */

		int cellsPerThread = 4;												/* number of cells per thread */

		if (n<= 8 * cellsPerThread){ 
   	    	dim3 threadsPerBlock(n/cellsPerThread, n/cellsPerThread);
   	    	size_t size = (threadsPerBlock.x*cellsPerThread)*sizeof(float) + (threadsPerBlock.y*cellsPerThread)*sizeof(float);
			for (k=0; k<n; k++)
				kernel3_t<<<1, threadsPerBlock, size>>>(k, n, A_dev, AT_dev, cellsPerThread);
		}

		else{
			dim3 threadsPerBlock(8,8);
    		dim3 blocksPerGrid(n/(threadsPerBlock.x * cellsPerThread), n/(threadsPerBlock.y * cellsPerThread));
    		size_t size = (threadsPerBlock.x*cellsPerThread)*sizeof(float) + (threadsPerBlock.y*cellsPerThread)*sizeof(float);
			
			for (k=0; k<n; k++)
				kernel3_t<<<blocksPerGrid, threadsPerBlock, size>>>(k, n, A_dev, AT_dev, cellsPerThread);

		}
	    cudaMemcpy(A_result32, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}

	/*************************************** 3rd Method using A^T - Ver.2 ***************************************/
	else if (f==3 && t==3){
		A_result33 = (float *) malloc (n*n*sizeof(float));

		cudaMemcpy(A_dev, A, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table A from CPU to GPU */
		cudaMemcpy(AT_dev, AT, n*n*sizeof(float), cudaMemcpyHostToDevice);	/* copy table AT from CPU to GPU */
		int cellsPerThread = 4;												/* number of cells per thread */

    	int threadsPerBlock = n/cellsPerThread; 
    	dim3 blocksPerGrid((n+threadsPerBlock)/(threadsPerBlock*cellsPerThread), n/cellsPerThread);
    	size_t size = 2*threadsPerBlock*cellsPerThread*sizeof(float);
    	
    	for (int k=0; k<n; k++) {
			kernel3_t_v2<<<blocksPerGrid, threadsPerBlock, size>>>(k, n, A_dev, AT_dev, cellsPerThread);;
    	}
	    
	   	cudaMemcpy(A_result33, A_dev, n*n*sizeof(float), cudaMemcpyDeviceToHost);
	}
    cudaFree(AT_dev);
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


/* method 1 using the transpose of A */
__global__ void kernel1_t(int k, int n, float *A, float *AT) 
{

	/* same as Floyd Warshal Method but with the use of gpu */
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;

	if (A[i*n+j] > AT[k*n+i] + A[k*n+j]){
		A[i*n+j] = AT[n*j+i] = AT[k*n+i] + A[k*n+j];
	} 

}

//--------------------------------------------------------------------------------------------------------------

/* one cell per thread, using shared memory */
__global__ void kernel2(int k, int n, float *A) 
{

	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;
	
	extern __shared__ float shA[];		

	shA[threadIdx.x]=A[i*n+k];	/* transfer A-array cell to shared mem cell */
	/* do the check using the shared memory cell */	  	

	if (A[i*n+j] > shA[threadIdx.x] + A[k*n+j]) {
		A[i*n+j] = shA[threadIdx.x] + A[k*n+j];
	}
}

//--------------------------------------------------------------------------------------------------------------


/* method 2 using the transpose of A */
__global__ void kernel2_t(int k, int n, float *A, float *AT) 
{

	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;
	
	__shared__ float shA[1];

	shA[threadIdx.x]=AT[n*k+i];	

	if (A[i*n+j] > shA[threadIdx.x] + A[k*n+j]) {
		A[i*n+j] = AT[j*n+i] = shA[threadIdx.x] + A[k*n+j];
	}
}

//--------------------------------------------------------------------------------------------------------------

/* multiple cellsPerThread (4) per thread, using shared memory */
__global__ void kernel3(int k, int n, float *A, int cellsPerThread) 
{

	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.y + threadIdx.y;
	int c, l, m;

	extern __shared__ float shared[];
	float *shA = (float *)&shared[blockDim.x*cellsPerThread]; 

	/* transfering 4 A-array cells to shared mem cells */ 
	c = 0; 
	for (l=i*cellsPerThread; l<(i*cellsPerThread) + cellsPerThread; l++) {
		shA[threadIdx.x*cellsPerThread + c] = A[l*n+k];
		c++;
	}
	c = 0;
	
	__syncthreads();
	
	/* do the check using the shared memory cells */
	for (l=i*cellsPerThread; l<(i*cellsPerThread) + cellsPerThread; l++) {
		for (m=j*cellsPerThread; m<(j*cellsPerThread) + cellsPerThread; m++) {
			if (A[l*n+m] > shA[threadIdx.x*cellsPerThread + c] + A[k*n+m]) {
				A[l*n+m] = shA[threadIdx.x*cellsPerThread + c] + A[k*n+m];
			}
		}
		c++;
	}
}

//--------------------------------------------------------------------------------------------------------------


/* method 3 using the transpose of A */
__global__ void kernel3_t(int k, int n, float *A, float *AT, int cellsPerThread) 
{


	int li,lj,l, m;
	int ci=0, cj=0;
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	int j = blockIdx.y*blockDim.x + threadIdx.y;
	
	extern __shared__ float shared[];
	float *shA1 = shared;
	float *shA2 = (float *)&shA1[blockDim.x*cellsPerThread];
	
	for (li=i*cellsPerThread; li<(i*cellsPerThread) + cellsPerThread; li++) {
		shA1[threadIdx.x*cellsPerThread + ci] = AT[k*n + li];
		ci++;
	}
	ci = 0;
	
	for (lj=j*cellsPerThread; lj<(j*cellsPerThread) + cellsPerThread; lj++) {
		shA2[threadIdx.y*cellsPerThread + cj] = A[k*n + lj];
		cj++;
	}
	cj = 0;
	
	__syncthreads();
	
	for (l=i*cellsPerThread; l<(i*cellsPerThread) + cellsPerThread; l++) {
		for (m=j*cellsPerThread; m<(j*cellsPerThread) + cellsPerThread; m++) {
			if ( A[l*n + m] > shA1[threadIdx.x*cellsPerThread + ci] + shA2[threadIdx.y*cellsPerThread + cj] ) {
				A[l*n + m] = AT[m*n + l] = shA1[threadIdx.x*cellsPerThread + ci] + shA2[threadIdx.y*cellsPerThread + cj];
			}
			cj++;
		}
		cj = 0;
		ci++;
	}
}

//--------------------------------------------------------------------------------------------------------------

__global__ void kernel1_t_v2(int k, int n, float *A, float *AT)
{
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	
	if (i >= n) 
		return;
	
	if (A[n*blockIdx.y + i] > AT[n*k + blockIdx.y] + A[k*n + i] ){
		A[n*blockIdx.y + i] = AT[i*n + blockIdx.y] = AT[n*k + blockIdx.y] + A[k*n + i];
	}
	
}

//--------------------------------------------------------------------------------------------------------------

__global__ void kernel2_t_v2(int k, int n, float *A, float *AT)
{
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	
	if (i >= n) 
		return;

	//use shared mem
	__shared__ float shA;	
	shA = AT[n*k + blockIdx.y];

	if (A[n*blockIdx.y + i] > shA + A[k*n + i])
		A[n*blockIdx.y + i] = AT[i*n + blockIdx.y] = shA + A[k*n + i];
}

//--------------------------------------------------------------------------------------------------------------

__global__ void kernel3_t_v2(int k, int n, float *A, float *AT, int cellsPerThread)
{
	int li, lj, l, m;
	int ci = 0, cj = 0;
	int i = blockIdx.x*blockDim.x + threadIdx.x;
	
	if (i >= n) return;
	
	extern __shared__ float s[];
	float *shA1 = s;
	float *shA2 = (float *)&shA1[blockDim.x*cellsPerThread];
	
	for (li=blockIdx.y*cellsPerThread; li<(blockIdx.y*cellsPerThread) + cellsPerThread; li++) {
		shA1[threadIdx.x*cellsPerThread + ci] = AT[k*n + li];
		ci++;
	}
	ci = 0;
	
	for (lj=i*cellsPerThread; lj<(i*cellsPerThread) + cellsPerThread; lj++) {
		shA2[threadIdx.x*cellsPerThread + cj] = A[k*n + lj];
		cj++;
	}
	cj = 0;
	
	__syncthreads();

	for (l=blockIdx.y*cellsPerThread; l<(blockIdx.y*cellsPerThread) + cellsPerThread; l++) {
		for (m=i*cellsPerThread; m<(i*cellsPerThread) + cellsPerThread; m++) {
			if (A[l*n + m] > shA1[threadIdx.x*cellsPerThread + ci] + shA2[threadIdx.x*cellsPerThread + cj] ) {
				A[l*n + m] = AT[m*n + l] = shA1[threadIdx.x*cellsPerThread + ci] + shA2[threadIdx.x*cellsPerThread + cj];
			}
			cj++;
		}
		cj = 0;
		ci++;
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

