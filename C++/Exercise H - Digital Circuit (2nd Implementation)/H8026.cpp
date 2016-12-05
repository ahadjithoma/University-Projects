#include <stdlib.h>
#include <iostream>
using namespace std;

/*******************circuit A***********************/
class cA{
    int a,b,c,d;
    float power, R;
public:
    cA();
    float get_R(){return R;};
    void set_values(int a, int b);
    int get_c(){return c;};
    int get_d(){return d;};
    float get_power(){return power;};
};

cA::cA(){
    cout<<"Dwse tim antistash R gia to kuklwma tupou A: ";
    cin>>R;
}

void cA::set_values(int a, int b){

    if (a && b){
        c=1;
        d=1;
        power=2;
    }
    if (!a && !b){
        c=0;
        d=0;
        power=0;
    }
    if (a && !b){
        c=0;
        d=1;
        power=1;
    }
    if (!a && b){
        c=0;
        d=1;
        power=1;
    }
}
/*******************circuit B***********************/

class cB{
    int a,b,c,d;
    float power, R;
public:
    cB();
    float get_R(){return R;};
    void set_values(int a, int b);
    int get_c(){return c;};
    int get_d(){return d;};
    float get_power(){return power;};
};

cB::cB(){
    cout<<"Dwse tim antistash R gia to kuklwma tupou B: ";
    cin>>R;
}

void cB::set_values(int a, int b){

    if (a && b){
        c=0;
        d=1;
        power=2.5;
    }
    if (!a && !b){
        c=0;
        d=0;
        power=0.5;
    }
    if (a && !b){
        c=1;
        d=1;
        power=1.5;
    }
    if (!a && b){
        c=0;
        d=1;
        power=1.5;
    }
}

/***************make circuit**********************/

template <class cA, class cB>
class make_circuit{
    float *R;
    int nCA, nCB, *cir, *pos;
    cA *pA;
    cB *pB;
public:
    make_circuit();
    void cir_sort();
    int get_nC_total(){return (nCA+nCB);};
    cA *get_pA(){return pA;};
    cB *get_pB(){return pB;};
    int *get_cir(){return cir;};
    int *get_pos(){return pos;};
};

template <class cA, class cB>
make_circuit<cA, cB>::make_circuit(){
    cout<<"nCA: ";
    cin>>nCA;
    cout<<"nCB: ";
    cin>>nCB;
    pA = new cA[nCA];
    pB = new cB[nCB];

    cir=(int *)malloc((nCA+nCB)*sizeof(int));
    pos=(int *)malloc((nCA+nCB)*sizeof(int));

}

template <class cA, class cB>
void make_circuit<cA, cB>::cir_sort(){
    int i,j;
    float temp_float;
    float temp_int;
    if ((R=((float *)malloc((nCA+nCB)*sizeof(float))))==NULL){
        cout<<"allocation problem";
        exit(10);
    }


    for (i=0; i<(nCA+nCB); i++){
        if (i<nCA){
            R[i]=pA[i].get_R();
            cir[i]=0;
            pos[i]=i;
        }
        else {
            R[i]=pB[i].get_R();
            cir[i]=1;
            pos[i]=i;
        }
    }

    for (i=0; i<(nCA+nCB); i++){
        for (j=i+1; j<(nCA+nCB); j++){
            if (R[i]>R[j]){
                temp_float=R[i];
                R[i]=R[j];
                R[j]=temp_float;

                temp_int=cir[i];
                cir[i]=cir[j];
                cir[j]=temp_int;

                temp_int=pos[i];
                pos[i]=pos[j];
                pos[j]=temp_int;
            }
        }
    }
}
/***************calc circuit**********************/
template<class cA, class cB>
float calc_circuit(cA *pA, cB *pB, int *pos, int *cir, int numC, int a, int b, int &c, int &d ){
    int i;
    float power=0;

    for (i=0; i<numC; i++){
        if (cir[i]==0){
            pA[pos[i]].set_values(a,b);
            c=pA[pos[i]].get_c();
            d=pA[pos[i]].get_d();
            power+=pA[pos[i]].get_power();
            a=c;
            b=d;
        }
        else if(cir[i]==1){
            pB[pos[i]].set_values(a,b);
            c=pB[pos[i]].get_c();
            d=pB[pos[i]].get_d();
            power+=pB[pos[i]].get_power();
            a=c;
            b=d;
        }
    }
    return power;
}

main(){
    int a_in,b_in,c_out,d_out;
    float p;
    make_circuit<cA,cB> c;
    c.cir_sort();
    cout<<"Dwse tin timi tis eisodou a: ";
    cin>>a_in;
    cout<<"Dwse tin timi tis eisodou b: ";
    cin>>b_in;
    p=calc_circuit(c.get_pA(), c.get_pB(),c.get_pos(), c.get_cir(),c.get_nC_total(), a_in,b_in,c_out,d_out);

    cout<<endl<<"POWER = "<<p<<endl;
    cout<<"c Out = "<<c_out<<endl;
    cout<<"d Out = "<<d_out<<endl;
}
