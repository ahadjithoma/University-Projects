/*
    k-nn Search Algorithm (MPI Parallel)

    Created by: Hadjithoma Andreas
                antreasc@auth.gr || andreas_h92@hotmail.com
                January 2016
*/



#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <mpi.h>

float randZeroToOne();
void print_Q_C();
void initPoints();
void initSubCubes();
void sortPointsInSubCubes();
int limits(int x, int y, int z);
void knnSearch();
float distance(float a1, float a2, float a3, float b1, float b2, float b3);
void bruteForceMethod();
int test();
void proc_print_Q_C();
void mpiImplement();


//----------------------------------------------------------------------------------------//

struct timeval startwtime, endwtime;
double knn_seq_time, bruteForce_seq_time, knn_time_final;

struct minD{
    float *knn, *brt;
}min;

struct subCubes{
    int cntQ, cntC;
    int *dispQ, *scountsQ, *rcountsQ, *dispC;
    int *scountsC, *rcountsC;
    int *pntAddrQ; //points to the position of the point in p.pointsQ[][] array
    int *pntAddrC;
}***sC, ***procSC;

struct cube{
    int n,m,k,P;
    int subCubesNum;
    float mStep, nStep, kStep;
}c;

struct points{
    int N;
    float  *pointsQx, *pointsQy, *pointsQz;
    float  *pointsCx, *pointsCy, *pointsCz;
}p, procPnt;

int pID, nProcs, mpi_error;
int buf, recv_buf, send_buf;
float bufF;

//----------------------------------------------------------------------------------------//

void main(int argc, char** argv)
{   
    int i,j,k,l,h;
    
    /* Initialize MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pID);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcs);
    srand(time(NULL)*(unsigned)(pID+1));

    /* check arguments */
    if (argc!=5 && pID==0){      
        printf("Enter: ./<name> N n m k\n");
        exit(0);
    }
    p.N = 1<<atoi(argv[1]);
    c.n = 1<<atoi(argv[2]);
    c.m = 1<<atoi(argv[3]);
    c.k = 1<<atoi(argv[4]);

    c.subCubesNum=c.m*c.n*c.k;
    c.nStep=1.0/c.n;
    c.mStep=1.0/c.m;
    c.kStep=1.0/c.k;
    min.knn = (float *)malloc(p.N*sizeof(float));
    min.brt = (float *)malloc(p.N*sizeof(float));
    if (pID==0) {
        //printf("N = 2^%d \nn = 2^%d \nm = 2^%d \nk = 2^%d \n", atoi(argv[1]),atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
        printf("%d\t%d\t%dx%dx%d" ,nProcs, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
    }

    /**knn MPI**/

    initSubCubes();

    initPoints();

    MPI_Barrier(MPI_COMM_WORLD);     
    
    if (pID==0) gettimeofday (&startwtime, NULL);
    
    sortPointsInSubCubes();

    MPI_Barrier(MPI_COMM_WORLD);     

    /* print points for check */
    /*
    printf("process#%d\n", pID); 
    print_Q_C();

    MPI_Barrier(MPI_COMM_WORLD);    
    */

    mpiImplement();
    
    MPI_Barrier(MPI_COMM_WORLD);

    //if (pID==0) printf("Done initializing.. (all processes)\n");

    knnSearch();

    MPI_Barrier(MPI_COMM_WORLD); 

    if (pID==0) {
        gettimeofday (&endwtime, NULL);
        knn_seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
        printf("\tKNN TIME:\t%f\n",knn_seq_time);
    }

    MPI_Barrier(MPI_COMM_WORLD); 

    /* Brute Force method */
    /*
    if (pID==0) {
        gettimeofday (&startwtime, NULL);
        bruteForceMethod();
        gettimeofday (&endwtime, NULL);
        bruteForce_seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6 + endwtime.tv_sec - startwtime.tv_sec);
        printf("BRUTE FORCE TIME: %f\n", bruteForce_seq_time);
        test();    
    }
    */
    
    MPI_Finalize();

}

//----------------------------------------------------------------------------------------//

/*  Function: randZeroToOne() 
    returns a float number 
    from 0 to 1 [0.0, 0.1]
*/
float randZeroToOne()
{   
    return (((long double)rand()+1)/((long double)RAND_MAX+1));
}

//----------------------------------------------------------------------------------------//

/*  Function: print_Q_C() 
    prints all Q and C points() 
*/

void print_Q_C()
{
    int i,j;
    printf(" |\t-------------Q------------|*|-------------C-------------\n");
    printf(" |\t      x      y      z     |*|      x      y      z      \n");
    for (i=0; i<p.N/nProcs; i++){
        printf("%d|\t",i);
        printf("%lf|%lf|%lf|",p.pointsQx[i], p.pointsQy[i], p.pointsQz[i]);
        
        printf("*|");
        printf("%lf|%lf|%lf|",p.pointsCx[i], p.pointsCy[i], p.pointsCz[i]);
        
        printf("\n");
    }
}


//----------------------------------------------------------------------------------------//

/*  Function: initPoints()
    Allocates Q and C Tables and initialize 
    them with random numbers (0,1] using randZeroToOne() 
*/
void initPoints()
{
    int i,j;
    
    p.pointsQx=(float *)malloc(p.N*sizeof(float ));
    p.pointsCx=(float *)malloc(p.N*sizeof(float ));
    p.pointsQy=(float *)malloc(p.N*sizeof(float ));
    p.pointsCy=(float *)malloc(p.N*sizeof(float ));
    p.pointsQz=(float *)malloc(p.N*sizeof(float ));
    p.pointsCz=(float *)malloc(p.N*sizeof(float ));
    procPnt.pointsQx=(float *)malloc(p.N*sizeof(float ));
    procPnt.pointsCx=(float *)malloc(p.N*sizeof(float ));
    procPnt.pointsQy=(float *)malloc(p.N*sizeof(float ));
    procPnt.pointsCy=(float *)malloc(p.N*sizeof(float ));
    procPnt.pointsQz=(float *)malloc(p.N*sizeof(float ));
    procPnt.pointsCz=(float *)malloc(p.N*sizeof(float ));

    for (i=0; i<p.N/nProcs; i++){
        p.pointsQx[i]=randZeroToOne();
        p.pointsCx[i]=randZeroToOne();
        p.pointsQy[i]=randZeroToOne();
        p.pointsCy[i]=randZeroToOne();
        p.pointsQz[i]=randZeroToOne();
        p.pointsCz[i]=randZeroToOne();
        
    }

}

//----------------------------------------------------------------------------------------//

/*  Function: initSubCubes()
    Allocates all the subcubes of the whole Cube
    and all the necessary variables
*/
void initSubCubes()
{
    int i,j,k,l, cubesPerProc, nmkPerProc;

    /* Allocate subCubes */
    sC = (struct subCubes ***)malloc(c.n*sizeof(struct subCubes **));
    procSC = (struct subCubes ***)malloc(c.n*sizeof(struct subCubes **));
    for (i=0; i<c.n; i++) {
        sC[i] = (struct subCubes **) malloc(c.m*sizeof(struct subCubes *));
        procSC[i] = (struct subCubes **) malloc(c.m*sizeof(struct subCubes *));
        for (j=0; j<c.m; j++) {
            sC[i][j] = (struct subCubes *)malloc(c.k*sizeof(struct subCubes));
            procSC[i][j] = (struct subCubes *)malloc(c.k*sizeof(struct subCubes));
        }
    }

    for (i=0; i<c.n; i++) {
        for (j=0; j<c.m; j++) {
            for (k=0; k<c.k; k++) {
                sC[i][j][k].cntQ=0;
                sC[i][j][k].cntC=0;
                sC[i][j][k].pntAddrQ = (int *)malloc(1*sizeof(int));
                sC[i][j][k].pntAddrC = (int *)malloc(1*sizeof(int));
                sC[i][j][k].dispQ = (int *)malloc(nProcs*sizeof(int));
                sC[i][j][k].dispC = (int *)malloc(nProcs*sizeof(int));

                sC[i][j][k].scountsQ = (int *)malloc(nProcs*sizeof(int));
                sC[i][j][k].rcountsQ = (int *)malloc(nProcs*sizeof(int));
                sC[i][j][k].scountsC = (int *)malloc(nProcs*sizeof(int));
                sC[i][j][k].rcountsC = (int *)malloc(nProcs*sizeof(int));

                sC[i][j][k].dispQ[0] = 0; 
                sC[i][j][k].dispC[0] = 0; 
            }
        }
    }
}


//----------------------------------------------------------------------------------------//

/*  Function: sortPointsInSubCubes() 
    Sorts all the points to the specific subcube that they belong
*/
void sortPointsInSubCubes()
{
    int i,j,k,l,h, flag,
        cubeQx, cubeQy, cubeQz,
        cubeCx, cubeCy ,cubeCz,
        cntQ, cntC;
    float lo, hi;

    for (l=0; l<p.N/nProcs; l++){///Scan all the points and find their coordinates
        cntQ=0;
        cntC=0;

        /* sub-cube n-axis (x) */
        lo=0.000000;
        hi=c.nStep;
        flag=0;
        for (i=0; i<c.n; i++){
            if (p.pointsQx[l]>lo && p.pointsQx[l]<hi){
                cubeQx=i; //keep sub-cube coordinates
                flag=1;
            }
            if (p.pointsCx[l]>lo && p.pointsCx[l]<hi){
                cubeCx=i; //keep sub-cube coordinates
                if (flag) break;
            }
            lo+=c.nStep;
            hi+=c.nStep;
        }

        /* sub-cube m-axis (y) */
        lo=0.000000;
        hi=c.mStep;
        flag=0;
        for (i=0; i<c.m; i++){
            if (p.pointsQy[l]>lo && p.pointsQy[l]<hi){
                cubeQy=i; //keep sub-cube coordinates
                flag=1;
            }
            if (p.pointsCy[l]>lo && p.pointsCy[l]<hi){
                cubeCy=i; //keep sub-cube coordinates
                if (flag) break;
            }
            lo+=c.mStep;
            hi+=c.mStep;
        }

        /* sub-cube k-axis (z) */
        lo=0.000000;
        hi=c.kStep;
        flag=0;
        for (i=0; i<c.k; i++){
            if (p.pointsQz[l]>lo && p.pointsQz[l]<hi){
                cubeQz=i; //keep sub-cube coordinates
                flag=1;
            }
            if (p.pointsCz[l]>lo && p.pointsCz[l]<hi){
                cubeCz=i; //keep sub-cube coordinates
                if (flag) break;
            }
            lo+=c.kStep;
            hi+=c.kStep;
        }

        cntQ = sC[cubeQx][cubeQy][cubeQz].cntQ; //for more ease
        cntC = sC[cubeCx][cubeCy][cubeCz].cntC;

        sC[cubeQx][cubeQy][cubeQz].pntAddrQ[cntQ]=l+(pID*p.N)/nProcs; //save the position of the point
        sC[cubeCx][cubeCy][cubeCz].pntAddrC[cntC]=l+(pID*p.N)/nProcs;

        sC[cubeQx][cubeQy][cubeQz].cntQ++; //increase points counter of the specific sub-cube
        sC[cubeCx][cubeCy][cubeCz].cntC++;

        sC[cubeQx][cubeQy][cubeQz].pntAddrQ=(int *)realloc(sC[cubeQx][cubeQy][cubeQz].pntAddrQ, (cntQ+2)*sizeof(int)); //reallocate the array
        sC[cubeCx][cubeCy][cubeCz].pntAddrC=(int *)realloc(sC[cubeCx][cubeCy][cubeCz].pntAddrC, (cntC+2)*sizeof(int));
    }
}

//----------------------------------------------------------------------------------------//

/*  Function: mpiImplement()
    Does all the necessary processes communications
*/ 

void mpiImplement()
{
    int i,j,k,l,h;
    int *displacement, *rcounts, *scounts;

    for (i=0; i<c.n; i++) {
        for (j=0; j<c.m; j++) {
            for (k=0; k<c.k; k++) {
                sC[i][j][k].scountsQ[pID] = sC[i][j][k].cntQ;
                sC[i][j][k].scountsC[pID] = sC[i][j][k].cntC;
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);     
 
    /* tell the other processors how much data is coming */ 

    for (i=0; i<c.n; i++) {
        for (j=0; j<c.m; j++) {
            for (k=0; k<c.k; k++) {                
                    MPI_Allgather(&sC[i][j][k].scountsQ[pID],1,MPI_INT, sC[i][j][k].rcountsQ,1,MPI_INT,MPI_COMM_WORLD);
                    MPI_Allgather(&sC[i][j][k].scountsC[pID],1,MPI_INT, sC[i][j][k].rcountsC,1,MPI_INT,MPI_COMM_WORLD);
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD); 

    /* calculate displacements and the size of the arrays */

    for (i=0; i<c.n; i++) {
        for (j=0; j<c.m; j++) {     
            for (k=0; k<c.k; k++) {
                sC[i][j][k].dispQ[0]=0;
                for (h=1; h<nProcs; h++){
                    sC[i][j][k].dispQ[h] = sC[i][j][k].rcountsQ[h-1] + sC[i][j][k].dispQ[h-1];
                    sC[i][j][k].dispC[h] = sC[i][j][k].rcountsC[h-1] + sC[i][j][k].dispC[h-1];
                }
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);   

    /* Get the total cnt of points from each processor */

    for (i=0; i<c.n; i++) {
        for (j=0; j<c.m; j++) {
            for (k=0; k<c.k; k++) { 
                MPI_Allreduce(&sC[i][j][k].cntQ, &procSC[i][j][k].cntQ, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
                MPI_Allreduce(&sC[i][j][k].cntC, &procSC[i][j][k].cntC, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
            }
        }
    }    

    MPI_Barrier(MPI_COMM_WORLD);     

    /* Allocate the total cnt for each processor */

    for (i=0; i<c.n; i++) {
        for (j=0; j<c.m; j++) {
            for (k=0; k<c.k; k++) {
                procSC[i][j][k].pntAddrQ = (int *)malloc((procSC[i][j][k].cntQ)*sizeof(int));
                procSC[i][j][k].pntAddrC = (int *)malloc((procSC[i][j][k].cntC)*sizeof(int));
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);    

    /* Send pntAddr to all processors */


    for (i=0; i<c.n; i++) {
        for (j=0; j<c.m; j++) {
            for (k=0; k<c.k; k++) {               
                MPI_Allgatherv(sC[i][j][k].pntAddrQ, sC[i][j][k].cntQ , MPI_INT, 
                    procSC[i][j][k].pntAddrQ, sC[i][j][k].rcountsQ, sC[i][j][k].dispQ, MPI_INT, MPI_COMM_WORLD);
                MPI_Allgatherv(sC[i][j][k].pntAddrC, sC[i][j][k].cntC , MPI_INT, 
                    procSC[i][j][k].pntAddrC, sC[i][j][k].rcountsC, sC[i][j][k].dispC, MPI_INT, MPI_COMM_WORLD);
                    
            }
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    /* Calculate displacement for passing points table */

    displacement = (int *)malloc(nProcs*sizeof(int));
    rcounts = (int *)malloc(nProcs*sizeof(int));
    scounts = (int *)malloc(nProcs*sizeof(int));
    
    displacement[0] = 0;
    rcounts[0]=p.N/nProcs;
    scounts[0]=p.N/nProcs;
    for (i=1; i<nProcs; i++){
        displacement[i]=displacement[i-1]+p.N/nProcs;
        rcounts[i]=p.N/nProcs;
        scounts[i]=p.N/nProcs;
    }   

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Allgatherv(p.pointsQx, p.N/nProcs, MPI_FLOAT, procPnt.pointsQx, rcounts, displacement, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Allgatherv(p.pointsQy, p.N/nProcs, MPI_FLOAT, procPnt.pointsQy, rcounts, displacement, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Allgatherv(p.pointsQz, p.N/nProcs, MPI_FLOAT, procPnt.pointsQz, rcounts, displacement, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Allgatherv(p.pointsCx, p.N/nProcs, MPI_FLOAT, procPnt.pointsCx, rcounts, displacement, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Allgatherv(p.pointsCy, p.N/nProcs, MPI_FLOAT, procPnt.pointsCy, rcounts, displacement, MPI_FLOAT, MPI_COMM_WORLD);
    MPI_Allgatherv(p.pointsCz, p.N/nProcs, MPI_FLOAT, procPnt.pointsCz, rcounts, displacement, MPI_FLOAT, MPI_COMM_WORLD);
}

//----------------------------------------------------------------------------------------//

/*  Function: knnSearch()
    Impliments the knn Search Algorithm (mpi implimentation)
*/
void knnSearch()
{
    int i,j,k, l, r, h, p=0, ii, jj, kk, cntQ, cntC, cntQQ, cntCC, pnt1Pos, pnt2Pos;
    float minDis=1, newDis=1;
    int minPos, cnt;
    /* 3 for's -> scan every available cube */

    for (i=0; i<c.n; i++){
        for(j=0; j<c.m; j++){
            for(k=pID*c.k/nProcs; k<(pID+1)*c.k/nProcs; k++){
                //printf("\n----CUBE %d,%d,%d----\n",i,j,k);

                /* the number of points in cube (i,j,k) */
                cntQ=procSC[i][j][k].cntQ;

                /* scan every point in cube (i,j,k) and find the closest neighbour/C-point */
                for(h=0; h<cntQ; h++){
                    pnt1Pos=procSC[i][j][k].pntAddrQ[h]; /* Array position of the q point */
                    cnt=0;
                    //printf("-\n");

                    /* 3 for's -> compare the point h with all points from the neighbouring/sidestream cubes */
                    for(ii=-1; ii<=1 ; ii++){
                        for(jj=-1; jj<=1; jj++){
                            for(kk=-1; kk<=1; kk++){

                                /* if statement: if cube is a neighbouring/sidestream cube */
                                if (limits((ii+i), (jj+j), (kk+k))==1){
                                        //printf("%d. %d,%d,%d\n",cnt, ii+i,jj+j,kk+k);
                                        cnt++;


                                    /* find the minimum Distance */
                                    cntCC=procSC[ii+i][jj+j][kk+k].cntC;  /* Νumber of the Q points to the specific sidestream subcube */

                                    for (r=0; r<cntCC; r++){
                                        pnt2Pos=procSC[ii+i][jj+j][kk+k].pntAddrC[r];
                                        newDis = distance(procPnt.pointsQx[pnt1Pos], procPnt.pointsQy[pnt1Pos], procPnt.pointsQz[pnt1Pos], 
                                            procPnt.pointsCx[pnt2Pos], procPnt.pointsCy[pnt2Pos], procPnt.pointsCz[pnt2Pos]);
                                        if (newDis<minDis){
                                            minDis=newDis;
                                            minPos=pnt2Pos;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    min.knn[pnt1Pos]=minDis;
                    /* print point Result */
                    /*printf("#%dpoint_%d (%f,%f,%f) it's close to pointC_%d (%f,%f,%f) -> Distance = %f\n", pID, pnt1Pos,
                        procPnt.pointsQx[pnt1Pos],procPnt.pointsQy[pnt1Pos],procPnt.pointsQz[pnt1Pos], minPos,
                            procPnt.pointsCx[minPos],procPnt.pointsCy[minPos],procPnt.pointsCz[minPos], minDis);
                    */
                    minDis=1;
                }
            }
        }
    }
}

//----------------------------------------------------------------------------------------//

/*  Function: distance() 
    Calculates the distance between to points(x,y,z)
*/
float distance(float a1, float a2, float a3, float b1, float b2, float b3)
{
    float dis;

    dis = sqrt(pow(b1-a1,2)+pow(b2-a2,2)+pow(b3-a3,2));
    return dis;
}

//----------------------------------------------------------------------------------------//

/*  Function: limits() 
    checks if a (x,y,z) subCube coordinate  exceeds or not the whole Cube limit  
*/
int limits(int x, int y, int z)
{
    if (x < 0 || y < 0 || z < 0)
        return 0;
    if (x >= c.n || y >= c.m || z >= c.k)
        return 0;

    return 1;
}

//----------------------------------------------------------------------------------------//

/*  Function: bruteForceMethod()
    Impliments the knn search algorithm with brute force method: Everything with everything
*/ 
void bruteForceMethod()
{
    int i,j,k, minPosition;
    float minDistance=1;
    float disTemp;

    for (i=0; i<p.N; i++){
        minDistance=1;

        for (j=0; j<p.N; j++){
            disTemp=distance(procPnt.pointsQx[i], procPnt.pointsQy[i], procPnt.pointsQz[i], 
                procPnt.pointsCx[j], procPnt.pointsCy[j], procPnt.pointsCz[j]);
            if (disTemp < minDistance){
                    minDistance=disTemp;
                    minPosition=j;
            }
        }
        min.brt[i]=minDistance;        
        /* print point Result */
        /*printf("point_%d (%f,%f,%f) it's close to pointC_%d (%f,%f,%f) -> Distance = %f\n",i,
            procPnt.pointsQx[i],procPnt.pointsQy[i],procPnt.pointsQz[i], minPosition,
                procPnt.pointsCx[minPosition],procPnt.pointsCy[minPosition],procPnt.pointsCz[minPosition], minDistance);
        */
    }
}

//----------------------------------------------------------------------------------------//

/*  Function: test()
    checks if mpi and brute-force impliments have the same results
*/ 
int test()
{
    int i;
    printf("TESTING.......\n");

    for (i=0; i<p.N; i++){
        if (min.brt[i]!=min.knn[i] && min.knn[i]!=0){
            printf("\nFAIL\n");
            return 0;
        }
    }
    printf("\nPASS\n");
    return 1;
}
