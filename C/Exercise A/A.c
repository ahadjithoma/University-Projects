#include <stdio.h>

 void main()
{
    float r1, r2, r3, r4, rtomis1, rtomis2, rn, In, Imax, V, sumA=0, sumB=0;
    int i, n, cnt=0, cntA=0, cntB=0;

    /************Eisagwgi antisewn kai elegxos diasthmatwn**************/
    do {
        printf("Dwse antistasi r1: ");
        scanf("%f",&r1);
        printf("Dwse antistasi r2: ");
        scanf("%f",&r2);
        printf("Dwse antistasi r3: ");
        scanf("%f",&r3);
        printf("Dwse antistasi r4: ");
        scanf("%f",&r4);
        if  (r1>=r3 && r2>=r4)
           printf("Parousiastike Sfalma. Prepei na isxiei: r1<r3 kai r2<r4.\nProspathise ksana!\n");
        else if (r1>=r3)
           printf("Parousiastike Sfalma. Prepei na isxiei: r1<r3.\nProspathise ksana!\n");
        else if  (r2>=r4)
           printf("Parousiastike Sfalma. Prepei na isxiei: r2<r4.\nProspathise ksana!\n");
        else if (r1<0 || r2<0 || r3<0 || r4<0)
           printf("Parousiastike Sfalma. Oi antistaseis prepei na einai thetikes.\nProspathiste ksana!\n");
        }
    while (r1>=r3 || r2>=r4 || (r1<0 || r2<0 || r3<0 || r4<0));


    /************Euresi tomis twn diasthmatwn************/
    if (r3<=r2)
        {
            rtomis1=r3;
            rtomis2=r2;
        }

    /***********Eisagwgi arithmou apothematos kai tasis V************/
    printf("\nDwse tin timi tis Tasis V: ");
    scanf("%f", &V);
    printf("\nDwse ton arithmo antistesawn pou uparxoun sto apothema: ");
    scanf("%d", &n);


    /**********Eisagwgi antistasewn, Imax kai elegxos omadwn***********/
    for (i = 1; i <= n; i++)
        {
            printf("\nDwse tin %di antistasi: ",i);
            do
                {
                    scanf("%f", &rn);
                    if (rn<0)
                            printf("H antistasi prepei na einai thetiki.\nDwse ksana tin %di antistasi: ", i);
                }
            while (rn<0);
            printf("\nDwse tin megisti entasi tis %dis antistasis: ", i);
            scanf("%f", &Imax);
            In=V/rn;
            if (In<=Imax)
                { /***Elegxos omadwn***/
                    if (rn>=rtomis1 && rn<=rtomis2) /*tomh*/
                        {
                            cnt = cnt++;
                            if (cnt%2)
                                {
                                    printf("=> H %di antistasi me timi %.2f anikei stin A omada\n\n", i,rn);
                                    cntA++;
                                    sumA=sumA+rn;
                                }
                            else
                                {
                                    printf("=> H %di antistasi me timi %.2f anikei stin B omada\n\n", i,rn);
                                    cntB++;
                                    if (sumB==0)
                                        sumB=rn;
                                    else
                                        sumB=(sumB*rn)/(sumB+rn);
                                }

                        }
                    else if (rn>=r1 && rn<=r2) /*omada A*/
                        {
                            printf("=> H %di antistasi me timi %.2f anikei stin A omada\n\n", i,rn);
                            cntA++;
                            sumA=sumA+rn;
                        }
                    else if (rn>=r3 && rn<=r4) /*omada B*/
                        {
                             printf("=> H %di antistasi me timi %.2f anikei stin B omada\n\n", i,rn);
                             cntB++;
                             if (sumB==0)
                                sumB=rn;
                             else
                                sumB=(sumB*rn)/(sumB+rn);
                        }
                    else
                        printf("=> H %di antistasi me timi %.2f DEN anikei se kapoia omada\n\n", i,rn);
                }
            else
                printf("=> H %di antistasi me timi %.2f DEN anikei se kapoia omada\n\n", i,rn);
        }

        /******Ektuposi Apotelesmatwn******/
        printf("**********************************************\n\n");
        printf("Oi antistaseis stin omada A einai: %d\n\n",cntA);
        printf("Oi antistaseis stin omada B einai: %d\n\n",cntB);
        printf("H oliki antistasi tis omadas A einai: %.2f\n\n",sumA);
        printf("H oliki antistasi tis omadas B einai: %.2f\n\n",sumB);
}
