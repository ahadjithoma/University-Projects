
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <math.h>
#include <omp.h>

struct timeval startwtime, endwtime;
double seq_time;


int N, N_threads, A_threads;  	// N_threads = max threads number, A_threads = active threads
int *a;         				// data array to be sorted

pthread_mutex_t key = PTHREAD_MUTEX_INITIALIZER; //mutex key

const int ASCENDING  = 1;
const int DESCENDING = 0;

/*SUB-PROCEDURES*/
void init(void);
void print(void);
void sort(void);
void test(void);
inline void exchange(int i, int j);
void compare(int i, int j, int dir);
int cmpfuncDes (const void * a, const void * b);
int cmpfuncAsc (const void * a, const void * b);
void fileWrite(int i, int j, char *name, double a);

/*SERIAL BITONIC*/
void bitonicMerge(int lo, int cnt, int dir);
void recBitonicSort(int lo, int cnt, int dir);
void impBitonicSort(void);

/*THREADS*/
void *pthBitonicSort(void *args);
void *pthBitonicMerge(void *args);
void pthSort();

/*THREADS+QSORT*/
void *pthQBitonicSort(void *args);
void pthQSort();

/*openMP*/
void ompBitonicSort(void);
/********/


/* ---------------------------------  Main --------------------------------- */ 

int main(int argc, char **argv) {

if (argc != 3 || 1<<atoi(argv[2])>256) {
printf("Give q and p\nwhere table_lenght = 2^q and threads = 2^p \n");
exit(1);
}

N = 1<<atoi(argv[1]);
N_threads = 1<<atoi(argv[2]);
a = (int *) malloc(N * sizeof(int));
printf("N threads = %d\n",N_threads);


//Imperative Serial
init();
gettimeofday (&startwtime, NULL);
impBitonicSort();
gettimeofday (&endwtime, NULL);
seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
printf("Imperative wall clock time\t = %f", seq_time);
test();
fileWrite(atoi(argv[1]),atoi(argv[2]), "Imperative.txt", (double)seq_time);

//Recursive Serial
init();
gettimeofday (&startwtime, NULL);
sort();
gettimeofday (&endwtime, NULL);
seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
printf("Recursive wall clock time\t = %f", seq_time);
test();
fileWrite(atoi(argv[1]),atoi(argv[2]), "Recursive.txt", (double)seq_time);

//Quick Sort
init();
gettimeofday (&startwtime, NULL);
qsort(a , N , sizeof(int) , cmpfuncAsc);
gettimeofday (&endwtime, NULL);
seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
printf("Quick Sort wall clock time\t = %f", seq_time);
test();
fileWrite(atoi(argv[1]),atoi(argv[2]), "qSort.txt", (double)seq_time);

//Recursive Parallel - pthreads
init();
gettimeofday (&startwtime, NULL);
pthSort();
gettimeofday (&endwtime, NULL);
seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
printf("pThreads wall clock time\t = %f", seq_time);
test();
pthread_mutex_destroy(&key);
fileWrite(atoi(argv[1]),atoi(argv[2]), "pthreads.txt", (double)seq_time);

//Imperative Parallel - openMP
init();
gettimeofday (&startwtime, NULL);
ompBitonicSort();
gettimeofday (&endwtime, NULL);
seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
printf("openMP wall clock time\t\t = %f", seq_time);
test(); 
fileWrite(atoi(argv[1]),atoi(argv[2]), "openMP.txt", (double)seq_time);

//Recursive Parallel - pthreads+qSort
init();
gettimeofday (&startwtime, NULL);
pthQSort();
gettimeofday (&endwtime, NULL);
seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
printf("pThreads+qSort wall clock time\t = %f", seq_time);
test();
pthread_mutex_destroy(&key);
fileWrite(atoi(argv[1]),atoi(argv[2]), "pthreads+qSort.txt", (double)seq_time);

}



/* --------------------------------- SUB-PROCEDURES  --------------------------------- */ 

//procedure fileWrite(..) : writes times in txt file
void fileWrite(int i, int j, char *name, double time){

	FILE *f = fopen(name, "a");
	if (f == NULL)
	{
		printf("Error opening file!\n");
		exit(1);
	}
	
	if (j==1){
		fprintf(f,"\r\nN = %d\r\n\r\n%f\r\n",i,(double)time);
	}
	else{
		fprintf(f, "%f \r\n",(double)time);
	}
	
	fclose(f); 
}

//procedure cmpfuncAsc(..) :compare for qsort in Ascending Sort
int cmpfuncAsc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

//procedure cmpfuncDes(..) :compare for qsort in Descending Sort
int cmpfuncDes (const void * a, const void * b)
{
   return ( *(int*)b - *(int*)a );
}

// procedure test() : verify sort results
void test() {
  int pass = 1;
  int i;
  for (i = 1; i < N; i++) {
    pass &= (a[i-1] <= a[i]);
  }

  printf(" --> %s\n",(pass) ? "PASSed" : "FAILed");
}


//procedure init() : initialize array "a" with data 
void init() {
  int i;
  for (i = 0; i < N; i++) {
    a[i] = rand() % N; // (N - i);
  }
}

//procedure  print() : print array elements 
void print() {
  int i;
  for (i = 0; i < N; i++) {
    printf("%d\n", a[i]);
  }
  printf("\n");
}


// INLINE procedure exchange() : pair swap
inline void exchange(int i, int j) {
  int t;
  t = a[i];
  a[i] = a[j];
  a[j] = t;
}



// procedure compare() 
/** The parameter dir indicates the sorting direction, ASCENDING 
	or DESCENDING; if (a[i] > a[j]) agrees with the direction,    
	then a[i] and a[j] are interchanged.
**/
inline void compare(int i, int j, int dir) {
  if (dir==(a[i]>a[j])) 
    exchange(i,j);
}




// Procedure bitonicMerge() 
/** It recursively sorts a bitonic sequence in ascending order, 
	if dir = ASCENDING, and in descending order otherwise. 
	The sequence to be sorted starts at index position lo,
	the parameter cbt is the number of elements to be sorted. 
 **/
void bitonicMerge(int lo, int cnt, int dir) {
  if (cnt>1) {
    int k=cnt/2;
    int i;
    for (i=lo; i<lo+k; i++)
      compare(i, i+k, dir);
    bitonicMerge(lo, k, dir);
    bitonicMerge(lo+k, k, dir);
  }
}



// function recBitonicSort() 
/** first produces a bitonic sequence by recursively sorting 
    its two halves in opposite sorting orders, and then
    calls bitonicMerge to make them in the same order 
 **/
void recBitonicSort(int lo, int cnt, int dir) {
  if (cnt>1) {
    int k=cnt/2;
    recBitonicSort(lo, k, ASCENDING);
    recBitonicSort(lo+k, k, DESCENDING);
    bitonicMerge(lo, cnt, dir);
  }
}


// function sort() 
/** Caller of recBitonicSort for sorting the entire array of length N 
	in ASCENDING order
 **/
void sort() {
  recBitonicSort(0, N, ASCENDING);
}




// imperative version of bitonic sort
void impBitonicSort() {

  int i,j,k;
  
  for (k=2; k<=N; k=2*k) {
    for (j=k>>1; j>0; j=j>>1) {
      for (i=0; i<N; i++) {
	int ij=i^j;
	if ((ij)>i) {
	  if ((i&k)==0 && a[i] > a[ij]) 
	      exchange(i,ij);
	  if ((i&k)!=0 && a[i] < a[ij])
	      exchange(i,ij);
	}
      }
    }
  }
}

/* ----------------------------- PTHREADS + QSORT  ----------------------------- */ 

// struct for passing arguments
typedef struct qs_threads_args{
	int lo, cnt, dir;	
}qs_threads_args;


// function pthQsort() 
/** Caller of pthQBitonicSort for sorting the entire array of length N 
	in ASCENDING order
**/
void pthQSort(){
	qs_threads_args argsSort;
	A_threads=0;
	argsSort.lo=0;
	argsSort.cnt=N;
	argsSort.dir=ASCENDING;
	pthQBitonicSort(&argsSort);	
}

// pthreads+qSort version of recursive bitonic sort
void *pthQBitonicSort(void *args){
	
	int lo  = ((qs_threads_args *) args) -> lo;
	int dir = ((qs_threads_args *) args) -> dir;
	int cnt = ((qs_threads_args *) args) -> cnt;

	if (cnt>1){
		
		int k = cnt/2;
	
		if (A_threads < N_threads/2) {
			qs_threads_args argsT1, argsT2;
			pthread_t t1, t2;			
			
			argsT1.lo=lo;
			argsT1.cnt=k;
			argsT1.dir=ASCENDING;
			
			pthread_mutex_lock (&key);
			A_threads+=2;
		    pthread_mutex_unlock (&key);

			
			argsT2.lo=lo+k;
			argsT2.cnt=k;
			argsT2.dir=DESCENDING;
			
			pthread_create(&t1, NULL, pthQBitonicSort, &argsT1);
			pthread_create(&t2, NULL, pthQBitonicSort, &argsT2);

			pthread_join(t1, NULL);
			pthread_join(t2, NULL);	
			
			pthread_mutex_lock (&key);
			A_threads+=-2;
		    pthread_mutex_unlock (&key);
			
		}
		else{			
			qsort(a+lo , k , sizeof(int) , cmpfuncAsc);
			qsort(a+lo+k, k , sizeof(int), cmpfuncDes);	
		}
		
		qs_threads_args argsM;
		
		argsM.lo=lo;
		argsM.cnt=cnt;
		argsM.dir=dir;
		
		pthBitonicMerge(&argsM);            
	}
}



/* --------------------------------- PTHREADS  --------------------------------- */

// struct for passing arguments 
typedef struct threads_args{
	int lo, cnt, dir;	
}threads_args;



// function pthsort() 
/** Caller of pthBitonicSort for sorting the entire array of length N 
	in ASCENDING order
**/
void pthSort(){
	threads_args argsSort;
	A_threads=0;
	argsSort.lo=0;
	argsSort.cnt=N;
	argsSort.dir=ASCENDING;
	pthBitonicSort(&argsSort);	
}

// pthreads version of recursive bitonic Merge
void *pthBitonicMerge(void *args){
	int lo  = ((threads_args *) args) -> lo;
	int dir = ((threads_args *) args) -> dir;
	int cnt = ((threads_args *) args) -> cnt;
	
	if (cnt>1) {
		int k=cnt/2;
		int i;
		for (i=lo; i<lo+k; i++)
				compare(i, i+k, dir);
			
		if (A_threads<N_threads-1){						
			threads_args argsM1, argsM2;
			pthread_t t1, t2;
			
			pthread_mutex_lock (&key);
			A_threads+=2;
		    pthread_mutex_unlock (&key);
			
			argsM1.lo=lo;
			argsM1.dir=dir;
			argsM1.cnt=k;

			pthread_create(&t1, NULL, pthBitonicMerge, &argsM1);

			argsM2.lo=lo+k;
			argsM2.dir=dir;
			argsM2.cnt=k;
			
			pthread_create(&t2, NULL, pthBitonicMerge, &argsM2);
			
			pthread_join(t1, NULL);
			pthread_join(t2, NULL);
				
		}
		else{
			bitonicMerge(lo, k, dir);
			bitonicMerge(lo+k, k, dir);
		}
	}
}

// pthreads version of recursive bitonic sort
void *pthBitonicSort(void *args){
	
	int lo  = ((threads_args *) args) -> lo;
	int dir = ((threads_args *) args) -> dir;
	int cnt = ((threads_args *) args) -> cnt;

	if (cnt>1){
		
		int k = cnt/2;
	
		if (A_threads < N_threads) {
			threads_args argsT1, argsT2;
			pthread_t t1, t2;			
			
			argsT1.lo=lo;
			argsT1.cnt=k;
			argsT1.dir=ASCENDING;
			
			pthread_mutex_lock (&key);
			A_threads+=2;
		    pthread_mutex_unlock (&key);

			
			argsT2.lo=lo+k;
			argsT2.cnt=k;
			argsT2.dir=DESCENDING;
			
			pthread_create(&t1, NULL, pthBitonicSort, &argsT1);
			pthread_create(&t2, NULL, pthBitonicSort, &argsT2);

			pthread_join(t1, NULL);
			pthread_join(t2, NULL);	
			
			pthread_mutex_lock (&key);
			A_threads+=-2;
		    pthread_mutex_unlock (&key);
			
		}
		else{			
			recBitonicSort(lo, k, ASCENDING);
			recBitonicSort(lo+k, k, DESCENDING);	
		}
		
		threads_args argsM;
		
		argsM.lo=lo;
		argsM.cnt=cnt;
		argsM.dir=dir;
		
		pthBitonicMerge(&argsM);            
	}
}

/* --------------------------------- openMP  --------------------------------- */ 


// openMP imperative version of bitonic sort
void ompBitonicSort() {

  int i,j,k;
  omp_set_num_threads(N_threads);  

  for (k=2; k<=N; k=2*k) {
    for (j=k>>1; j>0; j=j>>1) {
	#pragma omp parallel for schedule(dynamic, N/N_threads) 
      for (i=0; i<N; i++) {
		int ij=i^j;
		if ((ij)>i) {
			if ((i&k)==0 && a[i] > a[ij]) 
				exchange(i,ij);
			if ((i&k)!=0 && a[i] < a[ij])
				exchange(i,ij);
		}
	  }
    }
  }
}
