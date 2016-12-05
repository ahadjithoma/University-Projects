#include <stdlib.h>
#include <iostream>
using namespace std;

/********************neuron************************/
class neuron{
    int n, state, id, *connected_id;
    float *weight, katwfli;
public:
    friend istream &operator >(istream &s, neuron &a);
    void *operator new[](size_t size);
    int get_n(){return n;};
    int get_state(){return state;};
    void set_state(int i){state=i;}
    int get_id(){return id;};
    float get_katwfli(){return katwfli;};
    float get_weight(int i){return weight[i];};
    int get_conn_id(int i){return connected_id[i];};
};

istream &operator >(istream &s, neuron &a){
    int i;
    a.state=1;
    cout<<"Dwse to katwfli energopoihshs tou neurwna "<<a.id<<": ";
    cin>>a.katwfli;
    cout<<"Dwse tis tautotites twn neurwnwn pou sindeonte me ton neurwna "<<a.id<<endl;
    for (i=0; i<a.n; i++){
        cout<<i+1<<": ";
        cin>>a.connected_id[i];
    }
    cout<<"Dwse ta varoi twn sunapsewn pou sindeonte me ton neurwna "<<a.id<<endl;
    for (i=0; i<a.n; i++){
        cout<<i+1<<": ";
        cin>>a.weight[i];
    }
    return s;
}

void *neuron::operator new[](size_t size){
    int i, n;
    neuron *a;
    a=(neuron *)malloc(size);
    n=size/sizeof(neuron);

    for (i=0; i<n; i++){
        a[i].id=i;
        cout<<"Dwse twn arithmo neurwnwn pou sindeonte ston neurwna "<<a[i].id<<": ";
        cin>>a[i].n;
        a[i].connected_id=new int[a[i].n];
        a[i].weight=new float[a[i].n];
        cin>a[i];
    }
    return a;
}



/********************network***********************/
class network{
    int n, state;
    neuron *N;
public:
    friend ostream &operator <(ostream &s, network a);
    void *operator new(size_t size);
    int calk_state();

};

void *network::operator new(size_t size){
    network *a;
    a=(network *)malloc(size);
    cout<<"Dwse ton arithmo twn neurwnwn tou diktiou: ";
    cin>>a->n;
    a->N=new neuron[a->n];
    return a;
}

int network::calk_state(){
    int max_n,f,j,i,k, conn_num, conn_id;
    float sum;
    int *new_state, old_state;
    new_state = new int[n];

    cout<<"Dwse ton megisto arithmo epanalipsewn: ";
    cin>>max_n;

    for (k=0; k<max_n; k++){
        f=1;
        for (i=0; i<n; i++){
            sum=0;
            conn_num=N[i].get_n();
            old_state=N[i].get_state();
            for (j=0;j<conn_num; j++){
                conn_id=N[i].get_conn_id(j);
                sum+=N[i].get_weight(j)*N[conn_id].get_state();
            }
        if(sum>N[i].get_katwfli())
          new_state[i]=1;
        else
          new_state[i]=-1;

        if(old_state!=new_state[i])
            f=0;
        }

    if(f)
        break;
    for(j=0;j<n;j++)
        N[j].set_state(new_state[j]);
    }
    if(f)
        state=1;
    else
        state=0;

    return state;
}


ostream &operator <(ostream &s, network a){
  int i;

  if(a.state)
    cout<<"STABLE"<<endl;
  else
    cout<<"UNSTABLE"<<endl;

  for(i=0;i<a.n;i++)
    cout<<"Neuron: "<<a.N[i].get_id()<<" = "<<a.N[i].get_state()<<endl;

  return s;
}


/*********************main*************************/
main(){
    network *ntw;
    ntw=new network;
    ntw->calk_state();
    cout<ntw[0];
}
