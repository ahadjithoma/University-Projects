#include <stdio.h>
#define max 20
#include <math.h>
#include <process.h>

void main()
{
    float X[max], Y[max],R, sumx=0, sumy=0, Kx, Ky, min_dis, max_dis=0, dist=0, Xp, Yp, ploio, p_u;
    int num, i, cnt, f=0;

    /**********Eisagwgi dedomenwn************/
    do {
        printf("Dwse ton arithmo twn ufalwn: ");
        scanf("%d",&num);
        if (num>20)
            printf("To programma den leitourgei gia arithmo ufalwn panw apo 20.\n");
        else if (num<0)
            printf("O arithmos twn ufalwn prepei na einai thetikos\n");
        }
    while (num>20 || num<0);

    if (num != 0){
        printf("Dwse tis suntetagmenes twn ufalwn");
    for(i=0; i<num; i++){
            printf("\nDwse X ufalou: ");
            scanf("%f",&X[i]);
            printf("Dwse Y ufalou: ");
            scanf("%f",&Y[i]);
        }

    printf("\nDwse tin apostasi asfaleias: ");
    scanf("%f", &min_dis);

    /*************Ypologismos Kentrou***********/

    for (i=0; i<num; i++) {
        sumx = sumx + X[i];
        sumy = sumy + Y[i];
    }
    Kx=sumx/num;
    Ky=sumy/num;

    printf("\nTo kentro tis epikindinis perioxis einai: (%.2f, %.2f)\n\n", Kx, Ky);

    /***********Ypologismos max_dis************/
    for(i=0;i<num;i++){
        dist=sqrt(pow((X[i]-Kx),2)+pow((Y[i]-Ky),2));
        if(dist>max_dis) {
            max_dis=dist;
        }
    }

    R=min_dis+max_dis; /**Aktina Kuklou**/

    /***********suntetagmenes ploiou + elegxos poreias***********/
    for (i;;){
        printf("\nDwse tis suntetagmenes tou ploiou.");
        printf("\nDwse to X: ");
        scanf("%f",&Xp);
        printf("Dwse to Y: ");
        scanf("%f",&Yp);
        ploio = sqrt(pow((Xp-Kx),2)+pow((Yp-Ky),2));
        if (ploio<=R) {
            f=1;
            cnt=0;
            for (i=0; i<num; i++){
                p_u = sqrt(pow((Xp-X[i]),2)+pow((Yp-Y[i]),2));
                if (p_u<=min_dis){
                    cnt+=1;
                    printf("Apostasi ufalou me sintetagmenes (%.2f, %.2f) apo ploio einai: %.2f\n",X[i], Y[i], p_u);
                }
            }
        printf("=>Yparxoun %d ufaloi stin epikindini zwnh\n", cnt);
        }
        else if (ploio>R && f==1) {
            printf("To ploio vgike apo tin epikindini zwni");
            break;
    }}}
    else {
        printf("Den uparxoun ufaloi");
        exit(1);
        }
}
