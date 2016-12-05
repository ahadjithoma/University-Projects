# include <stdio.h>
# include <math.h>

void nbco_or(float *vl, float *st, float *me);
void ntco_or(float *vl, float *st, float *me);


void main ()
{
    float st, vl, me, sd;
    float ptr_vl;
    float guess;

    /****Eisagwgi Dedomenwn****/

    printf("Dwse ti thesi tou stoxou: ");
    scanf("%f", &st);
    printf("Dwse ti thesi tou vlimmatos: ");
    scanf("%f", &vl);
    printf("Dwse tin megisti timi stin opoia tha allazei thesi o stoxos/vlima(me): ");
    scanf("%f", &me);
    printf("Dwse apostasi katastrofis(sd): ");
    scanf("%f", &sd);

    for (;;){

        printf("\nThesi stoxou: %.2f", st);
        printf("\nThesi vlimatos: %.2f", vl);
        if (fabs(vl-guess)<=sd){
            printf("\nBravo! Exeis katastreyei to Vlima!");
            break;
        }
        if (fabs(vl-st)<=sd){
            printf("\nto Vlima katastrepse ton stoxo..  ");
            break;
        }
        printf("\nDwse tin thesi stin opoia pisteueis oti vriskete to vlima: ");
        scanf("%f",&guess);

        ntco_or(&vl, &st, &me); /*Nea thesi stoxou*/
        nbco_or(&vl, &st, &me); /*Nea thesu vlimatos*/
    }
}


/*****Sinartisi Allagis thesis Vlimatos*****/
void nbco_or(float *vl, float *st, float *me)
{
    float dis, ch;
    int f=0, num;

    ch=*me;
    dis=fabs(*st-*vl); /*apostasi metaksi stoxou kai vlimatos*/
    do{
        num=rand();
        if (num<=ch){
            if (fabs((num+*vl)-*st)<=dis){
                *vl=fabs(num+*vl); f=1;
            }
            else if (fabs((num-*vl)-*st)<=dis){
                *vl=num-*vl; f=1;
            }
        }

    }while  (num>ch || !f);
}



/*****Sinartisi Allagis thesis Stoxou*****/

void ntco_or(float *vl, float *st, float *me)
{
    int f=0, num;
    float ch, dis;

    ch=*me/4;
    dis=fabs(*st-*vl); /*apostasi metaksi stoxou kai vlimatos*/
    do {
        num=rand();
        if (num<=ch) {
            if (fabs((num+*st)-*vl)<=dis){
                *st=num+*st;
                f=1;
            }
            else if (fabs((num-*st)-*vl)<=dis){
                *st=num-*st;
                f=1;
            }
    }
    }while (num>ch || !f);
}
