#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void next_vertex(int **connection,float **weights,int *edges,int k,int num,int *flags,float sumw);

void main()
{

    int k,num,i,j,*edges, *flags, **connection;
    float **weights, sumw=0;

    printf("Dwse ton arithmo twn korufwn: ");
    scanf("%d", &num);

    /**Elegxos diathesimis Mnimis**/
    if ((edges=(int *) malloc (num*sizeof(int)))==NULL){
        printf("Den uparxei arketi mnimi gia thn ektelesh tou programmatos..");
        exit(0);
    }
    if ((connection=(int **) malloc (num*sizeof(int *)))==NULL){
        printf("Den uparxei arketi mnimi gia thn ektelesh tou programmatos..");
        exit(0);
    }
    if ((weights=(float **) malloc(num*sizeof(float *)))==NULL){
        printf("Den uparxei arketi mnimi gia thn ektelesh tou programmatos..");
        exit(0);
    }
    if ((flags=(int *)malloc(num*sizeof(int)))==NULL){
        printf("Den uparxei arketi mnimi gia thn ektelesh tou programmatos..");
        exit(0);
    }


    for (i=0;i<num; i++){
        printf("Dwse ton arithmo korifwn pou sindeonte me tin korifi %d: ",i); /**arithmos korifwn pou sindeonte metaksi tous**/
        scanf("%d", &edges[i]);
        if ((weights[i]=(float *) malloc(edges[i]*sizeof(float)))==NULL){
            printf("Den uparxei arketi mnimi gia thn ektelesh tou programmatos..");
            exit(0);
        }
        if ((connection[i]=(int *)malloc(edges[i]*sizeof(int)))==NULL){
            printf("Den uparxei arketi mnimi gia thn ektelesh tou programmatos..");
            exit(0);
        }
    }

    /**Eisagwgi dedomenwn**/
    for (i=0; i<num; i++){
        printf("Dwse tous arithmous twn korifwn pou sindeonte me tin %d korifi: \n", i); /**Connection**/
        for (j=0; j<edges[i]; j++){
            scanf("%d", &connection[i][j]);
        }
    }

    for (i=0; i<num; i++){
        printf("Dwse ta varoi gia tin korifi %d: \n", i);                               /**Weights**/
        for (j=0; j<edges[i]; j++){
            printf("varos (%d,%d): ",i, connection[i][j]);
            scanf("%f", &weights[i][j]);
        }
    }

    printf("Dwse tin korifi ekkinisis: ");
    scanf("%d", &k);
    printf("Monopati: %d ",k);
    flags[k]=1;
    next_vertex(connection, weights, edges, k, num, &flags, sumw);

}

void next_vertex(int **connection,float **weights,int *edges,int k,int num,int *flags, float sumw)
{
    int newk,cnt,i,j;
    float minw=10000, m, sum;

    for (j=0; j<edges[k]; j++) {
        if (connection[k][j]!=-1)
            if (weights[k][j]<minw){
                minw=weights[k][j];
                newk=connection[k][j];
        }
    }
    sumw+=minw;
    for (i=0; i<num; i++) {
        for (j=0; j<edges[i]; j++) {
            if (connection[i][j]==k) {
                connection[i][j]=-1;
            }
        }
    }


    flags[newk]=1;
    k=newk;
    printf(" %d", k);

    for (j=0; j<edges[k]; j++) {
        sum+=connection[k][j];
    }

    for (i=0;i<num; i++){
        if (flags[i]==1)
            cnt++;
    }

    if (cnt==num) {
        printf(". Kostos %f. ",sumw);
        printf("To monopati oloklhrw8hke.\n");
    }
    else if (sum==(-1)*edges[k]) {
        printf(". Kostos %f. ",sumw);
        printf("To monopati den oloklhrw8hke.\n");
    }
    else
    next_vertex(connection, weights, edges, k, num, flags, sumw);

}
