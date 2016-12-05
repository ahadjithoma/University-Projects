#include <stdio.h>
#define MAX 5

main()
{
    int adj[MAX][MAX]={0}, weight[MAX][MAX]={0}, T[MAX]={0};
    int i, j, K, newK, sumW=0, minW, f, f_adj;


    /************Eisagwgi dedomenwn************/

    printf("0: NO\n1: YES\n");
    for (i=0; i<MAX; i++){
        for (j=0; j<MAX; j++){
            if (i != j && i<j){
                do{
                printf("\nstis korifes (%d,%d)  uparxei akmi?  " , i,j);
                scanf("%d", &adj[i][j]);
                adj[j][i] = adj[i][j];  /**(Simmetrikos)**/
                if (adj[i][j]>1 || adj[i][j]<0)
                    printf("Dwse swsti timi\n");
                }while (adj[i][j]>1 || adj[i][j]<0); /**Elegxos swstis eisagwgis dedomenwn**/


                if (adj[i][j]==1){
                    printf("Dwse to varos tis akmis (%d,%d): ", i,j); /**Eisagwgi Varous**/
                    do{
                        scanf("%d", &weight[i][j]);
                        if (weight[i][j]>=1000 || weight[i][j]<=0)    /**Elegxos Varous<1000**/
                            printf("Dwse  varos metaksi 1-1000: ");
                    }while (weight[i][j]>=1000 || weight[i][j]<=0);
                    weight[j][i]=weight[i][j];   /**(Simmetrikos)**/
                }
            }
        }
    }

    /***************Ektiposi Pinakwn***************/
    printf("\nPINAKAS adj\n");        /**ADJ**/
    for (i=0; i<MAX; i++){
            for (j=0; j<MAX; j++){
                printf("  (%d, %d): ",i,j);
                printf("%d", adj[i][j]);
            }
        printf("\n\n");
    }
    printf("\nPINAKAS weight\n");    /**WEIGHT**/
    for (i=0; i<MAX; i++){
            for (j=0; j<MAX; j++){
                printf("  (%d, %d): ",i,j);
                printf("%d", weight[i][j]);
            }
        printf("\n\n");
    }

    /************Efarmogi Algorithmou***********/
    do{       /**Eisodos Korifis**/
        printf("\n\nDwse tin korifi pou thelis na ksekinisis: ");
        scanf("%d", &K); printf("%d  ",K);
        if (K>=MAX || K<0)
            printf("\nDen uparxei tetoia korifi");
    } while (K>=MAX || K<0);
    T[K]=1;

    /**Algorithmos**/
    do{
        f_adj=0;
        minW=1000;

        for (j=0; j<MAX; j++){
            if (adj[K][j]==1){
                f_adj=1;
                if (weight[K][j]<minW){
                    minW=weight[K][j];
                    newK=j;
                }   /**if**/
            }       /**if**/
        }           /**for j**/
        T[newK]=1;

       if (f_adj){
            sumW=minW+sumW; /**Sinoliko Varos**/
            adj[K][newK]=0; adj[newK][K]=0; /**Midenismos akmwn**/
            printf("%d  ", newK);
            K=newK;
       }

        f=0;
        for (i=0; i<MAX; i++)
            if (T[i])
                f=f+1;

        } while(f<MAX && f_adj);

    printf("\nTo sinoliko Varos einai %d", sumW);



    /*******Elegxos oloklirosis diadromis*******/

    if (f==MAX)
        printf("\nH diadromi oloklirothike");
    else if (f!=MAX)
        printf("\nH diadromi den oloklirothike\n\n");

    printf("\n\n");

}/*Main*/
