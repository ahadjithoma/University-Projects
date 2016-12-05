#include <iostream>
#include <stdlib.h>
using namespace std;

class Vector;
class matrix{
    int r,c;
    float **m;
public:
    void operator()(int n, int k);
    int operator!(void);
    float* operator[](int k);
    int get_r_c();
    friend Vector operator*(matrix a, Vector b);
};

void matrix::operator()(int n, int k){

    int i,j;

    r=n;
    c=k;

    if ((m=(float **)malloc(r*sizeof(float*)))==NULL){
        cout<<"Allocation problem..";
        exit(0);
    }
    for (i=0; i<r; i++)
        if ((m[i]=(float *)malloc(c*sizeof(float)))==NULL){
            cout<<"Allocation problem..";
            exit(1);
        }

    for (i=0; i<r; i++)
        for (j=0; j<c; j++){
            cout<<"Stoixeio "<<i+1<<","<<j+1<<": ";
            cin>>m[i][j];
        }
}

int matrix::operator!(void){
    int i,j;
    bool *fl, fl1=1;
    float sumR, sumC;

    if (r!=c)
        return(1);
    if (r=c){
        if ((fl=(bool*)malloc(r*sizeof(bool)))==NULL){
            cout<<"Allocation problem..";
            exit(2);
        }
        for (i=0; i<r; i++)
            fl[i]=0;

        for (i=0; i<r; i++){
            sumR=0;
            sumC=0;
            for(j=0; j<c; j++){
                if (i!=j){
                    sumR+=abs(m[i][j]);
                    sumC+=abs(m[j][i]);
                }
            }
            if (abs(m[i][i])>sumR || abs(m[i][i])>sumC)
                fl[i]=1;
        }

        for (i=0; i<r; i++)
            if (fl[i]==0)
                fl1=0;

        if (fl1)
            return(0);
        else if (fl1==0)
            return(1);
    }
}

float* matrix::operator[](int k){
    return m[k]; /**an eixame return *m[k]???**/
}

int matrix::get_r_c(){

    if (r==c)
        return(r);
    else
        return(0);
}

/**************************************************************/
class Vector{
    int n;
    float *v;
public:
    void operator()(int k);
    int operator()(Vector d, float e);
    void operator=(matrix a);
    Vector operator*(Vector d);
    Vector operator-(Vector d);
    Vector operator+(Vector d);
    Vector operator/(Vector d);
    float operator[](int i);
    int get_n();
    friend Vector operator*(matrix a, Vector b);
    void vector_allocation(int k);
    void print_result();
};

void Vector::operator()(int k){

    int i;
    if((v=(float *)malloc(k*sizeof(float)))==NULL){
        cout<<"Allocation problem..";
        exit(2);
    }
    n=k;
    for (i=0; i<k; i++){
        cout<<"Stoixeio "<<i+1<<": ";
        cin>>v[i];
    }
}

int Vector::operator()(Vector d, float e){
    int i, fl=1;

    if (d.n==n){
        for (i=0; i<n; i++){
            if (abs(v[i]-d.v[i]>=e))
                fl=0;
        }
        if (fl)
            return(1);
        else if (!fl)
            return(0);
    }
    else return(0);
}

void Vector::operator=(matrix a){
    int i;
    if (a.get_r_c()==n)
        for (i=0; i<n; i++)
            v[i]=a[i][i];
    else if (a.get_r_c()==0){
        cout<<"problem: r <> c\n";
        exit(13);
    }
    else {
        cout<<"n="<<n<<endl;
        cout<<"pinakas="<<a.get_r_c()<<endl;
        cout<<"problem: size of matrix <> n\n";
        exit(14);
    }
}

Vector Vector::operator*(Vector d){
    Vector g;
    int i;

    if (n==d.n){
        if ((g.v=(float*)malloc(n*sizeof(float)))==NULL){
            cout<<"Allocation problem..";
            exit(4);
        }

        for (i=0; i<n; i++)
            g.v[i]=v[i]*d.v[i];

            g.n=n;
            return(g);
    }
    else
    {
        cout<<"ERROR Vectors size not the same";
        cout<<"\nn="<<n<<"\nd.n="<<d.n;
        exit(9);
    }
}

Vector Vector::operator-(Vector d){
    Vector g;
    int i;

    if (n==d.n){
        if ((g.v=(float*)malloc(n*sizeof(float)))==NULL){
            cout<<"Allocation problem..";
            exit(4);
        }

        for (i=0; i<n; i++)
            g.v[i]=v[i]-d.v[i];

            g.n=n;
            return(g);
    }
    else
    {
        cout<<"ERROR Vectors size not the same";
        cout<<"\nn="<<n<<"\nd.n="<<d.n;
        exit(16);
    }
}

Vector Vector::operator+(Vector d){
    Vector g;
    int i;

    if (n==d.n){
        if ((g.v=(float*)malloc(n*sizeof(float)))==NULL){
            cout<<"Allocation problem..";
            exit(4);
        }

        for (i=0; i<n; i++)
            g.v[i]=v[i]+d.v[i];

            g.n=n;
            return(g);
    }
    else
    {
        cout<<"ERROR Vectors size not the same";
        cout<<"\nn="<<n<<"\nd.n="<<d.n;
        exit(10);
    }
}

Vector Vector::operator/(Vector d){
    Vector g;
    int i;

    if (n==d.n){
        if ((g.v=(float*)malloc(n*sizeof(float)))==NULL){
            cout<<"Allocation problem..";
            exit(4);
        }

        for (i=0; i<n; i++)
            g.v[i]=v[i]/d.v[i];

            g.n=n;
            return(g);
    }
    else
    {
        cout<<"ERROR Vectors size not the same";
        exit(8);
    }
}

float Vector::operator[](int i){
    return(v[i]);
}

int Vector::get_n(){
    return(n);
}

void Vector::vector_allocation(int k){
    n=k;
    if((v=(float*)malloc(k*sizeof(float)))==NULL){
        cout<<"Allocation problem..";
        exit(12);
    }
}

void Vector::print_result(){
    int i;

    cout<<"LUSI\n";
    for (i=0; i<get_n(); i++){
        cout<<i+1<<": "<<v[i]<<endl;
    }
}
/***********************************************/
Vector operator*(matrix a, Vector b){
    Vector t;
    float temp;
    int i, j;

    if (a.get_r_c()==b.get_n()){
        if ((t.v=(float*)malloc(b.get_n()*sizeof(float)))==NULL){
            cout<<"Allocation problem..";
            exit(5);
        }
        for (i=0; i<b.get_n(); i++){
            t.v[i]=0;
            for (j=0; j<b.get_n(); j++){
                t.v[i]+=a.m[i][j]*b.v[j];
            }
        }
    t.n=b.get_n();
    return(t);
    }
    else if (a.get_r_c()==0){
        cout<<"ERROR table not square (r<>c)";
        exit(6);
    }
    else {
        cout<<"ERROR r<>n";
        exit(7);
    }
}

/************************************************/

class system_solve{
    matrix a;
    Vector b;
    int p;
public:
    system_solve(void);
    Vector solve(int k, float e);
    int get_p(){return p;};
};

system_solve::system_solve(void){
    int i,j;

    cout<<"Dwse to megethos tou Dianusmatos: ";
    cin>>i;
    p=i;
    cout<<"DIANUSMA"<<endl;
    b(i);
    cout<<"---------------"<<endl;
    cout<<"Dwse to megethos tou Pinaka: \n";
    cin>>i;
    cin>>j;
    cout<<"PINAKAS"<<endl;
    a(i,j);
    if (!a){
        cout<<"O pinakas den einai uperterwn..";
        exit(11);
    }
    cout<<"---------------"<<endl;

}

Vector system_solve::solve(int k, float e){
    Vector  xi1, xi, d;
    int cnt=0;

    d.vector_allocation(p);
    d=a;
    cout<<"Dwse tin arxiki lusi"<<endl;
    cout<<"DIANUSMA ARXIKIS LUSIS"<<endl;
    xi(k);
    xi1.vector_allocation(k);

    do{
        xi1=xi;
        /**ok till here*/
        xi=(b-a*xi+d*xi)/d;
        cnt++;
    } while((cnt<k) && (!xi1(xi,e)));
    return xi;
}
/************************************************/
main(){
    int k, vsize;
    Vector result;
    float e;

    cout<<"Dwse e: ";
    cin>>e;
    cout<<"Dwse k: ";
    cin>>k;
    system_solve ss;
    vsize=ss.get_p();

    result.vector_allocation(vsize);
    result=ss.solve(k,e);
    result.print_result();
}
