#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

class specialist;
class project
{
    int n;
    int *special, *days;
public:
    void set_project();
    friend void program(project *proj, specialist *spec, int &snum, int &pnum);
};

class specialist
{
    int code;
    float salary;
public:
    void set_specialist();
    friend void program(project *proj, specialist *spec, int &snum, int &pnum);


};
/****************************************************/
void project::set_project()
{
    int i,j;

    cout<<"Texnikoi pou apaitounte gia t ergo: ";
    cin>>n;

    if ((special=(int*)malloc(n*sizeof(int)))==NULL)
    {
        cout<<"Allocation problem";
        exit(1);
    }
    if ((days=(int*)malloc(n*sizeof(int)))==NULL)
    {
        cout<<"Allocation problem";
        exit(0);
    }

    for (i=0; i<n; i++)
    {
        cout<<"Kwdikos "<<i+1<<"is eidikothtas: ";
        cin>>special[i];
        cout<<"Aparaitites meres: ";
        cin>>days[i];
    }

}
/****************************************************/
void specialist::set_specialist()
{
    cout<<"kwdikos eidikotitas: ";
    cin>>code;
    cout<<"Hmerisia amoivi eidikotitas: ";
    cin>>salary;
}
/****************************************************/
void program(project *proj, specialist *spec, int &snum, int &pnum)
{
    int i,j,k, *fltbl, fl, start_d, end_d, min_d, max_d;
    float cost;

    /**Desmeusi mnimis*/
    if ((fltbl=(int *)malloc(snum*sizeof(int)))==NULL){
        cout<<"Allocation problem";
        exit(5);
    }
    for (i=0; i<snum; i++)
        fltbl[i]=0;

    cout<<"------------"<<endl;
    start_d=1;
    for (i=0; i<pnum; i++){
        min_d=fltbl[0];
        max_d=fltbl[0];
        cost=0;
        for (j=0; j<proj[i].n; j++){
            fl=0;
            for (k=0; k<snum; k++){
                if (proj[i].special[j]==spec[k].code){
                    fl=1;
                    cost+=spec[k].salary*proj[i].days[j];
                    if (fltbl[k]<min_d)
                        min_d=fltbl[k];
                    fltbl[k]+=proj[i].days[j];
                    if (fltbl[k]>max_d)
                        max_d=fltbl[k];
                    break;
                }
                if (fl==0 && k==snum-1){/**pros8iki eidikotitas*/
                    cout<<"H eidikotita "<<proj[i].special[j]<<" den uparxei"<<endl;

                    if ((spec=(class specialist*)realloc(spec, (snum+1)*sizeof(class specialist)))==NULL){
                        cout<<"Allocation problem";
                        exit(6);
                    }
                    if ((fltbl=(int *)realloc(fltbl,(snum+1)*sizeof(int)))==NULL){
                        cout<<"Allocation problem";
                        exit(7);
                    }

                    cout<<"Dwse tin imerisia amoivi tis eidikotitas "<<proj[i].special[j]<<": ";
                    cin>>spec[snum].salary;
                    cost+=spec[snum].salary*proj[i].days[j];
                    spec[snum].code=proj[i].special[j];
                    fltbl[snum]=0;
                    min_d=0;
                    fltbl[snum]=proj[i].days[j];
                    if (fltbl[snum]>max_d)
                        max_d=fltbl[snum];
                    snum++;
                    break;
                }
            }//3o for
        }//2o for
        start_d+=min_d;
        end_d=start_d+max_d;
        cout<<"*******************\n";
        cout<<"To kostos tou "<<i+1<<"ou Project einai "<<cost<<"\n";
        cout<<"H imera enarksis tou "<<i+1<<"ou Project einai "<<start_d<<" kai i imera peratwsis einai "<<end_d<<"\n";
        cout<<"*******************\n";
    }//1o for
}
/****************************************************/
main()
{
    int i,j, pnum, snum;
    project *proj;
    specialist *spec;

    cout<<"Dwse arithmo ergwn pou uparxoun: ";
    cin>>pnum;
    cout<<"Dwse aritho texnikwn pou uparxoun: ";
    cin>>snum;


    if ((proj=(class project*)malloc(pnum*sizeof(class project)))==NULL){
        cout<<"Allocation problem";
        exit(0);
    }
    if ((spec=(class specialist*)malloc(snum*sizeof(class specialist)))==NULL){
        cout<<"Allocation Problem";
        exit(1);
    }

    for (i=0; i<snum; i++)
    {
        cout<<"----------------\n";
        cout<<"Stoixeia "<<i+1<<"is eidikotitas\n";
        spec[i].set_specialist();
    }

    for (i=0; i<pnum; i++)
    {
        cout<<"----------------\n";
        cout<<"Ergo "<<i+1<<endl;
        proj[i].set_project();
    }

    program(proj, spec, snum, pnum);
}
