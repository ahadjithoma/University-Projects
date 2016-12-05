#include <iostream>
#include <stdlib.h>

using namespace std;

class circuit{
public:
    virtual void terminals(int a, int b, int &c, int &d)=0;
    virtual float power(int a, int b)=0;
};

/******************* CIRCUIT A ******************/
class A: public circuit{
public:
    void terminals(int a, int b, int &c, int &d);
    float power(int a, int b);
};

void A::terminals(int a, int b, int &c, int &d){

    if (a && b){
        c=1;
        d=1;
    }
    if (!a && !b){
        c=0;
        d=0;
    }
    if (a && !b){
        c=0;
        d=1;
    }
    if (!a && b){
        c=0;
        d=1;
    }
}

float A::power(int a, int b){
    if (a && b)
        return 2.0;
    if (!a && !b)
        return 0;
    if (a && !b)
        return 1.0;
    if (!a && b)
        return 1.0;
}
/******************* CIRCUIT B ******************/
class B: public circuit{
public:
    void terminals(int a, int b, int &c, int &d);
    float power(int a, int b);
};

void B::terminals(int a, int b, int &c, int &d){
    if (a && b){
        c=0;
        d=1;
    }
    if (!a && !b){
        c=0;
        d=0;
    }
    if (a && !b){
        c=1;
        d=1;
    }
    if (!a && b){
        c=0;
        d=1;
    }
}

float B::power(int a, int b){
    if (a && b)
        return 2.5;
    if (!a && !b)
        return 0.5;
    if (a && !b)
        return 1.5;
    if (!a && b)
        return 1.5;
}
/**************** CIRCUIT CALC **************/
float calc_circuit(int a, int b, int &c, int &d, int num, circuit **Cr){
    int i;
    float power=0;

    for (i=0; i<num; i++){
        Cr[i]->terminals(a, b, c, d);
        a=c;
        b=d;
        power+=Cr[i]->power(a,b);
    }
    return power;
}

/******************* MAIN ******************/
main(){
    int at, bt, ct, dt, numA, numB, num, i, j, opt, cntA=0, cntB=0;
    float power;
    circuit **Cr;
    A *a;
    B *b;
    cout<<"Arithmos stoixeiwdi kuklwmatwn tupou A: ";
    cin>>numA;
    cout<<"Arithmos stoixeiwdi kuklwmatwn tupou B: ";
    cin>>numB;
    num=numA+numB;
    a = new A [numA];
    b = new B [numB];
    Cr = new circuit *[num];

    i=0;
    do {
        if (!numA && !numB)
            exit(10);
        cout<<"Dwse ton tupo tou kuklwmatos pou tha sundethei sthn thesi "<<i+1<<endl;
        cout<<"1. A"<<endl;
        cout<<"2. B"<<endl;
        cin>>opt;

        if (opt==1){
            if (cntA>=numA){
                cout<<"Ta kuklomata tupou A teleiwsan..\n";
                continue;
            }
            Cr[i] = &a[cntA];
            i++;
            cntA++;
        }
        if (opt==2){
            if (cntB>=numB){
                cout<<"Ta kuklwmata tupou B teleiwsan..\n";
                continue;
            }
            Cr[i] = & b[cntB];
            i++;
            cntB++;
        }

    } while (i<num);

    for (i=0; i<2; i++){
        for (j=0; j<2; j++){
            at=i;
            bt=j;
            power=calc_circuit(at, bt, ct, dt, num, Cr);
            cout<<endl<<"--------"<<endl;
            cout<<"for a = "<<at;
            cout<<" and b = "<<bt<<endl;
            cout<<"power = "<<power<<" mwatt"<<endl;
            cout<<"c = "<<ct<<endl;
            cout<<"d = "<<dt<<endl;
        }
    }
    delete a;
    delete b;
    delete Cr;
}
